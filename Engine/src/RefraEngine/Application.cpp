#include "rfpch.h"
#include "Application.h"

#include <glad/glad.h>

#include "Input.h"

namespace rfe
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:  return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::Mat3:   return GL_FLOAT;
			case ShaderDataType::Mat4:   return GL_FLOAT;
			case ShaderDataType::Int:    return GL_INT;
			case ShaderDataType::Int2:   return GL_INT;
			case ShaderDataType::Int3:   return GL_INT;
			case ShaderDataType::Int4:   return GL_INT;
			case ShaderDataType::Bool:   return GL_BOOL;
		}

		RF_CORE_ASSERT(false, "Unknown Shader data type");
		return 0;
	}

	Application::Application() {
		RF_CORE_ASSERT(!s_Instance, "Application already exist");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);


		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 1.0,
			0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0,
			0.0f, 0.5f, 0.0f, 0.0, 0.0, 1.0, 1.0,
		};

		unsigned int indices[3] = { 0, 1, 2 };

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));

		{
			BufferLayout layout =
			{
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float4, "a_Color"}
			};

			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();

		for (const auto& el : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, el.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(el.Type),
				el.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)el.Offset);
			index++;
		}


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application(){
		s_Instance = nullptr;
	}

	void Application::Run()
	{
		WindowResizeEvent wnd(1920, 1080);
		if (wnd.IsInCategory(EventCategory::ApplicationCategory))
		{ 
			while (m_Running)
			{
				glClearColor(1, 0, 1, 1);
				glClear(GL_COLOR_BUFFER_BIT);
			
				m_Shader->Bind();
				glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount() , GL_UNSIGNED_INT, nullptr);

				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate();
				}

				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
				
				m_Window->OnUpdate();
			}
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
