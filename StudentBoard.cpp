#include "Board.hpp"
#include "BoardValues.hpp"

#include <algorithm>
#include <iostream>

/**
 * Construct a new Board:: Board object
 */
Board::Board(){
    // create new grid and fill it with empties
    grid = new int[WIDTH * HEIGHT];
    std::fill(grid, grid + (WIDTH * HEIGHT), EMPTY);
}

/**
 * Construct a new Board:: Board object
 * 
 * Copy the values from board other
 */
Board::Board(const Board& other){
    // create new grid and copy other's grid into it
    grid = new int[WIDTH * HEIGHT];
    std::copy(other.grid, other.grid + (WIDTH * HEIGHT), grid);

    // copy other's visible property
    visible = other.visible;
}


Board& Board::operator=(const Board& other){
    // create new grid in memory and copy vals into it
    int* temp = new int[WIDTH * HEIGHT];
    std::copy(other.grid, other.grid + (WIDTH * HEIGHT), temp);

    // delete the grid reference and replace it with the new grid
    delete grid;
    grid = temp;

    // copy other's visible property
    visible = other.visible;

    // return the board reference
    return *this;
}

Board::~Board(){
    delete(grid);
}

/**
 * Sets the visibility status of the board
 * If the board is set to not visible, only hits and misses will print
 * If the board is set to visible, ships will print as well 
 */
void Board::setVisible(bool v){
    visible = v;
}

int& Board::Internal::operator[](int index) {
    // shift by width offset so the index is 0, 1, etc instead of 1, 2, etc
    index -= WIDTH_OFFSET;
    if (index >= WIDTH || index < 0) {
        // if the index is out of bounds, throw an out-of-range exception
        throw std::out_of_range(std::to_string(index) + " is out of the range of allowable column indices");
    }

    return _grid[index];
}

Board::Internal Board::operator[](int index) {
    // shift by height offset so the index is 0, 1, etc instead of 'A', 'B', etc
    index -= HEIGHT_OFFSET;
    if (index >= HEIGHT || index < 0) {
        // if the index is out of bounds, throw an out-of-range exception
        throw std::out_of_range(std::to_string(index) + " is out of the range of allowable row indices");
    }

    return Board::Internal(grid + (index * WIDTH));
}

/**
 * Overload the stream operator for ease of printing board
 */
std::ostream& operator<<(std::ostream& os, Board& b) {
    // print empty tab for header
    os << "\t";

    // print column headers
    for (int i = 1; i <= WIDTH; i++) {
        os << i << "\t";
    }

    // print header new line
    os << std::endl;

    // print header divider line
    os << "----------------------------------------------------------------------------------------" << std::endl;

    // loop through rows
    for (int i = HEIGHT_OFFSET; i < HEIGHT_OFFSET + HEIGHT; i++) {
        // print row header
        os << (char)i << "\t";

        // loop through cols
        for (int j = WIDTH_OFFSET; j < WIDTH_OFFSET + WIDTH; j++) {
            // print char at index [row][col]
            if (b.visible) {
                os << (char)b[i][j] << "\t";      
            } else {
                if (b[i][j] == EMPTY || b[i][j] == HIT || b[i][j] == MISS) {
                    os << (char)b[i][j] << "\t";
                } else {
                    os << " \t";
                }
            }    
        }

        // print newline
        os << std::endl;
    }

    // return the stream
    return os;
}

// count function placeholder -- not used
int Board::count() const {
    return -1;
}

// operater< function placeholder -- not used
bool Board::operator< (const Board& other){
    return false;
}
