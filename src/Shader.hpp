#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
    unsigned int ID;
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void attach();
    void detach();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloat3(const std::string& name, float value1, float value2, float value3) const;

    void setMat4(const std::string& name, glm::mat4& matrix) const;

private:
    unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath);
    unsigned int createShader(const char* program, GLenum shaderType);
    const char* loadShaderFromFile(const std::string& filename);
};


#endif
