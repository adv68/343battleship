#include "Board.hpp"
#include "Direction.hpp"
#include "Game.hpp"
#include "Ship.hpp"
#include <iostream>
#include <random>
//http://coliru.stacked-crooked.com/a/c5b94870fdcd13f2
//random number generator reference code

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
 * Begin Game lets user and then computer setup boards then calls run()
 */
void Game::beginGame(){
	//welcome player to the game and set up ship boards
	std::cout << std::endl << "Welcome to Battleship!" << std::endl << std::endl;
	std::cout << "Board coordinates are in the form RowCol - A1, B4, etc." << std::endl << std::endl;
	player = Board();
	computer = Board();

	// define board visibility
	player.setVisible(true);
	computer.setVisible(false);

	//place the ships
	std::cout << "It's time to position your fleet..." << std::endl;
	placeShips();
	placeShipsPC();

	//run the game (alternate turns until win condition)
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
	bool validOrientation, validRow, validCol;

	// iterate through all ships to place them
	for (std::vector<Ship>::iterator iterator = ships.begin(); iterator != ships.end(); ++iterator) {
		// print the current board status
		std::cout << Game::player << std::endl;

		// get the current ship in the iterator
		Ship ship = *iterator;

		// loop until valid input
		while (true) {
			// print ask for orientation
			std::cout << "Enter orientation for " << ship << " (Vertical - V, Horizontal - H): ";

			// read orientation and ignore rest of input buffer
			std::cin >> orientation;
			std::cin.ignore();

			// print ask for coordinates
			std::cout << "Enter coordinate: ";

			// read row char and col num and ignore rest of input buffer
			std::cin >> row;
			std::cin >> col;
			std::cin.ignore();

			// reset validity testers
			validOrientation = false;
			validRow = false;
			validCol = false;

			// validate input
			// validate orientation
			if (orientation == 'H' || orientation == 'V') {
				validOrientation = true;
			} else {
				// if invalid orientation, tell the user
				std::cout << "Invalid Ship Orientation. Allowable values are 'H' and 'V'." << std::endl;
			}

			// validate row
			if (row >= HEIGHT_OFFSET && row < HEIGHT_OFFSET + HEIGHT) {
				validRow = true;
			} else {
				// if invalid row, tell the user
				std::cout << "Error: Row specified is not on the board. Allowable rows are " << (char)HEIGHT_OFFSET 
					<< " - " << (char)(HEIGHT_OFFSET + HEIGHT - 1) << std::endl;
			}

			// validate col
			if (col >= WIDTH_OFFSET && col < WIDTH_OFFSET + WIDTH) {
				validCol = true;
			} else {
				// if invalid col, tell the user
				std::cout << "Error: Column specified is not on the board. Allowable cols are " << WIDTH_OFFSET
					<< " - " << (WIDTH_OFFSET + WIDTH - 1) << std::endl;
			}

			// if input location is a valid input, try ship placement
			if (validOrientation && validRow && validCol) {
				// check if orientation and coords are valid ship placement
				if (place(col, row, orientation == 'H' ? HORIZONTAL : VERTICAL, ship, player)) {
					// loop through spaces of ship
					for (int i = 0; i < ship.getSpaces(); ++i) {
						// if the orientation is horizontal
						if (orientation == 'H') {
							// place ship char on board
							player[row][col + i] = ship.getChr();
						// if the orientation is vertical
						} else {
							// place ship char on board
							player[row + i][col] = ship.getChr();
						}
					}

					// since input is valid, break from while loop to continue to next ship
					break;
				} else {
					std::cout << "Invalid ship location. Try again." << std::endl;
				}
			}
		}
	}

	//show the player their board
	std::cout << player << std::endl;
	std::cout << "Player ships placed" << std::endl << std::endl;
}

/**
 * Handle the computer placing ships.
 */
void Game::placeShipsPC() {
	std::random_device randDev;
	std::mt19937 generator{randDev()};
	std::uniform_int_distribution<int> distrOrient(0, 1);
	std::uniform_int_distribution<int> distrRow(HEIGHT_OFFSET, HEIGHT_OFFSET + HEIGHT - 1);
	std::uniform_int_distribution<int> distrCol(WIDTH_OFFSET, WIDTH_OFFSET + WIDTH - 1);
  
	// iterate through all ships to place them
	for (std::vector<Ship>::iterator iterator = ships.begin(); iterator != ships.end(); ++iterator) {
		// get the current ship in the iterator
		Ship ship = *iterator;

		// loop until valid input
		while (true) {
			// get random orientation, row, and col
			char orientation;
			char row = (char)distrRow(generator);
			int col = distrCol(generator);
			if (distrOrient(generator) == 0) {
				orientation = 'H';
			} else {
				orientation = 'V';
			}
			
			// check if orientation and coords are valid ship placement
			// ignore else for computer - we don't want visible feedback in this scenario
			if (place(col, row, orientation == 'H' ? HORIZONTAL : VERTICAL, ship, computer)) {
				// loop through spaces of ship
				for (int i = 0; i < ship.getSpaces(); ++i) {
					// if the orientation is horizontal
					if (orientation == 'H') {
						// place ship char on board
						computer[row][col + i] = ship.getChr();
					// if the orientation is vertical
					} else {
						// place ship char on board
						computer[row + i][col] = ship.getChr();
					}
				}

				// since input is valid, break from while loop to continue to next ship
				break;
			} 
		}
	}
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
	//variables to count how many hits were made
	int humanHits = 0;
	int cpuHits = 0;

	// print beginning
	std::cout << "Beginning Game." << std::endl;

	//17 is the total number of ship spaces, if there are 17 hits for cpu or player, game over
	while(player.count() < 17 && computer.count() < 17) {
		// print boards and score
		std::cout << std::endl << "Player Board" << std::endl;
		std::cout << player << std::endl;
		std::cout << "Computer Board" << std::endl;
		std::cout << computer << std::endl;

		//call player turn
		Game::humanTurn();

		//check for player win by counting hits
		humanHits = 0;
		for (int i = HEIGHT_OFFSET; i < HEIGHT_OFFSET + HEIGHT; i++) {
        	for (int j = WIDTH_OFFSET; j < WIDTH_OFFSET + WIDTH; j++) {
				if(computer[i][j] == HIT){
					humanHits++;
				}
			}
		}
		
		// if human hits is 17 (or more) then the player won
		if(humanHits >= 17){
			std::cout << "Player won :)" << std::endl;
			break;
		}
		
		//call cpu turn
		Game::computerTurn();

		//check for cpu win by counting hits
		cpuHits = 0;
		for (int i = HEIGHT_OFFSET; i < HEIGHT_OFFSET + HEIGHT; i++) {
        	for (int j = WIDTH_OFFSET; j < WIDTH_OFFSET + WIDTH; j++) {
				if(player[i][j] == HIT){
					cpuHits++;
				}
			}
		}
		
		// if cpuHits is 17 (or more) then the computer won
		if(cpuHits >= 17){
			std::cout << "Computer won :(" << std::endl;
			break;
		}

		// print out the current score
		if (cpuHits > humanHits) {
			std::cout << "COMPUTER is winning " << cpuHits << " to " << humanHits << "." << std::endl;
		} else if (cpuHits < humanHits) {
			std::cout << "PLAYER is winning " << humanHits << " to " << cpuHits << "." << std::endl;
		} else {
			std::cout << "Game is TIED at " << cpuHits << "." << std::endl;
		}
	}
}

/**
 * Runs one human turn
 * 
 * This function will ask for user input, validate said input, and will not
 * return until a valid move has been made
 */
void Game::humanTurn() {
	// define vars for the shot
	char row;
	int col;
	bool playerShotValid = false;

	// print out info
	std::cout << "Player's turn" << std::endl;

	//ask player for a spot to hit until a valid coordinate is received
	while (playerShotValid == false) {
		// ask for coord
		std::cout << "Enter coordinate to attack:" << std::endl;

		// read coord from stdin and ignore the rest
		std::cin >> row;
		std::cin >> col;
		std::cin.ignore();

		// make sure coordinate is on board
		if (row >= 'A' && row <= 'J' && col >= 1 && col <= 10) {
		} else {
			std::cout << "Invalid Coordinate!" << std::endl;
		}

		// see if shot has already been taken
		if (Game::computer[row][col] != MISS && Game::computer[row][col] != HIT) {
			playerShotValid = true;
		}
		else{
			std::cout << "Coordinate already attacked! Try a different one" << std::endl;
		}
	}

	//update the shot board according to whether or not an enemy ship was at the position of the shot taken
	std::cout << std::endl << "Player taking shot at Pos: " << row << col << "!" << std::endl;
	if(Game::computer[row][col] == EMPTY){
		Game::computer[row][col] = MISS;
		std::cout << "Player: Shot was a MISS." << std::endl;
	}
	else{
		Game::computer[row][col] = HIT;
		std::cout << "Player: Shot was a HIT." << std::endl;
	}
}

/**
 * Runs one computer turn
 * 
 * This function will generate random shots, validate the shots, and will not
 * return until a valid move has been made
 */
void Game::computerTurn() {	
	//random number stuff
	std::random_device randDev;
	std::mt19937 generator{randDev()};
	std::uniform_int_distribution<int> distrRow(HEIGHT_OFFSET, HEIGHT_OFFSET + HEIGHT - 1);
	std::uniform_int_distribution<int> distrCol(WIDTH_OFFSET, WIDTH_OFFSET + WIDTH - 1);
    	
    //set shot taken to false until successful shot
	bool cpuShotTaken = false;

	while (cpuShotTaken == false){
		// generate random coordinate for shot
		char row = (char)distrRow(generator);
		int col = distrCol(generator);

		//check if the shot was taken already, if so go back and get new randoms
		if (Game::player[row][col] != MISS && Game::player[row][col] != HIT) {
			//since computerShots only logs shots taken, we can set all shots to MISS
			cpuShotTaken = true;

			//checking the player board for a hit compared to cpu shot board
			if(Game::player[row][col] != EMPTY){
				Game::player[row][col] = HIT;
				std::cout << "Computer: Shot was a HIT." << std::endl;
			}
			else{
				Game::player[row][col] = MISS;
				std::cout << "Computer: Shot was a MISS." << std::endl;
			}
		}
	}
}

/**
 * Create a game instance and start.
 */
int main(int argc, char** argv){
	(void)argc;
	(void)argv;
	Game g;

	g.beginGame();

	return 0;
}
