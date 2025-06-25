#ifndef PICTUREFADECONTROLLER_HPP
#define PICTUREFADECONTROLLER_HPP
#include "Picture.hpp"
class PictureFadeController{
private:
    Picture* picture;
    float alpha;
    float fadeSpeed;
public:
    PictureFadeController(Picture* picture);
    ~PictureFadeController();
    void render(float delta, glm::mat4 view, glm::mat4 model);
};

#endif