#pragma once
#include "Window.h"
#include "Timer.h"
#include "ImguiManager.h"

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
	ImguiManager imguiManager;
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class Drawable>> boxes;
};

