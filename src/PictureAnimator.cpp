#include "PictureAnimator.hpp"

PictureAnimator::PictureAnimator(float originalWidth, float originalHeight, 
    float durationSeconds, PictureFadeController* pictureFadeController):originalWidth(originalWidth), originalHeight(originalHeight), 
    durationSeconds(durationSeconds), elapsed(0.0f), pictureFadeController(pictureFadeController){}

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


void PictureAnimator::start(){
    elapsed = 0.0f;
}

void PictureAnimator::render(float delta){
    elapsed += delta;
    float t = glm::clamp(elapsed / durationSeconds, 0.0f, 1.0f);
    float x = glm::mix(-scaledWidth, 0.0f, easeOutBack(t));

    view = glm::mat4(1.0f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, 0.0f, 0.0f));

    model = glm::scale(model, glm::vec3(scaledWidth, scaledHeight, 1.0f));

    pictureFadeController->render(delta, view, model);
}
