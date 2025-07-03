#include "PictureAnimator.hpp"

PictureAnimator::PictureAnimator(float originalWidth, float originalHeight, 
    float durationSeconds, PictureFadeController* pictureFadeController, AnimationType animationType):originalWidth(originalWidth), originalHeight(originalHeight), 
    durationSeconds(durationSeconds), elapsed(0.0f), pictureFadeController(pictureFadeController), animationType(animationType){}

void PictureAnimator::setTargetSize(float screenWidth, float screenHeight){
    maxHeight = screenHeight;
    maxWidth = screenWidth;

    float scale = maxHeight / originalHeight;

	scaledWidth = originalWidth * scale;
	scaledHeight = originalHeight * scale;
}

float PictureAnimator::easeOutBack(float t) {
    float c1 = 1.70158f;
    float c3 = c1 + 1.0f;
    return 1 + c3 * pow(t - 1, 3) + c1 * pow(t - 1, 2);
}

glm::vec2 PictureAnimator::swing(float t){
    float x = sin(t*glm::two_pi<float>()) * (scaledWidth * 0.06f);
    float y = (scaledHeight * 0.8f) + abs(sin(t*glm::two_pi<float>() * 0.2f)) * (scaledWidth * 0.05f);
    return glm::vec2(x,y);
}

void PictureAnimator::start(){
    elapsed = 0.0f;
}

void PictureAnimator::render(float delta){
    elapsed += delta;

    float t = elapsed / durationSeconds;
    glm::vec2 offset;
    switch(animationType){
        case AnimationType::EaseOutBack:{
            float x = glm::mix(-scaledWidth, 0.0f, easeOutBack(glm::clamp(t, 0.0f,1.0f)));
            offset = glm::vec2(x + 100.0f,0.0f);
            break;
        }
        case AnimationType::Swing:{
            float looped = elapsed;
            offset = swing(looped);
            break;
        }
    }


    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(offset.x, offset.y, 0.0f));

    model = glm::scale(model, glm::vec3(scaledWidth, scaledHeight, 1.0f));

    pictureFadeController->render(delta, elapsed, view, model);
}
