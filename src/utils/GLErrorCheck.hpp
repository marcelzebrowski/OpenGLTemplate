// GLErrorCheck.hpp
#ifndef GLErrorCheck_HPP
#define GLErrorCheck_HPP

#include <glad/glad.h>
#include <iostream>

class GLHelper {
public:
    static void CheckGLError(const char* function, const char* file, int line) {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL Error in " << function << " (" << file << ":" << line << "): " << error << std::endl;
        }
    }
};

#define GL(x)   \
    x;          \
    GLHelper::CheckGLError(#x, __FILE__, __LINE__);

#endif // GLErrorCheck_HPP