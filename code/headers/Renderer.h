
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#pragma once

#include <glad/glad.h>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace mg
{
	void GLClearError();
	bool GLLogCall();

	class Renderer
	{

	private:

	public:

		void clear();
		void draw(VertexArray& vertexArray, IndexBuffer& indexBuffer, Shader& shader);
	};
}

