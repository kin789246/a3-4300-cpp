#pragma once

#include "GameEngine.h"
#include "Scene.h"
#include <vector>

class Scene_Menu : public Scene
{
    protected:

    std::string m_title;
    sf::Text m_menuText;
    std::vector<std::string> m_menuStrings;
    std::vector<std::string> m_levelPaths;
    std::vector<sf::Text> m_menuItems;
    size_t m_selectedMenuIndex = 0;

    void init();
    void update();
    void onEnd();
    void sDoAction(const Action& action);

    public:

    Scene_Menu(GameEngine* gameEngine = nullptr);
    void sRender();
};
