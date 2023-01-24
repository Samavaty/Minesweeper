// Minesweeper.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <stdLib.h>
//I DON'T KNOW
#include <vector>
//For Vectors
#include <cstdlib>
//For the random number generator
#include <Windows.h>
//For the sleep method to work
#include <algorithm>
//For The find function to work
#include <fstream>
//To Be able to work with files
#include <string>
//To use getline function

using namespace std;

enum relativelocation
{
    topleft,
    topright,
    downleft,
    downright,
    top,
    bottom,
    leftside,
    rightside,
    center
};
int VectorNumberCorrectionFunction(int currentRow, int currentColumn);
int EdgeBombNumberCorrectionFunction(relativelocation whichside, int currentRow, int currentColumn);
int BarBombsNumberCorrectionFunction(relativelocation whichside, int currentRow, int currentColumn);
int CenterBombNumberCorrectionFunction(int currentRow, int currentColumn);
int EmptySpaceRevealFunction(int currentRow, int currentColumn);
void LeftSideCheck(int currentPosition);
void RightSideCheck(int currentPosition);
void DownSideCheck(int currentPosition);
void UpSideCheck(int currentPosition);
void EmptySpaceVectorUpdate(int currentPosition);
void CheckForAHighScore(int score, string fullname, time_t startingTime, time_t finishingTime);
int ShowHallOfFame();
void ShowAboutTheCreator();
long numberOfColumns;
long numberOfRows;
int difficulty;
int numberOfAnticipatedBombs;
int selectedColumn;
int selectedRow;
vector<char> primaryTableVector;
vector<char> userTableVector;
bool gameOver = false;
long Score = 0;
bool gameCompleted = false;
string fullname;
time_t startingTime;
time_t finishingTime;

int main()
{
    //The Begining Of The Game
TheBegining:

    //The Starting Page
    int selectedOption;
    cout << "\t\t\t\t\t\t      MineSweeper\n\n";
    cout << "\t\t\t\t\t\t Welcome To MineSweeper\n\n";
    cout << "\t\t1.Enter The Game\n";
    cout << "\t\t2.Hall Of Fame\n";
    cout << "\t\t3.About The Creator\n";
    cout << "\t\t4.Exit\n\n\n";
    cout << "\tPlease Select An Option : ";
    cin >> selectedOption;
    switch (selectedOption)
    {
    case 1:
        system("cls");
        goto GameStartingPage;
        break;
    case 2:
        ShowHallOfFame();
        system("cls");
        goto TheBegining;
    case 3:
        system("cls");
        ShowAboutTheCreator();
        system("cls");
        goto TheBegining;
    case 4:
        exit(0);
    default:
        system("cls");
        goto TheBegining;
    }

GameStartingPage:
    // The Game Starting Page
#pragma region GameStartingPage

    cout << "\t\t\t\t\t\t    MineSweeper\n\n";
    cout << "\t\t\t\t\tEnter The Number Of Columns and Rows\n";
    do
    {
        cout << "Number Of Columns : ";
        cin >> numberOfColumns;

    } while (numberOfColumns < 5 || numberOfColumns>19);
    // Gets The correct Number of Columns and Then Columns as well as Bombs
    do
    {
        cout << "Number Of Row : ";
        cin >> numberOfRows;

    } while (numberOfRows < 5 || numberOfRows>19);

    long gameAria = numberOfRows * numberOfColumns;

    // Exeption Handling For difficulty
GettingNumberOfBombs:
    try
    {
        do
        {
            cout << "Difficulty (1:Easy - 2:Medium - 3:Hard) : ";
            cin >> difficulty;

        } while (difficulty != 1 && difficulty != 2 && difficulty != 3);
    }
    catch (const std::exception&)
    {
        difficulty = 0;
        goto GettingNumberOfBombs;
    }
    switch (difficulty)
    {
    case 1:
        numberOfAnticipatedBombs = gameAria / 20;
        break;
    case 2:
        numberOfAnticipatedBombs = gameAria / 10;
        break;
    case 3:
        numberOfAnticipatedBombs = gameAria / 5;
        break;
    }
    system("CLS");

#pragma endregion
    //The End of Starting Page

    int numberOfBombsNearby = 2;
    int numberOfActualBombs = 0;
    vector<int> bombLocationHolder;

#pragma region PrimaryVectorGenerationRegion

    //feeds the random number generator a key for it to generate new numbers
    srand((unsigned)time(NULL));
    //generates a location for bombs randomly in the vector

    //Creating a vector to store all the random bombs location
    vector<int> randomBombLocationHolder;
    for (int i = 0; i < numberOfAnticipatedBombs; i++)
    {
        randomBombLocationHolder.push_back(rand() % (gameAria));
    }
    //The intresting thing is that the random numbers might be the same so the actual bombs might be fewer than anticipated


    //Filling the primaryTableVector with ' ' place holders
    for (int i = 0; i < gameAria; i++)
    {
        primaryTableVector.push_back(' ');

    }
    //Putting the bombs in their locations from the randomBombLocationHolder
    for (int l = 0; l < numberOfAnticipatedBombs; l++)
    {
        primaryTableVector.at(randomBombLocationHolder.at(l)) = 'b';

    }
    //This gives us the number of actual bombs in the game
    //As well as stores their new location in our BombLocationHolder
    for (int i = 0; i < gameAria; i++)
    {
        if (primaryTableVector.at(i) == 'b')
        {
            bombLocationHolder.push_back(i);
            numberOfActualBombs++;
        }
    }




#pragma endregion

    //Completes The Primary Vector
#pragma region PrimaryVectorCompletionRegion

    //A 5*5 Vector for testing purposes
    //primaryTableVector = { 'b',' ', 'b', ' ', 'b', ' ', ' ', ' ', ' ', ' ', 'b', ' ', 'b', ' ', 'b', ' ', ' ', 'b', ' ', ' ', 'b', ' ', 'b', ' ', 'b' };

    //Draws The primaryTableVector for testing purposes
    //cout << "\n" << "Before any operations";
    //for (int j = 0; j < numberOfRows; j++)
    //{
    //    cout << "\n\n";
    //    for (int p = 0; p < numberOfColumns; p++)
    //    {
    //        cout << "|  " << primaryTableVector.at(j * numberOfColumns + p) << "  ";
    //    }
    //    cout << "|";
    //}


    //Fixing The numbers for edge bombs
    try
    {
        if (primaryTableVector.at(0) == 'b')
        {
            EdgeBombNumberCorrectionFunction(topleft, 0, 0);
        }
        if (primaryTableVector.at(numberOfColumns - 1) == 'b')
        {
            EdgeBombNumberCorrectionFunction(topright, 0, numberOfColumns - 1);
        }
        if (primaryTableVector.at((numberOfColumns * (numberOfRows - 1))) == 'b')
        {
            EdgeBombNumberCorrectionFunction(downleft, numberOfRows - 1, 0);
        }
        if (primaryTableVector.at((numberOfRows * numberOfColumns) - 1) == 'b')
        {
            EdgeBombNumberCorrectionFunction(downright, numberOfRows - 1, numberOfColumns - 1);
        }

    }
    catch (...)
    {
        cout << "There was an error in the the EDGE Bombs";
    }
    //Fixing The numbers for Bar area Bombs
    try
    {
        for (int i = 1; i < numberOfColumns - 1; i++)
        {
            if (primaryTableVector.at(i) == 'b')
            {
                BarBombsNumberCorrectionFunction(top, 0, i);
            }
            if (primaryTableVector.at(((numberOfRows - 1) * numberOfColumns) + i) == 'b')
            {
                BarBombsNumberCorrectionFunction(bottom, numberOfRows - 1, i);
            }
        }
        for (int i = 1; i < numberOfRows - 1; i++)
        {
            if (primaryTableVector.at(i * numberOfColumns) == 'b')
            {
                BarBombsNumberCorrectionFunction(leftside, i, 0);
            }
            if (primaryTableVector.at(((i + 1) * numberOfColumns) - 1) == 'b')
            {
                BarBombsNumberCorrectionFunction(rightside, i, numberOfColumns - 1);
            }
        }

    }
    catch (...)
    {
        cout << "There was an error in the Bar Bombs";
    }
    //Fixing The Numbers for Center Bombs
    try
    {
        for (int i = 1; i < numberOfRows - 1; i++)
        {
            for (int t = 1; t < numberOfColumns - 1; t++)
            {
                if (primaryTableVector.at((i * numberOfColumns) + t) == 'b')
                {
                    CenterBombNumberCorrectionFunction(i, t);
                }
            }
        }
    }
    catch (...)
    {
        cout << "There was a Problem with number assignment for central bombs";
    }
#pragma endregion

    //UserVectorGenerationRegion
    //Filling the userTableVector with ' ' place holders
    for (int i = 0; i < gameAria; i++)
    {
        userTableVector.push_back(' ');

    }

    // The Begining Of the Game Screen
#pragma region GameScreenAndGameLogicRegion
    gameCompleted = false;
    startingTime = GetCurrentTime();
    //selectedAction currentAction;
    char charCurrentAction;
    while (gameOver == false && gameCompleted == false)
    {
        system("cls");
        //Draws the primary vector for Testing purposes or if you want to win all the time
        //for (int j = 0; j < numberOfRows; j++)
        //{
        //    cout << "\n\n";
        //    for (int p = 0; p < numberOfColumns; p++)
        //    {
        //        cout << "|  " << primaryTableVector.at(j * numberOfColumns + p) << "  ";
        //    }
        //    cout << "|";
        //}
        //cout << "\n" << "The End of primary Vector" << "\n" << "The Begining of user Vector";
        string tempO;

        //Draws the user vector for a secound time
        for (int j = -1; j < numberOfRows; j++)
        {
            cout << "\n\n";
            if (j < 9)
            {
                tempO = "0" + to_string(j + 1);
            }
            else
            {

                tempO = to_string(j + 1);
            }
            cout << tempO << "  ";
            for (int p = 0; p < numberOfColumns; p++)
            {
                if (j == -1)
                {
                    if (p < 9)
                    {
                        cout << "|  " << p + 1 << "  ";

                    }
                    else
                    {
                        cout << "|  " << p + 1 << " ";

                    }
                }
                else
                {
                    cout << "|  " << userTableVector.at(j * numberOfColumns + p) << "  ";

                }
            }
            cout << "|";
        }

        try
        {
            //This part gets the selected row and column and action from the user
            do
            {
                cout << "\n" << "Which Column ? ";
                cin >> selectedColumn;

            } while (selectedColumn<1 || selectedColumn>numberOfColumns);
            do
            {
                cout << "\n" << "Which Row ? ";
                cin >> selectedRow;

            } while (selectedRow<1 || selectedRow>numberOfRows);
            do
            {
                cout << "\n" << "Which Action Reveal or Flag (r OR f OR E for Exit) ? ";
                cin >> charCurrentAction;

            } while (charCurrentAction != 'f' && charCurrentAction != 'r' && charCurrentAction != 'E');
            //when the user flags or reveals a flag
            if (charCurrentAction == 'r' && primaryTableVector
                .at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) == 'b' &&
                userTableVector.at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) == 'F')
            {
                continue;
            }
            else if (charCurrentAction == 'f' && primaryTableVector
                .at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) == 'b' &&
                userTableVector.at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) == 'F')
            {
                continue;
            }
            //When the user REVEALs a Bomb
            else if (charCurrentAction == 'r' && primaryTableVector
                .at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) == 'b')
            {
                gameOver = true;
                system("cls");
                cout << "\t\t\t\t\t\t" << "GAME OVER\n";
            }
            //When the user FLAGs a Bomb
            else if (charCurrentAction == 'f' && primaryTableVector
                .at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) == 'b')
            {
                userTableVector.at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) = 'F';
                Score += 1000 * difficulty;
            }
            //Whan the user REVEALs a Number
            else if (charCurrentAction == 'r' && primaryTableVector
                .at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) != 'b' &&
                primaryTableVector.at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) != ' ')
            {
                userTableVector.at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) =
                    primaryTableVector.at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1));
            }
            //When the user REVEALs an Empty Space
            else if (charCurrentAction == 'r' && primaryTableVector
                .at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) == ' ')
            {
                EmptySpaceRevealFunction(selectedRow - 1, selectedColumn - 1);
            }
            //When the user FLAGs an Empty Space
            else if (charCurrentAction == 'f' && primaryTableVector
                .at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) == ' ')
            {
                Score -= 100 * difficulty;
                cout << "\n\n\n\n\n\t\t\t\t\tYou've Flaged The Wrong Position";
                Sleep(1500);
            }
            //When the user FLAGs a Number
            else if (charCurrentAction == 'f' && primaryTableVector
                .at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) != 'b' &&
                primaryTableVector.at(((selectedRow - 1) * numberOfColumns) + (selectedColumn - 1)) != ' ')
            {
                Score -= 75 * difficulty;
                cout << "\n\n\n\n\n\t\t\t\t\tYou've Flaged The Wrong Position";
                Sleep(1500);
            }
            //when the user wants to exit to the main menu
            else if (charCurrentAction == 'E')
            {
                system("cls");
                Score = 0;
                startingTime = 0;
                gameOver = false;
                gameCompleted = false;
                goto TheBegining;
            }
            //check to see if the user has completed the game
            int tempX = 0;
            if (gameOver == false)
            {
                for (int i = 0; i < gameAria; i++)
                {
                    if (userTableVector.at(i) == ' ')
                    {
                        tempX++;
                    }
                }
                //this happens when the user wins the game
                if (tempX == 0)
                {
                    finishingTime = GetCurrentTime();
                    gameCompleted = true;
                    system("cls");
                    cout << "\t\t\t\t\t\t You've Won!\n\n\n";
                    cout << "\t\t\tPlease Enter Your Name : ";
                    cin >> fullname;
                    CheckForAHighScore(Score, fullname, startingTime, finishingTime);
                    Score = 0;
                    system("cls");
                    goto TheBegining;
                }

            }

        }
        catch (...)
        {
            cout << "something went wrong!";
        }

        if (gameOver == true)
        {
            finishingTime = GetCurrentTime();
            cout << "\t\t\t\tPlease Enter Your Name : ";
            cin >> fullname;
            CheckForAHighScore(Score, fullname, startingTime, finishingTime);
            gameOver = false;
            system("cls");
            Score = 0;
            goto TheBegining;
        }

    }
#pragma endregion


}

//Also need to add number of rows and test it again
//The Function That recieves coordinance of bombs on the edge of the vector
int EdgeBombNumberCorrectionFunction(relativelocation whichSide, int currentRow, int currentColumn) {

    if (whichSide == topleft)
    {
        VectorNumberCorrectionFunction(1, 0);
        VectorNumberCorrectionFunction(0, 1);
        VectorNumberCorrectionFunction(1, 1);
        return 0;
        //Success
    }
    else if (whichSide == topright)
    {
        VectorNumberCorrectionFunction(0, currentColumn - 1);
        VectorNumberCorrectionFunction(1, currentColumn);
        VectorNumberCorrectionFunction(1, currentColumn - 1);
        return 0;
        //Success
    }
    else if (whichSide == downleft)
    {
        VectorNumberCorrectionFunction(currentRow, 1);
        VectorNumberCorrectionFunction(currentRow - 1, 0);
        VectorNumberCorrectionFunction(currentRow - 1, 1);
        return 0;
        //Success
    }
    else if (whichSide == downright)
    {
        VectorNumberCorrectionFunction(currentRow, currentColumn - 1);
        VectorNumberCorrectionFunction(currentRow - 1, currentColumn);
        VectorNumberCorrectionFunction(currentRow - 1, currentColumn - 1);
        return 0;
        //Success
    }
    else
    {
        return 1;
        //Failed
    }

}

//The Function That recieves coordinance of bombs on the Bar Areas of the vector
int BarBombsNumberCorrectionFunction(relativelocation whichside, int currentRow, int currentColumn) {
    if (whichside == top)
    {
        VectorNumberCorrectionFunction(currentRow, currentColumn + 1);
        VectorNumberCorrectionFunction(currentRow, currentColumn - 1);
        VectorNumberCorrectionFunction(currentRow + 1, currentColumn);
        VectorNumberCorrectionFunction(currentRow + 1, currentColumn + 1);
        VectorNumberCorrectionFunction(currentRow + 1, currentColumn - 1);
        return 0;
        //Success
    }
    else if (whichside == bottom)
    {
        VectorNumberCorrectionFunction(currentRow, currentColumn + 1);
        VectorNumberCorrectionFunction(currentRow, currentColumn - 1);
        VectorNumberCorrectionFunction(currentRow - 1, currentColumn);
        VectorNumberCorrectionFunction(currentRow - 1, currentColumn + 1);
        VectorNumberCorrectionFunction(currentRow - 1, currentColumn - 1);
        return 0;
        //Success
    }
    else if (whichside == leftside)
    {
        VectorNumberCorrectionFunction(currentRow - 1, currentColumn);
        VectorNumberCorrectionFunction(currentRow + 1, currentColumn);
        VectorNumberCorrectionFunction(currentRow, currentColumn + 1);
        VectorNumberCorrectionFunction(currentRow + 1, currentColumn + 1);
        VectorNumberCorrectionFunction(currentRow - 1, currentColumn + 1);
        return 0;
        //Success
    }
    else if (whichside == rightside)
    {
        VectorNumberCorrectionFunction(currentRow - 1, currentColumn);
        VectorNumberCorrectionFunction(currentRow + 1, currentColumn);
        VectorNumberCorrectionFunction(currentRow, currentColumn - 1);
        VectorNumberCorrectionFunction(currentRow + 1, currentColumn - 1);
        VectorNumberCorrectionFunction(currentRow - 1, currentColumn - 1);
        return 0;
        //Success
    }
    else
    {
        return 1;
        //Failed
    }

}

//This Function Assigns The Correct Numbers to Central Bombs
int CenterBombNumberCorrectionFunction(int currentRow, int currentColumn) {
    VectorNumberCorrectionFunction(currentRow, currentColumn + 1);
    VectorNumberCorrectionFunction(currentRow, currentColumn - 1);
    VectorNumberCorrectionFunction(currentRow - 1, currentColumn);
    VectorNumberCorrectionFunction(currentRow - 1, currentColumn + 1);
    VectorNumberCorrectionFunction(currentRow - 1, currentColumn - 1);
    VectorNumberCorrectionFunction(currentRow + 1, currentColumn);
    VectorNumberCorrectionFunction(currentRow + 1, currentColumn + 1);
    VectorNumberCorrectionFunction(currentRow + 1, currentColumn - 1);
    return 0;
    //Success
}

//The function that replaces empty or not fully analyzed location with the apropriate value
int VectorNumberCorrectionFunction(int currentRow, int currentColumn) {
    int currentPosition = ((currentRow)*numberOfColumns) + currentColumn;
    switch (primaryTableVector.at(currentPosition))
    {
    case ' ':
        primaryTableVector.at(currentPosition) = '1';
        break;
    case '1':
        primaryTableVector.at(currentPosition) = '2';
        break;
    case '2':
        primaryTableVector.at(currentPosition) = '3';
        break;
    case '3':
        primaryTableVector.at(currentPosition) = '4';
        break;
    case '4':
        primaryTableVector.at(currentPosition) = '5';
        break;
    case '5':
        primaryTableVector.at(currentPosition) = '6';
        break;
    case '6':
        primaryTableVector.at(currentPosition) = '7';
        break;
    case '7':
        primaryTableVector.at(currentPosition) = '8';
        break;
    case 'b':
        break;
    default:
        //There was an ERROR!
        return 2;
        break;
    }
    return 0;
}

vector<int> emptySpaceLocations;
vector<int> checkedEmptySpaceLocation;
//The Function That reveals empty spaces
int EmptySpaceRevealFunction(int currentRow, int currentColumn) {
    int currentPosition = ((currentRow)*numberOfColumns) + currentColumn;
    relativelocation relativePosision;
    bool notFinished = true;
    emptySpaceLocations.push_back(currentPosition);
    //checkedEmptySpaceLocation.push_back(currentPosition);
    userTableVector.at(currentPosition) = '-';

    do
    {
        //Fing the correct value for Relative Position
#pragma region RelativeLocationOfThePositionInGame
        try
        {
            //figuring out the relative position
            if (currentPosition == 0)
            {
                relativePosision = topleft;
            }
            else if (currentPosition == numberOfColumns - 1)
            {
                relativePosision = topright;
            }
            else if ((numberOfColumns * (numberOfRows - 1)) == currentPosition)
            {
                relativePosision = downleft;
            }
            else if (((numberOfRows * numberOfColumns) - 1) == currentPosition)
            {
                relativePosision = downright;
            }
            for (int i = 1; i < numberOfColumns - 1; i++)
            {
                if (i == currentPosition)
                {
                    relativePosision = top;
                    break;
                }
                if ((((numberOfRows - 1) * numberOfColumns) + i) == currentPosition)
                {
                    relativePosision = bottom;
                    break;
                }
            }
            for (int i = 1; i < numberOfRows - 1; i++)
            {
                if ((i * numberOfColumns) == currentPosition)
                {
                    relativePosision = leftside;
                    break;
                }
                if ((((i + 1) * numberOfColumns) - 1) == currentPosition)
                {
                    relativePosision = rightside;
                    break;
                }
            }
            for (int i = 1; i < numberOfRows - 1; i++)
            {
                for (int t = 1; t < numberOfColumns - 1; t++)
                {
                    if (((i * numberOfColumns) + t) == currentPosition)
                    {
                        relativePosision = center;
                    }
                }
            }
        }
        catch (const std::exception&)
        {
            cout << "Failed To recognize the relative position of empty Space";
            Sleep(3000);
        }
#pragma endregion

        //this is for finding empty spaces that are connected
#pragma region FindingConnectedEmptySpaces
        try
        {
            if (relativePosision == center)
            {
                LeftSideCheck(currentPosition);
                RightSideCheck(currentPosition);
                UpSideCheck(currentPosition);
                DownSideCheck(currentPosition);
                EmptySpaceVectorUpdate(currentPosition);

            }
            else if (relativePosision == top)
            {
                LeftSideCheck(currentPosition);
                RightSideCheck(currentPosition);
                DownSideCheck(currentPosition);
                EmptySpaceVectorUpdate(currentPosition);
            }
            else if (relativePosision == bottom)
            {
                LeftSideCheck(currentPosition);
                RightSideCheck(currentPosition);
                UpSideCheck(currentPosition);
                EmptySpaceVectorUpdate(currentPosition);
            }
            else if (relativePosision == leftside)
            {
                RightSideCheck(currentPosition);
                UpSideCheck(currentPosition);
                DownSideCheck(currentPosition);
                EmptySpaceVectorUpdate(currentPosition);
            }
            else if (relativePosision == rightside)
            {
                LeftSideCheck(currentPosition);
                UpSideCheck(currentPosition);
                DownSideCheck(currentPosition);
                EmptySpaceVectorUpdate(currentPosition);
            }
            else if (relativePosision == topleft)
            {
                RightSideCheck(currentPosition);
                DownSideCheck(currentPosition);
                EmptySpaceVectorUpdate(currentPosition);
            }
            else if (relativePosision == topright)
            {

                LeftSideCheck(currentPosition);
                DownSideCheck(currentPosition);
                EmptySpaceVectorUpdate(currentPosition);
            }
            else if (relativePosision == downleft)
            {
                RightSideCheck(currentPosition);
                UpSideCheck(currentPosition);
                EmptySpaceVectorUpdate(currentPosition);
            }
            else if (relativePosision == downright)
            {
                LeftSideCheck(currentPosition);
                UpSideCheck(currentPosition);
                EmptySpaceVectorUpdate(currentPosition);
            }
        }
        catch (...)
        {
            cout << "Faild to complete empty space replacement";
            Sleep(3000);
        }
#pragma endregion

        //This Section checks to see if it has checked all the empty spaces
#pragma region MakingSureAllTheEmptySpacesAreChecked
        for (int i = 0; i < emptySpaceLocations.size(); i++)
        {
            int item = emptySpaceLocations.at(i);
            auto resualt = find(checkedEmptySpaceLocation.begin(), checkedEmptySpaceLocation.end(), item);
            if (resualt == checkedEmptySpaceLocation.end())
            {
                //it didnt find the item
                currentPosition = emptySpaceLocations.at(i);
                notFinished = true;
                break;
            }
            else
            {
                notFinished = false;
                //it did find the item
            }
        }
#pragma endregion


    } while (notFinished);
    emptySpaceLocations.clear();
    checkedEmptySpaceLocation.clear();
    return 0;
    //Success
}

//For finding empty spaces that are connected
void LeftSideCheck(int currentPosition) {
    //checks the left side
    if (primaryTableVector.at(currentPosition - 1) == ' ')
    {
        emptySpaceLocations.push_back(currentPosition - 1);
        userTableVector.at(currentPosition - 1) = '-';
    }
}
void RightSideCheck(int currentPosition) {
    //checks the right side
    if (primaryTableVector.at(currentPosition + 1) == ' ')
    {
        emptySpaceLocations.push_back(currentPosition + 1);
        userTableVector.at(currentPosition + 1) = '-';
    }
}
void UpSideCheck(int currentPosition) {
    //checks up
    if (primaryTableVector.at(currentPosition - numberOfColumns) == ' ')
    {
        emptySpaceLocations.push_back(currentPosition - numberOfColumns);
        userTableVector.at(currentPosition - numberOfColumns) = '-';
    }
}
void DownSideCheck(int currentPosition) {
    //checks down
    if (primaryTableVector.at(currentPosition + numberOfColumns) == ' ')
    {
        emptySpaceLocations.push_back(currentPosition + numberOfColumns);
        userTableVector.at(currentPosition + numberOfColumns) = '-';
    }
}
void EmptySpaceVectorUpdate(int currentPosition) {
    //adds the current position to the checkedEmptySpaceLocation Vetor
    checkedEmptySpaceLocation.push_back(currentPosition);
}

//It Show The Hall Of Fame Menu
int ShowHallOfFame() {

    try
    {
        system("cls");
        fstream TimesFile("Time.txt");
        fstream NamesFile("Name.txt");
        fstream ScoresFile("Score.txt");
        //how to write to files
        int i = 0;
        vector<string> Times;
        vector<string> Names;
        vector<string> Scores;
        string placeHolder1;
        string placeHolder2;
        string placeHolder3;
        //This part read the records from the files
        while (getline(TimesFile, placeHolder1)) {
            // Output the text from the file
            Times.push_back(placeHolder1);
        }
        while (getline(NamesFile, placeHolder2))
        {
            Names.push_back(placeHolder2);
        }
        while (getline(ScoresFile, placeHolder3))
        {
            Scores.push_back(placeHolder3);
        }
        string tempY;
        //THis part sorts the results using bubble sort algorithm
        for (int i = 0; i < Names.size(); i++)
        {
            for (int p = 0; p < Names.size() - 1; p++)
            {
                if (Times.at(i) < Times.at(p))
                {
                    tempY = Times.at(i);
                    Times.at(i) = Times.at(p);
                    Times.at(p) = tempY;
                    tempY = Names.at(i);
                    Names.at(i) = Names.at(p);
                    Names.at(p) = tempY;
                    tempY = Scores.at(i);
                    Scores.at(i) = Scores.at(p);
                    Scores.at(p) = tempY;
                }
            }
        }
        cout << "\n\t\t\t\t\t\tTHE HALL OF FAME\n";
        cout << "\n\n\t\t     Fullname\t\t\t\tTimes\t\t\t\tScores";
        for (int i = 0; i < Names.size(); i++)
        {
            cout << "\n\t\t" << Names.at(i) << "\t\t\t\t" << Times.at(i)
                << "   seconds\t\t\t\t" << Scores.at(i);
        }
        cout << "\n\n\t\t To Exit Enter 1 : ";
        TimesFile.close();
        NamesFile.close();
        ScoresFile.close();
        int tempU;
        do
        {
            cin >> tempU;
        } while (tempU != 1);
        return 0;
        //Done Successfully
    }
    catch (...)
    {
        system("cls");
        cout << "\n\n\t\tWe Were unable to Show you the HALL OF FAME";
        Sleep(3000);
        return 1;
    }

}
//This Function Check for a High Score
void CheckForAHighScore(int score, string fullname, time_t startingTime, time_t finishingTime) {
    //opening all the files in appending mode so as to not overwrite anything
    ofstream TimesFile("Time.txt", ios::app);
    ofstream NamesFile("Name.txt", ios::app);
    ofstream ScoresFile("Score.txt", ios::app);
    //Adding all the new results
    TimesFile << ((finishingTime - startingTime) / 1000) << "\n";
    NamesFile << fullname << "\n";
    ScoresFile << score << "\n";
    //Closing All the files
    TimesFile.close();
    NamesFile.close();
    ScoresFile.close();

}
//This Function Shows the About Creator Page
void ShowAboutTheCreator() {
    cout << "\t\t\t\t\t\t      MineSweeper\n\n";
    cout << "\t\t\t\t\t\t Welcome To MineSweeper\n\n";
    cout << "\t\t\t\t\t    This game was coded by S.Samavaty\n";
    cout << "\t\t\t\t     You are free to fork it or expand it in any way\n";
    cout << "\t\t\t\t\t\tI Hope you've enjoyed it";
    int tempI;
    do
    {
        cout << "\n\n\n\t\t\t\t Enter 1 to exit : ";
        cin >> tempI;
    } while (tempI != 1);
}
