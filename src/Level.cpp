#include "Level.hpp"
#include "model/Board.hpp"
#include "Types.hpp"
#include <fstream>
#include <filesystem>

Level::Level(const std::string &fname) : filename(fname) {}

bool Level::load(Board &board) const
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        return false;
    }
    board.clear();
    std::string line;
    int y = 0;
    while (std::getline(file, line))
    {
        for (int x = 0; x < (int)line.size(); ++x)
        {
            char c = line[x];
            switch (c)
            {
            case 'B':
                board.addEntity(new Entity(x, y, EntityType::BABA));
                break;
            case 'K':
                board.addEntity(new Entity(x, y, EntityType::KEY));
                break;
            case 'W':
                board.addEntity(new Entity(x, y, EntityType::WALL));
                break;
            case 'D':
                board.addEntity(new Entity(x, y, EntityType::DOOR));
                break;
            case 'F':
                board.addEntity(new Entity(x, y, EntityType::FLAG));
                break;
            case 'R':
                board.addEntity(new Entity(x, y, EntityType::ROCK));
                break;
            case 'S':
                board.addEntity(new Entity(x, y, EntityType::SKULL));
                break;
            case 'L':
                board.addEntity(new Entity(x, y, EntityType::LAVA));
                break;
            case '#':
                board.addEntity(new Entity(x, y, EntityType::WATER));
                break;
            case 'X':
                board.addEntity(new Entity(x, y, EntityType::BORDER));
                break;
            case 'T':
                board.addEntity(new Entity(x, y, EntityType::DECORATIVE));
                break;
            case 'b':
                board.addEntity(new Entity(x, y, EntityType::BABA_WORD));
                break;
            case 'i':
                board.addEntity(new Entity(x, y, EntityType::IS_WORD));
                break;
            case 'y':
                board.addEntity(new Entity(x, y, EntityType::YOU_WORD));
                break;
            case 'k':
                board.addEntity(new Entity(x, y, EntityType::KEY_WORD));
                break;
            case 'p':
                board.addEntity(new Entity(x, y, EntityType::PUSH_WORD));
                break;
            case 's':
                board.addEntity(new Entity(x, y, EntityType::STOP_WORD));
                break;
            case 'o':
                board.addEntity(new Entity(x, y, EntityType::OPEN_WORD));
                break;
            case 'h':
                board.addEntity(new Entity(x, y, EntityType::HOT_WORD));
                break;
            case 'm':
                board.addEntity(new Entity(x, y, EntityType::MELT_WORD));
                break;
            case 'l':
                board.addEntity(new Entity(x, y, EntityType::LAVA_WORD));
                break;
            case 'd':
                board.addEntity(new Entity(x, y, EntityType::DOOR_WORD));
                break;
            case 'u':
                board.addEntity(new Entity(x, y, EntityType::SHUT_WORD));
                break;
            case 'f':
                board.addEntity(new Entity(x, y, EntityType::FLAG_WORD));
                break;
            case 't':
                board.addEntity(new Entity(x, y, EntityType::WIN_WORD));
                break;
            case 'w':
                board.addEntity(new Entity(x, y, EntityType::WALL_WORD));
                break;
            case 'a':
                board.addEntity(new Entity(x, y, EntityType::WATER_WORD));
                break;
            case 'r':
                board.addEntity(new Entity(x, y, EntityType::ROCK_WORD));
                break;
            case 'e':
                board.addEntity(new Entity(x, y, EntityType::DEFEAT_WORD));
                break;
            case 'n':
                board.addEntity(new Entity(x, y, EntityType::SINK_WORD));
                break;
            case 'x':
                board.addEntity(new Entity(x, y, EntityType::SKULL_WORD));
                break;
            case 'q':
                board.addEntity(new Entity(x, y, EntityType::PULL_WORD));
                break;
            case 'z':
                board.addEntity(new Entity(x, y, EntityType::TELE_WORD));
                break;
            default:
                break;
            }
        }
        ++y;
    }
    file.close();
    return true;
}

const std::string &Level::getFilename() const
{
    return filename;
}
