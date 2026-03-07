#include "Renderer.h"

namespace Renderer {

	class VertexBuffer {

	public:

		VertexBuffer() {};

		void CreateBuffer(const void* data, GLuint size) {
	
			glGenBuffers(1, &m_RendererID);
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);

		};

		~VertexBuffer() { glDeleteBuffers(1, &m_RendererID); };
		void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); };
		void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); };

	private:
		GLuint m_RendererID;

	};
}
