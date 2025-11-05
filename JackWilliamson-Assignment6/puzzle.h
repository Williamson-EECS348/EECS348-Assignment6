#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <optional>
#include <utility>

class Puzzle {
public:
    static constexpr int BOARD_SIZE = 9 * 9;
    typedef std::array<char, BOARD_SIZE> board_t;

public:
    Puzzle(const std::string& filename);

    void solve();

    friend std::ostream& operator<<(std::ostream& os, const Puzzle& puzzle);
private:

    void print(const board_t& puzzle) const;

    std::vector<board_t> solve(std::vector<board_t>& solutions);

    std::optional<int> findNextEmptyCell() const;

    int index(int row, int col) const;
    int index(std::pair<int, int> pos) const;

    std::pair<int, int> position(int index) const;

    bool isValid(std::pair<int, int> pos, char ch) const;
    bool isValid(int row, int col, char ch) const;

    void readFile(const std::string& filename);

    board_t puzzle;
};