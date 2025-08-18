#ifndef EFFECT_HPP
#define EFFECT_HPP
class Effect {
protected:
    float startTime = 0.0f;
    float duration = -1.0f; // infinitely
    float elapsed = 0.0f;
    bool started = false;
    bool finished = false;
    float alpha = 1.0f;
protected:
    virtual void onUpdate(float deltaTime, float localTime) = 0;
    virtual void onRender() = 0;
public:
    Effect(float startTime, float duration);
    virtual ~Effect() = default;
    virtual void update(float sceneTime, float deltaTime);
    virtual void render();

    virtual bool isFinished() const;

    void setAlpha(float a);
    float getAlpha() const;
};
#endif