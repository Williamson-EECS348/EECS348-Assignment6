#pragma once // header gaurd

#include <iostream> // needed for std::cout
#include <array> // needed for std::array (I wanted to use a std::array since I know how big a board will be plus the other helper funtions)
#include <vector> // needed for std::vector (this is here for storing the solutions since there can be [0, 6.671 x 10^21) solutions)
#include <string> // needed for std::string
#include <optional> // needed for std::optional (for optional return values (better than returning an invalid index))
#include <utility> // needed for std::pair (for coordinate pairs)

/**
 * Puzzle class reads a file, stores a puzzle, and finds all solutions
 *  Would probably be better practice to move the reader out to another class
 *  but since this is just for an assignment it's fine
 */
class Puzzle {
public: // I put two public tags just to add some partition between functionality and these typedefs and consts
    static constexpr int BOARD_SIZE = 9 * 9; // constexpr means that this can and will be evaluated at compile time; basically just is giving a name to a magic number
    typedef std::array<char, BOARD_SIZE> board_t; // typedef the board array so its shorter and more clear what it is
    typedef std::pair<int, int> coordPair_t; // typedef the coordinate pair so its a bit shorter and a bit more clear what its used for

public:
    /**
     * @brief Construct a new Puzzle object
     * @param filepath the path to the file which holds the puzzle
     *      e.g. 'puzzle5.txt'
     */
    Puzzle(const std::string& filepath);

    /**
     * @brief Finds all solutions to the file stored in the file given in the contructor and prints them all out
     */
    void solve();

    /**
     * @brief allows the Puzzle to be easily printed by std::cout, etc.
     *      e.g. `std::cout << puzzle << std::endl;`
     */
    friend std::ostream& operator<<(std::ostream& os, const Puzzle& puzzle);
private:

    /**
     * @brief prints a `board_t` with nice formatting
     * @param puzzle the `board_t` to be printed
     */
    void print(const board_t& puzzle) const; // `const` is a promise that the method makes no changes to class; allows the method to be used when the object is const (e.g. `const Puzzle p`)

    /**
     * @brief Finds solutions to the `puzzle` and inserts them into the `solutions` vector
     * @note is called recursively
     * @param solutions vector of all solutions to the `puzzle`
     */
    void solve(std::vector<board_t>& solutions);

    /**
     * @brief Finds the index of the next empty cell in the `puzzle`
     * @return std::optional<int> index to the next empty cell; if none is found the optional will be empty
     */
    std::optional<int> findNextEmptyCell() const;

    /**
     * @brief gives the single index from a row and column coordinate pair
     */
    constexpr int index(int row, int col) const;    // these two methods are `constexpr` so that they are evaluated at compile time; since they are just little formulas this is fine
    constexpr int index(coordPair_t pos) const;     // these two methods are `constexpr` so that they are evaluated at compile time; since they are just little formulas this is fine

    /**
     * @brief given a single index in `puzzle` get the (row, column) coordinate pair
     * @param index the index for the wanted coordinate pair 
     * @return coordPair_t the coordinate pair related to the given index
     */
    constexpr coordPair_t position(int index) const; // this method is `constexpr` so it is evaluated at compile time; since it is just a little formula this is fine

    /**
     * @brief Checks if a given `coorPair_t` is a valid place for the `char ch`
     * @param pos the `coordPair_t` to check
     * @param ch the `char` that is wished to be placed in the `coordPair_t`
     * @return true if `ch` may be placed in the specified `coordPair_t`;
     * @return false if `ch` may not be placed in the specified `coordPair_t`
     */
    bool isValid(coordPair_t pos, char ch) const;

    /**
     * @brief Checks if a given some (`row`, `col`) pair is a valid place for the `char ch`
     * @param row the row part of the pair
     * @param col the col part of the pair
     * @param ch the `char` that is wished to be placed in the (`row`, `col`) pair
     * @return true if `ch` may be placed in the specified (`row`, `col`) pair;
     * @return false if `ch` may not be placed in the specified (`row`, `col`) pair
     */
    bool isValid(int row, int col, char ch) const;

    /**
     * @brief Reads the given file and loads it into `puzzle`
     * @param filepath string path to the file which should be read
     */
    void readFile(const std::string& filepath);

    // the member variable of the puzzle
    board_t puzzle;
};