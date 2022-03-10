#include "Board.hpp"
#include "Direction.hpp"
#include "Game.hpp"
#include "Ship.hpp"
#include <iostream>
#include <random>
//http://coliru.stacked-crooked.com/a/c5b94870fdcd13f2
//random number generator reference code

//summon some boards to record the shots taken
Board playerShots = Board();
Board computerShots = Board();

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
	//welcome player to the game and set up ship boards
	std::cout << "Welcome to Battleship!" << std::endl << std::endl;
	player = Board();
	computer = Board();

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
				std::cout << "Invalid placement. Try again." << std::endl;
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
void Game::placeShipsPC(){
	//random number stuff
	const int range_from  = 0;
    	const int range_to    = 10;
    	std::random_device                  rand_dev;
    	std::mt19937                        generator(rand_dev());
    	std::uniform_int_distribution<int>  distr(range_from, range_to);
  
	std::cout << "Placing CPU ships..." << std::endl;
	// iterate through all ships to place them
	for (std::vector<Ship>::iterator iterator = ships.begin(); iterator != ships.end(); ++iterator) {
		// commented out because we don't want player's to see enemy ships
		// print the current board status
		// std::cout << Game::computer << std::endl;

		// get the current ship in the iterator
		Ship ship = *iterator;

		// loop until valid input
		while (true) {
			//setting up orientation, row, and col for place() function
			char orientation;
			//get a random number and change it to a letter for row
			int rowInt = distr(generator);
			char rowChar = 'Z';
			if(rowInt == 1){
				rowChar = 'A';
			}
			if(rowInt == 2){
				rowChar = 'B';
			}
			if(rowInt == 3){
				rowChar = 'C';
			}
			if(rowInt == 4){
				rowChar = 'D';
			}
			if(rowInt == 5){
				rowChar = 'E';
			}
			if(rowInt == 6){
				rowChar = 'F';
			}
			if(rowInt == 7){
				rowChar = 'G';
			}
			if(rowInt == 8){
				rowChar = 'H';
			}
			if(rowInt == 9){
				rowChar = 'I';
			}
			if(rowInt == 10){
				rowChar = 'J';
			}
			//get a random column
			int col = distr(generator);
			//turn a random number into the orientation selector
			if(distr(generator)%2==1){
				orientation = 'H';
			}
			else{
				orientation = 'V';
			}
			
			// check if orientation and coords are valid ship placement
			if (place(col, rowChar, orientation == 'H' ? HORIZONTAL : VERTICAL, ship, computer)) {
				// loop through spaces of ship
				for (int i = 0; i < ship.getSpaces(); ++i) {
					// if the orientation is horizontal
					if (orientation == 'H') {
						// place ship char on board
						computer[rowChar][col + i] = ship.getChr();
					// if the orientation is vertical
					} else {
						// place ship char on board
						computer[rowChar + i][col] = ship.getChr();
					}
				}

				// since input is valid, break from while loop to continue to next ship
				break;
			} else {
				//commented out because player doesn't need to know about cpu ship placement
				//std::cout << "Invalid cpu placement. Try again." << std::endl;
			}
		}
	}
	
	std::cout << "CPU ships placed" << std::endl << std::endl;
	// commented out because we don't want player's to see enemy ships
	// print the current board status
	//std::cout << Game::computer << std::endl;
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
	int humanHits;
	int cpuHits;
	//17 is the total number of ship spaces, if there are 17 hits for cpu or player, game over
	while(humanHits != 17 && cpuHits != 17){
		//call player turn
		Game::humanTurn();
		//check for player win by counting hits
		humanHits = 0;
		for (int i = HEIGHT_OFFSET; i < HEIGHT_OFFSET + HEIGHT; i++) {
        		for (int j = WIDTH_OFFSET; j < WIDTH_OFFSET + WIDTH; j++) {
				if(playerShots[i][j]==HIT){
					humanHits++;
				}
			}
		}
		if(humanHits==17){
			std::cout << "Player won :)" << std::endl;
			break;
		}
		
		//call cpu turn
		Game::computerTurn();

		//check for cpu win by counting hits
		cpuHits = 0;
		for (int i = HEIGHT_OFFSET; i < HEIGHT_OFFSET + HEIGHT; i++) {
        		for (int j = WIDTH_OFFSET; j < WIDTH_OFFSET + WIDTH; j++) {
				if(Game::player[i][j]==HIT){
					cpuHits++;
				}
			}
		}
		if(cpuHits==17){
			std::cout << "Computer won :(" << std::endl;
			break;
		}
	}
}

void Game::humanTurn(){
	char row;
	int col;

	//show previous shots
	std::cout << "Player's turn" << std::endl;
	std::cout << "Previous shot board: " << std::endl << playerShots << std::endl;

	bool playerShotValid = false;

	//ask player for a spot to hit until a valid coordinate is received
	while (playerShotValid == false) {
		std::cout << "Enter coordinate to attack:" << std::endl;

		std::cin >> row;
		std::cin >> col;
		std::cin.ignore();

		if (row >= 'A' && row <= 'J' && col >= 1 && col <= 10) {
		} else {
			std::cout << "Invalid Coordinate!" << std::endl;
		}
		if(playerShots[row][col]!=MISS && playerShots[row][col]!=HIT){
			playerShotValid = true;
		}
		else{
			std::cout << "Coordinate already attacked! Try a different one" << std::endl;
		}
	}

	//update the shot board according to whether or not an enemy ship was at the position of the shot taken
	std::cout << std::endl << "Player taking shot at Pos: " << row << col << "!" << std::endl;
	if(Game::computer[row][col] == 0){
		playerShots[row][col] = MISS;
	}
	else{
		playerShots[row][col] = HIT;
	}
	
	//show the updated shot board
	std::cout << "Updated shot board: " << std::endl;
	std::cout << playerShots << std::endl;
}

void Game::computerTurn(){
	//print statements declaring cpu turn, etc.
	std::cout << "CPU's turn" << std::endl;
	std::cout << "Your current board: " <<std::endl;
	std::cout << Game::player << std::endl;
	std::cout << "CPU taking shot..." << std::endl;
	
	//random number stuff
	const int range_from  = 1;
    	const int range_to    = 10;
    	std::random_device                  rand_dev;
    	std::mt19937                        generator(rand_dev());
    	std::uniform_int_distribution<int>  distr(range_from, range_to);
    	
    	//set shot taken to false until successful shot
	bool cpuShotTaken = false;
	while (cpuShotTaken == false){
		//get a random number and change it to a letter for row
        	int rowInt = distr(generator);
        	char rowChar = 'Z';
        	if(rowInt == 1){
        		rowChar = 'A';
        	}
        	if(rowInt == 2){
        		rowChar = 'B';
        	}
        	if(rowInt == 3){
        		rowChar = 'C';
        	}
        	if(rowInt == 4){
        		rowChar = 'D';
        	}
        	if(rowInt == 5){
        		rowChar = 'E';
        	}
        	if(rowInt == 6){
        		rowChar = 'F';
        	}
        	if(rowInt == 7){
        		rowChar = 'G';
        	}
        	if(rowInt == 8){
        		rowChar = 'H';
        	}
        	if(rowInt == 9){
        		rowChar = 'I';
        	}
        	if(rowInt == 10){
        		rowChar = 'J';
        	}
        	//get a random column
        	int col = distr(generator);
        	//check if the shot was taken already, if so go back and get new randoms
        	if(computerShots[rowChar][col] != MISS){
        		//since computerShots only logs shots taken, we can set all shots to MISS
        		computerShots[rowChar][col] = MISS;
        		cpuShotTaken = true;

        		//checking the player board for a hit compared to cpu shot board
        		if(Game::player[rowChar][col] != 0){
        			Game::player[rowChar][col] = HIT;
        		}
        		else{
        			Game::player[rowChar][col] = MISS;
        		}
        	}
        }
	
	//show player their new board
	std::cout << "Your board after CPU turn: " <<std::endl;
	std::cout << Game::player << std::endl;
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
