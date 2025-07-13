#ifndef TEXT_HPP
#define TEXT_HPP
#include "Shader.hpp"
#include "Texture.hpp"

struct Glyph {
    char sign;
    int x;
    int y;
    int width;
    int height;
    int ascii;
};

class Text {

private:

    Glyph asciiTable[128];

    unsigned int VAO, VBO, EBO;
    Shader* shader;
    Texture* texture;
    glm::mat4* projection;
    glm::vec2* startPos;
    char currentLetter;
    float scale;

    void initGlyphs();
    void setupMesh();

public:

    Text(Shader* shader, Texture* texture);
    ~Text();
    void render();
    void updateVBO(const Glyph* glypgh);

    void update(char c, glm::vec2* startPos, float scale, glm::mat4* projection);

    Glyph& getGlyph(char c);
};
#endif