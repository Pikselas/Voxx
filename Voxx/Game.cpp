#include"resource.h"
#include"Scene.h"
#include"Engine/StandardWindow.h"

#include<queue>
#include<stack>
#include<random>

#include<chrono>

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
	enemy_ship.SetPosition(DirectX::XMVectorSet(500, 250, 0, 1));
	
	Scene scene(engine);

	scene.SetPath(path);
	scene.SetSpaceShip(spaceship);
	scene.SetBullet(bullet);
	scene.SetSky(background);
	scene.SetEnemyShip(enemy_ship);

	window.mouse.OnMove = std::bind(&Scene::SpaceShipController, &scene, std::placeholders::_1);
	window.mouse.OnLeftPress = [&](auto&) { scene.FireBullet(); };
	window.keyboard.EnableKeyRepeat();
	window.keyboard.OnKeyPress = [&](auto ev) 
		{
			switch (ev.KEY_CODE)
			{
				case 'A':
					enemy_ship.SetPosition(DirectX::XMVectorAdd(enemy_ship.GetPosition(), DirectX::XMVectorSet(-10, 0, 0, 0)));
					break;
				case 'D':
					enemy_ship.SetPosition(DirectX::XMVectorAdd(enemy_ship.GetPosition(), DirectX::XMVectorSet(10, 0, 0, 0)));
					break;
			}

			scene.SetEnemyShip(enemy_ship);
			scene.SpaceShipController(window);
		};

	while(window.IsOpen())
	{
		//auto start = std::chrono::high_resolution_clock::now();
		engine.ClearFrame();
		scene.Render();
		engine.RenderFrame();
		//auto time = std::chrono::high_resolution_clock::now() - start;
		//auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
		//OutputDebugStringA(std::to_string(ms).c_str());
		//OutputDebugStringA("\n");
		Window::DispatchWindowEventsNonBlocking();
	}

	return 0;
}