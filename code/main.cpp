
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <glad/glad.h>

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace sf;

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall()
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
        return false;
    }

    return true;
}

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
    Window window(VideoMode(800, 600), "MGLearnOpenGL", Style::Default, ContextSettings(0, 0, 0, 3, 3, ContextSettings::Core));

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

    // Create VAO required in Core Profile
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create vertex buffer on GPU
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, coordinates, GL_STATIC_DRAW);

    // Set up vertex attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

    // Create index buffer on GPU
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);

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

    do
    {
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

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        
        // Draw elements
        GLClearError();
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr);
        assert(GLLogCall());

        if (redChannel > 1.0f)
            increment = -0.05f;
        else if (redChannel < 0.0f)
            increment = 0.05f;

        redChannel += increment;

        window.display();

    } while (running);

    glDeleteProgram(shader);

    return 0;
}