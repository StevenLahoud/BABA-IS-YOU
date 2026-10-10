#include "LevelManager.hpp"

LevelManager::LevelManager() : currentLevelIndex(0) {
}

void LevelManager::loadLevels(const std::vector<std::string>& levelFiles) {
    levels.clear();
    for (size_t i = 0; i < levelFiles.size(); ++i) {
        levels.push_back(Level(levelFiles[i]));
    }
    currentLevelIndex = 0;
}


Level& LevelManager::getCurrentLevel() {
    return levels[currentLevelIndex];
}

void LevelManager::nextLevel() {
    if (currentLevelIndex + 1 < (int)levels.size()) {
        ++currentLevelIndex;
    }
}

void LevelManager::selectLevel(int index) {
    if (index >= 0 && index < (int)levels.size()) {
        currentLevelIndex = index;
    }
}

int LevelManager::getLevelCount() const {
    return (int)levels.size();
}

int LevelManager::getCurrentLevelIndex() const {
    return currentLevelIndex;
}
