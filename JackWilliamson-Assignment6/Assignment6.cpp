#include <iostream> // needed for std::cin and std::cout
#include <string> // needed for std::string

#include "puzzle.h" // the puzzle class which reads the file and solves

int main() {
    std::cout << "Enter the file name: "; // prompt user for file name
    std::string filename; // std::string to store the filename in
    std::cin >> filename; // get the filename from the user

    Puzzle puzzle{filename}; // instantiate the puzzle object with the file name
    std::cout << "Initial Puzzle:\n" << puzzle << '\n'; // print out the initial puzzle (using the Puzzle::operator<<)
    puzzle.solve(); // find all solutiuons to the puzzle and print them out
}