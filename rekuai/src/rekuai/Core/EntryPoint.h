#pragma once

#include "Core.h"
#include "App.h"

extern kuai::App* kuai::create_app();

int main(int argc, char** argv)
{
	kuai::Log::Init();

	auto app = kuai::create_app();
	app->run();
	delete app;

	return 0;
}