#pragma once


#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Renderer
{

	static int VAO_COUNT = 0;

	class VertexArray {

	public:
		VertexArray() { glGenVertexArrays(1, &m_RendererID); };

		~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); };

		void AddBuffer(const VertexBufferLayout& layout) {

			const auto& elements = layout.GetElements();
			unsigned int offset = 0;

			for (size_t i = 0; i < elements.size(); i++) {
				const auto& element = elements[i];
				glEnableVertexAttribArray(VAO_COUNT);
				glVertexAttribPointer(VAO_COUNT, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)offset);
				offset += element.count * VertexBufferElement::GetSizeOfType(element.type);

			}

			VAO_COUNT++;

		}

		void Bind() const { glBindVertexArray(m_RendererID); };
		void Unbind() const { glBindVertexArray(0); };

	private:

		GLuint m_RendererID;

		
	};

	
};
