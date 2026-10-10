#include "Game.hpp"
using namespace std;
#include <iostream>
int main()
{
    try
    {

        Game::getInstance()->run();
    }
    catch (const exception &e)
    {
        cerr << "ERROR : " << e.what() << endl;
        return EXIT_FAILURE;
    }
    return 0;
}