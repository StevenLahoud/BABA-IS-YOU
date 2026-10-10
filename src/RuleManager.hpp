#include "model/Board.hpp"
#include "Types.hpp"
#include <vector>
#include <string>

class RuleManager
{
private:
    std::vector<std::pair<EntityType, PropertyType>> activeRules;
    std::vector<std::pair<EntityType, EntityType>> activeTransformRules;
    std::string entityTypeToString(EntityType type) const;
    std::string propertyTypeToString(PropertyType type) const;

    void applyDefaultRules(Board &board);
    void applyProperties(Board &board, const vector<Entity *> &beforeIs, const vector<Entity *> &afterIs);

    bool isText(EntityType type);
    EntityType getEntityTypeFromWord(EntityType word);
    PropertyType getPropertyTypeFromWord(EntityType word);

public:
    std::vector<std::string> getRulesAsStrings() const;
    void applyRules(Board &board);
    bool checkWin(Board &board);
    void checkSink(Board &board);
    void checkOpenShut(Board &board);
    void checkDefeat(Board &board);
    void checkMelt(Board &board);
    void applyInteractions(Board &board);
};