#include "Assets.h"

void Assets::addTexture(const std::string& name, const std::string& path) {

}

void Assets::addAnimation(const std::string& name, Animation animation) {

}

void Assets::addFont(const std::string& name, const std::string& path) {

}

const sf::Texture& Assets::getTexture(const std::string& name) const {
    return m_textures.at(name);
}

const Animation& Assets::getAnimation(const std::string& name) const {
    return m_animations.at(name);
}

const sf::Font& Assets::getFont(const std::string& name) const {
    return m_fonts.at(name);
}

void Assets::loadFromFile(const std::string& path) {

}
