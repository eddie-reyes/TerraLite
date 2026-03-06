#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Renderer
{

	class VertexArray {

	private:
		unsigned int m_RendererID;

	public:
		VertexArray() { glGenVertexArrays(1, &m_RendererID); };

		~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); };

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {

			vb.Bind();
			const auto& elements = layout.GetElements();
			unsigned int offset = 0;

			for (size_t i = 0; i < elements.size(); i++) {
				const auto& element = elements[i];
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)offset);
				offset += element.count * VertexBufferElement::GetSizeOfType(element.type);

			}

		}

		void Bind() const { glBindVertexArray(m_RendererID); };
		void Unbind() const { glBindVertexArray(0); };
	};
};
