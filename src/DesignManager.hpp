#include <SFML/Graphics.hpp>
#include "model/Board.hpp"
#include "Types.hpp"

class DesignManager
{

private:
    sf::RenderWindow &window;

    sf::Texture CharactersTexture;
    sf::Texture StaticObjectsTexture;
    sf::Texture TiledObjectsTexture;
    sf::Texture TextObjectTexture;
    sf::Texture OtherObjectsTexture;

    sf::Sprite sprite;
    sf::Font font;
    sf::Text controlsText;

    int spriteSize;
    float scale;
    int animFrame;
    float animTimer;
    const int animFrameCount = 3;
    const float animFrameDuration = 0.25f;

    float victoryLetterTimer = 0.f;
    int lettersVisible = 0;
    const int totalLetters = 8; // CONGRATS
    const float letterDelay = 0.25f;

    void drawElement(const sf::Texture &texture, int sheetX, int sheetY, int gridX, int gridY, int fixX = 0, int fixY = 0);
    void drawEntity(Entity *e);
    void drawTiledEntity(Board &board, Entity *e);
    void loadTextureWithMask(const std::string &filename, sf::Texture &texture);

    bool isTiled(EntityType type);
    bool isText(EntityType type);
    int getAnimFrame() const { return animFrame; }

public:
    DesignManager(sf::RenderWindow &window, int spriteSize, float scale);
    void drawAll(Board &board);
    void drawVictoryScreen();
    void updateAnimation(float elapsed);
    void resetVictoryAnimation();
};