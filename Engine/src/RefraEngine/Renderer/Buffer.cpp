#include "rfpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace rfe
{
	VertexBuffer* VertexBuffer::Create(float* vertices, unsigned int size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
				RF_CORE_ASSERT(false, "VERTEX BUFFER CREATION::RendererAPI::None  is currently not supported");
				return nullptr;
			case RendererAPI::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
		}

		RF_CORE_ASSERT(false, "VERTEX BUFFER CREATION::Unknown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(unsigned int* vertices, unsigned int count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
				RF_CORE_ASSERT(false, "VERTEX BUFFER CREATION::RendererAPI::None  is currently not supported");
				return nullptr;
			case RendererAPI::OpenGL:
				return new OpenGLIndexBuffer(vertices, count);
		}

		RF_CORE_ASSERT(false, "VERTEX BUFFER CREATION::Unknown RendererAPI");
		return nullptr;
	}
}