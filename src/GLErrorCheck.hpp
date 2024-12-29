#ifndef GLErrorCheck_HPP
#define GLErrorCheck_HPP

#include <iostream>
#include <glad/glad.h>

// Fehlerprüfung Makro
#define GL(x)   \
    x;               \
    CheckGLError(#x, __FILE__, __LINE__);

// Fehlerprüfung Funktion
void CheckGLError(const char* function, const char* file, int line) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error in " << function << " (" << file << ":" << line << "): " << error << std::endl;
    }
}

#endif // GLErrorCheck_HPP