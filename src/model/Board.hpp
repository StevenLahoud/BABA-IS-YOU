#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "Entity.hpp"
#include "../Types.hpp"
#include "Cell.hpp"
using namespace std;
class Board
{
private:
    int width;
    int height;
    vector<vector<Cell>> grid;
    vector<Entity *> allEntities;

public:
    Board(int width, int height);
    virtual ~Board();
    Board(const Board &other);
    Board &operator=(const Board &other);
    const vector<Entity *> &getAllEntities() const;
    int getWidth();
    int getHeight();
    const Cell &getCell(int x, int y) const;
    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);
    void moveEntity(Entity *entity, int newX, int newY);
    vector<Entity *> findEntitiesOfType(EntityType type) const;
    vector<Entity *> findEntitiesWithProperty(PropertyType prop) const;
    void clearAllProperties();
    void clear();

    const vector<Entity *> &operator[](Position position) const;
    vector<Entity *> operator[](PropertyType prop) const;
    vector<Entity *> operator[](EntityType type) const;
};
#endif