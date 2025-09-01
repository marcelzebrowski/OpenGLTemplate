#include "render/D20WireframeAnimator.hpp"

D20WireframeAnimator::D20WireframeAnimator(D20Wireframe* d20Wireframe, float aspect)
    :d20Wireframe(d20Wireframe),alpha(0.0f), aspect(aspect){
}

D20WireframeAnimator::~D20WireframeAnimator(){

}


void D20WireframeAnimator::update(float delta, const glm::mat4* projection){
    if(projection && d20Wireframe){

        model = glm::mat4(1.0f);
        view = glm::mat4(1.0f);
        
        float xOffset = aspect * 1.5f;

        static float angle1 = 0.0f;
        static float angle2 = 0.0f;
        angle1 += delta / 2.5f;
        angle2 += delta / 5.0f;
        glm::vec3 start = glm::vec3(xOffset,0.0f, -5.0f); 
        model = glm::translate(model, start);
        model = glm::rotate(model, angle1, glm::vec3(1.0f,0.0f,1.0f));
        model = glm::rotate(model, angle2, glm::vec3(0.0f,1.0f,0.0f));

        if(alpha < 1.0f){
            alpha += delta/10.0f;
        }else{
            alpha = 1.0f;
        }


        d20Wireframe->update(delta, alpha, projection, &view, &model);
        

    }
}

void D20WireframeAnimator::render(){
    if(d20Wireframe){
        d20Wireframe->render();
    }
    
}