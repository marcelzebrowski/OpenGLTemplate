#include "PictureAnimator.hpp"

PictureAnimator::PictureAnimator(float originalWidth, float originalHeight, 
    float durationSeconds, PictureFadeController* pictureFadeController, AnimationType animationType):originalWidth(originalWidth), originalHeight(originalHeight), 
    durationSeconds(durationSeconds), elapsed(0.0f), pictureFadeController(pictureFadeController), animationType(animationType), offset(glm::vec2(0.0f,0.0f)),animationDirection(AnimationDirection::IN){}

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
    return 1 + c3 * pow(t - 1, 3) + (double) c1 * pow(t - 1, 2);
}

glm::vec2 PictureAnimator::swing(float t){
    float x = sin(t*glm::two_pi<float>()) * (scaledWidth * 0.06f);
    float y = (scaledHeight * 0.8f) + abs(sin(t*glm::two_pi<float>() * 0.2f)) * (scaledWidth * 0.05f);
    return glm::vec2(x,y);
}

void PictureAnimator::start(){
    elapsed = 0.0f;
    animationDirection = AnimationDirection::IN;
}

void PictureAnimator::startExit(){
    elapsed = 0.0f;
    animationDirection = AnimationDirection::OUT;
}

void PictureAnimator::update(float delta, glm::mat4* projection){
    this->projection = projection;
    elapsed += delta;

    float t = elapsed / durationSeconds;

    switch(animationType){
        case AnimationType::EaseOutBack:{
            float x;
            if(animationDirection == AnimationDirection::IN){
                x = glm::mix(-scaledWidth, 0.0f, easeOutBack(glm::clamp(t, 0.0f,1.0f)));   
            } 
            if(animationDirection == AnimationDirection::OUT){
                x = glm::mix(0.0f, -scaledWidth, easeOutBack(glm::clamp(t, 0.0f,1.0f)));
            }
            offset = glm::vec2(x,0.0f);
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

    if(animationDirection == AnimationDirection::IN){
        pictureFadeController->update(true,delta, elapsed, projection, &view, &model);
    }else{
        pictureFadeController->update(false,delta, elapsed, projection, &view, &model);
    }
}

void PictureAnimator::render(){
    pictureFadeController->render();
}


float PictureAnimator::getDurationSeconds() const{
    return durationSeconds;
}