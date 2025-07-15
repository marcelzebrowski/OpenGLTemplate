#ifndef MARCELSTIMER_H
#define MARCELSTIMER_H

#include <iostream>
#include <chrono>

using namespace std::chrono;

class MarcelsTimer {
public:
    MarcelsTimer() {
        initialize();
    };
    ~MarcelsTimer() = default;

    // Berechnet die verstrichene Zeit seit dem letzten Frame in Sekunden
    double delta() {
        auto currentFrame = Clock::now();
        std::chrono::duration<float> frameDelta = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float rawDelta = frameDelta.count();

        if(firstFrame){
            firstFrame = false;
            lastDelta = targetFrameTime;
            return lastDelta;
        }

        if(rawDelta > maxDelta) rawDelta = maxDelta;
        if(rawDelta < minDelta) rawDelta = minDelta;

        lastDelta = smooting * lastDelta + (1.0f - smooting) * rawDelta;
       
        accumulatedTime += lastDelta;
        frameCount++;
        return lastDelta;
    }

    // Berechnet die FPS (Frames pro Sekunde)
    double fps() {
        if (accumulatedTime >= 1.0) { // Wenn eine Sekunde vergangen ist
            currentFPS = frameCount / accumulatedTime;
            accumulatedTime = 0.0;
            frameCount = 0;
        }
        return currentFPS;
    }

    // Gibt FPS und Delta aus, ohne die Schleifenlogik zu stören
    void printStats() {
        std::cout << "delta: " << lastDelta << " sec\n";
    }

private:
    double lastDelta = 0.0;                      // Letzte Delta-Zeit
    double accumulatedTime = 0.0;                // Akkumulierte Zeit
    int frameCount = 0;                          // Anzahl der Frames in der letzten Sekunde
    double currentFPS = 0.0;                     // Berechnete FPS für die aktuelle Sekunde
    bool firstFrame = true;
    using Clock = std::chrono::steady_clock;
    Clock::time_point lastFrame;

    const float minDelta = 1.0f / 240.0f;
    const float maxDelta = 0.05f;
    const float targetFrameTime = 1.0f / 60.0f;
    const float smooting = 0.9f;

    void initialize() {
        lastFrame = Clock::now();
    }
};

#endif