#include "Renderer.h"

namespace Renderer {

	class IndexBuffer {

	public:

		IndexBuffer() {};

		void CreateBuffer(const void* data, GLuint count) {

			m_Count = count;
			glGenBuffers(1, &m_RendererID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);

		}

		~IndexBuffer() { 
			
			glDeleteBuffers(1, &m_RendererID);
		
		}
		void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); };
		void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); };

		inline GLuint GetCount() const { return m_Count; }

	private:
		GLuint m_RendererID;
		GLuint m_Count;
	};
}
