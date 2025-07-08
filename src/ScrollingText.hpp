#ifndef SCROLLINGTEXT_HPP
#define SCROLLINGTEXT_HPP

#include <string>
#include "Text.hpp"

class ScrollingText {
    public:
        ScrollingText(Text* textRenderer, const std::string& content, float screenWidth);

        void update(float delta);
        void render();

        void setSpeed(float pixelPerSeconds);
        void setAmplitude(float amplitudePixel);
        void setFrequency(float wavesPerScreen);
        void setStartDelay(float seconds);
        void setBasePosition(glm::vec2 pos);
        void setScale(float scale);

    private:
        Text* textRenderer;
        std::string content;

        float screenWidth;
        float elapsedTime = 0.0f;

        float speed = 100.0f;
        float amplitude = 30.0f;
        float frequency = 1.0f;
        float startDelay = 5.0f; // 0 not allowed!
        float scale = 1.0f;

        glm::vec2 basePosition = glm::vec2(0.0f,0.0f);
};


#endif