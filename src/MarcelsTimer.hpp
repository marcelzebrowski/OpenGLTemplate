#ifndef MARCELSTIMER_H
#define MARCELSTIMER_H

#include <iostream>
#include <ctime>
#include <ratio>
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
        auto currentFrame = high_resolution_clock::now();
        std::chrono::duration<float> frameDelta = currentFrame - lastFrame;
        lastFrame = currentFrame;
        lastDelta = frameDelta.count(); // Delta-Zeit zwischenspeichern
        accumulatedTime += lastDelta;  // Zeit für FPS-Zählung akkumulieren
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
        std::cout << "Delta Time: " << lastDelta << "s, FPS: " << fps() << std::endl;
    }

private:
    high_resolution_clock::time_point lastFrame; // Zeit des letzten Frames
    double lastDelta = 0.0;                      // Letzte Delta-Zeit
    double accumulatedTime = 0.0;                // Akkumulierte Zeit
    int frameCount = 0;                          // Anzahl der Frames in der letzten Sekunde
    double currentFPS = 0.0;                     // Berechnete FPS für die aktuelle Sekunde

    void initialize() {
        lastFrame = high_resolution_clock::now();
    }
};

#endif