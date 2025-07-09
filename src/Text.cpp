#include "Text.hpp"
#include <iostream>
#include "GLErrorCheck.hpp"

static float vertices[] = {
    // x, y, u, v
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  0.0f, 1.0f,
};

static unsigned int indices[]= {
    0,1,2, // first triangle
    0,2,3  // second triangle
};

Text::Text(Shader* shader, Texture* texture):shader(shader),texture(texture){
    initGlyphs();
    setupMesh();
}

Text::~Text(){
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}

void Text::render(const std::string& text, glm::vec2 startPos, float scale){
	
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader->attach();
        texture->attach(shader,"texture");

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);

        GL(glBindVertexArray(VAO));


        float xCursor = startPos.x;

        for(char c: text){
            int letterCount = 12;
            int ascii = (int)c;

            if(ascii < 32 || ascii > 126){
                continue;
            }

            const Glyph& character = asciiTable[ascii];
 
            int index = ascii - 32;

            int column = index % letterCount; // 12 = count
            int row = index / letterCount;
        
            model = glm::translate(model,glm::vec3(xCursor, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(scale));

            shader->setMat4("model", model);
            shader->setMat4("view",view);

            updateVBO(&character);
            
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            
            xCursor = character.width / 3000.0f;
        }


        glBindVertexArray(0);
        texture->detach();
    shader->detach();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void Text::render(char c, glm::vec2 startPos, float scale){

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader->attach();
        const Glyph& glyph = getGlyph(c);
        texture->attach(shader,"texture");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(startPos,0.0f));
        model = glm::scale(model, glm::vec3(scale));
        glm::mat4 view = glm::mat4(1.0f);

        GL(glBindVertexArray(VAO));


        shader->setMat4("model", model);
        shader->setMat4("view",view);

        updateVBO(&glyph);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        
        glBindVertexArray(0);
        texture->detach();
    shader->detach();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void Text::updateVBO(const Glyph* glyph){

    float uPx = (float) glyph->x;
    float vPx = (float) glyph->y;
    float heigth = (float) glyph->height;
    float width = (float) glyph->width;


    const float TEXTURE_WIDTH = 1800.0f;
    const float TEXTURE_HEIGHT = 1800.0f;

    const float u = uPx / TEXTURE_WIDTH;
    const float v = 1.0f - (vPx + heigth) / TEXTURE_HEIGHT;

    const float glyphW = width / TEXTURE_WIDTH;
    const float glyphH = heigth / TEXTURE_HEIGHT;
    float newVertices[4*4] = {
        // x, y, u, v
        0.0f, 0.0f,  u, v,
        glyphW, 0.0f,  u + glyphW, v,
        glyphW,  glyphH,  u + glyphW, v + glyphH,
        0.0f,  glyphH,  u, v + glyphH,
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newVertices),newVertices);

}

void Text::setupMesh(){

    // create vertex and index buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    // bin data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // linking vertex attributes
    //                   index, size,  type,   normalized,  stride,          offset
    // position
    glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // uv
    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2* sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Text::initGlyphs(){
                        //     x    y  width height
    asciiTable[32]  = {' ' ,    0,    0, 150, 200,  32};
    asciiTable[33]  = {'!' ,  150,    0, 150, 200,  33};
    asciiTable[34]  = {'"' ,  300,    0, 150, 200,  34};
    asciiTable[35]  = {'#' ,  450,    0, 150, 200,  35};
    asciiTable[36]  = {'$' ,  600,    0, 150, 200,  36};
    asciiTable[37]  = {'%' ,  750,    0, 150, 200,  37};
    asciiTable[38]  = {'&' ,  900,    0, 150, 200,  38};
    asciiTable[39]  = {'\'', 1050,    0, 150, 200,  39};
    asciiTable[40]  = {'(' , 1200,    0, 150, 200,  40};
    asciiTable[41]  = {')' , 1350,    0, 150, 200,  41};
    asciiTable[42]  = {'*' , 1500,    0, 150, 200,  42};
    asciiTable[43]  = {'+' , 1650,    0, 150, 200,  43};

    asciiTable[44]  = {',' ,    0,  200, 150, 200,  44};
    asciiTable[45]  = {'-' ,  150,  200, 150, 200,  45};
    asciiTable[46]  = {'.' ,  300,  200, 150, 200,  46};
    asciiTable[47]  = {'/' ,  450,  200, 150, 200,  47};
    asciiTable[48]  = {'0' ,  600,  200, 150, 200,  48};
    asciiTable[49]  = {'1' ,  750,  200, 150, 200,  49};
    asciiTable[50]  = {'2' ,  900,  200, 150, 200,  50};
    asciiTable[51]  = {'3' , 1050,  200, 150, 200,  51};
    asciiTable[52]  = {'4' , 1200,  200, 150, 200,  52};
    asciiTable[53]  = {'5' , 1350,  200, 150, 200,  53};
    asciiTable[54]  = {'6' , 1500,  200, 150, 200,  54};
    asciiTable[55]  = {'7' , 1650,  200, 150, 200,  55};

    asciiTable[56]  = {'8' ,    0,  400, 150, 200,  56};
    asciiTable[57]  = {'9' ,  150,  400, 150, 200,  57};
    asciiTable[58]  = {':' ,  300,  400, 150, 200,  58};
    asciiTable[59]  = {';' ,  450,  400, 150, 200,  59};
    asciiTable[60]  = {'<' ,  600,  400, 150, 200,  60};
    asciiTable[61]  = {'=' ,  750,  400, 150, 200,  61};
    asciiTable[62]  = {'>' ,  900,  400, 150, 200,  62};
    asciiTable[63]  = {'?' , 1050,  400, 150, 200,  63};
    asciiTable[64]  = {'@' , 1200,  400, 150, 200,  64};
    asciiTable[65]  = {'A' , 1350,  400, 150, 200,  65};
    asciiTable[66]  = {'B' , 1500,  400, 150, 200,  66};
    asciiTable[67]  = {'C' , 1650,  400, 180, 200,  67};

    asciiTable[68]  = {'D' ,    0,  600, 150, 200,  68};
    asciiTable[69]  = {'E' ,  150,  600, 180, 200,  69};
    asciiTable[70]  = {'F' ,  300,  600, 110, 200,  70};
    asciiTable[71]  = {'G' ,  450,  600, 150, 200,  71};
    asciiTable[72]  = {'H' ,  600,  600, 100, 200,  72};
    asciiTable[73]  = {'I' ,  750,  600, 100, 200,  73};
    asciiTable[74]  = {'J' ,  900,  600, 150, 200,  74};
    asciiTable[75]  = {'K' , 1050,  600, 150, 200,  75};
    asciiTable[76]  = {'L' , 1200,  600, 150, 200,  76};
    asciiTable[77]  = {'M' , 1350,  600, 150, 200,  77};
    asciiTable[78]  = {'N' , 1500,  600, 130, 200,  78};
    asciiTable[79]  = {'O' , 1650,  600, 150, 200,  79};

    asciiTable[80]  = {'P' ,    0,  800, 150, 200,  80};
    asciiTable[81]  = {'Q' ,  150,  800, 150, 200,  81};
    asciiTable[82]  = {'R' ,  300,  800, 100, 200,  82};
    asciiTable[83]  = {'S' ,  450,  800, 150, 200,  83};
    asciiTable[84]  = {'T' ,  600,  800, 190, 200,  84};
    asciiTable[85]  = {'U' ,  750,  800, 120, 200,  85};
    asciiTable[86]  = {'V' ,  900,  800, 150, 200,  86};
    asciiTable[87]  = {'W' , 1050,  800, 150, 200,  87};
    asciiTable[88]  = {'X' , 1200,  800, 150, 200,  88};
    asciiTable[89]  = {'Y' , 1350,  800, 150, 200,  89};
    asciiTable[90]  = {'Z' , 1500,  800, 150, 200,  90};
    asciiTable[91]  = {'[' , 1650,  800, 150, 200,  91};

    asciiTable[92]  = {'\\',    0, 1000, 150, 200,  92};
    asciiTable[93]  = {']' ,  150, 1000, 150, 200,  93};
    asciiTable[94]  = {'^' ,  300, 1000, 150, 200,  94};
    asciiTable[95]  = {'_' ,  450, 1000, 150, 200,  95};
    asciiTable[96]  = {'`' ,  600, 1000, 150, 200,  96};
    asciiTable[97]  = {'a' ,  750, 1000, 150, 200,  97};
    asciiTable[98]  = {'b' ,  900, 1000, 150, 200,  98};
    asciiTable[99]  = {'c' , 1050, 1000, 130, 200,  99};
    asciiTable[100] = {'d' , 1200, 1000, 150, 200, 100};
    asciiTable[101] = {'e' , 1350, 1000, 150, 200, 101};
    asciiTable[102] = {'f' , 1500, 1000, 150, 200, 102};
    asciiTable[103] = {'g' , 1650, 1000, 150, 200, 103};

    asciiTable[104] = {'h' ,    0, 1200, 150, 200, 104};
    asciiTable[105] = {'i' ,  150, 1200, 150, 200, 105};
    asciiTable[106] = {'j' ,  300, 1200, 150, 200, 106};
    asciiTable[107] = {'k' ,  450, 1200, 100, 200, 107};
    asciiTable[108] = {'l' ,  600, 1200, 150, 200, 108};
    asciiTable[109] = {'m' ,  750, 1200, 150, 200, 109};
    asciiTable[110] = {'n' ,  900, 1200, 150, 200, 110};
    asciiTable[111] = {'o' , 1050, 1200, 100, 200, 111};
    asciiTable[112] = {'p' , 1200, 1200, 150, 200, 112};
    asciiTable[113] = {'q' , 1350, 1200, 150, 200, 113};
    asciiTable[114] = {'r' , 1500, 1200, 150, 200, 114};
    asciiTable[115] = {'s' , 1650, 1200, 100, 200, 115};

    asciiTable[116] = {'t' ,    0, 1400, 150, 200, 116};
    asciiTable[117] = {'u' ,  150, 1400, 130, 200, 117};
    asciiTable[118] = {'v' ,  300, 1400, 150, 200, 118};
    asciiTable[119] = {'w' ,  450, 1400, 150, 200, 119};
    asciiTable[120] = {'x' ,  600, 1400, 150, 200, 120};
    asciiTable[121] = {'y' ,  750, 1400, 150, 200, 121};
    asciiTable[122] = {'z' ,  900, 1400, 150, 200, 122};
    asciiTable[123] = {'{' , 1050, 1400, 150, 200, 123};
    asciiTable[124] = {'|' , 1200, 1400, 150, 200, 124};
    asciiTable[125] = {'}' , 1350, 1400, 150, 200, 125};
    asciiTable[126] = {'~' , 1650, 1400, 150, 200, 126};
}

Glyph& Text::getGlyph(char c){
    return asciiTable[(int)c];
}