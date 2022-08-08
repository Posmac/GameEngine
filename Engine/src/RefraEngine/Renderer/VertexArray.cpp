#include "rfpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace rfe
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
				RF_CORE_ASSERT(false, "VERTEX ARRAY CREATION::RendererAPI::None  is currently not supported");
				return nullptr;
			case RendererAPI::OpenGL:
				return new OpenGLVertexArray();
		}

		RF_CORE_ASSERT(false, "VERTEX ARRAY CREATION::Unknown RendererAPI");
		return nullptr;
	}
}