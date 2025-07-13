#ifndef PICTUREFADECONTROLLER_HPP
#define PICTUREFADECONTROLLER_HPP
#include "Picture.hpp"
class PictureFadeController{
private:
    Picture* picture;
    float alpha;
    float fadeSpeed;
    glm::mat4* projection;
    glm::mat4* view;
    glm::mat4* model;
    float elapsed;

public:
    PictureFadeController(Picture* picture);
    ~PictureFadeController();
    void render();
    void update(bool fadeIn, float delta, float elapsed, glm::mat4* projection, glm::mat4* view, glm::mat4* model);
};

#endif