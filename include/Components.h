#pragma once

#include "Animation.h"

// set a flag: flag |= (int)PlayerState
// unset a flag: flag &= ~(int)PlayerState
// flipping a flag: flag ^= (int)PlayerState
// checking if a flag set: return (flag & (int)PlayerState) == (int)PlayerState
// checking multiple flags set: return (flag &(int)PlayerState) != 0
enum struct PlayerState {
    STAND = 1 << 0,
    STANDSHOOT = 1 << 1,
    AIR = 1 << 2,
    AIRSHOOT = 1 << 3,
    RUN = 1 << 4,
    RUNSHOOT = 1 << 5
};

class Component
{
    public:
        bool has = false;
};

class CTransform : public Component
{
    public:
        Vec2 pos = { 0.0, 0.0 };
        Vec2 prevPos = { 0.0, 0.0 };
        Vec2 scale = { 1.0, 1.0 };
        Vec2 velocity = { 0.0, 0.0 };
        float angle = 0;

        CTransform() {}
        CTransform(const Vec2& p) : pos(p) {}
        CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, float a)
            : pos(p), prevPos(p), scale(sc), velocity(sp), angle(a) {}
};

class CLifespan : public Component
{
    public:
        int lifespan = 0;
        int frameCreated = 0;
        CLifespan() {}
        CLifespan(int duration, int frame) 
            : lifespan(duration), frameCreated(frame) {}
};

class CInput : public Component
{
    public:
        bool up = false;
        bool left = false;
        bool right = false;
        bool down = false;
        bool shoot = false;
        bool canShoot = true;
        bool canJump = true;

        CInput() {}
};

class CBoundingBox : public Component
{
    public:
        Vec2 size;
        Vec2 halfSize;
        CBoundingBox() {}
        CBoundingBox(const Vec2& s) 
            : size(s), halfSize(s.x / 2.0, s.y / 2.0) {}
};

class CAnimation : public Component
{
    public:
        Animation animation;
        bool repeat = false;
        CAnimation() {}
        CAnimation(const Animation& animation, bool r) 
        : animation(animation), repeat(r) {}
};

class CGravity : public Component
{
    public:
        float gravity = 0;
        CGravity() {}
        CGravity(float g) : gravity(g) {}
};

class CState : public Component
{
    public:
    PlayerState state;
    PlayerState preState; 
    bool changeAnimate = false;
    CState() {}
    CState(const PlayerState s) : state(s), preState(s) {}
}; 
