#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <string>
#include <vector>

class Board;

class Level {
public:
    Level(const std::string& filename);
    // Loads the level from file and populates the given board
    bool load(Board& board) const;
    const std::string& getFilename() const;
private:
    std::string filename;
};

#endif