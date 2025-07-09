#include "ScrollingText.hpp"
#include <cmath>

ScrollingText::ScrollingText(Text* textRenderer, const std::string& content, float screenWidth): textRenderer(textRenderer), content(content), screenWidth(screenWidth){}

void ScrollingText::update(float delta){
    elapsedTime += delta;
}

void ScrollingText::render(){
    if(elapsedTime < startDelay){
        return;
    }

    float scrollTime = elapsedTime - startDelay;
    float baseX = screenWidth - (scrollTime * speed);

    for(size_t i = 0; i < content.size(); ++i){
        char c = content[i];

        if(c < 32 || c > 126){
            continue;
        }

        const Glyph& glyph = textRenderer->getGlyph(c);

        float glyphWidth = (glyph.width / 1800.0f) * scale;
        float x = baseX;

        for(size_t j = 0; j < i; ++j){
            char pc = content[j];
            if(pc < 32 || pc > 126){
                continue;
            }
            x -= (textRenderer->getGlyph(pc).width / 1800.0f) * scale;
        }

        if(x + glyphWidth < 0.0f || x > screenWidth){
            continue;
        }

        float y = basePosition.y + sinf((x/screenWidth) * frequency * 2.0f * 3.14f) * amplitude;
        
        std::cout << "x " << x << " y " << y << std::endl;
        textRenderer->render(c, glm::vec2(x,y),scale);
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