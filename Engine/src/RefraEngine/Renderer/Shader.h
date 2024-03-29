#pragma once

#include <string>
#include <glm/glm.hpp>

namespace rfe {

	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;
	};

}