#pragma once

//#ifdef FLUSH_PLATFORM_WINDOW

//extern Flush::Application* Flush::CreateApplication();
// lib�ⲻ��Ҫ ʹ��extern ����,��ʹ��dll��ʱ��,��Ҫ����
int main(int argc, char** argv)
{
	Flush::Log::Init();
	Flush::Log::GetCoreLogger()->info("Initianlize FlushEngine!");
	Flush::Log::GetCoreLogger()->warn("Hello World!");
	
	// Use Macro
	//Flush_CORE_TRACE("Flush_CORE_TRACE"); 
	int a = 5;
	//FLUSH_ERROR("FLUSH_ERROR a =  {0}", a );

	auto app = Flush::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

//#endif