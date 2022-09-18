#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <memory>

class App
{
public:
	App();
	~App();
	//Master Frame/Message Loop
	int Go();
private:
	void Tick();
private:
	bool isShowDemoWindow = true;
	float speedFactor = 1.0f;
	Camera cam;
	std::unique_ptr<PointLight> plight = nullptr;
	ImguiManager imguiManager;
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	std::vector<class Box*> boxes;
};

