#include"resource.h"
#include"Goblin.h"
#include"Engine/StandardWindow.h"

#include<queue>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	StandardWindow window( "Voxx",1018 , 700 );
	window.SetIcon(IDI_ICON1 , 256 , 256);
	CoreEngine engine( window );

	auto space_ship = engine.CreateSprite(Image("C:/Users/Aritra Maji/Downloads/_81aa21ff-12d1-48ee-8d71-a8d7b7f7d64d-removebg-preview.png"));
	space_ship.SetPosition(DirectX::XMVectorSet(500, 550 , 0 , 0));

	auto path_comp = engine.CreateSprite(Image("C:/Users/Aritra Maji/Downloads/road.jpg"));

	auto back_ground = engine.CreateSprite(Image("C:/Users/Aritra Maji/Downloads/bg.jpg"));
	back_ground.SetPosition(DirectX::XMVectorSet(509, 240, 0, 0));

	struct RoadComp
	{
		float scale_x;
		float scale_y;
		ImageSprite comp;
	};

	std::queue<RoadComp> path_components;

	int pos_y = 450;
	float scale_x = 0.65;
	float scale_y = 0.3;

	for (int i = 0; i < 11; ++i)
	{
		auto comp = path_comp;
		comp.SetPosition(DirectX::XMVectorSet(509, pos_y, 0, 0));
		comp.SetTransformation(DirectX::XMMatrixScaling(scale_x, scale_y, 1.0));
		path_components.push(RoadComp{ scale_x , scale_y , comp });

		pos_y += 20;
		scale_x += 0.10;
		scale_y += 0.05;

	}

	window.mouse.OnMove = [&](auto&)
	{
		space_ship.SetPosition(DirectX::XMVectorSet(window.mouse.GetX(), 550, 0, 0));
	};

	while (window.IsOpen())
	{
		engine.ClearFrame();
		back_ground.Draw(engine);
		int size = path_components.size();
		while (size-- > 0)
		{
			auto comp = path_components.front();
			comp.comp.Draw(engine);
			auto pos = comp.comp.GetPosition();
			pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorSet(0, 1, 0, 0));
			comp.comp.SetPosition(pos);
			comp.scale_x += 0.005;
			comp.scale_y += 0.0025;
			comp.comp.SetTransformation(DirectX::XMMatrixScaling(comp.scale_x, comp.scale_y, 1.0));
			path_components.pop();

			if (DirectX::XMVectorGetY(pos) > 700)
			{
				comp.comp.SetPosition(DirectX::XMVectorSet(509, 460, 0, 0));
				comp.scale_x = 0.65;
				comp.scale_y = 0.3;
				comp.comp.SetTransformation(DirectX::XMMatrixScaling(comp.scale_x, comp.scale_y, 1.0));
			}

			path_components.push(comp);
		}

		space_ship.Draw(engine);

		engine.RenderFrame();
		Window::DispatchWindowEventsNonBlocking();
	}

	return 0;
}