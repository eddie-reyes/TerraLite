#include "Renderer.h"

namespace Renderer {

	Renderer::Renderer()
	{

	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Init()
	{

		m_TerrainGeometry.BuildPlane();

		VertexArray va;
		va.Bind();

		VertexBuffer vb(m_TerrainGeometry.GetVertices().data(), 4 * 3 * sizeof(float));
		vb.Bind();

		VertexBufferLayout layout;
		layout.Push<float>(3);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(m_TerrainGeometry.GetIndices().data(), 6);
	}

	void Renderer::Draw(float dt) {



	}

}

