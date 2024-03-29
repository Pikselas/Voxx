#include "resource.h"
#include "Scene.h"
#include "Engine/StandardWindow.h"

#include <queue>
#include <stack>
#include <random>
#include <thread>

#include "HealShield.h"

#include "ExplosionEffect.h"
#include "HealShieldEffect.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	StandardWindow window("Voxx", 1018, 700);
	window.SetIcon(IDI_ICON1, 256, 256);
	CoreEngine engine;

	auto window_renderer = engine.CreateRenderer(window);
	engine.SetRenderDevice(window_renderer);

	auto path = engine.CreateSprite(Image("C:/Users/Aritra Maji/Downloads/path.png"));
	auto background = engine.CreateSprite(Image("media/background-1.jpg"));
	auto laser_bullet = engine.CreateSprite(Image("media/bullet.png"));

	Scene scene(engine);

	scene.SetPath(path);
	scene.SetSky(background);
	scene.SetLaserBullet(laser_bullet);

	auto lib = LoadLibrary("client.dll");
	if (lib == NULL)
	{
		MessageBox(NULL, "Failed to load client.dll", "Error", MB_OK);
		return 0;
	}

	auto SendEvent = (void(*)(void*, int))GetProcAddress(lib, "Send");
	auto GetEvent = (void(*)(void(*)(void*, void*, int), void*))GetProcAddress(lib, "Fetch");
	if (SendEvent == NULL || GetEvent == NULL)
	{
		MessageBox(NULL, "Failed to load functions", "Error", MB_OK);
		return 0;
	}

	bool RemoteReady = false;

	struct GameData
	{
		Scene& scene;
		bool& RemoteReady;
		CoreEngine& engine;
	};

	GameData game_data{ scene , RemoteReady , engine };
	GetEvent([](void* game_data, void* data, int size)
		{
			auto* ev = (EventHolder*)data;
			auto& scene = ((GameData*)game_data)->scene;
			switch (ev->type)
			{
			case EventHolder::Type::Move:
			{
				auto move_data = ((Event<MoveEvent>*)data)->event_data;
				(scene).SetEnemyShipPosition(move_data.x);
			}
			break;
			case EventHolder::Type::Fire:
				scene.FireEnemyBullet();
				break;
			case EventHolder::Type::Ready:
				((GameData*)game_data)->RemoteReady = true;
				break;
			case EventHolder::Type::ActivateSkill:
			{
				auto skill_data = ((Event<SkillEvent>*)(data))->event_data.type_hash;
				if (typeid(HealShield).hash_code() == skill_data)
				{
					scene.SetEnemySkill<HealShield>(((GameData*)(game_data))->engine);
				}
			}
			break;
			case EventHolder::Type::DisableSkill:
				scene.RemoveEnemySkill();
				break;
		}
		}, &game_data);

	window.keyboard.OnKeyPress = [&](auto ev)
		{
			if (ev.KEY_CODE == 'A')
			{
				scene.SetSkill<HealShield>(engine);
				auto ev = CreateGameEvent(ActivateSkillEvent<HealShield>{});
				SendEvent(&ev, sizeof(ev));
			}
			else if (ev.KEY_CODE == 'D')
			{
				auto ev = CreateGameEvent(DisableSkillEvent{});
				SendEvent(&ev, sizeof(ev));
				scene.RemoveSkill();
			}
		};

	window.mouse.OnMove = [&](auto&)
		{
			MoveEvent ev;
			ev.x = window.mouse.GetX();
			auto e = CreateGameEvent(ev);
			SendEvent(&e, sizeof(e));
			scene.SpaceShipController(window);
		};
	window.mouse.OnLeftPress = [&](auto&)
		{
			auto e = CreateGameEvent(FireEvent{});
			SendEvent(&e, sizeof(e));
			scene.FireBullet();
		};
	window.mouse.OnRightPress = [&](auto&)
		{
			scene.FireTargetedMissile();
		};

	scene.OnLose = [&]()
		{
			RemoteReady = false;
			MessageBox(window.window_handle, "You lose", "Game over", MB_ICONERROR);
			auto ev = CreateGameEvent(ReadyEvent{});
			SendEvent(&ev, sizeof(ev));
			scene.Reset();
		};
	scene.OnWin = [&]()
		{
			RemoteReady = false;
			MessageBox(window.window_handle, "You win", "Game over", MB_ICONINFORMATION);
			auto ev = CreateGameEvent(ReadyEvent{});
			SendEvent(&ev, sizeof(ev));
			scene.Reset();
		};

	while (window.IsOpen())
	{
		if (true)
		{
			engine.ClearFrame(window_renderer);
			scene.Render();
			window_renderer.RenderFrame();
		}
		Window::DispatchWindowEventsNonBlocking();
	}

	return 0;
}