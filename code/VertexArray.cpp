
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#include "VertexArray.h"
#include "Renderer.h"

namespace mg
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &id);
	}

	void VertexArray::addBuffer(VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		bind();
		vb.bind();

		const auto elements = layout.getElements();
		unsigned int offset = 0;

		// Add each element to the vertex array object
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];

			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);

			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
	}

	void VertexArray::bind()
	{
		glBindVertexArray(id);
	}

	void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}
}