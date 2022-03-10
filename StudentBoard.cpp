#include "Board.hpp"
#include "BoardValues.hpp"

#include <algorithm>
#include <iostream>

Board::Board(){
    grid = new int[WIDTH * HEIGHT];

    std::fill(grid, grid + (WIDTH * HEIGHT), EMPTY);
}

Board::Board(const Board& other){
    this->visible = other.visible;
    
}

Board& Board::operator=(const Board& other){
    return *this;
    }

Board::~Board(){
    delete(grid);
}

void Board::setVisible(bool v){
}

int& Board::Internal::operator[](int index) {
    // shift by width offset so the index is 0, 1, etc
    index -= WIDTH_OFFSET;
    if (index >= WIDTH) {
        throw std::out_of_range(std::to_string(index) + " is greater value than or equal to grid width of " + std::to_string(WIDTH));
    }

    return _grid[index];
}

Board::Internal Board::operator[](int index) {
    // shift by height offset so the index is 0, 1, etc instead of 'A', 'B', etc
    index -= HEIGHT_OFFSET;
    if (index >= HEIGHT) {
        throw std::out_of_range(std::to_string(index) + " is greater value than or equal to grid height of " + std::to_string(HEIGHT));
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
            os << (char)b[i][j] << "\t";            
        }

        // print newline
        os << std::endl;
    }

    // return the stream
    return os;
}

int Board::count() const{
    return 0;
}

bool Board::operator< (const Board& other){
    return false;
}
