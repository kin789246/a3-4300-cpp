#include "Physics.h"
#include "Entity.h"
#include <memory>

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
    // todo: return the overlap rectangle of the bouding boxes of enetity a and b

    return Vec2(0, 0);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
    // todo: return the previous overlap rectangle size of 
    // the bouding boxes of enetity a and b
    // previous overlap uses the entity's previous position

    return Vec2(0, 0);
}
