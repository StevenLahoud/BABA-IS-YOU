#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP
#include <vector>
#include "Level.hpp"

class LevelManager {
private:
    std::vector<Level> levels;
    int currentLevelIndex;
public:
    LevelManager();
    void loadLevels(const std::vector<std::string>& levelFiles);
    Level& getCurrentLevel();
    void nextLevel();
    void selectLevel(int index);
    int getLevelCount() const;
    int getCurrentLevelIndex() const;
};

#endif