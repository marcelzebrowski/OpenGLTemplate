#include "ScrollingText.hpp"
#include <cmath>

ScrollingText::ScrollingText(Text* textRenderer, const std::string& content, float screenWidth, float screenHeight): textRenderer(textRenderer), content(content), screenWidth(screenWidth), screenHeight(screenHeight){}

void ScrollingText::update(float delta, glm::mat4* projection){
    elapsedTime += delta;
    this->projection = projection;

}

void ScrollingText::render(){
    if(elapsedTime < startDelay){
        return;
    }

    float scrollTime = elapsedTime - startDelay;
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

        float y = basePosition.y + cos((x/screenHeight) * frequency * 2.0f * 3.14f) * amplitude; // - 17 ?? im ersten Durchlauf
    
        glm::vec2 startPos = glm::vec2(x,y); // x ist bei 1023 ??

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