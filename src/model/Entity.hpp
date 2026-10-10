#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../Types.hpp"
#include <vector>
using namespace std;

enum class Direction
{
    RIGHT,
    LEFT,
    UP,
    DOWN
};

class Entity
{
private:
    int x;
    int y;
    EntityType type;
    vector<PropertyType> properties;
    Direction direction;
    int directionFrame;
    bool activeInRule; // is part of an active rule

public:
    Entity(int x, int y, EntityType type);
    int getX() const;
    int getY() const;
    void setPosition(int x, int y);
    EntityType getType() const;
    void setType(EntityType newType);
    bool hasProperty(PropertyType property) const;
    void addProperty(PropertyType property);
    void clearProperties();
    Direction getDirection() const;
    void setDirection(Direction direction);
    int getDirectionFrame() const;
    void setDirectionFrame(int frame);
    bool isActiveInRule() const;
    void setActiveInRule(bool active);
};

#endif