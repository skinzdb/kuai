#pragma once

#ifdef SM_PLATFORM_WINDOWS

extern Smong::App* Smong::CreateApp();

int main(int argc, char** argv)
{
	Smong::Log::Init();
	SM_CORE_WARN("Initialised Core Log");
	SM_INFO("Initialised Client Log");

	auto app = Smong::CreateApp();
	app->Run();
	delete app;
}

#endif