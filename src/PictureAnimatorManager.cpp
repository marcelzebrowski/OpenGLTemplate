#include "PictureAnimatorManager.hpp"

PictureAnimatorManager::PictureAnimatorManager(std::vector<PictureAnimator> animators, 
    float displayTimePerImage):animators(animators),displayTime(displayTimePerImage){
        this->timeAccumulator = 0.0f;
        this->currentIndex = 0;
        this->phase = AnimationPhase::IN;
}

void PictureAnimatorManager::update(float delta, glm::mat4* projection){
    
    timeAccumulator += delta;

    PictureAnimator& current = animators[currentIndex];

    switch(phase){
        case AnimationPhase::IN:{
            current.update(delta,projection);

            if(timeAccumulator >= current.getDurationSeconds()){
                phase = AnimationPhase::HOLD;
                timeAccumulator = 0.0f;
            }

            break;
        }
    
        case AnimationPhase::HOLD:{
            current.update(delta, projection);

            if(timeAccumulator >= displayTime){
                phase = AnimationPhase::OUT;
                timeAccumulator = 0.0f;
                current.startExit();
            }

            break;
        }

        case AnimationPhase::OUT:{
            current.update(delta, projection);
         

            if(timeAccumulator >= current.getDurationSeconds()){
                currentIndex = (currentIndex + 1) % animators.size();
                current.start();
                phase = AnimationPhase::IN;
                timeAccumulator = 0.0f;
            }

            break;
        }
    }


}

void PictureAnimatorManager::render(){
    PictureAnimator& current = animators[currentIndex];
    current.render();
}