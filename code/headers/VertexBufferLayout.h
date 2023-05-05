
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#pragma once

#include <glad/glad.h>

#include <assert.h>
#include <vector>

namespace mg
{
	// Define attributes of glVertexAttribPointer
	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		VertexBufferElement(unsigned int _type, unsigned int _count, unsigned char _normalized) :
			type(_type),
			count(_count),
			normalized(_normalized)
		{ }

		static unsigned int GetSizeOfType(unsigned int type)
		{
			switch (type)
			{
				case GL_FLOAT:			return 4;
				case GL_UNSIGNED_INT:	return 4;
				case GL_UNSIGNED_BYTE:	return 1;
			}

			assert(false);
			return 0;
		}
	};

	class VertexBufferLayout
	{

	private:

		std::vector<VertexBufferElement> elements;

		unsigned int stride;

	public:

		VertexBufferLayout() : stride(0)
		{ }

	public:

		const std::vector<VertexBufferElement>& getElements() const { return elements; }
		const unsigned int getStride() const { return stride; }

		template<typename T>
		void push(unsigned int count)
		{
			assert(false);
		}

		// Template specializations
		template<>
		void push<float>(unsigned int count)
		{
			elements.push_back(VertexBufferElement(GL_FLOAT, count, GL_FALSE));
			stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
		}

		template<>
		void push<unsigned int>(unsigned int count)
		{
			elements.push_back(VertexBufferElement(GL_UNSIGNED_INT, count, GL_FALSE));
			stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
		}

		template<>
		void push<unsigned char>(unsigned int count)
		{
			elements.push_back(VertexBufferElement(GL_UNSIGNED_BYTE, count, GL_TRUE));
			stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
		}
	};
}
