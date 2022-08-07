#include "App.h"

App::App() : wnd(800, 600, "3D Framework")
{}

int App::Go()
{
	while(true)
	{
		//Process pending messages
		if(const auto ecode = Window::ProcessMesssages())
		{
			return *ecode;
		}

		Tick();
	}
}

void App::Tick()
{
	wnd.GFX().ClearBuffer(0.5f, 0.5f, 0.5f);
	wnd.GFX().DrawTestTriangle(timer.Peek(), wnd.mouse.GetPosX() / 400.0f - 1.0f, -wnd.mouse.GetPosY() / 300.0f + 1.0f);
	wnd.GFX().RenderFrame();
}
