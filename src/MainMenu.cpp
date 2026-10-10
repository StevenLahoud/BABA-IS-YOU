#include "MainMenu.hpp"

MainMenu::MainMenu(sf::RenderWindow &win) : window(win), selectedOption(0), selectedLevelIndex(0)
{
    font.loadFromFile("ressources/project_baba_font.ttf");
    const char *options[3] = {"Start Game", "Level Select", "Exit"};
    for (int i = 0; i < 3; ++i)
    {
        menuItems[i].setString(options[i]);
        menuItems[i].setFont(font);
        menuItems[i].setCharacterSize(15);
        menuItems[i].setPosition(250.f, 300.f + i * 60.f);
    }
    updateMenu();
}

void MainMenu::display()
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
                    if (selectedOption < 2)
                        ++selectedOption;
                    updateMenu();
                }
                else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return)
                {
                    if (selectedOption == 1)
                    {
                        displayLevelSelect();
                        running = false;
                    }
                    else
                    {
                        running = false;
                    }
                }
            }
        }
        window.clear();
        const std::string titleStr = "Baba Is You";
        sf::Text titleText(titleStr, font, 30);
        titleText.setPosition(125.f, 180.f);
        titleText.setFillColor(sf::Color::White);
        window.draw(titleText);
        for (int i = 0; i < 3; ++i)
        {
            window.draw(menuItems[i]);
        }
        window.display();
    }
}
void MainMenu::displayLevelSelect()
{
    bool selecting = true;
    selectedLevelIndex = maxUnlocked - 1;
    while (selecting && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                selecting = false;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    if (selectedLevelIndex > 0)
                        --selectedLevelIndex;
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (selectedLevelIndex + 1 < maxUnlocked)
                        ++selectedLevelIndex;
                }
                else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return)
                {
                    selecting = false;
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    selecting = false;
                }
            }
        }
        window.clear();
        sf::Text titleText("Select Level", font, 28);
        titleText.setPosition(120.f, 120.f);
        titleText.setFillColor(sf::Color::White);
        window.draw(titleText);
        for (int i = 0; i < (int)levelNames.size(); ++i)
        {
            sf::Text levelText;
            levelText.setFont(font);
            levelText.setString(levelNames[i]);
            levelText.setCharacterSize(15);
            levelText.setPosition(130.f, 210.f + i * 50.f);
            if (i == selectedLevelIndex)
            {
                levelText.setFillColor(sf::Color::White);
            }
            else if (i < maxUnlocked - 1)
            {
                levelText.setFillColor(sf::Color::Green);
            }
            else
            {
                levelText.setFillColor(sf::Color(100, 100, 100));
            }
            window.draw(levelText);
        }
        window.display();
    }
}
void MainMenu::setLevelNames(const std::vector<std::string> &names)
{
    levelNames = names;
}

int MainMenu::getSelectedLevelIndex() const
{
    return selectedLevelIndex;
}

int MainMenu::getSelectedOption() const
{
    return selectedOption;
}

void MainMenu::updateMenu()
{
    for (int i = 0; i < 3; ++i)
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

void MainMenu::setUnlockedLevels(int count)
{
    maxUnlocked = count;
}
