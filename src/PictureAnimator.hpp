#ifndef PICTUREANIMATOR_HPP
#define PICTUREANIMATOR_HPP

#include "PictureFadeController.hpp"

class PictureAnimator {
public:
    PictureAnimator(float originalWidth, float originalHeight, float durationSeconds,
        PictureFadeController* pictureFadeController);

    void start();
    void render(float delta);

    void setTargetSize(float screenWidth, float screenHeight);

private:
    float originalWidth;
    float originalHeight;
    float scaledWidth;
    float scaledHeight;

    float durationSeconds;
    float elapsed;

    float maxHeight;
    float maxWidth;

    glm::mat4 model;
    glm::mat4 view;

    PictureFadeController* pictureFadeController;


    float easeOutBack(float t);
};

#endif
