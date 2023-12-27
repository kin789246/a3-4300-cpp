#include "Scene_Play.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include "SFML/System/Vector2.hpp"

#include <iostream>
#include <string>

Scene_Play::Scene_Play(GameEngine* gameEngine, std::string& levelPath)
    : Scene(gameEngine)
    , m_levelPath(levelPath)
{
    init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath) {
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE"); // toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // toggle drawing (C)ollision Box
    registerAction(sf::Keyboard::G, "TOGGLE_GRID"); // toggle drawing (G)rid

    // todo: register all other gameplay Actions
    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Tech"));
    loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(
    float gridX, 
    float gridY, 
    std::shared_ptr<Entity> entity
) {
    // todo: this function takes in a grid (x, y) position and an Entity
    // return a Vec2 indicating where the CENTER position of the Entity should be 
    // You must use the Entity's Animation size to position it correctly
    // The size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
    // The bottom-left corner of the Animation should aligan with the bottom-left
    // of the grid cell

    return Vec2(0, 0);
}

void Scene_Play::loadLevel(const std::string& fileName) {
    // reset the EntityManager every time we load a level
    m_entityManager = EntityManager();

    // todo: read in the level file and add the appropriate entities
    // use the PlayerConfig struct m_playerConfig to store player properties
    // this struct is defined at the top of Scene_Play.h

    // NOTE: all of the code below is sample code which shows you how to
    // set up and use entities with the new syntax, it should be removed

    spawnPlayer();

    // some sample entities
    auto brick = m_entityManager.addEntity("tile");
    // IMPORTANT: always add CAnimation Component first so that gridToMixPixel
    // can compute correctly
    brick->addComponent<CAnimation>(m_game->assets().getAnimation("Brick"), true);
    brick->addComponent<CTransform>(Vec2(96, 480));
    // NOTE: You final code should positon the entity with the grid x,y psition
    // read from the file: 
    // brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick));

    if (brick->getComponent<CAnimation>().animation.getName() == "Brick") {
        std::cout << "This could be a good way of identifying if a tile is a brick\n";
    }

    auto block = m_entityManager.addEntity("tile");
    block->addComponent<CAnimation>(m_game->assets().getAnimation("Block"), true);
    block->addComponent<CTransform>(Vec2(224, 480));
    // add a bounding box, this will now show up if we press 'C' key
    block->addComponent<CBoundingBox>(m_game->assets().getAnimation("Block").getSize());

    auto question = m_entityManager.addEntity("tile");
    block->addComponent<CAnimation>(m_game->assets().getAnimation("Question"), true);
    block->addComponent<CTransform>(Vec2(352, 480));

    // NOTE: THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
    // Components are now returned as references rather than pointers
    // If you do not specify a reference variable type, it will COPY the component
    // Here is an example:
    //
    // This will COPY the transform into the variable 'transform1' - it is INCORRECT
    // Any changes you make to transform1 will not be changed inside the entity
    // auto transform1 = entity->get<CTransform>()
    //
    // This will REFERENCE the transform with the variable 'transform2' - it is CORRECT
    // Now any changes you make to transform2 will be changed inside the entity
    // auto& transform2 = entity->get<CTransform>()
}

void Scene_Play::spawnPlayer() {
    // have a sample player entity which you can use to construct other entities
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
    m_player->addComponent<CTransform>(Vec2(224, 352));
    m_player->addComponent<CBoundingBox>(Vec2(48, 48));

    // todo: be sure to add the remaining components to the player
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity) {
    // todo: this should spawn a bullet at the given entity,
    // going in the direction the entity is facing
}

void Scene_Play::update() {
    m_entityManager.update();

    // todo: implement pause functionality
    
    sMovement();
    sLifespan();
    sCollision();
    sAnimation();
    sRender();
}

void Scene_Play::sMovement() {
    // todo: implement player movement / juming based on its CInput component
    // todo: implement gravity's effect on the player
    // todo: implement the maxmium player speed in both X and Y directions
    // NOTE: Setting an entity's scale.x to -1/1 will make it face to the left/right
}

void Scene_Play::sLifespan() {
    // todo: check lifespan of entities that have them, and destroy them if
    // it go over
}

void Scene_Play::sCollision() {
    // REMEMBER: SFML's (0,0) position is on the TOP-LEFT corner
    // this means jumping will have a negative y-component
    // and gravity will have a positive y-component
    // Also, something BELOW something else will have a y value GREATER than it
    // Also, something ABOVE something else will have a y value LESS than it

    // todo: implement Physics::getOverlap() function, use it inside this function
    //
    // todo: implement bullet / tile collisions
    // Destroy the tile if it has a Brick animation
    // todo: implement player / tile collisions and resolutions
    // update the CState component of the player to store whether 
    // it is currently on the ground or in the air. This will be used
    // by the Animation system
    // todo: check to see if the player has fallen down a hole (y > height())
    // todo: don't let the player walk of the left side of the map
}

void Scene_Play::sDoAction(const Action& action) {
    if (action.type() == "START") {
        if (action.name() == "TOGGLE_TEXTURE") {
            m_drawTextures = !m_drawTextures; 
        }
        else if (action.name() == "TOGGLE_COLLISION") { 
            m_drawCollision = !m_drawCollision; 
        }
        else if (action.name() == "TOGGLE_GRID") { 
            m_drawDrawGrid = !m_drawDrawGrid; 
        }
        else if (action.name() == "PAUSE") { 
            setPaused(!m_pause);
        }
        else if (action.name() == "QUIT") { 
            onEnd();
        }
    }
    else if (action.type() == "END") {

    }
}

void Scene_Play::sAnimation() {
    // todo: complete the animation class code first
    // todo: set the animation of the player based on its CState component
    // todo: for each entity with an animation, call entity->getComponent<CAnimation>().animation.update()
    // if the animation is not repeated, and it has ended, destroy the entity
}

void Scene_Play::onEnd() {
    // todo: when the scene ends, change back to the MENU scene
    // use m_game->changeScene(correct params);
}

void Scene_Play::sRender() {
    // color the background darker so you know that the game is paused
    if (!m_pause) {
        m_game->window().clear(sf::Color(100, 100, 255));
    }
    else {
        m_game->window().clear(sf::Color(50, 50, 150));
    }

    // set the viewport of the window to be centered on the player if it's far
    // enough right
    auto& pPos = m_player->getComponent<CTransform>().pos;
    float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
    sf::View view = m_game->window().getView();
    view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
    m_game->window().setView(view);

    // draw all Entity textures / animations
    if (m_drawTextures) {
        for (auto e : m_entityManager.getEntities()) {
            auto& transform = e->getComponent<CTransform>();
            if (e->hasComponent<CAnimation>()) {
                auto& animation = e->getComponent<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                m_game->window().draw(animation.getSprite());
            }
        }
    }

    // draw all Entity collision bounding boxes with a rectangle shape
    if (m_drawCollision) {
        for (auto e : m_entityManager.getEntities()) {
            if (e->hasComponent<CBoundingBox>()) {
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x-1, box.size.y-1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }

    // draw the grid so that can easily debug
    if (m_drawDrawGrid) {
        float leftX = m_game->window().getView().getCenter().x - width() / 2.0;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

        for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
            drawLine(Vec2(x, 0), Vec2(x, height()));
        }

        for (float y=0; y < height(); y += m_gridSize.y) {
            drawLine(Vec2(leftX, height()-y), Vec2(rightX, height()-y));

            for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
                std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x+3, height()-y-m_gridSize.y+2);
                m_game->window().draw(m_gridText);
            }
        }
    }
}

void Scene_Play::setPaused(bool pause) {

}
