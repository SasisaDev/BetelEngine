#pragma once

#include <Core/Application/Application.h>
#include <Log/Logger.h>

int GuardedMain()
{
	LOG(Log, LogStartup, "DEIRI Frameworks");

	Application app;

	WindowCreateInfo wininfo = {
		.title="Test",
		.width=1280,
		.height=720,
		.layerRefs=std::vector<IRenderLayerRef*>()
	};
	app.CreateWindow(wininfo);

	app.ApplicationLoop();

	return 0;
}