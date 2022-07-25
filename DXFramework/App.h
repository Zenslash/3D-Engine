#pragma once
#include "Window.h"
#include "Timer.h"

class App
{
public:
	App();
	//Master Frame/Message Loop
	int Go();
private:
	void Tick();
private:
	Window wnd;
	Timer timer;
};

