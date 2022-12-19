#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;

const int boardXsize = 10;
const int boardYsize = 21;
const int StringMaxSize = 150;
const int maxShipClassCount = 40;
const int carrierLength = 5;
const int battleshipLength = 4;
const int cruiserLength = 3;
const int destroyerLength = 2;
const int ShipClassWord = 3;

struct ship {
    int shipClass; //5-carrier 4 - battleship 3 - cruiser 2 - destroyer
    int shiplength;
    int shipNoseXposition;
    int shipNoseYposition;
    int shipPlaced;
    int shipNumber;
    int shipParts;
    char direction;
    int directionX;
    int directionY;
    char playerName;
};



struct player {
    char playername;
    int shipPartsAlive;
    int carriers;
    int battleships;
    int cruisers ;
    int destroyers;
    int shipcount ;
    int shippresent;
    int fleetcountarray;
    int shipsplaced;
    int borderUP;
    int borderDOWN;
};

ship CreateShip(player& Player, int shipNumber, int shipLength);                                                                                        //Function setting up ships data for CreatePlayerFleet function 
char Shooting(char gameboard[boardYsize][boardXsize], int X, int Y);                                                                                    //handles shooting
void InitializeBoard(char tab[boardYsize][boardXsize], int Xsize, int Ysize);                                                                           // First setup of array
void BasicPrintBoard(char tab[boardYsize][boardXsize], int Xsize, int Ysize, player& PlayerA, player& PlayerB);                                         // Basic array print
void SetFleet(player& Player);                                                                                                                          //Setting custom numbers of ships
void CreatePlayerFleet(player& Player, ship FleetArray[maxShipClassCount]);                                                                             //Setting up players fleet
void InitializeBoard2(char tab[boardYsize][boardXsize], int Xsize, int Ysize, ship FleetArrayA[maxShipClassCount], ship FleetArrayB[maxShipClassCount], player& PlayerA, player& PlayerB, char ShipClass[]);    //Placing ships on the board
void checkingTilesPlacing(int Xtile, int Ytile, player& Player, char Direction, int shipNumber, char ShipClass[]);                                      //First check if the X and Y are on the board
void WinningCondition(player& PlayerA, player& PlayerB);                                                                                                //checks if player won
int deadCounting(char board[boardYsize][boardXsize], int Ysize, int Xsize, player& Player);                                                             //counts how many parts for a given player is left
int checkingTilePlacingOnBoard(int Xsize, int Ysize, char Direction, int Ytile, int Xtile, int shipNumber, player& Player, char ShipClass[]);           //checks the tiles while setting up the ship 
int FindShip(int ShipNumber, char ShipClassName[ShipClassWord], player& playingPlayer, ship FleetArray[maxShipClassCount]);                             //Looking for specified(class, ship number) in players fleet //Returns array position
int ShipClassHandler(char ShipClassName[ShipClassWord]);                                                                                                //Changes char class names into numbers for easier handling
void InitializePlayer(player& Player, char Playername, int borderUP, int borderDown);                                                                   //Setups player data




int main()
{
    player PlayerA, PlayerB;
    char command[StringMaxSize];
    char startingPlayer = 'A';
    char fleetboard[boardYsize][boardXsize];
    ship FleetArrayA[maxShipClassCount];
    ship FleetArrayB[maxShipClassCount];
    int playersTurn = 0;

    InitializePlayer(PlayerA, 'A', 0, 10);
    InitializePlayer(PlayerB, 'B', 10, 21);
    CreatePlayerFleet(PlayerA, FleetArrayA);
    CreatePlayerFleet(PlayerB, FleetArrayB);

    while (cin >> command) {
        if (strcmp(command, "[state]") == 0) {
            while (cin >> command) {
                if (strcmp(command, "PRINT") == 0) {
                    int printtype;
                    cin >> printtype;
                    cin >> command;
                    if (strcmp(command, "[state]") == 0) {
                        PlayerA.shipPartsAlive = deadCounting(fleetboard, boardYsize, boardXsize, PlayerA);
                        PlayerB.shipPartsAlive = deadCounting(fleetboard, boardYsize, boardXsize, PlayerB);
                        BasicPrintBoard(fleetboard, boardXsize, boardYsize, PlayerA, PlayerB);
                    }
                }
                if (strcmp(command, "SET_FLEET") == 0) {
                    char player;
                    cin >> player;
                    if (player == 'A') {
                        SetFleet(PlayerA);
                    }
                    if (player == 'B') {
                        SetFleet(PlayerB);
                    }

                }
                if (strcmp(command, "[state]") == 0) {
                    break;
                }
            }
        }



        if (strcmp(command, "[playerA]") == 0) {
            if (playersTurn == 0) {
                while (cin >> command) {
                    if (strcmp(command, "PLACE_SHIP") == 0) {
                        int X, Y, shipNumber;
                        char Direction;
                        char ShipClassName[ShipClassWord];
                        cin >> Y >> X >> Direction >> shipNumber >> ShipClassName;
                        checkingTilesPlacing(X, Y, PlayerA, Direction, shipNumber, ShipClassName);
                        int ShipFound = FindShip(shipNumber, ShipClassName, PlayerA, FleetArrayA);
                        int shipPlaced = FleetArrayA[ShipFound].shipPlaced;
                        int ClassExceeded = FindShip(shipNumber, ShipClassName, PlayerA, FleetArrayA);

                        if (ClassExceeded == -1) {
                            cout << "INVALID OPERATION \"PLACE_SHIP " << Y << " " << X << " " << Direction << " " << shipNumber << " " << ShipClassName << "\"" << ": ALL SHIPS OF THE CLASS ALREADY SET";
                            return 0;
                        } if (shipPlaced == 1) {
                            cout << "INVALID OPERATION \"PLACE_SHIP " << Y << " " << X << " " << Direction << " " << shipNumber << " " << ShipClassName << "\"" << ": SHIP ALREADY PRESENT";
                            return 0;
                        }

                        FleetArrayA[ShipFound].direction = Direction;
                        FleetArrayA[ShipFound].shipPlaced = 1;
                        FleetArrayA[ShipFound].shipNoseXposition = X;
                        FleetArrayA[ShipFound].shipNoseYposition = Y;
                        PlayerA.shippresent++;
                        PlayerA.shipsplaced++;
                        int ShipLength = FleetArrayA[ShipFound].shiplength;
                        PlayerA.shipPartsAlive += ShipLength;
                        InitializeBoard2(fleetboard, boardXsize, boardYsize, FleetArrayA, FleetArrayB, PlayerA, PlayerB, ShipClassName);
                    }
                    if (strcmp(command, "SHOOT") == 0) {
                        int Xposition, Yposition;
                        cin >> Yposition >> Xposition;
                        if ((Xposition < 0) || (Yposition < 0) || (Yposition > boardYsize) || (Xposition > boardXsize)) {
                            cout << "INVALID OPERATION \"SHOOT " << Yposition << " " << Xposition << "\"" << ": FIELD DOES NOT EXIST";
                        }
                        else if (PlayerA.shippresent != PlayerA.fleetcountarray || PlayerB.shippresent != PlayerB.fleetcountarray) {
                            cout << "INVALID OPERATION \"SHOOT " << Yposition << " " << Xposition << "\"" << ": NOT ALL SHIPS PLACED";
                            exit(0);
                        }
                        else {
                            fleetboard[Yposition][Xposition] = Shooting(fleetboard, Xposition, Yposition);
                        }
                        PlayerA.shipPartsAlive = deadCounting(fleetboard, boardYsize, boardXsize, PlayerA);
                        PlayerB.shipPartsAlive = deadCounting(fleetboard, boardYsize, boardXsize, PlayerB);
                        WinningCondition(PlayerA, PlayerB);
                    }
                    if (strcmp(command, "[playerA]") == 0) {
                        break;
                    }
                }
                playersTurn = 1;
            }
            else {
                cout << "INVALID OPERATION \"[playerA] \": THE OTHER PLAYER EXPECTED";
                return 0;
            }
        }


        if (strcmp(command, "[playerB]") == 0) {
            if (playersTurn == 1) {
                while (cin >> command) {
                    if (strcmp(command, "PLACE_SHIP") == 0) {
                        int X, Y, shipNumber;
                        char Direction;
                        char ShipClassName[ShipClassWord];
                        cin >> Y >> X >> Direction >> shipNumber >> ShipClassName;
                        int ShipFound = FindShip(shipNumber, ShipClassName, PlayerB, FleetArrayB);
                        int shipPlaced = FleetArrayB[ShipFound].shipPlaced;
                        int ClassExceeded = FindShip(shipNumber, ShipClassName, PlayerB, FleetArrayB);


                        if (ClassExceeded == -1) {
                            cout << "INVALID OPERATION \"PLACE_SHIP " << Y << " " << X << " " << Direction << " " << shipNumber << " " << ShipClassName << "\"" << ": ALL SHIPS OF THE CLASS ALREADY SET";
                            return 0;
                        }
                        else if (shipPlaced == 1) {
                            cout << "INVALID OPERATION \"PLACE_SHIP " << Y << " " << X << " " << Direction << " " << shipNumber << " " << ShipClassName << "\"" << ": SHIP ALREADY PRESENT";
                            return 0;
                        }

                        FleetArrayB[ShipFound].direction = Direction;
                        FleetArrayB[ShipFound].shipPlaced = 1;
                        FleetArrayB[ShipFound].shipNoseXposition = X;
                        FleetArrayB[ShipFound].shipNoseYposition = Y;
                        PlayerB.shippresent++;
                        PlayerB.shipsplaced++;
                        int ShipLength = FleetArrayB[ShipFound].shiplength;
                        PlayerB.shipPartsAlive += ShipLength;
                        InitializeBoard2(fleetboard, boardXsize, boardYsize, FleetArrayA, FleetArrayB, PlayerA, PlayerB, ShipClassName);
                    }
                    if (strcmp(command, "SHOOT") == 0) {
                        int Xposition, Yposition;
                        cin >> Yposition >> Xposition;
                        if ((Xposition < 0) || (Yposition < 0) || (Yposition > boardYsize) || (Xposition > boardXsize)) {
                            cout << "INVALID OPERATION \"SHOOT " << Yposition << " " << Xposition << "\"" << ": FIELD DOES NOT EXIST";
                        }
                        else if (PlayerB.shippresent != PlayerB.fleetcountarray || PlayerA.shippresent != PlayerA.fleetcountarray) {
                            cout << "INVALID OPERATION \"SHOOT " << Yposition << " " << Xposition << "\"" << ": NOT ALL SHIPS PLACED";
                            exit(0);
                        }
                        else {
                            fleetboard[Yposition][Xposition] = Shooting(fleetboard, Xposition, Yposition);
                        }
                        PlayerA.shipPartsAlive = deadCounting(fleetboard, boardYsize, boardXsize, PlayerA);
                        PlayerB.shipPartsAlive = deadCounting(fleetboard, boardYsize, boardXsize, PlayerB);
                        WinningCondition(PlayerA, PlayerB);
                    }
                    if (strcmp(command, "[playerB]") == 0) {
                        break;
                    }
                }
                playersTurn = 0;
            }
            else {
                cout << "INVALID OPERATION \"[playerB] \": THE OTHER PLAYER EXPECTED";
                return 0;
            }
        }
    }
    return 0;
}




void InitializePlayer(player& Player, char Playername, int borderUP, int borderDown) {  //Setups player data
    Player.playername = Playername;
    Player.shipPartsAlive = 0;
    Player.borderDOWN = borderDown;
    Player.borderUP = borderUP;
    Player.carriers = 1;
    Player.battleships = 2;
    Player.cruisers = 3;
    Player.destroyers = 4;
    Player.shippresent = 0;
    Player.fleetcountarray = 10;
    Player.shipcount = 0;
}

void BasicPrintBoard(char tab[boardYsize][boardXsize], int Xsize, int Ysize, player& PlayerA, player& PlayerB) // Basic array print
{
    for (int i = 0; i < Ysize; i++) {
        for (int j = 0; j < Xsize; j++) {
            cout << tab[i][j];
        }
        cout << endl;
    }
    cout << "PARTS REMAINING:: " << PlayerA.playername << " : " << PlayerA.shipPartsAlive << " " << PlayerB.playername << " : " << PlayerB.shipPartsAlive << endl;
}  

void InitializeBoard(char tab[boardYsize][boardXsize], int Xsize, int Ysize) // First setup of array
{
    for (int i = 0; i < Ysize; i++) {
        for (int j = 0; j < Xsize; j++) {
            tab[i][j] = ' ';
        }
    }
} 

void CreatePlayerFleet(player& Player, ship FleetArray[maxShipClassCount])  //Setting up players fleets
{
    int numberOfShipClass[] = { Player.carriers, Player.battleships, Player.cruisers, Player.destroyers }; //array setting the number of types of ships of a given class
    for (int i = 3, k = 0; i >= 0; i--, k++) {     
        for (int j = 0; j < numberOfShipClass[k]; j++) {
            FleetArray[Player.shipcount] = CreateShip(Player, j, i);
            Player.shipcount++;
        }
    }
}

ship CreateShip(player& Player, int shipNumber, int shipLength) { //Function setting up ships data for CreatePlayerFleet function 
    ship NewShip;
    NewShip.playerName = Player.playername;
    NewShip.shiplength = shipLength + 2;
    NewShip.shipPlaced = 0;
    NewShip.shipParts = shipLength + 2;
    NewShip.shipNumber = shipNumber;
    NewShip.shipClass = shipLength + 2;
    NewShip.shipNoseXposition = -1;
    NewShip.shipNoseYposition = -1;
    return NewShip;
}

void SetFleet(player& Player) //Setting custom numbers of ships
{
        cin >> Player.carriers >> Player.battleships >> Player.cruisers >> Player.destroyers;
        Player.fleetcountarray = Player.carriers + Player.battleships + Player.cruisers + Player.destroyers;

}

int FindShip(int ShipNumber, char ShipClassName[ShipClassWord], player& playingPlayer, ship FleetArray[maxShipClassCount]) { //Looking for specified(class, ship number) in players fleet //Returns array position
    int shipClass = ShipClassHandler(ShipClassName);
    int shipplace = -1;
    for (int i = 0; i < maxShipClassCount; i++) {
        if ((FleetArray[i].shipClass == shipClass) && (FleetArray[i].shipNumber == ShipNumber)) {
            shipplace = i;
        }
    }
    return shipplace;
}

int ShipClassHandler(char ShipClassName[ShipClassWord]) {  //Changes char class names into numbers for easier handling
    if (ShipClassName[2] == 'R') {
        return 5;
    }
    else if (ShipClassName[2] == 'T') {
        return 4;
    }
    else if (ShipClassName[2] == 'U') {
        return 3;
    }
    else if (ShipClassName[2] == 'S') {
        return 2;
    }
    else {
        return 0;
    }
}

void checkingTilesPlacing(int Xtile, int Ytile, player& Player, char Direction, int shipNumber, char ShipClass[]) {   //First check if the X and Y are on the board
    if (Xtile < 0 || Ytile < 0 || Ytile > boardYsize || Xtile > boardXsize) {
        cout << "INVALID OPERATION \"PLACE_SHIP " << Ytile << " " << Xtile << " " << Direction << " " << shipNumber << " " << ShipClass << "\"" << ": NOT IN STARTING POSITION";
        exit(0);
    }
    if ((Player.borderUP > Ytile) || (Ytile > Player.borderDOWN)) {
        cout << "INVALID OPERATION \"PLACE_SHIP " << Ytile << " " << Xtile << " " << Direction << " " << shipNumber << " " << ShipClass << "\"" << ": NOT IN STARTING POSITION";
        exit(0);
    }

}

int checkingTilePlacingOnBoard(int Xsize, int Ysize, char Direction, int Ytile, int Xtile, int shipNumber , player& Player, char ShipClass[]) { //checks the tiles while setting up the ship 
    if (Xsize > boardXsize || Ysize < Player.borderUP || Xsize < 0 || Ysize > Player.borderDOWN) {
        cout << "INVALID OPERATION \"PLACE_SHIP " << Ytile << " " << Xtile << " " << Direction << " " << shipNumber << " " << ShipClass << "\"" << ": NOT IN STARTING POSITION";
        exit(0);
    }
    else {
        return 0;
    }
}

void InitializeBoard2(char tab[boardYsize][boardXsize], int Xsize, int Ysize, ship FleetArrayA[maxShipClassCount], ship FleetArrayB[maxShipClassCount], player& PlayerA, player& PlayerB, char ShipClass[]) { //Placing ships on the board
    char temptab[boardYsize][boardXsize];
    for (int i = 0; i < Ysize; i++) {
        for (int j = 0; j < Xsize; j++) {
            temptab[i][j] = ' ';
        }
    }
    int shipsnumber1 = PlayerA.shipsplaced;
    for (int i = 0; i < PlayerA.shipcount; i++) {
        int Xship = FleetArrayA[i].shipNoseXposition;
        int Yship = FleetArrayA[i].shipNoseYposition;
        char Direction = FleetArrayA[i].direction;
        int shiplength = FleetArrayA[i].shiplength;
        int shipNumber = FleetArrayA[i].shipNumber;

        if (Direction == 'N') {
            for (int length = 0; length < shiplength; length++) {
                checkingTilePlacingOnBoard(Xship, Yship, Direction, FleetArrayA[i].shipNoseYposition, FleetArrayA[i].shipNoseXposition, shipNumber, PlayerA, ShipClass);
                temptab[Yship][Xship] = '+';
                Yship++;
                shipsnumber1++;
            }
        }
        if (Direction == 'S') {
            for (int length = 0; length < shiplength; length++) {
                checkingTilePlacingOnBoard(Xship, Yship, Direction, FleetArrayA[i].shipNoseYposition, FleetArrayA[i].shipNoseXposition, shipNumber, PlayerA, ShipClass);
                temptab[Yship][Xship] = '+';
                Yship--;
                shipsnumber1++;
            }
        }
        if (Direction == 'E') {
            for (int length = 0; length < shiplength; length++) {
                checkingTilePlacingOnBoard(Xship, Yship, Direction, FleetArrayA[i].shipNoseYposition, FleetArrayA[i].shipNoseXposition, shipNumber, PlayerA, ShipClass);
                temptab[Yship][Xship] = '+';
                Xship--;
                shipsnumber1++;
            }
        }
        if (Direction == 'W') {
            for (int length = 0; length < shiplength; length++) {
                checkingTilePlacingOnBoard(Xship, Yship, Direction, FleetArrayA[i].shipNoseYposition, FleetArrayA[i].shipNoseXposition, shipNumber, PlayerA, ShipClass);
                temptab[Yship][Xship] = '+';
                Xship++;
                shipsnumber1++;
            }
        }
    }
    int shipsnumber2 = PlayerB.shipsplaced;
    for (int i = 0; i < PlayerB.shipcount; i++) {
        int Xship = FleetArrayB[i].shipNoseXposition;
        int Yship = FleetArrayB[i].shipNoseYposition;
        char Direction = FleetArrayB[i].direction;
        int shiplength = FleetArrayB[i].shiplength;
        int shipNumber = FleetArrayB[i].shipNumber;
        if (Direction == 'N') {
            for (int length = 0; length < shiplength; length++) {
                checkingTilePlacingOnBoard(Xship, Yship, Direction, FleetArrayB[i].shipNoseYposition, FleetArrayB[i].shipNoseXposition, shipNumber, PlayerB, ShipClass);
                temptab[Yship][Xship] = '+';
                Yship++;
                shipsnumber1++;

            }
        }
        if (Direction == 'S') {
            for (int length = 0; length < shiplength; length++) {
                checkingTilePlacingOnBoard(Xship, Yship , Direction, FleetArrayB[i].shipNoseYposition, FleetArrayB[i].shipNoseXposition, shipNumber, PlayerB, ShipClass);
                temptab[Yship][Xship] = '+';
                Yship--;
                shipsnumber1++;
            }
        }
        if (Direction == 'E') {
            for (int length = 0; length < shiplength; length++) {
                checkingTilePlacingOnBoard(Xship, Yship, Direction, FleetArrayB[i].shipNoseYposition, FleetArrayB[i].shipNoseXposition, shipNumber, PlayerB, ShipClass);
                temptab[Yship][Xship] = '+';
                Xship--;
                shipsnumber1++;
            }
        }
        if (Direction == 'W') {
            for (int length = 0; length < shiplength; length++) {
                checkingTilePlacingOnBoard(Xship, Yship, Direction, FleetArrayB[i].shipNoseYposition, FleetArrayB[i].shipNoseXposition, shipNumber, PlayerB, ShipClass);
                temptab[Yship][Xship] = '+';
                Xship++;
                shipsnumber1++;
            }
        }
    }

    for (int i = 0; i < Ysize; i++) {
        for (int j = 0; j < Xsize; j++) {
            tab[i][j] = temptab[i][j];
        }
    }

}

char Shooting(char gameboard[boardYsize][boardXsize], int X, int Y) { //handles shooting
    if (gameboard[Y][X] == '+') {
        return 'x';
    }
    else if (gameboard[Y][X] == 'x') {
        return 'x';
    }
    else {
        return ' ';
    }
}

int deadCounting(char board[boardYsize][boardXsize], int Ysize, int Xsize, player& Player) { //counts how many parts for a given player is left

    int dead = 0;
    for (int i = Player.borderUP; i < Player.borderDOWN; i++) {
        for (int j = 0; j < Xsize; j++) {
            if (board[i][j] == '+') {
                dead++;
            }

        }
    }

    return dead;
}

void WinningCondition(player& PlayerA, player& PlayerB) { //checks if player won
    if (PlayerA.shipPartsAlive == 0) {
        cout << "B won";
        exit(0);
    }
    if (PlayerB.shipPartsAlive == 0) {
        cout << "A won";
        exit(0);
    }
}
