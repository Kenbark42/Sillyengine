#include "Engine/Renderer/OpenGLShader.hpp"
#include "Engine/Core/Logger.hpp"

#include <glad/glad.h>

namespace Engine
{

    OpenGLShader::OpenGLShader(const std::string &name)
        : Shader(name), programId(0)
    {
    }

    OpenGLShader::~OpenGLShader()
    {
        if (programId)
        {
            glDeleteProgram(programId);
        }
    }

    bool OpenGLShader::compile(const std::string &vertexSource, const std::string &fragmentSource)
    {
        // Create vertex shader
        uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char *vertexSourcePtr = vertexSource.c_str();
        glShaderSource(vertexShader, 1, &vertexSourcePtr, nullptr);
        glCompileShader(vertexShader);

        // Check vertex shader compilation errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            Logger::error("Vertex shader compilation failed: " + std::string(infoLog));
            return false;
        }

        // Create fragment shader
        uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fragmentSourcePtr = fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentSourcePtr, nullptr);
        glCompileShader(fragmentShader);

        // Check fragment shader compilation errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            Logger::error("Fragment shader compilation failed: " + std::string(infoLog));
            return false;
        }

        // Create and link shader program
        programId = glCreateProgram();
        glAttachShader(programId, vertexShader);
        glAttachShader(programId, fragmentShader);
        glLinkProgram(programId);

        // Check linking errors
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programId, 512, nullptr, infoLog);
            Logger::error("Shader program linking failed: " + std::string(infoLog));
            return false;
        }

        // Delete shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        Logger::info("Shader '" + name + "' compiled successfully");
        return true;
    }

    void OpenGLShader::bind() const
    {
        glUseProgram(programId);
    }

    void OpenGLShader::unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::setInt(const std::string &name, int value)
    {
        glUniform1i(getUniformLocation(name), value);
    }

    void OpenGLShader::setFloat(const std::string &name, float value)
    {
        glUniform1f(getUniformLocation(name), value);
    }

    void OpenGLShader::setVector2(const std::string &name, const Vector2 &value)
    {
        glUniform2f(getUniformLocation(name), value.x, value.y);
    }

    void OpenGLShader::setVector3(const std::string &name, const Vector3 &value)
    {
        glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
    }

    void OpenGLShader::setVector4(const std::string &name, const Vector4 &value)
    {
        glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::setMatrix3(const std::string &name, const Matrix3 &value)
    {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, value.getData());
    }

    void OpenGLShader::setMatrix4(const std::string &name, const Matrix4 &value)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value.getData());
    }

    int OpenGLShader::getUniformLocation(const std::string &name)
    {
        // Check if location is already cached
        auto it = uniformLocationCache.find(name);
        if (it != uniformLocationCache.end())
        {
            return it->second;
        }

        // Get location from OpenGL
        int location = glGetUniformLocation(programId, name.c_str());
        if (location == -1)
        {
            Logger::warning("Uniform '" + name + "' not found in shader '" + this->name + "'");
        }

        // Cache location
        uniformLocationCache[name] = location;
        return location;
    }

} // namespace Engine