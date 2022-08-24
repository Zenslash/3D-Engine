#include "App.h"
#include <memory>

#include "SkinnedBox.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

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
	auto dt = timer.Mark();
	wnd.GFX().ClearBuffer(0.07f, 0.0f, 0.12f);
	for (auto& b : boxes)
	{
		b->Update(dt);
		b->Draw(wnd.GFX());
	}

	//imgui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool isShowDemoWindow = true;
	if (isShowDemoWindow)
	{
		ImGui::ShowDemoWindow(&isShowDemoWindow);
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	wnd.GFX().RenderFrame();
}
