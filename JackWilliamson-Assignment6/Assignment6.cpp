#include <iostream>
#include <string>

#include "puzzle.h"

int main() {
    std::cout << "Enter the file name: \n";
    std::string filename;
    std::cin >> filename;

    Puzzle puzzle{filename};
    std::cout << "Initial Puzzle:\n" << puzzle << '\n';
    puzzle.solve();
}