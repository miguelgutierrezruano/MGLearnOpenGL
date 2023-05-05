
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

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

using namespace sf;
using namespace mg;

using namespace std::chrono;

struct ShaderSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderSource ParseShader(const std::string& path)
{
    // Open file
    std::ifstream stream(path);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    // Declare string where to copy
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    // Read line from file
    std::string line;
    while (getline(stream, line))
    {
        // If line defines a shader change type
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        // Add line to shader string
        else
        {
            if(type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource (id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    // Error handling
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") << "shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
{
    unsigned int program = glCreateProgram();

    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderCode);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

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

    // Parse shader code
    ShaderSource source = ParseShader("../code/shaders/Basic.shader");

    // Compile and use vertex and fragment shader
    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
    glUseProgram(shader);

    glClearColor(0.1f, 0.1f, 0.1f, 1);

    int location = glGetUniformLocation(shader, "u_Color");
    glUniform4f(location, 1.0f, 0.0f, 1.0f, 1.0f);

    float redChannel = 0.0f;
    float increment = 0.05f;

    // Unbind every buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

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

        glUseProgram(shader);

        // Set color on main loop
        glUniform4f(location, redChannel, 0.0f, 1.0f, 1.0f);

        vertexArray.bind();
        indexBuffer.bind();
        
        // Draw elements
        GLClearError();
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr);
        assert(GLLogCall());

        if (redChannel > 1.0f)
            increment = -0.02f;
        else if (redChannel < 0.0f)
            increment = 0.02f;

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

    glDeleteProgram(shader);

    return 0;
}