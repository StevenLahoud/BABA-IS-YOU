#include "DesignManager.hpp"
#include <iostream>

using namespace std;

DesignManager::DesignManager(sf::RenderWindow &window, int spriteSize, float scale)
    : window{window}, spriteSize{spriteSize}, scale{scale}, animFrame{0}, animTimer{0.0f}
{
    loadTextureWithMask("Characters.png", CharactersTexture);
    loadTextureWithMask("StaticObject.png", StaticObjectsTexture);
    loadTextureWithMask("TiledObjects.png", TiledObjectsTexture);
    loadTextureWithMask("TextObject.png", TextObjectTexture);
    loadTextureWithMask("BabaIsYouOtherObjects.png", OtherObjectsTexture);

    sprite.setScale(scale, scale);
    if (!font.loadFromFile("ressources/project_baba_font.ttf"))
    {
        cout << "project_baba_font.ttf is needed" << endl;
    }

    controlsText.setFont(font);
    controlsText.setString("R:Restart         Z:Undo         Y:Redo         P:Pause");
    controlsText.setCharacterSize(8);
    controlsText.setFillColor(sf::Color::White);
    float windowHeight = window.getSize().y;
    controlsText.setPosition(10.f, windowHeight - 35.f);
}

void DesignManager::drawAll(Board &board)
{
    window.clear(sf::Color::Black);

    for (int y = 0; y < board.getHeight(); y++)
    {
        for (int x = 0; x < board.getWidth(); x++)
        {

            std::vector<Entity *> cellEntities = board[{x, y}];

            for (Entity *e : cellEntities)
            {

                if (e->getType() == EntityType::DECORATIVE)
                {
                    drawEntity(e);
                }
                if (e->getType() == EntityType::LAVA)
                {
                    drawTiledEntity(board, e);
                }
            }
        }
    }

    for (int y = 0; y < board.getHeight(); y++)
    {
        for (int x = 0; x < board.getWidth(); x++)
        {
            std::vector<Entity *> cellEntities = board.getCell(x, y).getEntities();

            for (Entity *e : cellEntities)
            {

                if (e->getType() != EntityType::DECORATIVE && e->getType() != EntityType::LAVA)
                {

                    if (isTiled(e->getType()))
                    {
                        drawTiledEntity(board, e);
                    }
                    else
                    {
                        drawEntity(e);
                    }
                }
            }
        }
    }
    window.draw(controlsText);
    window.display();
}

void DesignManager::drawElement(const sf::Texture &texture, int sheetX, int sheetY, int GridX, int GridY, int fixX, int fixY)
{
    int spacing = 1;
    int margin = 1;
    sprite.setTexture(texture);
    int animSheetY = sheetY + getAnimFrame();
    int left = margin + (sheetX * (spriteSize + spacing));
    int top = margin + (animSheetY * (spriteSize + spacing));
    sprite.setTextureRect(sf::IntRect(left + fixX, top + fixY, spriteSize, spriteSize));
    sprite.setPosition(GridX * spriteSize * scale, GridY * spriteSize * scale);
    window.draw(sprite);
}

void DesignManager::drawTiledEntity(Board &board, Entity *e)
{
    int x = e->getX();
    int y = e->getY();
    EntityType type = e->getType();
    int tileKind = 0;

    if (x < board.getWidth() - 1 && board.getCell(x + 1, y).containsEntityType(type)) // right
    {
        tileKind += 1;
    }

    if (y > 0 && board.getCell(x, y - 1).containsEntityType(type)) // up
    {
        tileKind += 2;
    }

    if (x > 0 && board.getCell(x - 1, y).containsEntityType(type)) // left
    {
        tileKind += 4;
    }

    if (y < board.getHeight() - 1 && board.getCell(x, y + 1).containsEntityType(type)) // down
    {
        tileKind += 8;
    }

    switch (type)
    {
    case EntityType::WALL:
        drawElement(TiledObjectsTexture, 19 + tileKind, 60, x, y);
        break;
    case EntityType::WATER:
        drawElement(TiledObjectsTexture, 19 + tileKind, 63, x, y);
        break;
    case EntityType::LAVA:
        drawElement(TiledObjectsTexture, 19 + tileKind, 36, x, y);
        break;
    default:
        break;
    }
}

void DesignManager::drawEntity(Entity *e)
{
    int x = e->getX();
    int y = e->getY();

    int directionSet = 0;
    if (e->getType() == EntityType::BABA)
    {
        switch (e->getDirection())
        {
        case Direction::RIGHT:
            directionSet = 0;
            break;
        case Direction::UP:
            directionSet = 4;
            break; // Colonne suivante
        case Direction::LEFT:
            directionSet = 8;
            break;
        case Direction::DOWN:
            directionSet = 12;
            break;
        }
    }

    int act = 0;
    if (isText(e->getType()))
    {
        if (e->isActiveInRule())
        {
            act = 1;
        }
    }

    switch (e->getType())
    {
    case EntityType::BABA:
        drawElement(CharactersTexture, 23 + directionSet + e->getDirectionFrame(), 0, x, y);
        break;
    case EntityType::ROCK:
        drawElement(StaticObjectsTexture, 9, 24, x, y);
        break;
    case EntityType::FLAG:
        drawElement(StaticObjectsTexture, 4, 9, x, y);
        break;
    case EntityType::KEY:
        drawElement(StaticObjectsTexture, 4, 15, x, y);
        break;
    case EntityType::DOOR:
        drawElement(StaticObjectsTexture, 9, 6, x, y);
        break;
    case EntityType::SKULL:
        drawElement(OtherObjectsTexture, 10, 40, x, y, 0, -4);
        break;
    case EntityType::DECORATIVE:
        drawElement(StaticObjectsTexture, 9, 30, x, y);
        break;
    case EntityType::BABA_WORD:
        drawElement(CharactersTexture, 21 + act, 0, x, y);
        break;
    case EntityType::IS_WORD:
        drawElement(TextObjectTexture, 10 + act, 3, x, y);
        break;
    case EntityType::YOU_WORD:
        drawElement(TextObjectTexture, 10 + act, 9, x, y);
        break;
    case EntityType::LAVA_WORD:
        drawElement(TiledObjectsTexture, 17 + act, 36, x, y);
        break;
    case EntityType::FLAG_WORD:
        drawElement(StaticObjectsTexture, 2 + act, 9, x, y);
        break;
    case EntityType::SKULL_WORD:
        drawElement(OtherObjectsTexture, 5 + act, 40, x, y, 0, -4);
        break;
    case EntityType::WIN_WORD:
        drawElement(TextObjectTexture, 10 + act, 45, x, y, 0, -3);
        break;
    case EntityType::ROCK_WORD:
        drawElement(StaticObjectsTexture, 7 + act, 24, x, y);
        break;
    case EntityType::DEFEAT_WORD:
        drawElement(TextObjectTexture, 1 + act, 29, x, y, 0, 4);
        break;
    case EntityType::PUSH_WORD:
        drawElement(TextObjectTexture, 1 + act, 12, x, y);
        break;
    case EntityType::PULL_WORD:
        drawElement(TextObjectTexture, 4 + act, 12, x, y);
        break;
    case EntityType::WALL_WORD:
        drawElement(TiledObjectsTexture, 17 + act, 60, x, y);
        break;
    case EntityType::STOP_WORD:
        drawElement(TextObjectTexture, 7 + act, 12, x, y);
        break;
    case EntityType::WATER_WORD:
        drawElement(TiledObjectsTexture, 17 + act, 63, x, y);
        break;
    case EntityType::SINK_WORD:
        drawElement(TextObjectTexture, 1 + act, 30, x, y, 0, 54);
        break;
    case EntityType::KEY_WORD:
        drawElement(StaticObjectsTexture, 2 + act, 15, x, y);
        break;
    case EntityType::OPEN_WORD:
        drawElement(TextObjectTexture, 10 + act, 27, x, y, 0, 54);
        break;
    case EntityType::DOOR_WORD:
        drawElement(StaticObjectsTexture, 7 + act, 6, x, y);
        break;
    case EntityType::SHUT_WORD:
        drawElement(TextObjectTexture, 13 + act, 27, x, y, 0, 54);
        break;
    case EntityType::HOT_WORD:
        drawElement(TextObjectTexture, 4 + act, 29, x, y, 0, 4);
        break;
    case EntityType::MELT_WORD:
        drawElement(TextObjectTexture, 7 + act, 29, x, y, 0, 4);
        break;
    case EntityType::TELE_WORD:
        drawElement(TextObjectTexture, 4 + act, 45, x, y, 0, -3);
        break;
    default:
        break;
    }
}

void DesignManager::loadTextureWithMask(const string &filename, sf::Texture &texture)
{
    sf::Image image;
    image.loadFromFile("ressources/" + filename);
    image.createMaskFromColor(sf::Color(84, 165, 75));
    texture.loadFromImage(image);
}

bool DesignManager::isTiled(EntityType type)
{
    return (type == EntityType::WALL || type == EntityType::WATER || type == EntityType::LAVA);
}

bool DesignManager::isText(EntityType type)
{
    return type > EntityType::TEXT_START;
}

void DesignManager::drawVictoryScreen()
{
    window.clear(sf::Color::Black);
    if (lettersVisible > 0)
    {
        drawElement(TextObjectTexture, 4, 78, 4, 6, 0, -3);
    }
    if (lettersVisible > 1)
    {
        drawElement(TextObjectTexture, 0, 84, 5, 6, 0, -3);
    }
    if (lettersVisible > 2)
    {
        drawElement(TextObjectTexture, 12, 81, 6, 6, 0, -3);
    }
    if (lettersVisible > 3)
    {
        drawElement(TextObjectTexture, 12, 78, 7, 6, 0, -3);
    }
    if (lettersVisible > 4)
    {
        drawElement(TextObjectTexture, 6, 84, 8, 6, 0, -3);
    }
    if (lettersVisible > 5)
    {
        drawElement(TextObjectTexture, 0, 78, 9, 6, 0, -3);
    }
    if (lettersVisible > 6)
    {
        drawElement(TextObjectTexture, 10, 84, 10, 6, 0, -3);
    }
    if (lettersVisible > 7)
    {
        drawElement(TextObjectTexture, 8, 84, 11, 6, 0, -3);
    }
    if (lettersVisible % 3 == 2)
    {
        drawElement(StaticObjectsTexture, 19, 27, 1, 1);
        drawElement(StaticObjectsTexture, 19, 27, 14, 1);
        drawElement(StaticObjectsTexture, 19, 27, 1, 12);
        drawElement(StaticObjectsTexture, 19, 27, 14, 12);
    }

    window.display();
}

void DesignManager::updateAnimation(float elapsed)
{
    animTimer += elapsed;
    if (animTimer >= animFrameDuration)
    {
        animFrame = (animFrame + 1) % animFrameCount;
        animTimer = 0.0f;
    }
    if (lettersVisible < totalLetters)
    {
        victoryLetterTimer += elapsed;
        if (victoryLetterTimer >= letterDelay)
        {
            lettersVisible++;
            victoryLetterTimer = 0.f;
        }
    }
}

void DesignManager::resetVictoryAnimation()
{
    lettersVisible = 0;
    victoryLetterTimer = 0.f;
}