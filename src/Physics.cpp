#include "Physics.h"
#include "Entity.h"
#include <cstdlib>
#include <memory>

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
    // todo: return the overlap rectangle size of the bouding boxes of enetity a and b
    Vec2 posA = a->getComponent<CTransform>().pos;
    Vec2 sizeA = a->getComponent<CBoundingBox>().halfSize;
    Vec2 posB = b->getComponent<CTransform>().pos;
    Vec2 sizeB = b->getComponent<CBoundingBox>().halfSize;
    Vec2 delta{ std::abs(posA.x - posB.x), std::abs(posA.y - posB.y) };
    float ox = sizeA.x + sizeB.x - delta.x;
    float oy = sizeA.y + sizeB.y - delta.y;
    return Vec2(ox, oy);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
    // todo: return the previous overlap rectangle size of 
    // the bouding boxes of enetity a and b
    // previous overlap uses the entity's previous position
    Vec2 posA = a->getComponent<CTransform>().prevPos;
    Vec2 sizeA = a->getComponent<CBoundingBox>().halfSize;
    Vec2 posB = b->getComponent<CTransform>().prevPos;
    Vec2 sizeB = b->getComponent<CBoundingBox>().halfSize;
    Vec2 delta{ std::abs(posA.x - posB.x), std::abs(posA.y - posB.y) };
    float ox = sizeA.x + sizeB.x - delta.x;
    float oy = sizeA.y + sizeB.y - delta.y;
    return Vec2(ox, oy);
}
