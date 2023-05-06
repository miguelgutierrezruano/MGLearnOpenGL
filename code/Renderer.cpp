
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#include "Renderer.h"

namespace mg
{
    void GLClearError()
    {
        while (glGetError() != GL_NO_ERROR);
    }

    bool GLLogCall()
    {
        while (GLenum error = glGetError())
        {
            std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
            return false;
        }

        return true;
    }

    void Renderer::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::draw(VertexArray& vertexArray, IndexBuffer& indexBuffer, Shader& shader)
    {
        shader.bind();
        vertexArray.bind();
        indexBuffer.bind();

        glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}
