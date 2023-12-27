#include "Entity.h"
#include <tuple>

Entity::Entity(const size_t i, const std::string& t)
    : m_id(i)
    , m_tag(t) 
{
    m_components = std::make_tuple(
        CTransform(), 
        CLifespan(),
        CInput(),
        CBoundingBox(),
        CAnimation(),
        CGravity(),
        CState()
    );
}

bool Entity::isActive() const {
    return m_active;
}

const std::string& Entity::tag() const {
    return m_tag;
}

const size_t Entity::id() const {
    return m_id;
}

void Entity::destroy() {
    m_active = false;
}
