#ifndef EFFECT_HPP
#define EFFECT_HPP
class Effect {
public:
    virtual ~Effect() = default;
    virtual void update(float delta) = 0;
    virtual void render() = 0;
};
#endif