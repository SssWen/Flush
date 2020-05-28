#pragma once

#include "Flush.h"
//#ifdef FLUSH_PLATFORM_WINDOW

//extern Flush::Application* Flush::CreateApplication();
// lib�ⲻ��Ҫ ʹ��extern ����,��ʹ��dll��ʱ��,��Ҫ����
int main(int argc, char** argv)
{
	Flush::Log::Init();
	Flush::Log::GetCoreLogger()->info("Initianlize FlushEngine!");
	Flush::Log::GetCoreLogger()->warn("Hello World!");
	
	FLUSH_PROFILE_BEGIN_SESSION("Startup", "FlushProfile-Startup.json");

	auto app = Flush::CreateApplication();
	app->Run();
	delete app;
	FLUSH_PROFILE_END_SESSION();
	return 0;
}

//#endif