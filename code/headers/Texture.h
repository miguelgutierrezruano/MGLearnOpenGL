
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#pragma once

#include "Renderer.h"

namespace mg
{
	class Texture
	{

	private:

		unsigned int id;
		std::string filePath;
		unsigned char* localBuffer;

		int width;
		int height;
		int bitsPerPixel;

	public:

		Texture(const std::string& path);
	   ~Texture();

		void bind(unsigned slot = 0);
		void unbind();

		int getWidth () const { return  width; }
		int getHeight() const { return height; }
	};
}
