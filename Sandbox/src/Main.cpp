#pragma once

#include <Smongine.h>

class BaseLayer : public Smong::Layer
{
public:
	BaseLayer() : Layer("Base") {}

	void Update() override
	{
		
	}

	void OnEvent(Smong::Event& event) override
	{
		SM_TRACE("{0}", event);
	}

};

class Sandbox : public Smong::App {
public:
	Sandbox() {
		PushLayer(new BaseLayer());
	}

	~Sandbox() {

	}
};

Smong::App* Smong::CreateApp()
{
	return new Sandbox();
}

