#include"resource.h"
#include"Scene.h"
#include"Engine/StandardWindow.h"

#include<queue>
#include<stack>
#include<random>
#include<thread>

#include"GameEvents.h"

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
	};

	GameData game_data{ scene , RemoteReady };
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
			}
		}, &game_data);

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

	std::function<std::vector<DirectX::XMVECTOR>(float, std::vector<DirectX::XMVECTOR>)> GetCurvePoint = [&](float t, std::vector<DirectX::XMVECTOR> control_points)
		{
			if (control_points.size() > 1)
			{
				std::vector<DirectX::XMVECTOR> new_points;
				for (int i = 0; i < control_points.size() - 1; ++i)
				{
					new_points.emplace_back(DirectX::XMVectorLerp(control_points[i], control_points[i + 1], t));
				}
				return GetCurvePoint(t, new_points);
			}
			return control_points;
		};

	auto p1 = DirectX::XMVectorSet(509, 700, 0, 1);
	auto p2 = DirectX::XMVectorSet(0, 0, 0, 1);
	auto p3 = DirectX::XMVectorSet(0, 0, 0, 1);

	/*window.mouse.OnMove = [&](auto& wnd)
		{
			auto [x, y] = wnd.mouse.GetXY();
			p3 = DirectX::XMVectorSet( x , y , 0 , 1 );
		};

	float f = 0.1;*/

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