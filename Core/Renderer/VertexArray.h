#pragma once


#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Renderer
{

	class VertexArray {


	public:
		VertexArray() { glGenVertexArrays(1, &m_RendererID); };

		~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); };

		void AddBuffer(unsigned int& vboCount, const VertexBufferLayout& layout) {

			const auto& element = layout.GetElement();

			glEnableVertexAttribArray(vboCount);
			glVertexAttribPointer(vboCount, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)0);
			
			vboCount++;
		}

		void Bind() const { glBindVertexArray(m_RendererID); };
		void Unbind() const { glBindVertexArray(0); };

	private:

		GLuint m_RendererID;

		
	};

	
};
