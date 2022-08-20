#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <vector>     // For vectors
#include <sstream>    // For reading in CSV file and splitting by commas
using namespace std;


class Node; // this is the declaration, so that it can be used inside class Game below
            // the definition of the Node class comes after the Game class

class Game {
    public:
        // Default up to Fully-Qualified Constructor
        Game(string region="N/A", int rankOne=-1, string teamOne="N/A", int scoreOne=-1,
             int rankTwo=-1, string teamTwo="N/A", int scoreTwo=-1, 
             string winningTeam="N/A", int roundNumber=-1, int gameNumber=-1)
        {
            this->region = region;
            this->team1Rank = rankOne;
            this->team1Name = teamOne;
            this->team1Score = scoreOne;
            this->team2Rank = rankTwo;
            this->team2Name = teamTwo;
            this->team2Score = scoreTwo;
            this->winningTeam = winningTeam;
            this->roundNum = roundNumber;
            this->gameNum = gameNumber;
            this->subBrackets = NULL;
        }

        // Copy Constructor
        Game(const Game & otherGame) {
            this->region = otherGame.region;
            this->team1Rank = otherGame.team1Rank;
            this->team1Name = otherGame.team1Name;
            this->team1Score = otherGame.team1Score;
            this->team2Rank = otherGame.team2Rank;
            this->team2Name = otherGame.team2Name;
            this->team2Score = otherGame.team2Score;
            this->winningTeam = otherGame.winningTeam;
            this->roundNum = otherGame.roundNum;
            this->gameNum = otherGame.gameNum;
            this->subBrackets = otherGame.subBrackets;
        }

        // Getters / Accessor methods
        string getRegion(){ return region; }
        int getRoundNum(){ return roundNum; }
        string getTeamOneName() { return team1Name; }
        int getTeamOneRank() { return team1Rank; }
        int getTeamOneScore() { return team1Score; }
        string getTeamTwoName() { return team2Name; }
        int getTeamTwoRank() { return team2Rank; }
        int getTeamTwoScore() { return team2Score; }
        string getWinner(){ return winningTeam; }
        Node* getSubBrackets() { return subBrackets; }

        // Setters / Mutator methods
        void setTeamOneName(string newTeamOneName) { this->team1Name = newTeamOneName; }
        void setTeamTwoName(string newTeamTwoName) { this->team2Name = newTeamTwoName; }
        void setWinner(string newWinner) { this->winningTeam = newWinner; }
        void setSubBrackets(Node* newSubBrackets) { this->subBrackets = newSubBrackets; }

        // Utility function to display the game
        void displayGame() {
            cout << "Round " << roundNum << ", Game " << gameNum << ": "
                 << team1Name << " vs " << team2Name << ". "
                 << "Winner: " << winningTeam << "\n";
        }
    
    private:
        string region;
        int roundNum;
        int gameNum;
        string team1Name;
        string team2Name; 
        string winningTeam;
        int team1Rank;
        int team2Rank;
        int team1Score;
        int team2Score;
        Node* subBrackets; // pHead; this is a pointer to a Node to represent the sub-brackets as a linked list
};

// Node class in order to implement linked lists
class Node {

    public:
        Node() {
            pNext = NULL;
        }

        Node(Game theSubBracket, Node* thePNext) {
            subBracket = theSubBracket;
            pNext = thePNext;
        }

        // ideally these should be private, but we are making them public to make the code a bit simpler
        Game subBracket; // data
        Node* pNext;
};


//--------------------------------------------------------------------------------
// Display welcome message, introducing the user to the program
void displayWelcomeMessage()
{
    cout << "NCAA March Madness\n\n"
        << "This program reads in and analyzes data from NCAA Basketball Tournaments (March Madness) spanning 2021–2022. "
        << "It displays the path taken to the championship, determines which region is expected to win at a given round, "
        << "and you can also compare the actual brackets to your own predictions! The hierarchy of the brackets is also implemented "
        << "via linked lists to evaluate the resulting brackets if earlier games had a different outcome.\n"
        << endl;
} //end displayWelcomeMessage()


//--------------------------------------------------------------------------------
// Read in the game data and store in a vector
void readInGameData(string filename, vector<Game>& games) {

    string region, teamOne, teamTwo, winningTeam;
    int rankOne, rankTwo, roundNumber, gameNumber, scoreOne, scoreTwo;

    ifstream inStream;
    inStream.open(filename);
    assert(inStream.fail() == false);

    string row;
    getline(inStream, row); // read in header from file, and ignore

    // read in each line of the file
    while (getline(inStream, row)) {
        stringstream s_stream(row); // create stringstream from the row

        // separate the line by commas into each column
        int colNum = 0;
        while (s_stream.good()) {
            string columnVal;
            getline(s_stream, columnVal, ','); // get first string delimited by comma

            // use the substring to fill in the appropriate part of the BillboardRecord
            switch (colNum) {
                case 0:
                    region = columnVal;
                    break;
                case 1:
                    rankOne = stoi(columnVal);
                    break;
                case 2:
                    teamOne = columnVal; 
                    break;
                case 3:
                    if(columnVal != "") { scoreOne = stoi(columnVal); }
                    else { scoreOne = -1; }
                    break;
                case 4:
                    rankTwo = stoi(columnVal);
                    break;
                case 5:
                    teamTwo = columnVal;
                    break;
                case 6:
                    if(columnVal != "") { scoreTwo = stoi(columnVal); }
                    else { scoreTwo = -1; }
                    break;
                case 7:
                    winningTeam = columnVal;
                    break;
                case 8:
                    roundNumber = stoi(columnVal);
                    break;
                case 9:
                    gameNumber = stoi(columnVal);
                    break;
                default: // should not get here, because there are only 10 columns
                    cout << "More columns in the file than expected... Exiting program" << endl;
                    exit(0);
                    break;
            }
            colNum++;
        }
        // Create instance of the class using the column values
        Game oneGame;
        oneGame = Game(region, rankOne, teamOne, scoreOne, rankTwo, teamTwo, scoreTwo, winningTeam, roundNumber, gameNumber);
        // and add it to the vector
        games.push_back(oneGame);

    } // end while(getline(instream, row))

} // end readInGameData() function


//--------------------------------------------------------------------------------
// Identify the sub-brackets for each game and modify the class instances in the vector accordingly
void determineSubBrackets(vector<Game> &games) {

    // Nested loop to find the sub-brackets for each game
    // We go in reverse (csv file data is round 6 --> round 1), 
    // starting at round 1 and going up to round 6
    //      Note - this is so that when we add subbrackets for round 6, the subbrackets for the games
    //             in round 5 have been determined, and so on and so forth
    for(int i=games.size()-1; i>=0; i--) {
        for(int j=games.size()-1; j>=0; j--) {
            // Check that we are not looking at the same game
            if(i==j) {
                continue;
            }
            // games[j] is a sub-bracket of games[i] if it is from the previous round,
            // and if the winning team from games[j] is one of the teams in games[i]
            if( (games.at(j).getRoundNum() == (games.at(i).getRoundNum() - 1)) &&
                (games.at(j).getWinner() == games.at(i).getTeamOneName() || games.at(j).getWinner() == games.at(i).getTeamTwoName()) ) {
                    // Prepend games[j] to the linked list of sub-brackets for games[i]
                    Node *pTemp = new Node; // new temporary Node
                    // set data/subBracket of node to a sub-bracket game
                    pTemp-> subBracket = games.at(j);
                    // if list was empty, set pNext to NULL;
                    // if it was populated, set to node previously at start of list
                    pTemp-> pNext = games.at(i).getSubBrackets();
                    // prepend new game to front of list
                    games.at(i).setSubBrackets(pTemp);
                }
        }
    }

} // end of determineSubBrackets() function


// ----------------------------------------------------------------------
// MENU OPTION 1 :: 
void displaySubBrackets(vector<Game> &games) 
{
    // Prompt the user to select the round number of the game they would like to look at
    int numRound = 0;
    cout << "Enter the round number of the game: ";
    cin >> numRound;

    // Prompt the user to select the winning team of the game they would like to look at
    string winningTeam;
    cout << "Enter the winning team of the game: ";
    cin.ignore();
    getline(cin, winningTeam);

    for (int gameIndex = 0; gameIndex < games.size(); gameIndex++) {
        // If the particular game with that round number and winning team exists...
        if (games.at(gameIndex).getRoundNum() == numRound 
          && games.at(gameIndex).getWinner() == winningTeam) {

            // 1) display game info
            cout << "\nThe game is:\n";
            games.at(gameIndex).displayGame();

            // 2) traverse linked list 
            Node *linkedList = games.at(gameIndex).getSubBrackets();
            Node *currentNode = linkedList;

            // 3) check for sub-brackets
            if (currentNode == NULL) {
                cout << "\nThis game does not have any sub-brackets." << endl;
            } else {
                cout << "\nSub-brackets of this game are:\n";
                while (currentNode != NULL) {
                    
                    // 4) display game info for each sub-bracket
                    currentNode->subBracket.displayGame();
                    currentNode = currentNode -> pNext;
                }
            }
            break;
        }

        // if, after looping through the entire vector, no such game exists...
        if (gameIndex == games.size()-1 && (games.at(gameIndex).getRoundNum() != numRound 
          || games.at(gameIndex).getWinner() != winningTeam)) {
            cout << "Sorry, no games matching that round number and winning team were found." << endl;
        }
    }
} // end displaySubBrackets()


// ----------------------------------------------------------------------
// MENU OPTION 2 :: iterate data backwards for games this team won before the championship
void findPathToChampionship(Game game) 
{
    Node *linkedList = game.getSubBrackets();
    Node *currentNode = linkedList;

    // Base case: the game does not have any sub-brackets
    if (currentNode == NULL) { 
      return; 
    }

    // Make recursive call on previous game where winning team also won
    while (currentNode != NULL) {
        if (currentNode->subBracket.getWinner() == game.getWinner()) {
            findPathToChampionship(currentNode->subBracket);
            // Display after recursion is complete to print in reverse order
            currentNode->subBracket.displayGame(); // from first game to championship
        }
        currentNode = currentNode -> pNext;
    }
} // end findPathToChampionship()


// ----------------------------------------------------------------------
// MENU OPTION 3 :: 
void undoChampionshipGame(Game &gameToUndo, int numRoundsToUndo) 
{
    Node *linkedList = gameToUndo.getSubBrackets();
    Node *currentNode = linkedList;

    // Base case: there are no more rounds left to undo
    if (numRoundsToUndo == 1) {

        // Modify the winner of this game (the game where the undo begins)
        if (gameToUndo.getWinner() == gameToUndo.getTeamOneName()) {
            // the winner was team 1, so we modify the winner to be team 2
            gameToUndo.setWinner(gameToUndo.getTeamTwoName());
        } else if (gameToUndo.getWinner() == gameToUndo.getTeamTwoName()) {
            // the winner was team 2, so we modify the winner to be team 1
            gameToUndo.setWinner(gameToUndo.getTeamOneName());
        }
        return;
    }

    // Find and make a recursive call on the previous game where the winning team also won that game
    while (currentNode != NULL) {
        if (currentNode->subBracket.getWinner() == gameToUndo.getWinner()) {

            undoChampionshipGame(currentNode->subBracket, numRoundsToUndo-1);
          
            // Modify after recursion:
            if (gameToUndo.getTeamOneName() == gameToUndo.getWinner()) {
                // the old winner of this game was team 1, so we modify team 1 to be the new winner of the subbracket
                gameToUndo.setTeamOneName(currentNode->subBracket.getWinner());
                // this team is also the winner of this game
                gameToUndo.setWinner(currentNode->subBracket.getWinner());
            } else if (gameToUndo.getTeamTwoName() == gameToUndo.getWinner()) {
                // the old winner of this game was team 2, so we modify team 2 to be the new winner of the subbracket
                gameToUndo.setTeamTwoName(currentNode->subBracket.getWinner());
                // this team is also the winner of this game
                gameToUndo.setWinner(currentNode->subBracket.getWinner());
            }
        }
        currentNode = currentNode -> pNext;
    }
} // end undoChampionshipGame()


// ----------------------------------------------------------------------
// Main() function of the program
int main()
{
    // Vector to hold tournament games
    vector<Game> games;

    displayWelcomeMessage();

    // Prompt user for the name of the file with the data
    string fileName;
    cout << "Enter the name of the file with the data for the NCAA tournament: ";
    cin >> fileName;

    // Read in the data from the file into a vector
    readInGameData(fileName, games);
    // Also identify sub-brackets for each game
    determineSubBrackets(games);

    
    Game championshipGame = games.at(0);
  
    // Infinite loop to allow handling menu options
    int menuOption = 0;
    while (true) {
        
        // Prompt user for menu selection
        cout << "\nSelect a menu option:\n"
             << "   1. See the sub-brackets of one game\n"
             << "   2. Display the path of the winning team to the championship\n"
             << "   3. Undo the championship game for a given number of rounds\n"
             << "   4. Exit\n"
             << "Your choice --> ";
        cin >> menuOption;

        // If option 4 was chosen then exit the program
        if (menuOption == 4) {
            cout << "Exiting program...";
            break;     // Break out of loop to exit program.
        }

        if (menuOption == 1) {
            displaySubBrackets(games);
            continue;
        }

        else if (menuOption == 2) {
            cout << "Path to the championship:" << endl;
            findPathToChampionship(championshipGame);
            championshipGame.displayGame();
            continue;
        }

        else if (menuOption == 3) {
            int numRoundsToUndo = 0;  
            do {
                cout << "Enter the number of rounds that you would like to undo (max of 6 allowed): ";
                cin >> numRoundsToUndo;
                cout << endl;
                if (numRoundsToUndo < 1 || numRoundsToUndo > 6) {
                    cout << "Invalid entry. Try again." << endl;
                }
            } while (numRoundsToUndo < 1 || numRoundsToUndo > 6);

          
            // Work backwards from championship up for number of inputted rounds
            undoChampionshipGame(championshipGame, numRoundsToUndo);

            // Display new path to championship
            findPathToChampionship(championshipGame);
            // Display the championship game
            championshipGame.displayGame();
            continue;
        }

        else {
            cout << "Invalid value.  Please re-enter a value from the menu options below." << endl;
        }

    } // end while(true)

    return 0;
} // end main()