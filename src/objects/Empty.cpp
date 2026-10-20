#include "objects/Empty.hpp"

Empty::Empty(){
}

Empty::~Empty(){
}

void Empty::update(float delta){
}

void Empty::render(){
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}