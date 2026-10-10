#include "Cell.hpp"
#include <iostream>
using namespace std;

Cell::Cell()
{
}

Cell::~Cell()
{
}

void Cell::addEntity(Entity *entity)
{
    entities.push_back(entity);
}

void Cell::removeEntity(Entity *entity)
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (entities[i] == entity)
        {
            entities.erase(entities.begin() + i);
            return;
        }
    }
}

const vector<Entity *> &Cell::getEntities() const
{
    return entities;
}

bool Cell::containsEntityType(EntityType enType) const
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (entities[i]->getType() == enType)
        {
            return true;
        }
    }
    return false;
}

bool Cell::containsPropertyType(PropertyType property) const
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        if (entities[i]->hasProperty(property))
        {
            return true;
        }
    }
    return false;
}

bool Cell::isEmpty() const
{
    return entities.size() == 0;
}
