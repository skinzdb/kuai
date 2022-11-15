#pragma once

#ifdef SM_PLATFORM_WINDOWS

extern App* CreateApp();

int main(int argc, char** argv)
{
	printf("CUMBLOR\n");
	auto app = CreateApp();
	app->Run();
	delete app;
}

#endif