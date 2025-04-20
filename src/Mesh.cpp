#include "Mesh.hpp"


namespace MeshRenderer{
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

   
        setupMesh();
    
        
    }

    Mesh::~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Mesh::setupMesh(){
        // create vertex and index buffer
        glGenVertexArrays(1,&VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
	
	    // bind data
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // bind indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        
        // -- linking vertex attributes (interpret the vertex data)

        //                   index, size,  type,   normalized,  stride,          offset
        // Vertex-Attribute definieren
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Position
        

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal)); // Normal
        

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texCoords)); // Texturkoordinaten
        

        if (!glIsVertexArray(VAO)) {
            std::cerr << "[Fehler] VAO " << VAO << " ist kein gültiges Vertex Array!" << std::endl;
        }


        // -- unbind
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Mesh::Draw(Shader* shader){
        
    
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;

        for (size_t i = 0; i < textures.size(); i++) {

             // todo hier die texturen auslesen anhand des namen
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = textures[i].type;

            if (name == "texture_diffuse") {
                number = std::to_string(diffuseNr++);
            }
            else if (name == "texture_specular") {
                number = std::to_string(specularNr++);
            }

            shader->setFloat(("material." + name + number).c_str(), i);
            if (glIsTexture(textures[i].id)) {
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            } else {
                std::cerr << "Invalid texture id: " << textures[i].id << std::endl;
            }
        }

      

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
        

    


    }
}