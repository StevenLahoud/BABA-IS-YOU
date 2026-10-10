#include "Entity.hpp"
#include <vector>
using namespace std;

Entity::Entity(int x, int y, EntityType type) : x{x}, y{y}, type{type}, direction{Direction::RIGHT}, directionFrame{0}, activeInRule{false} {}

int Entity::getX() const
{
    return x;
}

int Entity::getY() const
{
    return y;
}

void Entity::setPosition(int x2, int y2)
{
    x = x2;
    y = y2;
}

EntityType Entity::getType() const
{
    return type;
}

bool Entity::hasProperty(PropertyType property) const
{
    for (size_t i = 0; i < properties.size(); i++)
    {
        if (properties[i] == property)
        {
            return true;
        }
    }
    return false;
}

void Entity::setType(EntityType newType)
{
    type = newType;
}

void Entity::addProperty(PropertyType property)
{
    if (!hasProperty(property))
    {
        properties.push_back(property);
    }
}

void Entity::clearProperties()
{
    properties.clear();
}

Direction Entity::getDirection() const
{
    return direction;
}

void Entity::setDirection(Direction d)
{
    direction = d;
}

int Entity::getDirectionFrame() const
{
    return directionFrame;
}

void Entity::setDirectionFrame(int frame)
{
    directionFrame = frame;
}

bool Entity::isActiveInRule() const
{
    return activeInRule;
}

void Entity::setActiveInRule(bool active)
{
    activeInRule = active;
}