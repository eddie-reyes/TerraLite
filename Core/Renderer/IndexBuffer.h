#include "Renderer.h"

namespace Renderer {

	class IndexBuffer {

	public:
		IndexBuffer(const void* data, GLuint count) {

			glGenBuffers(1, &m_RendererID);
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);

		}

		~IndexBuffer() { glDeleteBuffers(1, &m_RendererID); }
		void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); };
		void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); };

	private:
		GLuint m_RendererID;

	};
}
