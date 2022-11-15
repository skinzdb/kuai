#pragma once

#include "Core.h"

class SMONGINE_API App {
public:
	App();
	virtual ~App();

	void Run();
};

App* CreateApp();