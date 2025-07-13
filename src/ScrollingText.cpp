#include "ScrollingText.hpp"
#include <cmath>
#include <numbers>

ScrollingText::ScrollingText(Text* textRenderer, const std::string& content, float screenWidth, float screenHeight): textRenderer(textRenderer), content(content), screenWidth(screenWidth), screenHeight(screenHeight){}

void ScrollingText::update(float delta, glm::mat4* projection){
    elapsedTime += delta;
    this->projection = projection;

    setContent(content);

}

void ScrollingText::render(){
    if(elapsedTime < startDelay){
        return;
    }

    float scrollDuration = (screenWidth + totalTextWidth) / speed;

    float scrollTime = fmod(elapsedTime-startDelay, scrollDuration);

    float baseX = screenWidth - (scrollTime * speed);
    float x = baseX;

    for(size_t i = 0; i < content.size(); ++i){
        char c = content[i];

        if(c < 32 || c > 126){
            continue;
        }

        const Glyph& glyph = textRenderer->getGlyph(c);

        float glyphWidth = (float)glyph.width;


        if(x + glyphWidth < 0.0f || x > screenWidth){
            x += glyphWidth;
            continue;
        }

        float waveX = cos((x/screenHeight) * frequency * 2.0f * std::numbers::pi_v<float>) * amplitude;
        float waveY = sin(elapsedTime * 0.5f) * 300.0f;

        float y = basePosition.y + waveX + waveY;

        glm::vec2 startPos = glm::vec2(x,y);

        textRenderer->update(c,&startPos,scale, projection);
        textRenderer->render();

        x += glyphWidth;
    }
}

void ScrollingText::setSpeed(float pixelPerSeconds){
    speed = pixelPerSeconds;    
}

void ScrollingText::setAmplitude(float amplitudePixel){
    amplitude = amplitudePixel;
}

void ScrollingText::setFrequency(float wavesPerScreen){
    frequency = wavesPerScreen;
}

void ScrollingText::setStartDelay(float seconds) {
    startDelay = seconds;
}

void ScrollingText::setBasePosition(glm::vec2 pos){
    basePosition = pos;
}

void ScrollingText::setScale(float scale){
    this->scale = scale;
}

void ScrollingText::setContent(const std::string& content){
    this->content = content;
    totalTextWidth = 0.0f;

    for(char c : content){
        if(c < 32 || c > 126) continue;

        totalTextWidth += textRenderer->getGlyph(c).width;
    }

}