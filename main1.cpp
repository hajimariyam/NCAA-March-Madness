#include <iostream>
#include <fstream>    // For file input and output
#include <cassert>    // To validate if file is open
#include <vector>
#include <cstring>    // For C string function strcpy
#include <sstream>    // For reading in CSV file and splitting by commas
#include <algorithm>  // For find(), which searches within a vector
#include <cstdlib>    // For abs()
using namespace std;  // To avoid prefacing cin/cout with std::


class TournamentRecord {
    public:
        void SetRegion(string regionToSet) { region = regionToSet; }
        string GetRegion() { return region; }

        void SetTeam1(string team1ToSet) { team1 = team1ToSet; }
        string GetTeam1() { return team1; }

        void SetTeam2(string team2ToSet) { team2 = team2ToSet; }
        string GetTeam2() { return team2; }

        void SetWinningTeam(string winningTeamToSet) { winningTeam = winningTeamToSet; }
        string GetWinningTeam() { return winningTeam; }

        void SetRank1(int rank1ToSet) { rank1 = rank1ToSet; }
        int GetRank1() { return rank1; }

        void SetRank2(int rank2ToSet) { rank2 = rank2ToSet; }
        int GetRank2() { return rank2; }

        void SetScore1(int score1ToSet) { score1 = score1ToSet; }
        int GetScore1() { return score1; }

        void SetScore2(int score2ToSet) { score2 = score2ToSet; }
        int GetScore2() { return score2; }

        void SetNumRound(int numRoundToSet) { numRound = numRoundToSet; }
        int GetNumRound() { return numRound; }

        void SetNumGame(int numGameToSet) { numGame = numGameToSet; }
        int GetNumGame() { return numGame; }
        
    private:
        string region, team1, team2, winningTeam;
        int rank1, rank2, score1, score2, numRound, numGame;
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
}//end displayWelcomeMessage()


//--------------------------------------------------------------------------------
// Read in the Billboard Weekly Hot 100 chart data and store this in a vector
void readInTournamentData(string filename, vector<TournamentRecord>& tournamentRecords) {

    ifstream inStream;
    inStream.open(filename);
    assert(inStream.fail() == false);

    string recordLine;
    getline(inStream, recordLine); // read in header from file, and ignore

    // read in each line of the file
    while (getline(inStream, recordLine)) {
        stringstream s_stream(recordLine); //create string stream from the string
        TournamentRecord oneRecord;

        // separate the line by commas into each column
        int colNum = 1;
        while (s_stream.good()) {
            string columnVal;
            getline(s_stream, columnVal, ','); //get first string delimited by comma

            // use the substring to fill in the appropriate part of the TournamentRecord
            switch (colNum) {
                case 1: {
                    oneRecord.SetRegion(columnVal);
                    break;
                }
                case 2:
                    oneRecord.SetRank1(stoi(columnVal));
                    break;
                case 3:
                    oneRecord.SetTeam1(columnVal);
                    break;
                case 4:
                    if (columnVal != "") { oneRecord.SetScore1(stoi(columnVal)); }
                    else { oneRecord.SetScore1(0); }
                    break;
                case 5:
                    oneRecord.SetRank2(stoi(columnVal));
                    break;
                case 6:
                    oneRecord.SetTeam2(columnVal);
                    break;
                case 7:
                    if (columnVal != "") { oneRecord.SetScore2(stoi(columnVal)); }
                    else { oneRecord.SetScore2(0); }
                    break;
                case 8:
                    oneRecord.SetWinningTeam(columnVal);
                    break;
                case 9:
                    oneRecord.SetNumRound(stoi(columnVal));
                    break;
                case 10:
                    oneRecord.SetNumGame(stoi(columnVal));
                    break;
                default: // should not get here, because there are only 10 columns
                    cout << "More columns in the file than expected... Exiting program" << endl;
                    exit(0);
                    break;
            }
            colNum++;
        }
        // add the record to the vector
        tournamentRecords.push_back(oneRecord);
    }
} // end readInTournamentData()


// ----------------------------------------------------------------------
// MENU OPTION 1 :: Display overall information about the data
void overallTournamentStats(vector<TournamentRecord>& tournamentRecords) {
  
    // Number of games played in the tournament         
    cout << "Total number of games played in tournament: " << tournamentRecords.size() << endl;

    // Final Four contestants with region
    cout << "The Final Four contestants are:" << endl;
  
    for (int i = 0; i < tournamentRecords.size(); i++) {
        if (tournamentRecords.at(i).GetNumRound() == 4) {
            cout << "        " << tournamentRecords.at(i).GetRegion() << " region:    " 
                 << tournamentRecords.at(i).GetWinningTeam() << endl;
        }
    }
} // end overallTournamentStats()


// ----------------------------------------------------------------------
// MENU OPTION 2 :: Display the winning team's path to championship
void pathToChampionship(vector<TournamentRecord>& tournamentRecords) {

    string championTeam = tournamentRecords.at(0).GetWinningTeam();
  
    cout << "Path to the championship:" << endl;

    // iterate data backwards for games this team won before the championship
    for (int i = tournamentRecords.size()-1; i >= 0; i--) {
        if (tournamentRecords.at(i).GetWinningTeam() == championTeam) {
            cout << "Round " << tournamentRecords.at(i).GetNumRound()
                 << ", Game " << tournamentRecords.at(i).GetNumGame() << ": "
                 << tournamentRecords.at(i).GetTeam1() << " vs "
                 << tournamentRecords.at(i).GetTeam2() 
                 << ". Winner: " << tournamentRecords.at(i).GetWinningTeam()
                 << endl;
        }
    }
} // end pathToChampionship()


// ----------------------------------------------------------------------
// MENU OPTION 3 :: Display region expected to win championship based on given round
void expectedChampionRegion(vector<TournamentRecord>& tournamentRecords) {

    // Prompt user to select a round from 2-5
    int inputRound;
    cout << "Enter a round to be evaluated:\n"
         << "   Select 2 for round 2\n"
         << "   Select 3 for round 3 'Sweet 16'\n"
         << "   Select 4 for round 4 'Elite 8'\n"
         << "   Select 5 for round 5 'Final 4'\n"
         << "Your choice --> ";
    cin >> inputRound;
    cout << "Analyzing round " << inputRound << "...\n\n";

    // ROUNDS 2-4
    // Create vector with the names of the regions in this file
    vector <string> fourRegions;
    for (int i = 3; i < tournamentRecords.size(); i++) {        // Start at 3 to skip the 3 games that are from the championship or final four
        string region = tournamentRecords.at(i).GetRegion();

        // Add the region to the vector if it is not found in the vector already
        if (find(fourRegions.begin(), fourRegions.end(), region) == fourRegions.end()) {
            fourRegions.push_back(region);
        }
        // There are always 4 regions, so once we have all 4 we can break out of this loop
        if(fourRegions.size() == 4) {
            break;
        }
    }

    // Create vector (parallel to fourRegions) for sum of ranks of teams per region who won games in given round
    vector <int> sumOfWinningRanksInRegion(4, 0);      // initialize 4 elements at 0

    // Determine which region is expected to win the championship
    if (inputRound >= 2 && inputRound <= 4) {
        for (int game = 0; game < tournamentRecords.size(); game++) {
            // Check if game is in selected round
            if (tournamentRecords.at(game).GetNumRound() == inputRound) {
                for (int region = 0; region < fourRegions.size(); region++) {
                    // Add to sum of ranks for respective region
                    if (tournamentRecords.at(game).GetRegion() == fourRegions.at(region)) {
                        // Add rank of winning team, 1 or 2
                        if (tournamentRecords.at(game).GetWinningTeam() == tournamentRecords.at(game).GetTeam1()) {
                            sumOfWinningRanksInRegion.at(region) += tournamentRecords.at(game).GetRank1();
                        }
                        else if (tournamentRecords.at(game).GetWinningTeam() == tournamentRecords.at(game).GetTeam2()) {
                            sumOfWinningRanksInRegion.at(region) += tournamentRecords.at(game).GetRank2();
                        }
                    }
                }
            }
        }

        // Initialize with first element it will check (due to "less than" functionality)
        string expectedWinningRegion = fourRegions.at(0);
        int lowestSumOfRanks = sumOfWinningRanksInRegion.at(0);

        // Search for smallest sum and linked region
        for (int i = 0; i < sumOfWinningRanksInRegion.size(); i++) {
            if (sumOfWinningRanksInRegion.at(i) < lowestSumOfRanks) {
                lowestSumOfRanks = sumOfWinningRanksInRegion.at(i);
                expectedWinningRegion = fourRegions.at(i);
            }
        }
      
        cout << "The region expected to win is: " << expectedWinningRegion << endl;
    }

    // ROUND 5
    else if (inputRound == 5) {
        // Initialize with first element it will check (due to "less than" functionality)
        string bestRankingTeam = tournamentRecords.at(1).GetTeam1();
        int bestRank = tournamentRecords.at(1).GetRank1();

        // Search for best ranking team (between 1-16, 1 is best) in Final Four
        for (int i = 1; i <= 2; i++) {
            if (tournamentRecords.at(i).GetRank1() < bestRank) {
                bestRank = tournamentRecords.at(i).GetRank1();
                bestRankingTeam = tournamentRecords.at(i).GetTeam1();
            }
            if (tournamentRecords.at(i).GetRank2() < bestRank) {
                bestRank = tournamentRecords.at(i).GetRank2();
                bestRankingTeam = tournamentRecords.at(i).GetTeam2();
            }
        }

        // Search for the game (round 5-1=4) that sent bestRankingTeam to the finals
        for (int i =0; i<tournamentRecords.size(); i++) {
            if (tournamentRecords.at(i).GetNumRound() == 4 && tournamentRecords.at(i).GetWinningTeam() == bestRankingTeam) {
                cout << "The region expected to win is: " << tournamentRecords.at(i).GetRegion() << endl;
            }
        }
    }
} // end expectedChampionRegion()


// ----------------------------------------------------------------------
// MENU OPTION 4 :: Identify the best underdog within a given round
void bestUnderdog(vector<TournamentRecord>& tournamentRecords) {

    // Prompt user to select a round from 2-6
    int inputRound;

    cout << "Enter a round to be evaluated:\n"
         << "   Select 2 for round 2\n"
         << "   Select 3 for round 3 'Sweet 16'\n"
         << "   Select 4 for round 4 'Elite 8'\n"
         << "   Select 5 for round 5 'Final 4'\n"
         << "   Select 6 for round 6 'Championship'\n"
         << "Your choice --> ";
    cin >> inputRound;

    // Search for the worst ranking team in the games for the selected round
    string bestUnderdogTeam;
    int bestUnderdogRank = 0;  // a higher rank number indicates that the team is worse so we can start at 0

    for (int i = 0; i < tournamentRecords.size(); i++) {
        // Search for lowest rank (highest number) in Teams 1 
        if (tournamentRecords.at(i).GetNumRound() == inputRound
          && tournamentRecords.at(i).GetRank1() > bestUnderdogRank) {
            bestUnderdogRank = tournamentRecords.at(i).GetRank1();
            bestUnderdogTeam = tournamentRecords.at(i).GetTeam1();
        }
        // Search for lowest rank (highest number) in Teams 2
        if (tournamentRecords.at(i).GetNumRound() == inputRound 
          && tournamentRecords.at(i).GetRank2() > bestUnderdogRank) {
            bestUnderdogRank = tournamentRecords.at(i).GetRank2();
            bestUnderdogTeam = tournamentRecords.at(i).GetTeam2();
        }
    }

    cout << "The best underdog team is " << bestUnderdogTeam << " which has rank " << bestUnderdogRank << ".\n";
} // end bestUnderdog()


// ----------------------------------------------------------------------
// MENU OPTION 5 :: Find the shoo-in and nail-biting games within a given round, or overall
void specialGames(vector<TournamentRecord>& tournamentRecords) {

    // Prompt user to select a round from 1-6, or 7 for all rounds
    int inputRound;

    cout << "Enter a round to be evaluated:\n"
         << "   Select 1 for round 1\n"
         << "   Select 2 for round 2\n"
         << "   Select 3 for round 3 'Sweet 16'\n"
         << "   Select 4 for round 4 'Elite 8'\n"
         << "   Select 5 for round 5 'Final 4'\n"
         << "   Select 6 for round 6 'Championship'\n"
         << "   Select 7 for the overall tournament\n"
         << "Your choice --> ";
    cin >> inputRound;
    if (inputRound == 7) { cout << "Analyzing the overall tournament...\n\n"; } 
    else { cout << "Analyzing round " << inputRound << "...\n\n"; }

    // Shoo-in game has greatest difference in scores
    int greatestScoreDifference = 0;
    int shooInIndex;
  
    for (int i = 0; i < tournamentRecords.size(); i++) {
        // Skip game if user has selected a particular round and if this game is not in that round, otherwise...
        if ((inputRound != 7 && tournamentRecords.at(i).GetNumRound() == inputRound) || inputRound == 7) {
            int scoreDifference = abs(tournamentRecords.at(i).GetScore1() - tournamentRecords.at(i).GetScore2());
            if (scoreDifference > greatestScoreDifference) {
                greatestScoreDifference = scoreDifference;
                shooInIndex = i;      // to display game details
            }
        }
    }
  
    // Nail-biting game has smallest difference in scores
    int smallestScoreDifference = 10000;      // random large number
    int nailBitingIndex;
  
    for (int i = 0; i < tournamentRecords.size(); i++) {
        if ((inputRound != 7 && tournamentRecords.at(i).GetNumRound() == inputRound) || inputRound == 7) {
            int scoreDifference = abs(tournamentRecords.at(i).GetScore1() - tournamentRecords.at(i).GetScore2());
            if (scoreDifference < smallestScoreDifference) {
                smallestScoreDifference = scoreDifference;
                nailBitingIndex = i;      // to display game details
            }
        }
    }
  
    cout << "The shoo-in game was:\n"
         << "Round " << tournamentRecords.at(shooInIndex).GetNumRound()
         << ", Game " << tournamentRecords.at(shooInIndex).GetNumGame()
         << ": " << tournamentRecords.at(shooInIndex).GetTeam1() << " vs " 
         << tournamentRecords.at(shooInIndex).GetTeam2() << ". Winner: " 
         << tournamentRecords.at(shooInIndex).GetWinningTeam() << "\nThe difference was "
         << greatestScoreDifference << " points.\n\n";

    cout << "The nail-biting game was:\n"
         << "Round " << tournamentRecords.at(nailBitingIndex).GetNumRound()
         << ", Game " << tournamentRecords.at(nailBitingIndex).GetNumGame()
         << ": " << tournamentRecords.at(nailBitingIndex).GetTeam1() << " vs " 
         << tournamentRecords.at(nailBitingIndex).GetTeam2() << ". Winner: " 
         << tournamentRecords.at(nailBitingIndex).GetWinningTeam() << "\nThe difference was "
         << smallestScoreDifference << " points.\n\n";
} // end specialGames()


// ----------------------------------------------------------------------
// MENU OPTION 6 :: Compare the actual brackets to your predicted brackets
void compareBrackets(vector<TournamentRecord>& tournamentRecords) {
    string fileName;
    
    cout << "Enter the name of the file with your predicted brackets:\n";
    cin >> fileName;

    // Read in the data from the file into a vector
    vector<TournamentRecord> tournamentPredictions;
    readInTournamentData(fileName, tournamentPredictions);

    // Compare actual and predicted brackets by comparing the winning teams
    int userScore = 0;
    int correctlyPredictedGames = 0;
  
    for (int i = 0; i < tournamentRecords.size(); i++) {
        // Note that this works because we assume that the files list the games in the same order
        if (tournamentRecords.at(i).GetWinningTeam() == tournamentPredictions.at(i).GetWinningTeam()) {
            // Keep score for each winner predicted correctly
            userScore += (tournamentRecords.at(i).GetNumRound() * 5);
            correctlyPredictedGames += 1;
        }
    }

    cout << "You correctly predicted the winner for " << correctlyPredictedGames << " games.\n"
         << "This means that you have a score of " << userScore << ".\n";

    if (userScore >= 250) {
        cout << "Great job! You could consider entering your predictions to win money!\n"; } 
    else {
        cout << "You may want to learn more about basketball to improve your predictions next year.\n"; }

} // end compareBrackets()

  
int main() {
  
    displayWelcomeMessage();

    // Prompt user for the name of the file with the data
    string fileName;
    cout << "\nEnter the name of the file with the data for the NCAA tournament: ";
    cin >> fileName;

    // Read in the data from the file into a vector
    vector<TournamentRecord> tournamentRecords;
    readInTournamentData(fileName, tournamentRecords);

    // Infinite loop to allow handling menu options
    int menuOption = 0;
    while (true) {
        // Prompt user for menu selection
        cout << "\nSelect a menu option:\n"
                << "   1. Display overall information about the data\n"
                << "   2. Display the path of the winning team to the championship\n"
                << "   3. Determine which region is expected to win the championship based on a given round\n"
                << "   4. Identify the best underdog within a given round\n"
                << "   5. Find the shoo-in and nail-biting games within a given round, or overall\n"
                << "   6. Compare the actual brackets to your predicted brackets\n"
                << "   7. Exit\n"
                << "Your choice --> ";
        cin >> menuOption;

        // If option 7 was chosen then exit the program
        if (menuOption == 7) {
            cout << "Exiting program...";
            break;     // Break out of loop to exit program.
        }

        if (menuOption == 1) {
            overallTournamentStats(tournamentRecords);
            continue;
        }
        else if (menuOption == 2) {
            pathToChampionship(tournamentRecords);
            continue;
        }
        else if (menuOption == 3) {
            expectedChampionRegion(tournamentRecords);
            continue;
        }
        else if (menuOption == 4) {
            bestUnderdog(tournamentRecords);
            continue;
        }
        else if (menuOption == 5) {
            specialGames(tournamentRecords);
            continue;
        }
        else if (menuOption == 6) {
            compareBrackets(tournamentRecords);
            continue;
        } 
        else {
            cout << "\nInvalid value.  Please re-enter a value from the menu options below.\n";
        }
    } //end while(true)

        return 0;

} // end main()
