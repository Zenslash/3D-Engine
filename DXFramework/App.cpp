#include "App.h"
#include <memory>
#include "Pyramid.h"
#include "Box.h"
#include "ModelTest.h"
#include "SkinnedBox.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "VertexLayout.h"
#include "imgui/imgui.h"

GDIPlusManager gdpm;

App::App() : wnd(1280, 720, "3D Framework")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	std::uniform_real_distribution<float> cdist(0.0f, 1.0f);
	std::uniform_int_distribution<int> sdist{ 0,4 };
	std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
	std::uniform_int_distribution<int> tdist{ 3,30 };

	const DirectX::XMFLOAT3 mat = { cdist(rng),cdist(rng),cdist(rng) };
	for (auto i = 0; i < 1; i++)
	{
		/*drawables.push_back(std::make_unique<SkinnedBox>(
			wnd.GFX(), rng, adist, ddist, odist,
			rdist));*/
		/*drawables.push_back(std::make_unique<Box>(
			wnd.GFX(), rng, adist, ddist,
			odist, rdist, mat));*/

		drawables.push_back(std::make_unique<ModelTest>(
			wnd.GFX(), rng, adist, ddist,
			odist, rdist, mat, 0.1f));
	}
	for (auto& d : drawables)
	{
		if (auto pt = dynamic_cast<Box*>(d.get()))
		{
			boxes.push_back(pt);
		}
	}

	wnd.GFX().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	wnd.GFX().SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f));

	plight = std::make_unique<PointLight>(wnd.GFX());
}

App::~App()
{
	plight.release();
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
	wnd.GFX().SetCamera(cam.GetCameraMatrix());
	plight->Bind(wnd.GFX(), cam.GetCameraMatrix());
	for (auto& b : drawables)
	{
		b->Update(dt);
		b->Draw(wnd.GFX());
	}
	plight->Draw(wnd.GFX());

	SpawnTimeWindow();
	cam.SpawnControlWindow();
	plight->SpawnControlWindow();
	SpawnBoxWindowManagerWindow();
	SpawnBoxWindows();

	wnd.GFX().RenderFrame();
}

void App::SpawnTimeWindow() noexcept
{
	//imgui wnd to control simulation speed
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 4.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();
}

void App::SpawnBoxWindowManagerWindow() noexcept
{
	//Control window for boxes
	if (ImGui::Begin("Boxes"))
	{
		using namespace std::string_literals;
		const auto preview = comboBoxIndex ? std::to_string(*comboBoxIndex) : "Choose a box...";
		if (ImGui::BeginCombo("Box Number", preview.c_str()))
		{
			for (int i = 0; i < boxes.size(); i++)
			{
				bool selected = false;
				if (comboBoxIndex)
				{
					selected = *comboBoxIndex == i;
				}

				if (ImGui::Selectable(std::to_string(i).c_str(), selected))
				{
					comboBoxIndex = i;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Spawn Control Window") && comboBoxIndex)
		{
			boxControlIds.insert(*comboBoxIndex);
			comboBoxIndex.reset();
		}
	}
	ImGui::End();
}

void App::SpawnBoxWindows() noexcept
{
	for (auto id = boxControlIds.begin(); id != boxControlIds.end();)
	{
		if (!boxes[*id]->SpawnControlWindow(*id, wnd.GFX()))
		{
			id = boxControlIds.erase(id);
		}
		else
		{
			id++;
		}
	}
}
