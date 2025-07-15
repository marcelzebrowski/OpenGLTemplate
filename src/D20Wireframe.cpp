#include "D20Wireframe.hpp"
#include "GLErrorCheck.hpp"
#include <set>
#include <array>

D20Wireframe::D20Wireframe(Shader* shader):shader(shader){
    generateGeometry();
    setupMesh();
}

D20Wireframe::~D20Wireframe(){
    glDeleteBuffers(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}

void D20Wireframe::setupMesh(){
    // create vertex and index buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    // bin data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // bind indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // linking vertex attributes
    //                   index, size,  type,   normalized,  stride,          offset
    // position
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// opengl redbook
void D20Wireframe::generateGeometry() {
    float PHI = (1.0f + std::sqrt(5.0f)) / 2.0f;

    vertices = {
        {-1,  PHI,  0}, { 1,  PHI,  0}, {-1, -PHI,  0}, { 1, -PHI,  0},
        { 0, -1,  PHI}, { 0,  1,  PHI}, { 0, -1, -PHI}, { 0,  1, -PHI},
        { PHI,  0, -1}, { PHI,  0,  1}, {-PHI,  0, -1}, {-PHI,  0,  1}
    };

    // normalize all vertices to project them onto the unit sphere
    for (auto& v : vertices)
        v = glm::normalize(v);

    std::vector<std::array<int,3>> faces = {
        {0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
        {1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
        {3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
        {4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}
    };

    std::set<std::pair<int,int>> edgeSet;
    for(const auto& face : faces){
        for(int i = 0; i < 3; ++i){
            int a = face[i];
            int b = face[(i + 1) % 3];
            if(a > b) std::swap(a, b);
            edgeSet.emplace(a, b);
        }
    }

    indices.clear();
    for (const auto& e : edgeSet) {
        indices.push_back(e.first);
        indices.push_back(e.second);
    }
}


void D20Wireframe::update(float delta, float alpha, const glm::mat4* projection, const glm::mat4* view, const glm::mat4* model){
    this->alpha = alpha;
    this->model = model;
    this->projection = projection;
    this->view = view;
}

void D20Wireframe::render(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader->attach();
        GL(glBindVertexArray(VAO));
        shader->setMat4("view",*view);
        shader->setMat4("model",*model);
        shader->setMat4("projection",*projection);
        shader->setFloat("alpha",alpha);
        
        glDrawElements(GL_LINES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    shader->detach();
    glDisable(GL_BLEND);
}