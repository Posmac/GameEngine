#pragma once

#include "Core.h"

#include "Window.h"
#include "RefraEngine/LayerStack.h"
#include "RefraEngine/Events/Event.h"
#include "RefraEngine/Events/ApplicationEvent.h"

#include "RefraEngine/ImGui/ImGuiLayer.h"

#include "RefraEngine/Renderer/Shader.h"

namespace rfe
{
	class RF_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int VAO, VBO, EBO;
		std::unique_ptr<Shader> m_Shader;

	private:
		static Application* s_Instance;
	};
	// To be defined in CLIENT
	Application* CreateApplication();
}