#include"resource.h"
#include"Scene.h"
#include"Engine/StandardWindow.h"

#include<queue>
#include<stack>
#include<random>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	StandardWindow window( "Voxx",1018 , 700 );
	window.SetIcon(IDI_ICON1 , 256 , 256);
	CoreEngine engine( window );

	auto path = engine.CreateSprite(Image("media/road-1.jpg"));
	auto spaceship = engine.CreateSprite(Image("media/space_ship.png"));
	auto background = engine.CreateSprite(Image("media/background-1.jpg"));

	Scene scene(engine);

	scene.SetPath(path);
	scene.SetSpaceShip(spaceship);
	scene.SetSky(background);

	window.mouse.OnMove = std::bind(&Scene::SpaceShipController, &scene, std::placeholders::_1);

	while(window.IsOpen())
	{
		engine.ClearFrame();
		scene.Render();
		engine.RenderFrame();
		Window::DispatchWindowEventsNonBlocking();
	}

	return 0;
}