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
    void render(bool fadeIn, float delta, float elapsed, glm::mat4 view, glm::mat4 model);
};

#endif