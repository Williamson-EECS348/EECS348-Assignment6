#include "puzzle.h" // the puzzle class definition

#include <fstream> // neeeded for std::ifstream to read the file
#include <stdexcept> // needed for execptions; used when reading the file fails
#include <sstream> // needed for std::stringstream

Puzzle::Puzzle(const std::string& filename) { // see `puzzle.h`
    readFile(filename); // read the file and populate the `puzzle` in the constructor
}

void Puzzle::solve() { // see `puzzle.h`
    std::vector<board_t> solutions; // make the solutions vector that will be used by the private solve method
    solve(solutions); // find all solutions; solutions are passed by ref so it will be mutated and populated by the solutions

    // check if there are any solutions
    if (solutions.empty()) {
        // if there are not any solutions inform the user
        std::cout << "No Solution Found!\n";
    } else {
        // if solution(s) are found
        std::cout << "Solution(s) found!\n"; // inform the user
        for (const auto& solution : solutions) { // loop through all solutions
            print(solution); // print out all the solution boards
        }
    }
    std::cout << std::endl; // print an extra new line (with std::endl) to make it look nicer and to flush stream
}

void Puzzle::solve(std::vector<board_t>& solutions) { // see `puzzle.h`
    std::optional<int> cell = findNextEmptyCell(); // find the next empty cell index and store it

    if (!cell) { // if there is no "next empty cell" then the whole puzzle must be filled validly
        solutions.push_back(puzzle); // add a copy of the puzzle to the valid solutions
        return; // stop checking
    }

    int cellVal = cell.value(); // we know the cell has a value (because of the above check) so we store it (because im lazy and dont want to type `cell.value()` every time (I could also make the argument its faster but thats so negligible its almost not worth mentioning, especially if I turn on any form of optimization))
    coordPair_t pos = position(cellVal); // find the coordinate pair of the position (since `isValid` uses a pair because its easier to do the calculations)
    std::string s{"123456789"}; // we make a string of all the possible values (a string has an iterator so it can be used in a for-each loop)
    for (const char& c : s) { // loop through each char of the string `s`
        if (isValid(pos, c)) { // if we can put the `char` there
            puzzle[cellVal] = c; // then put it there
            solve(solutions); // find the next valid solution

            puzzle[cellVal] = '_'; // revert the cell back (either to find a valid path or to start over and find the next valid solution)
        }
    }
}

std::optional<int> Puzzle::findNextEmptyCell() const { // see `puzzle.h`
    for (int i = 0; i < BOARD_SIZE; i++) { // loop through the entire board; we need the index term so we use a vanilla for-loop
        if (puzzle[i] == '_') // if the current index in the puzzle is empty (i.e. it is an underscore (_))
            return std::make_optional<int>(i); // make an optional int that contains the i-th index (which is the next empty cell)
    }
    return std::nullopt; // return an empty optional to the user (could user `return {};` but I think `nullopt` is clearer)
}

constexpr int Puzzle::index(int row, int col) const { // see `puzzle.h`
    return row * 9 + col; // formula to find the current index given a pair
}

constexpr int Puzzle::index(coordPair_t pos) const { // see `puzzle.h`
    return index(pos.first, pos.second); // call the `index(int, int)` method with the pair split open
}

constexpr Puzzle::coordPair_t Puzzle::position(int index) const { // see `puzzle.h`
    // the (discrete) inverse of of `index()`
    int row = index / 9; // find the current row based on the index
    int col = index % 9; // find the current column based on the index
    return coordPair_t{row, col}; // return the pair of the column and index to the user
}

bool Puzzle::isValid(int row, int col, char ch) const { // see `puzzle.h`
    constexpr int COL_ROW_SIZE = 9; // symbolic constant for the loop; basically is just how long one row or column of the puzzle is
    for (int i = 0; i < COL_ROW_SIZE; i++) { // loop through the length of the puzzle
        if (puzzle[index(row, i)] == ch) return false; // keeps the row constant while the column is checked to see if the column is valid; if at any point the current cell is `ch` then we know the column is invalid
        if (puzzle[index(i, col)] == ch) return false; // keeps the column constant while the row is checked to see if the row is valid; if at any point the current cell is `ch` then we know the row is invalid
    }

    constexpr int BOX_LENGTH = 3; // symbolic constant; basically is just how long one side of the box is
    /*
     * this looks dumb and pointless since we are dividing and then multiplying by the same number but since all values are ints
     * the division is actually floor division so:
     *      given a row or col ∈ {0, 1, 2, 3, 4, 5, 6, 7, 8} → {0, 0, 0, 3, 3, 3, 6, 6, 6}
     */
    int boxRow = (row / BOX_LENGTH) * BOX_LENGTH; // get the index of the top corner in the box
    int boxCol = (col / BOX_LENGTH) * BOX_LENGTH; // get the index of the top corner in the box
    for (int i = 0; i < BOX_LENGTH; i++) { // loop through all rows in the box
        for (int j = 0; j < BOX_LENGTH; j++) { // loop through all cols in the box
            if (puzzle[index(boxRow + i, boxCol + j)] == ch) return false; // if at any point one of the values in the box is equal to ch then the placement is invalid
        }
    }

    return true; // if we haven't run into an error condition then we can assume that the placement is valid
}

bool Puzzle::isValid(coordPair_t pos, char ch) const { // see `puzzle.h`
    return isValid(pos.first, pos.second, ch); // calls `isValid(int, int)` with the pair split apart
}

void Puzzle::readFile(const std::string& filename) { // see `puzzle.h`
    std::ifstream file(filename); // open the file at the specified file path
    if (!file.is_open()) { // if the file did not open
        throw std::runtime_error("ERROR: File failed to open properly!"); // if it didn't throw error
        return; // stop reading the file
    }

    std::string line; // string where each line will be stored
    int i = 0; // index
    while (std::getline(file, line)) { // continuously read in the line from the file
        std::stringstream ss(line); // convert the line into a string stream
        char ch; // create a char to store into
        while (ss >> ch) { // keep streaming the string into the char until we cant anymore
            puzzle[i] = ch; // add the char to the current index
            i++; // increment the index
        }
    }

    file.close(); // close the file since we are done with it
}

void Puzzle::print(const board_t& puzzle) const { // see `puzzle.h`
    // loop 9 times, once for each row
    for (int i = 0; i < 9; i++) {
        // if the row is a mutiple of 3 (i.e. 0, 3, 6) then it needs a cross bar
        if (i % 3 == 0) {
            std::cout << "+-------+-------+-------+\n";
        }

        // loop 9 more times, once for each column
        for (int j = 0; j < 9; j++) {
            // if the column is a multiple of 3 (i.e. 0, 3, 6) it needs a "|"
            if (j % 3 == 0)
                std::cout << "| ";
                // print the entry at that point in the puzzle
            std::cout << puzzle.at(index(i, j)) << " ";
        }
        // at the end of all the columns add a "|"
        std::cout << "|\n";
    }
    // at the end of all the rows add a cross bar
    std::cout << "+-------+-------+-------+\n";
}

std::ostream& operator<<(std::ostream& os, const Puzzle& puzzle) { // see `puzzle.h`
    // loop 9 times, once for each row
    for (int i = 0; i < 9; i++) {
        // if the row is a mutiple of 3 (i.e. 0, 3, 6) then it needs a cross bar
        if (i % 3 == 0) {
            os << "+-------+-------+-------+\n";
        }

        // loop 9 more times, once for each column
        for (int j = 0; j < 9; j++) {
            // if the column is a multiple of 3 (i.e. 0, 3, 6) it needs a "|"
            if (j % 3 == 0)
                os << "| ";
            // print the entry at that point in the puzzle
            os << puzzle.puzzle[puzzle.index(i, j)] << " ";
        }
        // at the end of all the columns add a "|"
        os << "|\n";
    }
    // at the end of all the rows add a cross bar
    os << "+-------+-------+-------+\n";

    return os;
}