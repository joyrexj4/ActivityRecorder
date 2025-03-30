#include <ctime>
#include "activity.h"
#include <iostream>

void printCLI(int num)
{
    switch (num)
    {
    case 0:
        std::cout << "#\t1. View activity sessions\t#" << "\n";
        std::cout << "#\t2. Start activity\t\t#" << "\n";
        std::cout << "#\t3. Create new activity\t\t#" << "\n";
        std::cout << "\nPress (Q) to quit." << "\n";
        break;
    case 1:
        std::cout << "#\t View activities\t\t#" << "\n";
        break;
    case 2:
        break;
    case 3:
        std::cout << "#\t Enter activity name\t\t#" << "\n";
        break;
    default:
        std::cout << "Error loading CLI ..." << "\n";
        break;
    }
}
void clearConsole()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}
void selectionMenu()
{
    char userSelection;
    do
    {
        printCLI(0);
        std::cin >> userSelection;
        std::system("cls");
        switch (toupper(userSelection))
        {
        case '1':
            printCLI(1);
            viewActivity();
            break;
        case '2':
            printCLI(2);
            startActivity();
            break;
        case '3':
            printCLI(3);
            createActivity();
            break;
        case 'Q':
            std::cout << "Exiting the program ..." << "\n";
            std::exit(0);
        default:
            std::cout << "Error. Try again" << "\n";
            continue;
        }
    } while (true);
}

int main()
{
    selectionMenu();
    return 0;
}