#include "App.h"
#include <memory>

#include "SkinnedBox.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"

GDIPlusManager gdpm;

App::App() : wnd(800, 600, "3D Framework")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 80; i++)
	{
		boxes.push_back(std::make_unique<SkinnedBox>(
			wnd.GFX(), rng, adist,
			ddist, odist, rdist
			));
	}

	wnd.GFX().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	wnd.GFX().SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
}

App::~App()
{

}

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
	auto dt = timer.Mark() * speedFactor;

	wnd.GFX().BeginFrame(0.07f, 0.0f, 0.12f);
	for (auto& b : boxes)
	{
		b->Update(dt);
		b->Draw(wnd.GFX());
	}

	//imgui wnd to control simulation speed
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 4.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();

	wnd.GFX().RenderFrame();
}
