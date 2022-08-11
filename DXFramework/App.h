#pragma once
#include "Window.h"
#include "Timer.h"

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
	Window wnd;
	Timer timer;
	std::vector<std::unique_ptr<class Box>> boxes;
};

