#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PauseMenu {
public:
    PauseMenu(sf::RenderWindow& window);
    void setRules(const std::vector<std::string>& rules);
    void display();
    int getSelectedOption() const; // 0: Resume, 1: Main Menu
private:
    sf::RenderWindow& window;
    sf::Font font;
    std::vector<std::string> rules;
    int selectedOption;
    sf::Text menuItems[2];
    void updateMenu();
};

#endif