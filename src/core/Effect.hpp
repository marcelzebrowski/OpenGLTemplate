#ifndef EFFECT_HPP
#define EFFECT_HPP
#include <glm/glm.hpp>
class Effect {
protected:
    float startTime = 0.0f;
    float duration = -1.0f; // infinitely
    float elapsed = 0.0f; // how long the scene has been going on
    bool started = false;
    bool finished = false;
    float alpha = 1.0f;
    glm::mat4 projection, model, view;
protected:
    virtual void onUpdate(float deltaTime, float elapsed) = 0; // abstract
    virtual void onRender() = 0;
public:
    Effect(float startTime, float duration);
    virtual ~Effect() = default;
    virtual void update(float sceneTime, float deltaTime);
    virtual void render();

    virtual bool isFinished() const;

    void setAlpha(float a);
    float getAlpha() const;
    virtual void setMatrices(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
    virtual void setProjectionMatrix(const glm::mat4& projection);
};
#endif