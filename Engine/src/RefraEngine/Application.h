#pragma once

#include "Core.h"

#include "Window.h"
#include "RefraEngine/LayerStack.h"
#include "RefraEngine/Events/Event.h"
#include "RefraEngine/Events/ApplicationEvent.h"

#include "RefraEngine/ImGui/ImGuiLayer.h"

#include "RefraEngine/Renderer/Shader.h"
#include "RefraEngine/Renderer/Buffer.h"
#include "RefraEngine/Renderer/VertexArray.h"

#include "RefraEngine/Renderer/OrthographicCamera.h"

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

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_BlueVertexArray;

		OrthographicCamera m_Camera;
	private:
		static Application* s_Instance;
	};
	// To be defined in CLIENT
	Application* CreateApplication();
}