#include "Board.hpp"
#include "Direction.hpp"
#include "Game.hpp"
#include "Ship.hpp"
#include <iostream>
#include <random>

/**
 * Constructor will create the ships vector and add ships to it.
 */
Game::Game(){
	// add the five battleships to the ships vector
	ships.push_back(Ship(2, "Patrol Boat", PATROLBOAT));
	ships.push_back(Ship(3, "Destroyer", DESTROYER));
	ships.push_back(Ship(3, "Submarine", SUBMARINE));
	ships.push_back(Ship(4, "Battleship", BATTLESHIP));
	ships.push_back(Ship(5, "Aircraft Carrier", CARRIER));
}

/**
 * Begin Game let's user and then computer setup boards then calls run()
 */
void Game::beginGame(){
	player = Board();
	computer = Board();

	placeShips();
	placeShipsPC();

	run();
}

/**
 * Handle the human placing ships.
 * 
 * Loops through the vector of ships and asks the user where to place each ship
 * User must supply an orientation - Horizontal (H) or Vertical (V) and a
 * coordinate in the form A1 where 'A' is the row and '1' is the column
 */
void Game::placeShips(){
	// declare variables
	char orientation, row;
	int col;

	// iterate through all ships to place them
	for (std::vector<Ship>::iterator iterator = ships.begin(); iterator != ships.end(); ++iterator) {
		// print the current board status
		std::cout << player << std::endl;

		// get the current ship in the iterator
		Ship ship = *iterator;

		// loop until valid input
		while (true) {
			// print ask for orientation
			std::cout << "Enter orientation (Vertical - V, Horizontal - H): ";

			// read orientation and ignore rest of input buffer
			std::cin >> orientation;
			std::cin.ignore();

			// print ask for coordinates
			std::cout << "Enter coordinate: ";

			// read row char and col num and ignore rest of input buffer
			std::cin >> row;
			std::cin >> col;
			std::cin.ignore();

			// check if orientation and coords are valid ship placement
			if (place(col, row, orientation == 'H' ? HORIZONTAL : VERTICAL, ship, player)) {
				// loop through spaces of ship
				for (int i = 0; i < ship.getSpaces(); ++i) {
					// if the orientation is horizontal
					if (orientation == 'H') {
						// place ship char on board
						player[row][col + i] = ship.getChr();
					// if the orientatino is vertical
					} else {
						// place ship char on board
						player[row + i][col] = ship.getChr();
					}
				}

				// since input is valid, break from while loop to continue to next ship
				break;
			} else {
				std::cout << "Invalid placement. Try again." << std::endl;
			}
		}
	}

	std::cout << player << std::endl;
}

/**
 * Handle the computer placing ships.
 */
void Game::placeShipsPC(){
}

/**
 * Helper method that checks if it is safe to put a ship
 * at a particular spot with a particular direction.
 * 
 * Takes the coordinates for first row (y) and first col (x), the orientation (Direction d),
 * the ship to try to place (Ship s), and the board to check on (Board b)
 */
bool Game::place(const int& x, const int& y, Direction d, const Ship& s, Board& b){
	// if oriention is horizontal
	if (d == HORIZONTAL) {
		// if coordinate is valid given the ship size
		if (y >= 'A' && y <= 'J' && x >= 1 && (x + s.getSpaces() - 1) <= 10) {
			// loop through squares where ship would be placed
			for (int i = 0; i < s.getSpaces(); i++) {
				// check if each square is empty
				if (b[y][x + i] != EMPTY) {
					// if one is not empty, place is not safe so return false
					return false;
				}
			}
			
			// if all squares are empty, the loop will exit without returning and we can return true
			return true;
		}
	// if orientation is vertical
	} else {
		// if coordinate is valid given the ship size
		if (y >= 'A' && (y + s.getSpaces() - 1) <= 'J' && x >= 1 && x <= 10) {
			// loop through squares where ship would be placed
			for (int i = 0; i < s.getSpaces(); i++) {
				// check if each square is empty
				if (b[y + i][x] != EMPTY) {
					// if one is not empty, place is not safe so return false;
					return false;
				}
			}

			// if all squares are empty, the loop will exit without returning and we can return true
			return true;
		}
	}

	// if we haven't returned then one of the checks failed, so return false
	return false;
}

/**
 * Call human turn/computer turn until someone wins.
 */
void Game::run(){
	
}

void Game::humanTurn(){
	char row;
	int col;

	std::cout << Game::player << std::endl;

	while (true) {
		std::cout << "Enter coordinate to attack:" << std::endl;

		std::cin >> row;
		std::cin >> col;
		std::cin.ignore();

		if (row >= 'A' && row <= 'J' && col >= 1 && col <= 10) {
			break;
		} else {
			std::cout << "Invalid Coordinate!" << std::endl;
		}
	}

	std::cout << "Pos: " << row << col << std::endl;
}

void Game::computerTurn(){
}

/**
 * Create a game instance and start.
 */
int main(int argc, char** argv){
	(void)argc;
	(void)argv;
	Game g;

	g.beginGame();

	g.humanTurn();

	return 0;
}
