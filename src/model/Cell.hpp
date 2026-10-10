#include <vector>
#include "Entity.hpp"
#include "../Types.hpp"
using namespace std;

class Cell
{
private:
    vector<Entity *> entities;

public:
    Cell();
    ~Cell();

    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);
    const vector<Entity *> &getEntities() const;
    bool containsEntityType(EntityType type) const;
    bool containsPropertyType(PropertyType property) const;
    bool isEmpty() const;
};
