#ifndef SCROLLINGTEXT_HPP
#define SCROLLINGTEXT_HPP

#include <string>
#include "objects/Text.hpp"

class ScrollingText {
    public:
        ScrollingText(Text* textRenderer, const std::string& content, float screenWidth, float screenHeight);

        void update(float delta, glm::mat4* projection);
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
        float screenHeight;
        float elapsedTime = 0.0f;

        float speed = 100.0f;
        float amplitude = 30.0f;
        float frequency = 1.0f;
        float startDelay = 5.0f; // 0 not allowed!
        float scale = 1000.0f;

        glm::vec2 basePosition = glm::vec2(0.0f,0.0f);
        glm::mat4* projection;

        float totalTextWidth;

        void setContent(const std::string& content);
};


#endif