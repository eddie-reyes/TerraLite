#pragma once

#include <assert.h>
#include <vector>
#include <glad/gl.h>

namespace Renderer {

	struct VertexBufferElement {
		unsigned int type;
		unsigned int count;
		unsigned char normalized;
		static unsigned int GetSizeOfType(unsigned int type) {
			switch (type) {
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
			}
			assert(false);
			return 0;
		}
	};


	class VertexBufferLayout {

	private:

		VertexBufferElement m_Element;
		unsigned int m_Stride;

	public:

		VertexBufferLayout() : m_Stride(0) {}
		template<typename T>
		void Create(unsigned int count) {
			//static_assert(false);
		}
		template<>
		void Create<float>(unsigned int count) {
			m_Element = { GL_FLOAT, count, GL_FALSE };
			m_Stride = count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
		}
		template<>
		void Create<unsigned int>(unsigned int count) {
			m_Element = { GL_UNSIGNED_INT, count, GL_FALSE };
			m_Stride = count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
		}
		template<>
		void Create<unsigned char>(unsigned int count) {
			m_Element = { GL_UNSIGNED_BYTE, count, GL_TRUE };
			m_Stride = count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
		}

		inline const VertexBufferElement& GetElement() const { return m_Element; }
		inline unsigned int GetStride() const { return m_Stride; }


	};
};




