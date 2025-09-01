#ifndef D20WIREFRAMEANIMATOR_HPP
#define D20WIREFRAMEANIMATOR_HPP

#include "objects/D20Wireframe.hpp"

class D20WireframeAnimator {
public:
    D20WireframeAnimator(D20Wireframe* d20Wireframe, float aspect);
    ~D20WireframeAnimator();
    void render();
    void update(float delta, const glm::mat4* projection);

private:
    D20Wireframe* d20Wireframe;
    glm::mat4* projection = nullptr;
    glm::mat4 model;
    glm::mat4 view;
    float alpha;
    float aspect;
};


#endif



