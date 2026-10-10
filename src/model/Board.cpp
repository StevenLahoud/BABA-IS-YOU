#include "Board.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>
using namespace std;

Board::Board(int width, int height) : width{width}, height{height}
{
    grid.resize(height);
    for (int y = 0; y < height; y++)
    {
        grid[y].resize(width);
    }
}

Board::~Board()
{
    clear();
}

void Board::clear()
{
    for (size_t i = 0; i < allEntities.size(); i++)
    {
        delete allEntities[i];
    }
    allEntities.clear();
    grid.clear();
    grid.resize(height);
    for (int y = 0; y < height; y++)
    {
        grid[y].resize(width);
    }
}

Board::Board(const Board &other)
{
    this->width = other.width;
    this->height = other.height;
    grid.resize(height);
    for (int y = 0; y < height; y++)
    {
        grid[y].resize(width);
    }
    for (size_t i = 0; i < other.allEntities.size(); i++)
    {
        Entity *e = other.allEntities[i];
        Entity *newE = new Entity(*e);
        addEntity(newE);
    }
}

Board &Board::operator=(const Board &other)
{
    if (this == &other)
        return *this;
    clear();
    width = other.width;
    height = other.height;
    grid.resize(height);
    for (int y = 0; y < height; y++)
    {
        grid[y].resize(width);
    }

    for (size_t i = 0; i < other.allEntities.size(); i++)
    {
        Entity *e = other.allEntities[i];
        Entity *newE = new Entity(*e);
        addEntity(newE);
    }
    return *this;
}

const vector<Entity *> &Board::getAllEntities() const
{
    return allEntities;
}

int Board::getWidth()
{
    return width;
}

int Board::getHeight()
{
    return height;
}

const Cell &Board::getCell(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        throw out_of_range("Board :: getCell : postition out of bound");
    }
    return grid[y][x];
}

void Board::addEntity(Entity *entity)
{
    if (entity == nullptr)
    {
        throw invalid_argument("Board::addEntity : entity null");
    }
    allEntities.push_back(entity);
    int x = entity->getX();
    int y = entity->getY();
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        grid[y][x].addEntity(entity);
    }
}

void Board::removeEntity(Entity *entity)
{
    int x = entity->getX();
    int y = entity->getY();
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        grid[y][x].removeEntity(entity);
    }
    allEntities.erase(
        remove(allEntities.begin(), allEntities.end(), entity),
        allEntities.end());
    delete entity;
}

void Board::moveEntity(Entity *entity, int newX, int newY)
{
    int oldX = entity->getX();
    int oldY = entity->getY();
    if (oldX >= 0 && oldX < width && oldY >= 0 && oldY < height)
    {
        grid[oldY][oldX].removeEntity(entity);
    }
    entity->setPosition(newX, newY);
    if (newX >= 0 && newX < width && newY >= 0 && newY < height)
    {
        grid[newY][newX].addEntity(entity);
    }
}

vector<Entity *> Board::findEntitiesOfType(EntityType type) const
{
    vector<Entity *> v;
    for (size_t i = 0; i < allEntities.size(); i++)
    {
        if (allEntities[i]->getType() == type)
        {
            v.push_back(allEntities[i]);
        }
    }
    return v;
}

vector<Entity *> Board::findEntitiesWithProperty(PropertyType property) const
{
    vector<Entity *> v;
    for (size_t i = 0; i < allEntities.size(); i++)
    {
        if (allEntities[i]->hasProperty(property))
        {
            v.push_back(allEntities[i]);
        }
    }
    return v;
}

void Board::clearAllProperties()
{
    for (Entity *e : allEntities)
    {
        e->clearProperties();
    }
}

const vector<Entity *> &Board::operator[](Position position) const
{
    return getCell(position.x, position.y).getEntities();
}

vector<Entity *> Board::operator[](PropertyType prop) const
{
    return findEntitiesWithProperty(prop);
}

vector<Entity *> Board::operator[](EntityType type) const
{
    return findEntitiesOfType(type);
}
