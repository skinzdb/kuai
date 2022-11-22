#pragma once

#include <Smongine.h>

class Sandbox : public Smong::App {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Smong::App* Smong::CreateApp()
{
	return new Sandbox();
}