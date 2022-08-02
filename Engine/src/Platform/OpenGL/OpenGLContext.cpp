#include "rfpch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace rfe
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		RF_CORE_ASSERT(windowHandle, "Window handle is null")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RF_CORE_ASSERT(status, "Failed to initialize Glad!");

		RF_CORE_INFO("OpenGL Info:");
		RF_CORE_INFO("    Vendor {0}:", glGetString(GL_VENDOR));
		RF_CORE_INFO("    Renderer {0}:", glGetString(GL_RENDERER));
		RF_CORE_INFO("    Version {0}:", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}