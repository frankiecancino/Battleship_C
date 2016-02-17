/* 
 * File:   main.c
 * Author: Frankie Cancino
 * Homework 1
 * Created on June 12, 2015, 10:49 PM
 */


#include <stdio.h>
#include <stdlib.h> //for malloc and rand
#include <stdbool.h>
#include <time.h>   // to seed rand() function

#define BOARD_SIZE_ROW 10
#define BOARD_SIZE_COLUMN 10

const char WATER = '~';
const char HIT = '*';
const char MISS = 'x';
const char MYSTERY = '?';

const char CARRIER = 'c';
const char BATTLESHIP = 'b';
const char CRUISER = 'r';
const char SUBMARINE = 's';
const char DESTROYER = 'd';

typedef struct {
    int ROW;
    int COLUMN;
} Coordinates;

typedef enum {
    USER, COMPUTER, OPPONENT
} Player;

typedef enum {
    NORTH, SOUTH, EAST, WEST
} Direction;

typedef enum {
    CARRIER_LENGTH = 5,
    BATTLESHIP_LENGTH = 4,
    CRUISER_LENGTH = 3,
    SUBMARINE_LENGTH = 3,
    DESTROYER_LENGTH = 2
} ShipLength;

void init_game_board(char gameBoard[][BOARD_SIZE_COLUMN], const char initValue);
void place_ships_randomly(char gameBoard[][BOARD_SIZE_COLUMN]);
void place_ships_manually(char gameBoard[][BOARD_SIZE_COLUMN]);
bool check_ship_placement(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates startingPoint, Direction dir, ShipLength len);
void place_ship(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates startingPoint, Direction dir, const char shipType, ShipLength len);

void print_game_board(char gameBoard[][BOARD_SIZE_COLUMN], Player USER);

void acquire_strike_coordinates(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates* strikeAttempt);
bool check_strike_coords(int row, int col);

bool fire_missile(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates coord);
void update_game_board(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates coord, const char value);

int main(int argc, char** argv) {
    char userGameBoard[BOARD_SIZE_ROW][BOARD_SIZE_COLUMN];
    char computerGameBoard[BOARD_SIZE_ROW][BOARD_SIZE_COLUMN];
    char userGameState[BOARD_SIZE_ROW][BOARD_SIZE_COLUMN];
    
    int userHealth = 17;
    int computerHealth = 17;

    Coordinates strikeAttempt;
    bool didHit;

    srand(time(NULL));

    init_game_board(userGameBoard, WATER);
    init_game_board(userGameState, MYSTERY);
    init_game_board(computerGameBoard, WATER);

    //Randomly place computer ships.
    place_ships_randomly(computerGameBoard);
    
    //Manually place player ships.
    place_ships_manually(userGameBoard);

    //Take turns firing at each other until someone wins.
    do {
        //USER GOES FIRST!
        //Print Users Game Board
        print_game_board(userGameBoard, USER);
        //Print users view of opponents game board
        print_game_board(userGameState, OPPONENT);
        print_game_board(computerGameBoard, COMPUTER);

        //acquire AND DISPLAY the strike coordinates
        acquire_strike_coordinates(computerGameBoard, &strikeAttempt);
        printf("Strike coordinates are: %d, %d\n", strikeAttempt.ROW, strikeAttempt.COLUMN);
        
        //call fire_missile HINT: set the return value equal to didHit
        
        didHit = fire_missile(computerGameBoard, strikeAttempt);
            
        //If it was a hit then update the game boards accordingly. 
        if (didHit == true) {
            update_game_board(userGameState, strikeAttempt, HIT);
            update_game_board(computerGameBoard, strikeAttempt, HIT);
            computerHealth = computerHealth - 1;
        } else if(didHit == false) {
            //if not, update the game boards accordingly.
            update_game_board(userGameState, strikeAttempt, MISS);
            update_game_board(computerGameBoard, strikeAttempt, MISS);
        }
        // END USER TURN
        
        //store random values into strikeAttempt for computers turn.
        strikeAttempt.ROW = rand()%10;
        strikeAttempt.COLUMN = rand()%10;
        
        //call fire_missile HINT: set the return value equal to didHit
        
        didHit = fire_missile(userGameBoard, strikeAttempt);
            
        //If it was a hit then update the game boards accordingly. 
        if (didHit == true) {
            update_game_board(userGameBoard, strikeAttempt, HIT);
            userHealth = userHealth - 1;
        } else if(didHit == false){
            //if not, update the game boards accordingly.
            update_game_board(userGameBoard, strikeAttempt, MISS);
        }
        
        /* @TODO */

    } while (computerHealth > 0 && userHealth > 0);

    //When the code reaches this point, someone has won. 
    if (computerHealth == 0){
        printf("\n~~~Congratulations! You won!~~~\n");
        //Print Users game board one last time
        print_game_board(userGameBoard, USER);
        //print computers screen.
        print_game_board(computerGameBoard, COMPUTER);
    } else {
        printf("\n~~~Sorry! You lost!~~~\n");
        //Print Users game board one last time
        print_game_board(userGameBoard, USER);
        //print computers screen.
        print_game_board(computerGameBoard, COMPUTER);
    }
    
    


    return (EXIT_SUCCESS);
}

void update_game_board(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates coord, const char value) {
    gameBoard[coord.ROW][coord.COLUMN] = value;
}

bool fire_missile(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates coord) {
    //store the value of the game board at the position into a char for easy comparison.
    char strikeZone = gameBoard[coord.ROW][coord.COLUMN];

    //IF the strike zone is not water, miss, or hit then it is a ship.
    if(strikeZone == WATER || strikeZone == MISS)
        return false;
    else {
        return true;
    }
}

/**
 * This function places the value initValue into every cell of the gameboard. 
 * It will be either WATER or MYSTERY, and is passed in to this function.
 */
void init_game_board(char gameBoard[][BOARD_SIZE_COLUMN], const char initValue) {
    int i = 0, j = 0;
    
    for(i = 0; i < BOARD_SIZE_COLUMN; i++){
        for(j = 0; j < BOARD_SIZE_COLUMN; j++){
            gameBoard[i][j] = initValue;
        }
    }
}

void print_game_board(char gameBoard[][BOARD_SIZE_COLUMN], Player player) {
    int i, j;

    char* whichPlayer;
    if (player == USER) {
        whichPlayer = "Player";
    } else if (player == COMPUTER) {
        whichPlayer = "Computer";
    } else {
        whichPlayer = "Opponent";
    }

    printf("~~~ %s\'s game board ~~~\n\n", whichPlayer);

    //Add a 2 column padding for the row labels.
    printf("%2c", ' ');
    for (i = 0; i < BOARD_SIZE_ROW; i++) {
        printf("%2d", i);
    }
    printf("\n");


    for (i = 0; i < BOARD_SIZE_ROW; i++) {
        //at the beginning of every row, add a row label.
        printf("%2d", i);
        for (j = 0; j < BOARD_SIZE_COLUMN; j++) {
            //For each row, print the characters
            printf("%2c", gameBoard[i][j]);
        }
        //Drop to the next line
        printf("\n");
    }
}

void acquire_strike_coordinates(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates* strikeAttempt) {
    int row, col;
    bool coordsAreValid = false;

    do {
        printf("Please enter in the coordinates that you want to strike: \n");
        printf("Enter row: ");
        scanf("%d", &row);
        printf("Enter col: ");
        scanf("%d", &col);

        coordsAreValid = check_strike_coords(row, col);
        if (coordsAreValid == false) {
            printf("Sorry, those coordinates are invalid. Please try again.\n\n");
            print_game_board(gameBoard, USER);
        }

    } while (check_strike_coords(row, col) == false);

    strikeAttempt->ROW = row;
    strikeAttempt->COLUMN = col;
}

bool check_strike_coords(int row, int col) {

    if(row < 0 || row > 9)
        return false;
    else return true;
    
    if(col < 0 || col > 9)
        return false;
    else return true;
}

void place_ships_randomly(char gameBoard[][BOARD_SIZE_COLUMN]) {
    //Declare a startingPoint and randomly initialize the values. 
    Coordinates startingPoint;
    startingPoint.ROW = rand()%10;
    startingPoint.COLUMN = rand()%10;

    Direction dir = (Direction) rand() % 4;

    while (check_ship_placement(gameBoard, startingPoint, dir, CARRIER_LENGTH) != true) {
        //Change values and try again
        startingPoint.ROW = rand()%10;
        startingPoint.COLUMN = rand()%10;
    }

    place_ship(gameBoard, startingPoint, dir, CARRIER, CARRIER_LENGTH);

    while (check_ship_placement(gameBoard, startingPoint, dir, BATTLESHIP_LENGTH) != true) {
        //Change values and try again
        startingPoint.ROW = rand()%10;
        startingPoint.COLUMN = rand()%10;
    }

    place_ship(gameBoard, startingPoint, dir, BATTLESHIP, BATTLESHIP_LENGTH);

    while (check_ship_placement(gameBoard, startingPoint, dir, CRUISER_LENGTH) != true) {
        //Change values and try again
        startingPoint.ROW = rand()%10;
        startingPoint.COLUMN = rand()%10;
    }

    place_ship(gameBoard, startingPoint, dir, CRUISER, CRUISER_LENGTH);

    while (check_ship_placement(gameBoard, startingPoint, dir, SUBMARINE_LENGTH) != true) {
        //Change values and try again
        startingPoint.ROW = rand()%10;
        startingPoint.COLUMN = rand()%10;
    }

    place_ship(gameBoard, startingPoint, dir , SUBMARINE, SUBMARINE_LENGTH);

    while (check_ship_placement(gameBoard, startingPoint, dir, DESTROYER_LENGTH) != true) {
        //Change values and try again
        startingPoint.ROW = rand()%10;
        startingPoint.COLUMN = rand()%10;
    }

    place_ship(gameBoard, startingPoint, dir, DESTROYER, DESTROYER_LENGTH);

}

bool check_ship_placement(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates startingPoint, Direction dir, ShipLength len) {
    int i;

    //First, make sure placing the ship will not pass any of the borders
    if (dir == WEST) {
        if ((startingPoint.ROW - ((int) len) - 1) <= -1) {
            return false;
        }
        for (i = 0; i < len; i++) { 
            if (gameBoard[startingPoint.ROW][startingPoint.COLUMN - i] != WATER) {
                return false;
            }
        }

    } else if (dir == EAST) {
        if ((startingPoint.ROW + ((int) len) + 1) >= 10) {
            return false;
        }
        for (i = 0; i < len; i++) {
            if (gameBoard[startingPoint.ROW][startingPoint.COLUMN + i] != WATER) {
                return false;
            }
        }
    } else if (dir == NORTH) {
        if ((startingPoint.COLUMN - ((int) len) - 1) <= -1) {
            return false;
        }
        for (i = 0; i < len; i++) {
            if (gameBoard[startingPoint.ROW - i][startingPoint.COLUMN] != WATER) {
                return false;
            }
        }

    } else if (dir == SOUTH) {
        if ((startingPoint.COLUMN + ((int) len) + 1) >= 10) {
            return false;
        }

        for (i = 0; i < len; i++) {
            if (gameBoard[startingPoint.ROW + i][startingPoint.COLUMN] != WATER) {
                return false;
            }
        }
    }
    return true;

}

void place_ship(char gameBoard[][BOARD_SIZE_COLUMN], Coordinates startingPoint, Direction dir, const char shipType, ShipLength len) {
    int i = 0;
    
    if(dir == WEST){
        for(i = 0; i < len; i++){
            gameBoard[startingPoint.ROW][startingPoint.COLUMN - i] = shipType;
        }
    }
    else if(dir == EAST){
        for(i = 0; i < len; i++){
            gameBoard[startingPoint.ROW][startingPoint.COLUMN + i] = shipType;
        }
    }
    else if(dir == NORTH){
        for(i = 0; i < len; i++){
            gameBoard[startingPoint.ROW - i][startingPoint.COLUMN] = shipType;
        }
    }
    else if(dir == SOUTH){
        for(i = 0; i < len; i++){
            gameBoard[startingPoint.ROW + i][startingPoint.COLUMN] = shipType;
        }
    }
}

void place_ships_manually(char gameBoard[][BOARD_SIZE_COLUMN]) {
    //Declare a startingPoint for the ship.
    int Row = 0, Column = 0;
    Coordinates startingPoint;
    startingPoint.ROW = Row;
    startingPoint.COLUMN = Column;
    //Declare a direction that ship will be placed in.
    int tempDir; //user to read in direction using scanf because we have to type cast it to a direction. 
    Direction dir;

    // In this function, it is ok to use scanf again. I don't want you to worry about parsing.
    printf("\n\n~~~Placing ships!~~~\n\n");

    //Place Battleship
    print_game_board(gameBoard, USER);
    printf("Placing your carrier: \n");

    //acquire startingPoint coordinates. 
    printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
    scanf("%d", &Row);
    printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
    scanf("%d", &Column);
    startingPoint.ROW = Row;
    startingPoint.COLUMN = Column;
    //acquire ship direction
    printf("Directions: North(0), South(1), East(2), West(3)\n");
    printf("Please enter a direction for the ship to \"face\": ");
    scanf("%d", &tempDir);
    dir = (Direction) tempDir;

    //Keep asking the user for values until they place the battleship properly.
    while (check_ship_placement(gameBoard, startingPoint, dir, CARRIER_LENGTH) == false) {
        printf("Something was wrong with the starting coordinates and direction you entered. Please try again.\n");
        //Change values and try again
        print_game_board(gameBoard, USER);
        printf("Placing your carrier: \n");

        //acquire startingPoint coordinates. 
        printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
        scanf("%d", &Row);
        printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
        scanf("%d", &Column);

        //acquire ship direction
        printf("Directions: North(0), South(1), East(2), West(3)\n");
        printf("Please enter a direction for the ship to \"face\": ");
        scanf("%d", &tempDir);
        dir = (Direction) tempDir;
    }

    place_ship(gameBoard, startingPoint, dir, CARRIER, CARRIER_LENGTH);

    //Repeat this process for the rest of the ships
    print_game_board(gameBoard, USER);
    printf("Placing your battleship: \n");

    //acquire startingPoint coordinates. 
    printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
    scanf("%d", &Row);
    printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
    scanf("%d", &Column);
    startingPoint.ROW = Row;
    startingPoint.COLUMN = Column;
    //acquire ship direction
    printf("Directions: North(0), South(1), East(2), West(3)\n");
    printf("Please enter a direction for the ship to \"face\": ");
    scanf("%d", &tempDir);
    dir = (Direction) tempDir;

    //Keep asking the user for values until they place the battleship properly.
    while (check_ship_placement(gameBoard, startingPoint, dir, BATTLESHIP_LENGTH) == false) {
        printf("Something was wrong with the starting coordinates and direction you entered. Please try again.\n");
        //Change values and try again
        print_game_board(gameBoard, USER);
        printf("Placing your battleship: \n");

        //acquire startingPoint coordinates. 
        printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
        scanf("%d", &Row);
        printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
        scanf("%d", &Column);
        startingPoint.ROW = Row;
        startingPoint.COLUMN = Column;
        //acquire ship direction
        printf("Directions: North(0), South(1), East(2), West(3)\n");
        printf("Please enter a direction for the ship to \"face\": ");
        scanf("%d", &tempDir);
        dir = (Direction) tempDir;
    }

    place_ship(gameBoard, startingPoint, dir, BATTLESHIP, BATTLESHIP_LENGTH);

    print_game_board(gameBoard, USER);
    printf("Placing your cruiser: \n");

    //acquire startingPoint coordinates. 
    printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
    scanf("%d", &Row);
    printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
    scanf("%d", &Column);
    startingPoint.ROW = Row;
    startingPoint.COLUMN = Column;
    //acquire ship direction
    printf("Directions: North(0), South(1), East(2), West(3)\n");
    printf("Please enter a direction for the ship to \"face\": ");
    scanf("%d", &tempDir);
    dir = (Direction) tempDir;

    //Keep asking the user for values until they place the battleship properly.
    while (check_ship_placement(gameBoard, startingPoint, dir, CRUISER_LENGTH) == false) {
        printf("Something was wrong with the starting coordinates and direction you entered. Please try again.\n");
        //Change values and try again
        print_game_board(gameBoard, USER);
        printf("Placing your cruiser: \n");

        //acquire startingPoint coordinates. 
        printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
        scanf("%d", &Row);
        printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
        scanf("%d", &Column);
        startingPoint.ROW = Row;
        startingPoint.COLUMN = Column;
        //acquire ship direction
        printf("Directions: North(0), South(1), East(2), West(3)\n");
        printf("Please enter a direction for the ship to \"face\": ");
        scanf("%d", &tempDir);
        dir = (Direction) tempDir;
    }

    place_ship(gameBoard, startingPoint, dir, CRUISER, CRUISER_LENGTH);
    
    print_game_board(gameBoard, USER);
    printf("Placing your submarine: \n");

    //acquire startingPoint coordinates. 
    printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
    scanf("%d", &Row);
    printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
    scanf("%d", &Column);
    startingPoint.ROW = Row;
    startingPoint.COLUMN = Column;
    //acquire ship direction
    printf("Directions: North(0), South(1), East(2), West(3)\n");
    printf("Please enter a direction for the ship to \"face\": ");
    scanf("%d", &tempDir);
    dir = (Direction) tempDir;

    //Keep asking the user for values until they place the battleship properly.
    while (check_ship_placement(gameBoard, startingPoint, dir, SUBMARINE_LENGTH) == false) {
        printf("Something was wrong with the starting coordinates and direction you entered. Please try again.\n");
        //Change values and try again
        print_game_board(gameBoard, USER);
        printf("Placing your submarine: \n");

        //acquire startingPoint coordinates. 
        printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
        scanf("%d", &Row);
        printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
        scanf("%d", &Column);
        startingPoint.ROW = Row;
        startingPoint.COLUMN = Column;
        //acquire ship direction
        printf("Directions: North(0), South(1), East(2), West(3)\n");
        printf("Please enter a direction for the ship to \"face\": ");
        scanf("%d", &tempDir);
        dir = (Direction) tempDir;
    }

    place_ship(gameBoard, startingPoint, dir, SUBMARINE, SUBMARINE_LENGTH);
    
    print_game_board(gameBoard, USER);
    printf("Placing your destroyer: \n");

    //acquire startingPoint coordinates. 
    printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
    scanf("%d", &Row);
    printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
    scanf("%d", &Column);
    startingPoint.ROW = Row;
    startingPoint.COLUMN = Column;
    //acquire ship direction
    printf("Directions: North(0), South(1), East(2), West(3)\n");
    printf("Please enter a direction for the ship to \"face\": ");
    scanf("%d", &tempDir);
    dir = (Direction) tempDir;

    //Keep asking the user for values until they place the battleship properly.
    while (check_ship_placement(gameBoard, startingPoint, dir, DESTROYER_LENGTH) == false) {
        printf("Something was wrong with the starting coordinates and direction you entered. Please try again.\n");
        //Change values and try again
        print_game_board(gameBoard, USER);
        printf("Placing your battleship: \n");

        //acquire startingPoint coordinates. 
        printf("Please enter starting point row (0 - %d): ", BOARD_SIZE_ROW - 1);
        scanf("%d", &Row);
        printf("Please enter starting point column (0 - %d): ", BOARD_SIZE_COLUMN - 1);
        scanf("%d", &Column);
        startingPoint.ROW = Row;
        startingPoint.COLUMN = Column;
        //acquire ship direction
        printf("Directions: North(0), South(1), East(2), West(3)\n");
        printf("Please enter a direction for the ship to \"face\": ");
        scanf("%d", &tempDir);
        dir = (Direction) tempDir;
    }

    place_ship(gameBoard, startingPoint, dir, DESTROYER, DESTROYER_LENGTH);
}
