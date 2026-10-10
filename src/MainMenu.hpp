#ifndef MAINMENU_HPP
#define MAINMENU_HPP
#include <SFML/Graphics.hpp>

class MainMenu
{
private:
    sf::RenderWindow &window;
    int selectedOption;
    int selectedLevelIndex;
    int maxUnlocked = 1;
    std::vector<std::string> levelNames;
    sf::Font font;
    sf::Text menuItems[3];
    void updateMenu();
    void displayLevelSelect();

public:
    MainMenu(sf::RenderWindow &window);
    void display();
    int getSelectedOption() const; // 0: Start, 1: Level Select, 2: Exit
    int getSelectedLevelIndex() const;
    void setLevelNames(const std::vector<std::string> &names);
    void setUnlockedLevels(int count);
};

#endif