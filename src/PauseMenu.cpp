#include "PauseMenu.hpp"

PauseMenu::PauseMenu(sf::RenderWindow &win) : window(win), selectedOption(0)
{
    font.loadFromFile("ressources/project_baba_font.ttf");
    const char *options[2] = {"Resume", "Main Menu"};
    for (int i = 0; i < 2; ++i)
    {
        menuItems[i].setFont(font);
        menuItems[i].setString(options[i]);
        menuItems[i].setCharacterSize(17);
        menuItems[i].setPosition(250.f, 100.f + i * 60.f);
    }
    updateMenu();
}

void PauseMenu::setRules(const std::vector<std::string> &r)
{
    rules = r;
}

void PauseMenu::display()
{
    bool running = true;
    while (running && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                running = false;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    if (selectedOption > 0)
                        --selectedOption;
                    updateMenu();
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedOption < 1)
                        ++selectedOption;
                    updateMenu();
                }
                else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return)
                {
                    running = false;
                }
            }
        }
        window.clear(sf::Color(30, 30, 30));

        // Draw rules
        float y = 230.f;
        for (size_t i = 0; i < rules.size(); ++i)
        {
            sf::Text ruleText(rules[i], font, 12);
            ruleText.setPosition(250.f, y);
            ruleText.setFillColor(sf::Color::White);
            window.draw(ruleText);
            y += 40.f;
        }

        // Draw menu options
        for (int i = 0; i < 2; ++i)
        {
            window.draw(menuItems[i]);
        }
        window.display();
    }
}

int PauseMenu::getSelectedOption() const
{
    return selectedOption;
}

void PauseMenu::updateMenu()
{
    for (int i = 0; i < 2; ++i)
    {
        if (i == selectedOption)
        {
            menuItems[i].setFillColor(sf::Color::White);
        }
        else
        {
            menuItems[i].setFillColor(sf::Color(100, 100, 100));
        }
    }
}