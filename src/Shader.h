#pragma once
#include <string>

#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    /* data */
    std::string m_FilePath;
    std::string m_VertexShaderPath;
    std::string m_FragmentShaderPath;
    unsigned int m_RendererID;

    // caching for uniforms
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string& filepath);
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, glm::mat4& matrix);
private:
    int GetUniformLocation(const std::string& name);
    ShaderProgramSource ParseShader(const std::string& filepath);
    std::string LoadShaderAsString(const std::string& filepath);

    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int CompileShader(const std::string& source, unsigned int type);
};