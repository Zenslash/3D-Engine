#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& gfx, float radius)
	: mesh(gfx, radius), constantBuffer(gfx)
{
	Reset();
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Point Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &constantBufferData.pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &constantBufferData.pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &constantBufferData.pos.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Color");
		ImGui::SliderFloat("Intensity", &constantBufferData.diffuseIntentisy, 0.0f, 10.0f, "%.1f");
		ImGui::ColorEdit3("Diffuse Color", &constantBufferData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient Color", &constantBufferData.ambientColor.x);

		ImGui::Text("Attenuation");
		ImGui::SliderFloat("AttConst", &constantBufferData.attConst, 0.0f, 5.0f, "%.1f");
		ImGui::SliderFloat("AttLin", &constantBufferData.attLin, 0.0f, 5.0f, "%.1f");
		ImGui::SliderFloat("AttQuad", &constantBufferData.attQuad, 0.0f, 5.0f, "%.1f");

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	constantBufferData = 
	{
		{0.0f, 0.0f, 0.0f},
		{0.2f, 0.2f, 0.2f},
		{1.0f, 1.0f, 1.0f},
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
}

void PointLight::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	mesh.SetPos(constantBufferData.pos);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx) const noexcept
{
	constantBuffer.Update(gfx, constantBufferData);
	constantBuffer.Bind(gfx);
}
