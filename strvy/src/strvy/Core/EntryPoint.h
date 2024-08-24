#pragma once


#ifdef SV_PLATFORM_WINDOWS

extern strvy::Application* strvy::createApplication();

int main(int argc, char** argv)
{
	strvy::Log::init();
	//SV_CORE_WARN("Initialized Log!");
	//int a = 5;
	//SV_INFO("Hello Var={0}", a);

	SV_PROFILE_BEGIN_SESSION("Startup", "strvyProfile-Startup.json");
	auto app = strvy::createApplication();
	SV_PROFILE_END_SESSION();

	SV_PROFILE_BEGIN_SESSION("Runtime", "strvyProfile-Runtime.json");
	app->run();
	SV_PROFILE_END_SESSION();

	SV_PROFILE_BEGIN_SESSION("Shutdown", "strvyProfile-Shutdown.json");
	delete app;
	SV_PROFILE_END_SESSION();
}

#endif