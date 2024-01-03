#include "Scene_Play.h"
#include "Scene_Menu.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"

#include "SFML/System/Vector2.hpp"

#include <iostream>
#include <string>
#include <fstream>

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
    // keymaps for playing
    registerAction(sf::Keyboard::W, "JUMP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Space, "SHOOT");

    m_gridText.setCharacterSize(9);
    m_gridText.setFont(m_game->assets().getFont("Mario"));
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
    float offsetX, offsetY;
    auto eSize = entity->getComponent<CAnimation>().animation.getSize();
    float eScale;
    switch ((int)eSize.y) {
        case 16:
            eScale = 4.0;
            break;
        case 24: case 32:
            eScale = 2.0;
            break; 
        default:
            eScale = 1.0;
    }
    offsetX = (m_gridSize.x - eSize.x * eScale) / 2.0;
    offsetY = (m_gridSize.y - eSize.y * eScale) / 2.0;
    return Vec2(
        gridX * m_gridSize.x + m_gridSize.x / 2.0 - offsetX,
        height() - gridY * m_gridSize.y - m_gridSize.y / 2.0 + offsetY
    );
}

void Scene_Play::loadLevel(const std::string& fileName) {
    // reset the EntityManager every time we load a level
    m_entityManager = EntityManager();

    // todo: read in the level file and add the appropriate entities
    // use the PlayerConfig struct m_playerConfig to store player properties
    // this struct is defined at the top of Scene_Play.h

    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Could not load config.txt file!\n";
        exit(-1);
    }

    std::string head;
    while (file >> head) {
        if (head == "Tile") {
            std::string name;
            float x, y;
            file >> name >> x >> y;
            auto tile = m_entityManager.addEntity("tile");
            tile->addComponent<CAnimation>(
                m_game->assets().getAnimation(name), true
            );
            tile->addComponent<CTransform>(
                gridToMidPixel(x, y, tile),
                Vec2(0, 0),
                Vec2(4, 4),
                0
            );
            tile->addComponent<CBoundingBox>(m_gridSize);
        }
        else if (head == "Dec") {
            std::string name;
            float x, y;
            file >> name >> x >> y;
            auto dec = m_entityManager.addEntity("dec");
            dec->addComponent<CAnimation>(
                m_game->assets().getAnimation(name), true
            );
            dec->addComponent<CTransform>(
                gridToMidPixel(x, y, dec),
                Vec2(0, 0),
                Vec2(4, 4),
                0
            );
        }
        else if (head == "Player") {
            file >> m_playerConfig.X >> m_playerConfig.Y
                >> m_playerConfig.CX >> m_playerConfig.CY
                >> m_playerConfig.SPEED
                >> m_playerConfig.JUMP
                >> m_playerConfig.MAXSPEED
                >> m_playerConfig.GRAVITY
                >> m_playerConfig.WEAPON;
            spawnPlayer();
        }
        else {
            std::cerr << "head to " << head << "\n";
            std::cerr << "The config file format is incorrect!\n";
            exit(-1);
        }
    }

    // NOTE: all of the code below is sample code which shows you how to
    // set up and use entities with the new syntax, it should be removed

    // spawnPlayer();

    // some sample entities
    // auto brick = m_entityManager.addEntity("tile");
    // IMPORTANT: always add CAnimation Component first so that gridToMixPixel
    // can compute correctly
    // brick->addComponent<CAnimation>(
    //     m_game->assets().getAnimation("Brick"), 
    //     true
    // );
    // brick->addComponent<CTransform>(
    //     Vec2(96, 480),
    //     Vec2(0, 0),
    //     Vec2(4, 4),
    //     0
    // );
    // NOTE: You final code should positon the entity with the grid x,y psition
    // read from the file: 
    // brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick));

    // if (brick->getComponent<CAnimation>().animation.getName() == "Brick") {
    //     std::cout << 
    //         "This could be a good way of identifying if a tile is a brick\n";
    // }

    // auto block = m_entityManager.addEntity("tile");
    // block->addComponent<CAnimation>(
    //     m_game->assets().getAnimation("Block"), 
    //     true
    // );
    // block->addComponent<CTransform>(
    //     Vec2(224, 480),
    //     Vec2(0, 0),
    //     Vec2(4, 4),
    //     0
    // );
    // add a bounding box, this will now show up if we press 'C' key
    // block->addComponent<CBoundingBox>(
    //     m_game->assets().getAnimation("Block").getSize()
    // );

    // auto question = m_entityManager.addEntity("tile");
    // question->addComponent<CAnimation>(
    //     m_game->assets().getAnimation("Question"),
    //     true
    // );
    // question->addComponent<CTransform>(
    //     Vec2(352, 480),
    //     Vec2(0, 0),
    //     Vec2(4, 4),
    //     0
    // );

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
    // todo: be sure to add the remaining components to the player
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CAnimation>(
        m_game->assets().getAnimation("Stand"),
        true
    );
    m_player->addComponent<CTransform>(
        gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player),
        Vec2(m_playerConfig.SPEED, 0),
        Vec2(-2, 2),
        0
    );
    m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));
    m_player->addComponent<CInput>();
    m_player->addComponent<CState>(PlayerState::STAND);
    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity) {
    // todo: this should spawn a bullet at the given entity,
    // going in the direction the entity is facing
    auto bullet = m_entityManager.addEntity("bullet");
    bullet->addComponent<CAnimation>(
        m_game->assets().getAnimation(m_playerConfig.WEAPON),
        true
    );
    bullet->addComponent<CTransform>(
        entity->getComponent<CTransform>().pos,
        Vec2(-5 * entity->getComponent<CTransform>().scale.x, 0),
        entity->getComponent<CTransform>().scale,
        0
    );
    bullet->addComponent<CLifespan>(90, m_currentFrame);
    bullet->addComponent<CBoundingBox>(
        bullet->getComponent<CAnimation>().animation.getSize()
    );
}

void Scene_Play::update() {
    m_entityManager.update();

    // todo: implement pause functionality
    if (!m_pause) {
        sMovement();
        sLifespan();
        sCollision();
        m_currentFrame++;
    }
    
    sAnimation();
    sRender();
}

void Scene_Play::sMovement() {
    // todo: implement player movement / juming based on its CInput component
    // todo: implement gravity's effect on the player
    // todo: implement the maxmium player speed in both X and Y directions
    // NOTE: Setting an entity's scale.x to -1/1 will make it face to the left/right
    
    // reset player speed to zero
    m_player->getComponent<CTransform>().velocity.x = 0;

    if (m_player->getComponent<CInput>().left) {
        m_player->getComponent<CTransform>().velocity.x = -m_playerConfig.SPEED;
        m_player->getComponent<CTransform>().scale.x = 2;
    }
    else if (m_player->getComponent<CInput>().right) {
        m_player->getComponent<CTransform>().velocity.x = m_playerConfig.SPEED;
        m_player->getComponent<CTransform>().scale.x = -2;
    }
    if (m_player->getComponent<CInput>().up) {
        if (m_player->getComponent<CInput>().canJump) {
            m_player->getComponent<CInput>().canJump = false;
            m_player->getComponent<CTransform>().velocity.y = m_playerConfig.JUMP;
        }
    }
    else if (m_player->getComponent<CTransform>().velocity.y <= 0){
        m_player->getComponent<CTransform>().velocity.y = 0;
    }

    if (m_player->getComponent<CInput>().shoot) {
        if (m_player->getComponent<CInput>().canShoot) {
            spawnBullet(m_player);
            m_player->getComponent<CInput>().canShoot = false;
        }
    }

    // update all entities positons
    for (auto e : m_entityManager.getEntities()) {
        if (e->hasComponent<CGravity>()) {
            Vec2& v = e->getComponent<CTransform>().velocity; 
            v.y += e->getComponent<CGravity>().gravity;
            if ( v.y > m_playerConfig.MAXSPEED) {
                v.y = m_playerConfig.MAXSPEED;
            }
            // std::cout << e->getComponent<CTransform>().velocity.x << " "
            //     << e->getComponent<CTransform>().velocity.y << std::endl;
        }
        e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
        e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
    }
}

void Scene_Play::sLifespan() {
    // todo: check lifespan of entities that have them, and destroy them if
    // it go over
    for (auto e : m_entityManager.getEntities()) {
        if (e->hasComponent<CLifespan>()) {
            if (e->getComponent<CAnimation>().animation.getName() == "CoinSpin") {
                if (e->getComponent<CAnimation>().animation.hasEnded()) {
                    e->destroy();
                }
            }
            else {
                auto& eLife = e->getComponent<CLifespan>();
                if (m_currentFrame - eLife.frameCreated >= eLife.lifespan) {
                    e->destroy();
                }
            }
        }
    }
    // control bullet quantity, be spawned every 10 frames
    for (auto e : m_entityManager.getEntities("bullet")) {
        auto& bulletLife = e->getComponent<CLifespan>();
        if (m_currentFrame - bulletLife.frameCreated == 10) {
            m_player->getComponent<CInput>().canShoot = true;
        }
    }
}

void Scene_Play::sCollision() {
    // REMEMBER: SFML's (0,0) position is on the TOP-LEFT corner
    // this means jumping will have a negative y-component
    // and gravity will have a positive y-component
    // Also, something BELOW something else will have a y value GREATER than it
    // Also, something ABOVE something else will have a y value LESS than it

    // todo: implement Physics::getOverlap() function, use it inside this function
    // todo: implement bullet / tile collisions
    for (auto b : m_entityManager.getEntities("bullet")) {
        for (auto t : m_entityManager.getEntities("tile")) {
            if (t->getComponent<CAnimation>().animation.getName() == "Brick") {
                // check bullet and tile side collide
                Vec2 overlap = m_worldPhysics.GetOverlap(b, t);
                Vec2 pOverlap = m_worldPhysics.GetPreviousOverlap(b, t);
                if (0 < overlap.y && -m_gridSize.x < overlap.x) {
                    if (0 <= overlap.x && pOverlap.x <= 0) {
                        spawnBrickDebris(t);
                        b->destroy();
                    }
                }
            }
        }
    }
    // Destroy the tile if it has a Brick animation
    // todo: implement player / tile collisions and resolutions
    // update the CState component of the player to store whether 
    // it is currently on the ground or in the air. This will be used
    // by the Animation system
    
    // reset gravity
    m_player->getComponent<CGravity>().gravity = m_playerConfig.GRAVITY;
    for (auto t : m_entityManager.getEntities("tile")) {
        Vec2 overlap = m_worldPhysics.GetOverlap(m_player, t);
        Vec2 pOverlap = m_worldPhysics.GetPreviousOverlap(m_player, t);
        // check if player is on air
        // check tiles being below player
        float dy = t->getComponent<CTransform>().pos.y -
            m_player->getComponent<CTransform>().pos.y;
        if (0 < overlap.x && -m_gridSize.y < overlap.y && dy > 0) {
            // std::cout << "tile below player and distance < gridsize\n";
            if (0 <= overlap.y && pOverlap.y <= 0) {
                // std::cout << "collision resolution\n";
                // stand on tile
                m_player->getComponent<CInput>().canJump = true;
                m_player->getComponent<CGravity>().gravity = 0;
                m_player->getComponent<CTransform>().velocity.y = 0;
                // collision resolution
                m_player->getComponent<CTransform>().pos.y -= overlap.y;
                // std::cout << "player pos y = " 
                //     << m_player->getComponent<CTransform>().pos.y << std::endl;
            }
        }
        // check if player hits the tile
        if (0 < overlap.x && -m_gridSize.y < overlap.y && dy < 0) {
            if (0 <= overlap.y && pOverlap.y <= 0) {
                // std::cout << "hit the tile\n";
                m_player->getComponent<CTransform>().pos.y += overlap.y;
                m_player->getComponent<CTransform>().velocity.y = 0;
                if (t->getComponent<CAnimation>().animation.getName() == "Question") {
                    t->getComponent<CAnimation>().animation = 
                        m_game->assets().getAnimation("QuestionHit");
                    spawnCoinSpin(t);
                }
                if (t->getComponent<CAnimation>().animation.getName() == "Brick") {
                    spawnBrickDebris(t);
                }
            }
        }
        // check player and tile side collide
        float dx = t->getComponent<CTransform>().pos.x -
            m_player->getComponent<CTransform>().pos.x;
        if (0 < overlap.y && -m_gridSize.x < overlap.x) {
            if (0 <= overlap.x && pOverlap.x <= 0) {
                if (dx > 0) {
                    // tile is right of player
                    m_player->getComponent<CTransform>().pos.x -= overlap.x;
                }
                else {
                    // tile is left of player
                    m_player->getComponent<CTransform>().pos.x += overlap.x;
                }
            }
        }
    }
    // todo: check to see if the player has fallen down a hole (y > height())
    if (m_player->getComponent<CTransform>().pos.y > height()) {
        m_player->getComponent<CTransform>().pos =
            gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player);
    }
    // todo: don't let the player walk of the left side of the map
    if (m_player->getComponent<CTransform>().pos.x < 
        m_player->getComponent<CBoundingBox>().size.x / 2.0) {
        m_player->getComponent<CTransform>().pos.x =
            m_player->getComponent<CBoundingBox>().size.x / 2.0;
    }
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
        else if (action.name() == "JUMP") {
            if (m_player->getComponent<CInput>().canJump) {
                m_player->getComponent<CInput>().up = true;
            }
        }
        else if (action.name() == "DOWN") {
            m_player->getComponent<CInput>().down = true;
        }
        else if (action.name() == "LEFT") {
            m_player->getComponent<CInput>().left = true;
        }
        else if (action.name() == "RIGHT") {
            m_player->getComponent<CInput>().right = true;
        }
        else if (action.name() == "SHOOT") {
            if (m_player->getComponent<CInput>().canShoot) {
                m_player->getComponent<CInput>().shoot = true;
            }
        }
    }
    else if (action.type() == "END") {
        if (action.name() == "JUMP") {
            m_player->getComponent<CInput>().up = false;
        }
        else if (action.name() == "DOWN") {
            m_player->getComponent<CInput>().down = false;
        }
        else if (action.name() == "LEFT") {
            m_player->getComponent<CInput>().left = false;
        }
        else if (action.name() == "RIGHT") {
            m_player->getComponent<CInput>().right = false;
        }
        else if (action.name() == "SHOOT") {
            m_player->getComponent<CInput>().shoot = false;
        }
    }
}

void Scene_Play::sAnimation() {
    // todo: complete the animation class code first
    // todo: set the animation of the player based on its CState component
    // check player state
    if(m_player->getComponent<CTransform>().velocity.y != 0) {
        m_player->getComponent<CInput>().canJump = false;
        if (m_player->getComponent<CInput>().shoot) {
            changePlayerStateTo(PlayerState::AIRSHOOT);
        }
        else {
            changePlayerStateTo(PlayerState::AIR);
        }
    }
    else {
        if (m_player->getComponent<CTransform>().velocity.x != 0) {
            if (m_player->getComponent<CInput>().shoot) {
                changePlayerStateTo(PlayerState::RUNSHOOT);
            }
            else {
                changePlayerStateTo(PlayerState::RUN);
            }
        }
        else {
            if (m_player->getComponent<CInput>().shoot) {
                changePlayerStateTo(PlayerState::STANDSHOOT);
            }
            else {
                changePlayerStateTo(PlayerState::STAND);
            }
        }
    }
    
    // change player animation
    if (m_player->getComponent<CState>().changeAnimate) {
        std::string aniName;
        switch (m_player->getComponent<CState>().state) {
            case PlayerState::STAND:
                aniName = "Stand";
                break;
            case PlayerState::AIR:
                aniName = "Air";
                break;
            case PlayerState::RUN:
                aniName = "Run";
                break;
            case PlayerState::STANDSHOOT:
                aniName = "StandShoot";
                break;
            case PlayerState::AIRSHOOT:
                aniName = "AirShoot";
                break;
            case PlayerState::RUNSHOOT:
                aniName = "RunShoot";
                break;
        }
        m_player->addComponent<CAnimation>(
                m_game->assets().getAnimation(aniName), true
                );
    }

    for (auto e : m_entityManager.getEntities()) {
        if (e->getComponent<CAnimation>().animation.hasEnded() &&
            !e->getComponent<CAnimation>().repeat) {
            e->destroy();
        }
        if (e->hasComponent<CAnimation>()) {
            e->getComponent<CAnimation>().animation.update();
        }
    }
    // call entity->getComponent<CAnimation>().animation.update()
    // if the animation is not repeated, and it has ended, destroy the entity
    
}

void Scene_Play::onEnd() {
    // todo: when the scene ends, change back to the MENU scene
    // use m_game->changeScene(correct params);
    m_game->changeScene( "MENU", std::make_shared<Scene_Menu>(m_game));
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
                animation.getSprite().setPosition(
                    transform.pos.x, transform.pos.y
                );
                animation.getSprite().setScale(
                    transform.scale.x, transform.scale.y
                );
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
    m_pause = pause;
}

void Scene_Play::changePlayerStateTo(PlayerState s) {
    auto& prev = m_player->getComponent<CState>().preState; 
    if (prev != s) {
        prev = m_player->getComponent<CState>().state;
        m_player->getComponent<CState>().state = s; 
        m_player->getComponent<CState>().changeAnimate = true;
    }
    else { 
        m_player->getComponent<CState>().changeAnimate = false;
    }
}

void Scene_Play::spawnCoinSpin(std::shared_ptr<Entity> tile) {
    auto coin = m_entityManager.addEntity("coinspin");
    coin->addComponent<CAnimation>(
        m_game->assets().getAnimation("CoinSpin"),
        false
    );
    coin->addComponent<CTransform>(
        Vec2(
            tile->getComponent<CTransform>().pos.x,
            tile->getComponent<CTransform>().pos.y - m_gridSize.y
            ),
        Vec2(0, 0),
        tile->getComponent<CTransform>().scale,
        0
    );
    coin->addComponent<CLifespan>(30, m_currentFrame);
}

void Scene_Play::spawnBrickDebris(std::shared_ptr<Entity> tile) {
    tile->getComponent<CAnimation>().animation = 
        m_game->assets().getAnimation("BrickDebris");
    tile->addComponent<CLifespan>(10, m_currentFrame);
}
