
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <glad/glad.h>

#include <thread>
#include <chrono>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Shader.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

using namespace sf;
using namespace mg;

using namespace std::chrono;

int main()
{
    // Window with OpenGL context
    Window window(VideoMode(800, 600), "MGLearnOpenGL", Style::Default, ContextSettings(24, 0, 0, 3, 3, ContextSettings::Core));

    // Glad initialization
    GLenum glad_init = gladLoadGL();

    // Stop program if glad could not load properly
    assert(glad_init != 0);

    std::cout << glGetString(GL_VERSION) << std::endl;

    window.setVerticalSyncEnabled(true);

    bool running = true;

    float coordinates[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f, 
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    // Create VAO 
    mg::VertexArray vertexArray;

    // Create vertex buffer on GPU
    mg::VertexBuffer vertexBuffer(coordinates, 4 * 2 * sizeof(float));

    mg::VertexBufferLayout vertexBufferLayout;
    vertexBufferLayout.push<float>(2);

    vertexArray.addBuffer(vertexBuffer, vertexBufferLayout);

    // Create index buffer on GPU
    mg::IndexBuffer indexBuffer(indices, 6);

    mg::Shader shader("../code/shaders/Basic.shader");
    shader.bind();
    shader.setUniform4f("u_Color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

    glClearColor(0.1f, 0.1f, 0.1f, 1);


    float redChannel = 0.0f;
    float increment = 0.05f;

    // Unbind every buffer
    vertexArray.unbind();
    shader.unbind();
    vertexBuffer.unbind();
    indexBuffer.unbind();

    // Delta time variables
    auto  chrono = high_resolution_clock();
    float target_time = 1.f / 60;
    float delta_time = target_time;

    do
    {
        // Get time where frame started
        high_resolution_clock::time_point start = chrono.now();

        Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                {
                    running = false;
                    break;
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        shader.bind();
        shader.setUniform4f("u_Color", glm::vec4(redChannel, 0.0f, 1.0f, 1.0f));

        vertexArray.bind();
        indexBuffer.bind();
        
        // Draw elements
        GLClearError();
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr);
        assert(GLLogCall());

        if (redChannel > 1.0f)
            increment = -0.01f;
        else if (redChannel < 0.0f)
            increment = 0.01f;

        redChannel += increment;

        window.display();

        // Compute delta time
        float elapsed = duration<float>(chrono.now() - start).count();

        if (elapsed < target_time)
        {
            std::this_thread::sleep_for(duration<float>(target_time - elapsed));
        }

        delta_time = duration<float>(chrono.now() - start).count();

    } while (running);

    return 0;
}