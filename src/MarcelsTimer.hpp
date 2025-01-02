#ifndef MARCELSTIMER_H
#define MARCELSTIMER_H

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>

using namespace std::chrono;

class MarcelsTimer {

public:
	MarcelsTimer(){
        inititialize();
    };
	~MarcelsTimer() = default;

	double delta() {
		high_resolution_clock::time_point currentTime = high_resolution_clock::now();
		std::chrono::duration<float> delta = currentTime - lastTime;
        lastTime = currentTime;
        return delta.count();
	}

private:
	high_resolution_clock::time_point lastTime;
    
	void inititialize() {
		lastTime = high_resolution_clock::now();
	}
};

#endif