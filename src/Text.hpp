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

    void initGlyphs();
    void setupMesh();

public:

    Text(Shader* shader, Texture* texture);
    ~Text();
    void render(const std::string& text, glm::vec2 startPos, float scale);
    void render(char c, glm::vec2 startPos, float scale);
    void updateVBO(const Glyph* glypgh);

    Glyph& getGlyph(char c);
};
#endif