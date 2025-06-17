#include "Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath, bool orthogonal):orthogonalProjection(orthogonal){
    // -- create vertex shader
	const char* vertexShaderSource = loadShaderFromFile(vertexPath);

	// -- create fragment shader
	const char* fragmentShaderSource = loadShaderFromFile(fragmentPath);

    ID = createShaderProgram(vertexShaderSource, fragmentShaderSource);
}


Shader::~Shader(){
    glDeleteProgram(ID);
}

void Shader::attach(){
    glUseProgram(ID);
}

void Shader::detach(){
    glUseProgram(0);
}

void Shader::setBool(const std::string& name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat2(const std::string& name, float value1, float value2) const{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value1 , value2);
}

void Shader::setFloat3(const std::string& name, float value1, float value2, float value3) const{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}

void Shader::setFloat3(const std::string& name, glm::vec3 vec) const{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setMat4(const std::string& name, glm::mat4& matrix) const{
    unsigned int transformLoc = glGetUniformLocation(ID,name.c_str());
    glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(matrix));
}

unsigned int Shader::createShaderProgram(const char* vertexPath, const char* fragmentPath){
    unsigned int vertexShader = createShader(vertexPath, GL_VERTEX_SHADER);
	unsigned int fragmentShader = createShader(fragmentPath, GL_FRAGMENT_SHADER);
	
	int success;
	char infoLog[512];
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::CREATION_FAILED\n" << infoLog << std::endl;
	}

	// delete shader we no longer need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

unsigned int Shader::createShader(const char* program, GLenum shaderType){
    unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &program, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
}

const char* Shader::loadShaderFromFile(const std::string& filename){
    try {
        // Datei öffnen
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }

        // Dateiinhalt in einen Stringstream lesen
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        // Dateiinhalt in einen std::string speichern
        std::string shaderCode = buffer.str();

        // Dynamisch Speicher für const char* zuweisen
        char* shaderSource = new char[shaderCode.size() + 1];
        std::copy(shaderCode.begin(), shaderCode.end(), shaderSource);
        shaderSource[shaderCode.size()] = '\0'; // Null-terminieren

        return shaderSource;
    } catch (const std::exception& e) {
        std::cerr << "Error reading shader file: " << e.what() << std::endl;
        return nullptr;
    }
}
