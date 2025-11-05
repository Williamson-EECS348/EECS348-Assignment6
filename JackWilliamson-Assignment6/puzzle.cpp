#include "puzzle.h"

#include <fstream>
#include <stdexcept>
#include <string>
#include <sstream>

Puzzle::Puzzle(const std::string& filename) {
    readFile(filename);
}

void Puzzle::solve() {
    std::vector<board_t> solutions;
    solve(solutions);

    if (solutions.empty()) {
        std::cout << "No Solution Found!" << std::endl;
    } else {
        std::cout << "Solution(s) found!\n";
        for (const auto& solution : solutions) {
            print(solution);
        }
    }
    std::cout << std::endl;
}

std::vector<Puzzle::board_t> Puzzle::solve(std::vector<board_t>& solutions) {
    std::optional<int> cell = findNextEmptyCell();

    if (!cell.has_value()) {
        solutions.push_back(puzzle);
        return solutions;
    }

    std::pair<int, int> pos = position(cell.value());
    std::string s{"123456789"};
    for (const char c : s) {
        if (isValid(pos, c)) {
            puzzle[cell.value()] = c;
            solve(solutions);

            puzzle[cell.value()] = '_';
        }
    }
    return solutions;
}

std::optional<int> Puzzle::findNextEmptyCell() const {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (puzzle[i] == '_')
            return std::make_optional<int>(i);
    }
    return {};
}

int Puzzle::index(int row, int col) const { return row * 9 + col; }
int Puzzle::index(std::pair<int, int> pos) const { return index(pos.first, pos.second); }

std::pair<int, int> Puzzle::position(int index) const {
    int row = index / 9;
    int col = index % 9;
    return std::pair<int, int>{row, col};
}

bool Puzzle::isValid(int row, int col, char ch) const {
    constexpr int COL_ROW_SIZE = 9;
    for (int i = 0; i < COL_ROW_SIZE; i++) {
        if (puzzle[index(row, i)] == ch) return false;
        if (puzzle[index(i, col)] == ch) return false;
    }

    int boxRow = (row / 3) * 3;
    int boxCol = (col / 3) * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (puzzle[index(boxRow + i, boxCol + j)] == ch) return false;
        }
    }

    return true;
}

bool Puzzle::isValid(std::pair<int, int> pos, char ch) const { return isValid(pos.first, pos.second, ch); }

void Puzzle::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("ERROR: File failed to open properly!");
        return;
    }

    std::string line;
    int i = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        char ch;
        while (ss >> ch) {
            puzzle[i] = ch;
            i++;
        }
    }
}

void Puzzle::print(const board_t& puzzle) const {
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            std::cout << "+-------+-------+-------+\n";
        }

        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0)
                std::cout << "| ";
            std::cout << puzzle.at(index(i, j)) << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "+-------+-------+-------+\n";
}

std::ostream& operator<<(std::ostream& os, const Puzzle& puzzle) {
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            os << "+-------+-------+-------+\n";
        }

        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0)
                os << "| ";
            os << puzzle.puzzle[puzzle.index(i, j)] << " ";
        }
        os << "|\n";
    }
    os << "+-------+-------+-------+\n";

    return os;
}