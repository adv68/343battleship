#include "Board.hpp"
#include "BoardValues.hpp"

#include <algorithm>
#include <iostream>

Board::Board(){
    grid = new int[WIDTH * HEIGHT];
}

Board::Board(const Board& other){
    this->visible = other.visible;
    
}

Board& Board::operator=(const Board& other){
    
}

Board::~Board(){
    delete(grid);
}

void Board::setVisible(bool v){
}

int& Board::Internal::operator[](int index){
    // shift by width offset so the index is 0, 1, etc
    index -= widthOffset;
    if (index >= WIDTH) {
        throw std::out_of_range(std::to_string(index) + " is greater value than or equal to grid width of " + std::to_string(WIDTH));
    }

    return _grid[index];
}

Board::Internal Board::operator[](int index){
    // shift by height offset so the index is 0, 1, etc instead of 'A', 'B', etc
    index -= heightOffset;
    if (index >= HEIGHT) {
        throw std::out_of_range(std::to_string(index) + " is greater value than or equal to grid height of " + std::to_string(HEIGHT));
    }

    return Board::Internal(grid + (index * WIDTH));
}

std::ostream& operator<<(std::ostream& os, Board const& b){
    os << "\t";
    for (int i = 1; i <= WIDTH; i++) {
        os << i << "\t";
    }
    os << std::endl;
    os << "----------------------------------------------------------------------------------------" << std::endl;
    
    for (int i = 'A'; i < 'A' + HEIGHT; i++) {
        os << (char)i << "\t";
        for (int j = 1; j <= WIDTH; j++) {
            // Get indexing to work here
            //os << b << "\t";
        }
        os << std::endl;
    }

    return os;
}

int Board::count() const{
    return 0;
}

bool Board::operator< (const Board& other){
    return false;
}
