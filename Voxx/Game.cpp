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
	StandardWindow window( "Voxx",1018 , 700 );
	window.SetIcon(IDI_ICON1 , 256 , 256);
	CoreEngine engine( window );

	auto path = engine.CreateSprite(Image("media/road-1.jpg"));
	auto spaceship = engine.CreateSprite(Image("media/space_ship.png"));
	auto background = engine.CreateSprite(Image("media/background-1.jpg"));
	auto enemy_ship = engine.CreateSprite(Image("media/Enemy_ship_.png"));
	auto bullet = engine.CreateSprite(Image("media/bullet.png"));
	auto enemy_bullet = engine.CreateSprite(Image("media/bullet-2.png"));
	enemy_ship.SetPosition(DirectX::XMVectorSet(500, 250, 0, 1));
	
	Scene scene(engine);

	scene.SetPath(path);
	scene.SetSpaceShip(spaceship);
	scene.SetBullet(bullet);
	scene.SetEnemyBullet(enemy_bullet);
	scene.SetSky(background);
	scene.SetEnemyShip(enemy_ship);

	auto lib = LoadLibrary("client.dll");
	if(lib == NULL)
	{
		MessageBox(NULL, "Failed to load client.dll", "Error", MB_OK);
		return 0;
	}
	
	auto SendEvent = (void(*)(void*, int))GetProcAddress(lib, "Send");
	auto GetEvent = (void(*)(void(*)(void*,void*,int) , void*))GetProcAddress(lib, "Fetch");
	if(SendEvent == NULL || GetEvent == NULL)
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
		auto *ev = (EventHolder*)data;
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
			{
				(scene).FireEnemyBullet();
			}
			break;
			case EventHolder::Type::Ready:
			{
				((GameData*)game_data)->RemoteReady = true;
			}
			break;
		}
		//((Scene*)scene)->SetEnemyShipPosition(*ev);
	}, &game_data);

	window.mouse.OnMove = [&](auto&)
	{
		MoveEvent ev;
		ev.x = window.mouse.GetX();
		auto e = CreateGameEvent(ev);
		SendEvent(&e , sizeof(e));
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
		MessageBox(window.window_handle, "You lose", "Game over", MB_ICONINFORMATION);
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
	
	while(window.IsOpen())
	{
		if (RemoteReady)
		{
			engine.ClearFrame();
			scene.Render();
			engine.RenderFrame();
		}
		Window::DispatchWindowEventsNonBlocking();
	}

	return 0;
}