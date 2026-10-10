#include "RuleManager.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

EntityType RuleManager::getEntityTypeFromWord(EntityType word)
{
    switch (word)
    {
    case EntityType::BABA_WORD:
        return EntityType::BABA;
    case EntityType::ROCK_WORD:
        return EntityType::ROCK;
    case EntityType::FLAG_WORD:
        return EntityType::FLAG;
    case EntityType::WALL_WORD:
        return EntityType::WALL;
    case EntityType::WATER_WORD:
        return EntityType::WATER;
    case EntityType::KEY_WORD:
        return EntityType::KEY;
    case EntityType::DOOR_WORD:
        return EntityType::DOOR;
    case EntityType::SKULL_WORD:
        return EntityType::SKULL;
    case EntityType::LAVA_WORD:
        return EntityType::LAVA;
    default:
        return EntityType::NONE;
    }
}

PropertyType RuleManager::getPropertyTypeFromWord(EntityType word)
{
    switch (word)
    {
    case EntityType::YOU_WORD:
        return PropertyType::YOU;
    case EntityType::PUSH_WORD:
        return PropertyType::PUSH;
    case EntityType::STOP_WORD:
        return PropertyType::STOP;
    case EntityType::WIN_WORD:
        return PropertyType::WIN;
    case EntityType::SHUT_WORD:
        return PropertyType::SHUT;
    case EntityType::DEFEAT_WORD:
        return PropertyType::DEFEAT;
    case EntityType::SINK_WORD:
        return PropertyType::SINK;
    case EntityType::PULL_WORD:
        return PropertyType::PULL;
    case EntityType::HOT_WORD:
        return PropertyType::HOT;
    case EntityType::MELT_WORD:
        return PropertyType::MELT;
    case EntityType::TELE_WORD:
        return PropertyType::TELE;
    case EntityType::OPEN_WORD:
        return PropertyType::OPEN;
    default:
        return PropertyType::NONE;
    }
}

bool RuleManager::isText(EntityType type)
{
    return type > EntityType::TEXT_START;
}

void RuleManager::applyDefaultRules(Board &board)
{
    const std::vector<Entity *> &entities = board.getAllEntities();

    for (size_t i = 0; i < entities.size(); i++)
    {
        entities[i]->setActiveInRule(false);
        EntityType type = entities[i]->getType();
        if (isText(type))
        {
            entities[i]->addProperty(PropertyType::PUSH);
        }

        if (type == EntityType::BORDER)
        {
            entities[i]->addProperty(PropertyType::NONE);
        }
    }
}

std::string RuleManager::entityTypeToString(EntityType type) const
{
    static const char *names[] = {
        "BABA", "FLAG", "ROCK", "WALL", "WATER", "KEY",
        "DOOR", "SKULL", "LAVA", "BORDER", "NONE", "DECORATIVE"};
    int index = static_cast<int>(type);
    int arraySize = sizeof(names) / sizeof(names[0]);
    if (index < 0 || index >= arraySize)
    {
        throw out_of_range("RuleManager: Index out of bound ");
    }
    return names[index];
}

std::string RuleManager::propertyTypeToString(PropertyType type) const
{
    static const char *names[] = {
        "YOU", "WIN", "PUSH", "STOP", "OPEN", "SHUT",
        "DEFEAT", "SINK", "PULL", "HOT", "MELT", "WEAK",
        "MOVE", "TELE", "NONE"};
    return names[static_cast<int>(type)];
}

std::vector<std::string> RuleManager::getRulesAsStrings() const
{
    std::vector<std::string> rules;
    for (size_t i = 0; i < activeRules.size(); i++)
    {
        std::string ruleStr = entityTypeToString(activeRules[i].first) +
                              " IS " +
                              propertyTypeToString(activeRules[i].second);
        rules.push_back(ruleStr);
    }
    for (size_t i = 0; i < activeTransformRules.size(); i++)
    {
        std::string ruleStr = entityTypeToString(activeTransformRules[i].first) +
                              " IS " +
                              entityTypeToString(activeTransformRules[i].second);
        rules.push_back(ruleStr);
    }
    return rules;
}

void RuleManager::applyProperties(Board &board, const std::vector<Entity *> &beforeIs, const std::vector<Entity *> &afterIs)
{
    for (size_t i = 0; i < beforeIs.size(); i++)
    {
        for (size_t j = 0; j < afterIs.size(); j++)
        {
            EntityType entityType = getEntityTypeFromWord(beforeIs[i]->getType());
            PropertyType propertyType = getPropertyTypeFromWord(afterIs[j]->getType());
            EntityType transformEntity = getEntityTypeFromWord(afterIs[j]->getType());

            if (entityType != EntityType::NONE && propertyType != PropertyType::NONE)
            {
                // Store the rule as active
                activeRules.push_back(std::make_pair(entityType, propertyType));
                beforeIs[i]->setActiveInRule(true);
                afterIs[j]->setActiveInRule(true);
                vector<Entity *> target = board[entityType];
                for (size_t t = 0; t < target.size(); t++)
                {
                    target[t]->addProperty(propertyType);
                }
            }
            else if (entityType != EntityType::NONE && transformEntity != EntityType::NONE)
            {
                if (entityType != transformEntity)
                {
                    activeTransformRules.push_back(std::make_pair(entityType, transformEntity));
                    beforeIs[i]->setActiveInRule(true);
                    afterIs[j]->setActiveInRule(true);
                    vector<Entity *> target = board[entityType];
                    for (size_t t = 0; t < target.size(); t++)
                        target[t]->setType(transformEntity);
                }
            }
        }
    }
}

void RuleManager::applyRules(Board &board)
{
    board.clearAllProperties();
    activeRules.clear();
    activeTransformRules.clear();
    applyDefaultRules(board);

    vector<Entity *> isWords = board[EntityType::IS_WORD];
    for (size_t i = 0; i < isWords.size(); i++)
    {
        Entity *is = isWords[i];
        int x = is->getX();
        int y = is->getY();
        is->setActiveInRule(false);

        bool joinsRule = false;
        if (x > 0 && x < board.getWidth() - 1)
        {
            const vector<Entity *> &left = board[{x - 1, y}];
            const vector<Entity *> &right = board[{x + 1, y}];
            // If a rule is formed we mark this IS_WORD as active
            size_t beforeCount1 = activeRules.size();
            size_t beforeCount2 = activeTransformRules.size();
            applyProperties(board, left, right);
            if (activeRules.size() > beforeCount1 || activeTransformRules.size() > beforeCount2)
            {
                joinsRule = true;
            }
        }

        if (y > 0 && y < board.getHeight() - 1)
        {
            const vector<Entity *> &up = board[{x, y - 1}];
            const vector<Entity *> &down = board[{x, y + 1}];
            size_t beforeCount1 = activeRules.size();
            size_t beforeCount2 = activeTransformRules.size();
            applyProperties(board, up, down);
            if (activeRules.size() > beforeCount1 || activeTransformRules.size() > beforeCount2)
                joinsRule = true;
        }
        is->setActiveInRule(joinsRule);
    }
}

bool RuleManager::checkWin(Board &board)
{
    vector<Entity *> players = board[PropertyType::YOU];
    for (Entity *player : players)
    {
        int x = player->getX();
        int y = player->getY();
        const std::vector<Entity *> &entitiesOnSameCell = board[{x, y}];
        for (Entity *e : entitiesOnSameCell)
        {
            if (e->hasProperty(PropertyType::WIN))
            {
                return true;
            }
        }
    }
    return false;
}

void RuleManager::applyInteractions(Board &board)
{
    checkSink(board);
    checkOpenShut(board);
    checkDefeat(board);
    checkMelt(board);
}

void RuleManager::checkSink(Board &board)
{
    vector<Entity *> sinkEntity = board[PropertyType::SINK];
    for (Entity *sink : sinkEntity)
    {
        int x = sink->getX();
        int y = sink->getY();
        const std::vector<Entity *> &entitiesOnSameCell = board[{x, y}];
        for (Entity *e : entitiesOnSameCell)
        {
            if (sink != e && !(e->hasProperty(PropertyType::SINK)))
            {
                board.removeEntity(sink);
                board.removeEntity(e);
            }
        }
    }
}

void RuleManager::checkDefeat(Board &board)
{
    vector<Entity *> defeatEntity = board[PropertyType::DEFEAT];
    vector<Entity *> toKill;
    for (Entity *d : defeatEntity)
    {
        int x = d->getX();
        int y = d->getY();
        const vector<Entity *> &entitiesOnSameCell = board[{x, y}];
        for (Entity *e : entitiesOnSameCell)
        {
            if (e->hasProperty(PropertyType::YOU))
            {
                toKill.push_back(e);
            }
        }
    }
    for (Entity *e : toKill)
    {
        board.removeEntity(e);
    }
}

void RuleManager::checkMelt(Board &board)
{
    vector<Entity *> meltEntities = board[PropertyType::MELT];
    vector<Entity *> toKill;
    for (Entity *d : meltEntities)
    {
        int x = d->getX();
        int y = d->getY();
        const vector<Entity *> &entitiesOnSameCell = board[{x, y}];
        {
            for (Entity *e : entitiesOnSameCell)
            {
                if (e->hasProperty(PropertyType::HOT))
                {
                    toKill.push_back(d);
                    break;
                }
            }
        }
    }
    for (Entity *e : toKill)
    {
        board.removeEntity(e);
    }
}

void RuleManager::checkOpenShut(Board &board)
{
    vector<Entity *> shutEntity = board[PropertyType::SHUT];
    for (Entity *shut : shutEntity)
    {
        int x = shut->getX();
        int y = shut->getY();
        const std::vector<Entity *> &entitiesOnSameCell = board[{x, y}];
        for (Entity *e : entitiesOnSameCell)
        {
            if ((e->hasProperty(PropertyType::OPEN)))
            {
                board.removeEntity(shut);
                board.removeEntity(e);
            }
        }
    }
}
