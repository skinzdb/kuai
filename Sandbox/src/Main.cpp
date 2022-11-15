#pragma once

#include <Smongine.h>

class Sandbox : public App {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

App* CreateApp()
{
	return new Sandbox();
}