
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "Shader.h"

namespace mg
{
	Shader::Shader(const std::string& shaderPath)
		: path(shaderPath), id(0)
	{
        // Parse shader code
        ShaderSource source = parseShader(shaderPath);
        id = createShader(source.vertexSource, source.fragmentSource);
	}

	Shader::~Shader()
	{
        glDeleteProgram(id);
	}

	void Shader::bind() const
	{
        glUseProgram(id);
	}

	void Shader::unbind() const
	{
        glUseProgram(0);
	}

	void Shader::setUniform4f(const std::string& name, glm::vec4 vec)
	{
        glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
	}

    void Shader::setUniform1i(const std::string& name, int value)
    {
        glUniform1i(getUniformLocation(name), value);
    }

    void Shader::setUniformMat4f(const std::string& name, glm::mat4& mat)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, false, &mat[0][0]);
    }

    int Shader::getUniformLocation(const std::string& name)
    {
        if (uniformLocationCache.find(name) != uniformLocationCache.end())
            return uniformLocationCache[name];

        int location = glGetUniformLocation(id, name.c_str());

        if (location == -1)
            std::cout << "Warning: Uniform " << name << " doesn't exist!" << std::endl;

        uniformLocationCache[name] = location;

        return location;
    }

    ShaderSource Shader::parseShader(const std::string& path)
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
                if (type != ShaderType::NONE)
                    ss[(int)type] << line << '\n';
            }
        }

        return { ss[0].str(), ss[1].str() };
    }

    unsigned int Shader::compileShader(unsigned int type, const std::string& source)
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();

        glShaderSource(id, 1, &src, nullptr);
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

	unsigned int Shader::createShader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
	{
		unsigned int program = glCreateProgram();

		unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderCode);
		unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return program;
	}
}