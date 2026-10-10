#include <SFML/Graphics.hpp>

#include <vector>
#include "model/Board.hpp"
#include "DesignManager.hpp"
#include "RuleManager.hpp"
#include "MainMenu.hpp"
#include "PauseMenu.hpp"
#include "LevelManager.hpp"

class Game
{
private:
    Game();
    int unlockedLevels = 1;
    static Game *instance;
    int spriteSize;
    float scale;
    const int additionalHeight = 40;
    Board board;
    sf::RenderWindow window;
    DesignManager designManager;
    RuleManager ruleManager;

    MainMenu *mainMenu;
    PauseMenu *pauseMenu;
    LevelManager *levelManager;

    const int dirFrameCount = 4;
    bool exitGameLoop = false;
    sf::Clock animClock;
    bool tryPushPullChain(int startX, int startY, int dx, int dy, bool push);
    void InputHandler();
    vector<Board> history;
    vector<Board> undohistory;
    Entity *findTeleportDest(Entity *source, const vector<Entity *> &allTeleporters);
    bool isDestinationBlocked(int x, int y, Entity *e);
    void performTeleport(Entity *subject, Entity *dest);
    void handleTeleport();

    enum class GameState
    {
        MainMenu,
        Playing,
        Paused,
        Victory
    };
    GameState state = GameState::MainMenu;

public:
    Game(const Game &) = delete;
    void operator=(const Game &) = delete;
    static Game *getInstance();
    ~Game();
    void run();

    void initLevel();
    void update(int x, int y);

    void createEntity(int x, int y, EntityType e = EntityType::NONE, PropertyType p = PropertyType::NONE);
};