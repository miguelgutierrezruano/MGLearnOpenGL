
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#pragma once

namespace mg
{
	// TODO: Make template with different int types
	class IndexBuffer
	{

	private:

		// ID of vertex buffer given by OpenGL
		unsigned int id;

		unsigned int count;

	public:

		IndexBuffer(const unsigned int* data, unsigned int bufferCount);
	   ~IndexBuffer();

	public:

		void bind();
		void unbind();

		unsigned int GetCount() const { return count; }
	};
}

