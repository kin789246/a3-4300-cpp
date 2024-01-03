#include "Assets.h"
#include <fstream>
#include <iostream>

void Assets::addTexture(const std::string& name, const std::string& path) {
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cerr << "Could not load image " << path << "!\n";
        exit(-1);
    }
    m_textures[name] = texture;
}

void Assets::addAnimation(const std::string& name, Animation animation) {
    m_animations[name] = animation;
}

void Assets::addFont(const std::string& name, const std::string& path) {
    sf::Font font;
    if (!font.loadFromFile(path)) {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }
    m_fonts[name] = font;
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
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not load config.txt file!\n";
        exit(-1);
    }
    std::string head;
    while (file >> head) {
        if (head == "Font") {
            std::string font_name;
            std::string font_path;
            file >> font_name >> font_path;
            addFont(font_name, font_path);
        }
        else if (head == "Texture") {
            std::string name;
            std::string path;
            file >> name >> path;
            addTexture(name, path);
        }
        else if (head == "Animation") {
            std::string aniName;
            std::string texName;
            int frames, speed;
            file >> aniName >> texName >> frames >> speed;
            const sf::Texture& tex = getTexture(texName);
            addAnimation(
                aniName, 
                Animation(aniName, tex, frames, speed)
            );
        }
        else {
            std::cerr << "head to " << head << "\n";
            std::cerr << "The config file format is incorrect!\n";
            exit(-1);
        }
    }
}
