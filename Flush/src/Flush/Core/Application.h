#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "Core/LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "Flush/ImGui/ImGuiLayer.h"


int main(int argc, char** argv);
namespace Flush {
	
	class FLUSH_API Application // __declspec(dllexport)
	{	
	public:
		Application();
		virtual ~Application() = default;

		

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		void Run(); // make runloop only accessible on the engine side.
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		bool m_Minimized = false;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// to be define in client
	Application* CreateApplication();
}


