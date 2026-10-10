#ifndef TYPES_HPP
#define TYPES_HPP

enum class EntityType
{
    BABA,
    FLAG,
    ROCK,
    WALL,
    WATER,
    KEY,
    DOOR,
    SKULL,
    LAVA,
    BORDER,
    NONE,
    DECORATIVE,
    TEXT_START,
    BABA_WORD,
    ROCK_WORD,
    FLAG_WORD,
    WALL_WORD,
    WATER_WORD,
    KEY_WORD,
    DOOR_WORD,
    SKULL_WORD,
    IS_WORD,
    YOU_WORD,
    WIN_WORD,
    PUSH_WORD,
    STOP_WORD,
    SHUT_WORD,
    OPEN_WORD,
    DEFEAT_WORD,
    SINK_WORD,
    PULL_WORD,
    HOT_WORD,
    LAVA_WORD,
    MELT_WORD,
    TELE_WORD

};

enum class PropertyType
{
    YOU,
    WIN,
    PUSH,
    STOP,
    OPEN,
    SHUT,
    DEFEAT,
    SINK,
    PULL,
    HOT,
    MELT,
    TELE,
    NONE
};

struct Position
{
    int x;
    int y;
};

#endif