#include "Game.hpp"
#include <iostream>
#include <cmath>

using namespace std;

Game *Game::instance = nullptr;

Game *Game::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Game();
    }
    return instance;
}

Game::Game() : spriteSize{24}, scale{2.0f}, board(16, 13), window{sf::VideoMode(board.getWidth() * spriteSize * scale, (board.getHeight() * spriteSize * scale) + additionalHeight), "Baba Is You", sf::Style::Titlebar | sf::Style::Close}, designManager(window, spriteSize, scale)
{
    window.setFramerateLimit(15);
    mainMenu = new MainMenu(window);
    levelManager = new LevelManager();
    pauseMenu = new PauseMenu(window);
}

Game::~Game()
{
    delete mainMenu;
    delete levelManager;
    delete pauseMenu;
}

void Game::createEntity(int x, int y, EntityType e, PropertyType p)
{
    Entity *newEntity = new Entity(x, y, e);
    if (p != PropertyType::NONE)
    {
        newEntity->addProperty(p);
    }
    board.addEntity(newEntity);
}

void Game::initLevel()
{
    board.clear();
    levelManager->getCurrentLevel().load(board);
    ruleManager.applyRules(board);
}

void Game::run()
{
    std::vector<std::string> levelFiles;
    const int levelCount = 7;
    for (int i = 1; i <= levelCount; ++i)
    {
        levelFiles.push_back("levels/level" + std::to_string(i) + ".txt");
    }

    levelManager->loadLevels(levelFiles);
    // Passing level names to menu for display
    std::vector<std::string> levelNames;
    for (size_t i = 0; i < levelFiles.size(); ++i)
    {
        size_t slash = levelFiles[i].find_last_of("/\\");
        std::string name = (slash != std::string::npos) ? levelFiles[i].substr(slash + 1) : levelFiles[i];
        if (name.size() > 4 && name.substr(name.size() - 4) == ".txt")
        {
            name = name.substr(0, name.size() - 4);
            levelNames.push_back(name);
        }
    }

    mainMenu->setLevelNames(levelNames);

    while (window.isOpen())
    {
        // MAIN MENU LOOP
        mainMenu->setUnlockedLevels(unlockedLevels);
        state = GameState::MainMenu;
        mainMenu->display();

        int menuChoice = mainMenu->getSelectedOption();
        if (menuChoice == 2) // Exit
            return;

        int selectedLevel = 0;
        if (menuChoice == 1)
        {
            selectedLevel = mainMenu->getSelectedLevelIndex();
        }
        levelManager->selectLevel(selectedLevel);

        // Loading the selected level into the board
        levelManager->getCurrentLevel().load(board);
        ruleManager.applyRules(board);

        history.clear();
        undohistory.clear();

        state = GameState::Playing;
        exitGameLoop = false;

        sf::Clock clock;
        while (window.isOpen() && !exitGameLoop)
        {
            float elapsed = clock.restart().asSeconds();
            designManager.updateAnimation(elapsed);

            InputHandler(); // Pause menu sets exitGameLoop

            if (state == GameState::MainMenu)
            {
                exitGameLoop = true;
                break;
            }

            if (ruleManager.checkWin(board))
            {
                state = GameState::Victory;
                designManager.resetVictoryAnimation();
                bool waiting = true;
                while (window.isOpen() && waiting)
                {
                    sf::Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == sf::Event::Closed)
                        {
                            window.close();
                            return;
                        }
                        if (event.type == sf::Event::KeyPressed)
                        {
                            waiting = false;
                            state = GameState::Playing;
                        }
                    }
                    float delta = clock.restart().asSeconds();
                    designManager.updateAnimation(delta);
                    designManager.drawVictoryScreen();
                }

                int currentLevel = levelManager->getCurrentLevelIndex();
                if (currentLevel == unlockedLevels - 1)
                {
                    unlockedLevels++;
                }
                levelManager->nextLevel();
                int t = levelManager->getCurrentLevelIndex();
                if (t < levelManager->getLevelCount() && t < unlockedLevels)
                {

                    levelManager->getCurrentLevel().load(board);
                    ruleManager.applyRules(board);
                    history.clear();
                    undohistory.clear();
                }
                else
                {
                    exitGameLoop = true;
                }
            }
            else
            {
                designManager.drawAll(board);
            }
        }
    }
}

void Game::InputHandler()
{
    sf::Vector2u size = window.getSize();
    unsigned int width = size.x;
    unsigned int height = size.y;
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
            if (event.key.code == sf::Keyboard::Right)
                update(1, 0);
            if (event.key.code == sf::Keyboard::Left)
                update(-1, 0);
            if (event.key.code == sf::Keyboard::Up)
                update(0, -1);
            if (event.key.code == sf::Keyboard::Down)
                update(0, 1);
            if (event.key.code == sf::Keyboard::P && state == GameState::Playing)
            {
                std::vector<std::string> rules = ruleManager.getRulesAsStrings();
                pauseMenu->setRules(rules);
                pauseMenu->display();

                int pauseChoice = pauseMenu->getSelectedOption();

                if (pauseChoice == 1) // Main Menu
                {
                    state = GameState::MainMenu;
                    exitGameLoop = true;
                }
            }
            if (event.key.code == sf::Keyboard::R)
            {
                board.clear();
                history.clear();
                undohistory.clear();
                initLevel();
            }
            if (event.key.code == sf::Keyboard::Z)
            {
                if (!history.empty())
                {
                    undohistory.push_back(board);
                    board = history.back();
                    history.pop_back();
                    ruleManager.applyRules(board);
                }
            }
            if (event.key.code == sf::Keyboard::Y)
            {
                if (!undohistory.empty())
                {
                    history.push_back(board);
                    board = undohistory.back();
                    undohistory.pop_back();
                    ruleManager.applyRules(board);
                }
            }
        }
        if (event.type == sf::Event::Resized)
        {
            if ((event.size.width / event.size.height) < aspectRatio)
            {
                if (height < event.size.height)
                {
                    window.setSize(sf::Vector2u(static_cast<float>(event.size.height) * aspectRatio, event.size.height));
                }
                else
                {
                    window.setSize(sf::Vector2u(event.size.width, static_cast<float>(event.size.width) / aspectRatio));
                }
            }
            else
            {
                if (height < event.size.height)
                {
                    window.setSize(sf::Vector2u(event.size.width, static_cast<float>(event.size.width) / aspectRatio));
                }
                else
                {
                    window.setSize(sf::Vector2u(static_cast<float>(event.size.height) * aspectRatio, event.size.height));
                }
            }
        }
    }
}

bool Game::tryPushPullChain(int startX, int startY, int dx, int dy, bool push)
{
    std::vector<Entity *> chain;

    int x = startX;
    int y = startY;

    while (true)
    {
        if (x < 0 || x >= board.getWidth() || y < 0 || y >= board.getHeight())
        {
            if (push)
            {
                return false; // can't push chain outside board
            }
            else
            {
                break; // reached edge while pulling -> stop
            }
        }

        const std::vector<Entity *> &cell = board.getCell(x, y).getEntities();

        bool chainBroke = true;

        for (size_t i = 0; i < cell.size(); ++i)
        {
            if (push)
            {
                if (cell[i]->hasProperty(PropertyType::PUSH))
                {
                    chainBroke = false;
                    chain.push_back(cell[i]);
                }
                if (cell[i]->hasProperty(PropertyType::SHUT) && chain.back()->hasProperty(PropertyType::OPEN))
                {
                    chainBroke = true;
                    break;
                }
                if (cell[i]->hasProperty(PropertyType::STOP) || cell[i]->hasProperty(PropertyType::PULL))
                {
                    return false;
                }
            }
            else
            {
                if (cell[i]->hasProperty(PropertyType::PULL))
                {
                    chainBroke = false;
                    chain.push_back(cell[i]);
                }
            }
        }

        if (chainBroke)
        {
            break; // reached empty cell -> can push
        }

        // Move further into push direction
        if (push)
        {
            x += dx;
            y += dy;
            if (x < 0 || x >= board.getWidth() || y < 0 || y >= board.getHeight())
            {
                return false; // can't push chain outside board
            }
        }
        else
        {
            x -= dx;
            y -= dy;
        }
    }

    // full push chain and a free space at (x, y)
    // moving all the pushable objects from farthest to closest
    int targetX;
    int targetY;

    if (push)
    {
        for (int i = static_cast<int>(chain.size()) - 1; i >= 0; --i)
        {
            targetX = startX + dx * (i + 1);
            targetY = startY + dy * (i + 1);

            board.moveEntity(chain[i], targetX, targetY);
        }
    }
    else
    {
        for (int i = 0; i < static_cast<int>(chain.size()); i++)
        {
            targetX = chain[i]->getX() + dx;
            targetY = chain[i]->getY() + dy;

            board.moveEntity(chain[i], targetX, targetY);
        }
    }

    return true;
}

void Game::update(int x, int y)
{
    history.push_back(board);
    undohistory.clear();
    const vector<Entity *> v = board[PropertyType::YOU];

    for (size_t i = 0; i < v.size(); i++)
    {
        int oldX = v[i]->getX();
        int oldY = v[i]->getY();
        Direction oldDir = v[i]->getDirection();
        if (x == 1)
            v[i]->setDirection(Direction::RIGHT);
        if (x == -1)
            v[i]->setDirection(Direction::LEFT);
        if (y == -1)
            v[i]->setDirection(Direction::UP);
        if (y == 1)
            v[i]->setDirection(Direction::DOWN);

        if (oldDir != v[i]->getDirection())
        {
            v[i]->setDirectionFrame(0);
        }
        else
        {
            int frame = v[i]->getDirectionFrame();
            frame = (frame + 1) % dirFrameCount;
            v[i]->setDirectionFrame(frame);
        }

        int newX = oldX + x;
        int newY = oldY + y;
        int behindX = oldX - x;
        int behindY = oldY - y;
        // checking bounds
        if (newX < 0 || newX >= board.getWidth() || newY < 0 || newY >= board.getHeight())
        {
            continue;
        }
        // checking STOP and PUSH
        const vector<Entity *> &entitiesAtDest = board.getCell(newX, newY).getEntities();
        bool blocked = false;
        for (size_t j = 0; j < entitiesAtDest.size(); j++)
        {
            if (entitiesAtDest[j]->hasProperty(PropertyType::STOP) || entitiesAtDest[j]->hasProperty(PropertyType::PULL))
            {
                blocked = true;
                break;
            }
            if (entitiesAtDest[j]->hasProperty(PropertyType::PUSH))
            {
                if (!tryPushPullChain(newX, newY, x, y, true))
                {
                    blocked = true;
                    break;
                }
            }
        }
        if (blocked)
        {
            continue;
        }
        board.moveEntity(v[i], newX, newY);
        if (behindX >= 0 && behindX < board.getWidth() && behindY >= 0 && behindY < board.getHeight())
        {
            const vector<Entity *> &entitiesBehind = board.getCell(behindX, behindY).getEntities();

            for (size_t j = 0; j < entitiesBehind.size(); j++)
            {
                if (entitiesBehind[j]->hasProperty(PropertyType::PULL))
                {
                    tryPushPullChain(behindX, behindY, x, y, false);
                    break;
                }
            }
        }
    }
    ruleManager.applyRules(board);
    handleTeleport();
    ruleManager.applyInteractions(board);
}

Entity *Game::findTeleportDest(Entity *source, const vector<Entity *> &allTeleporters)
{
    for (Entity *candidate : allTeleporters)
    {
        if (candidate != source && candidate->getType() == source->getType())
        {
            return candidate;
        }
    }
    return nullptr;
}

bool Game::isDestinationBlocked(int x, int y, Entity *e)
{
    const std::vector<Entity *> &entitiesAtDest = board[{x, y}];

    for (Entity *destEntity : entitiesAtDest)
    {
        if (destEntity == e)
            continue;

        if (destEntity->hasProperty(PropertyType::STOP))
            return true;
    }
    return false;
}

void Game::performTeleport(Entity *subject, Entity *dest)
{
    int destX = dest->getX();
    int destY = dest->getY();
    board.moveEntity(subject, destX, destY);
}

void Game::handleTeleport()
{
    std::vector<Entity *> teleporters = board[PropertyType::TELE];
    if (teleporters.size() != 2)
        return;
    vector<Entity *> alreadyTeleported;
    for (Entity *source : teleporters)
    {
        std::vector<Entity *> entitiesOnSameCell = board[{source->getX(), source->getY()}];

        for (Entity *subject : entitiesOnSameCell)
        {
            if (subject == source)
                continue;

            bool dejaVu = false;
            for (Entity *e : alreadyTeleported)
            {
                if (e == subject)
                {
                    dejaVu = true;
                    break;
                }
            }
            if (dejaVu)
                continue;

            Entity *destination = findTeleportDest(source, teleporters);

            if (destination != nullptr)
            {
                if (!isDestinationBlocked(destination->getX(), destination->getY(), destination))
                {
                    performTeleport(subject, destination);
                    alreadyTeleported.push_back(subject);
                }
            }
        }
    }
}