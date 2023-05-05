
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace mg
{
	class VertexArray
	{
		
	private:

		unsigned int id;

	public:

		VertexArray();
	   ~VertexArray();

	   void addBuffer(VertexBuffer& vb, const VertexBufferLayout& layout);

	   void bind();
	   void unbind();
	};
}
