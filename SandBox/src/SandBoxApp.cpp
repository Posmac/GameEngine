#include <RefraEngine.h>
#include "glm/glm.hpp"

#include "imgui/imgui.h"

class ExampleLayer : public rfe::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (rfe::Input::IsKeyPressed(RF_KEY_TAB))
		{
			RF_CLIENT_TRACE("Tab key is pressed(poll)");
		}
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(rfe::Event& e) override
	{
		if (e.GetEventType() == rfe::EventType::KeyPressed)
		{
			rfe::KeyPressedEvent& ev = (rfe::KeyPressedEvent&)e;
			if (ev.GetKeyCode() == RF_KEY_TAB)
			{
				RF_CLIENT_TRACE("Tab key is pressed (event)");
			}
			RF_CLIENT_TRACE("{0}", (char)ev.GetKeyCode());
		}
	}
};

class SandBox : public rfe::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}

	~SandBox()
	{
		
	}
};

rfe::Application* rfe::CreateApplication()
{
	return new SandBox();
}