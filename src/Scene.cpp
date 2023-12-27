#include "Scene.h"

Scene::Scene() {}

Scene::Scene(GameEngine* gameEngine) {}

Scene::~Scene() {}

void Scene::doAction(const Action& action) {}

void Scene::simulate(const size_t frames) {}

void Scene::registerAction(int inputKey, const std::string& actionName) {}

size_t Scene::width() const {
    return 0;
}

size_t Scene::height() const {
    return 0;
}

size_t Scene::currentFrame() const {
    return 0;
}

bool Scene::hasEnded() const {
    return false;
}

ActionMap& Scene::getActionMap() {
    return m_actionMap;
}

void Scene::drawLine(const Vec2& p1, const Vec2& p2) {

}
