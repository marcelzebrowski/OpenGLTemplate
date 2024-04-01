#include "Shader.h"


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"



Shader::Shader(const std::string& filepath):m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource); 
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath):
    m_RendererID(0), m_VertexShaderPath(vertexShaderPath), m_FragmentShaderPath(fragmentShaderPath)
{
    m_RendererID = CreateShader(LoadShaderAsString(vertexShaderPath), LoadShaderAsString(fragmentShaderPath)); 
}

Shader::~Shader()
{
    GlCall(glDeleteProgram(m_RendererID));
}


void Shader::Bind() const{
    GlCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const{
    GlCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value){
    
    int location = GetUniformLocation(name);
    GlCall(glUniform1i(location,value));  
}


void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3){
    
    int location = GetUniformLocation(name);
    GlCall(glUniform4f(location,v0,v1,v2,v3));  
}


int Shader::GetUniformLocation(const std::string& name){

    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end() ){
        return m_UniformLocationCache[name];
    }

    int location = glGetUniformLocation(m_RendererID,name.c_str());
    
    if(location == -1){
        std::cout << "Warning: uniform '" << name << "' doesn't exists!" << std::endl;
    }
        
    m_UniformLocationCache[name] = location;
    
    return location;
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type){
    GLuint id = glCreateShader(type);
    const char* src = &source[0];
    glShaderSource(id,1,&src, nullptr); // 10:33
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if(!result){
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char* message = (char*)_malloca(lenght * sizeof(char));
        glGetShaderInfoLog(id, lenght, &lenght, message);

        std::cout << "Failed to compile "<< (type == GL_VERTEX_SHADER ? "vertex":"fragment") << " shader!" << std::endl; 
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader){

    std::cout << vertexShader.c_str() << std::endl;

    std::cout << fragmentShader.c_str() << std::endl;


    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    GlCall( glAttachShader(program, vs)) ;
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}



ShaderProgramSource Shader::ParseShader(const std::string& filepath){
    
    enum class ShaderType{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream stream(filepath);
    
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while(getline(stream,line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            }else if (line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
        }else{
            ss[(int) type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

std::string Shader::LoadShaderAsString(const std::string& filepath){
    std::ifstream stream(filepath);
    std::stringstream ss;
    while (stream.good()){
        std::string line;
        getline(stream, line);
        ss << line << '\n';
    }
    return ss.str();
}