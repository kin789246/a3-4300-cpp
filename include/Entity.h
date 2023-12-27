#pragma once

#include "Components.h"
#include <tuple>
#include <string>

class EntityManager;

typedef std::tuple<
    CTransform,
    CLifespan,
    CInput,
    CBoundingBox,
    CAnimation,
    CGravity,
    CState
> ComponentTuple;

class Entity
{
    friend class EntityManager;

    bool m_active = true;
    size_t m_id = 0;
    std::string m_tag = "default";
    ComponentTuple m_components;

    // constructor is private so we can never create entities
    // outside the EntityManager which had friend class
    Entity(const size_t id, const std::string& tag);

    public:

        bool isActive() const;
        const std::string& tag() const;
        const size_t id() const;
        void destroy();

        template<typename T>
        bool hasComponent() const {
            return getComponent<T>().has;
        }

        template<typename T, typename... TArgs>
        T& addComponent(TArgs&&... mArgs) {
            auto& component = getComponent<T>();
            component = T(std::forward<TArgs>(mArgs)...);
            component.has = true;
            return component;
        }

        template<typename T>
        T& getComponent() {
            return std::get<T>(m_components);
        }

        template<typename T>
        const T& getComponent() const {
            return std::get<T>(m_components);
        }

        template<typename T>
        void removeComponent() {
            getComponent<T>() = T();
        }
};
