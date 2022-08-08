#include "rfpch.h"
#include "Application.h"

#include "RefraEngine/Renderer/Renderer.h"

#include "Input.h"

namespace rfe
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		RF_CORE_ASSERT(!s_Instance, "Application already exist");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 1.0,
			0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0,
			0.0f, 0.5f, 0.0f, 0.0, 0.0, 1.0, 1.0,
		};

		unsigned int indices[3] = { 0, 1, 2 };

		std::shared_ptr<VertexBuffer> vertexBuffer; 
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		{
			BufferLayout layout =
			{
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float4, "a_Color"}
			};

			vertexBuffer->SetLayout(layout);
		}
		std::shared_ptr<IndexBuffer> indexBuffer; 
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));

		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_BlueVertexArray.reset(VertexArray::Create());

		float blueVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		unsigned int blueIndices[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<VertexBuffer> blueVertexBuffer;
		blueVertexBuffer.reset(VertexBuffer::Create(blueVertices, sizeof(blueVertices)));
		{
			BufferLayout layout =
			{
				{ShaderDataType::Float3, "a_Position"}
			};

			blueVertexBuffer->SetLayout(layout);
		}
		std::shared_ptr<IndexBuffer> blueIndexBuffer;
		blueIndexBuffer.reset(IndexBuffer::Create(blueIndices, sizeof(blueIndices) / sizeof(unsigned int)));

		m_BlueVertexArray->AddVertexBuffer(blueVertexBuffer);
		m_BlueVertexArray->SetIndexBuffer(blueIndexBuffer);

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

		std::string blueVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			void main()
			{
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string blueFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(0.0, 0.2, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Shader(blueVertexSrc, blueFragmentSrc));
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
				RenderCommand::SetClearColor({ 1.0, 0.3, 0.2, 1.0 });
				RenderCommand::Clear();

				Renderer::BeginScene();

				m_BlueShader->Bind();
				Renderer::Submit(m_BlueVertexArray);

				m_Shader->Bind();
				Renderer::Submit(m_VertexArray);

				Renderer::EndScene();

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
