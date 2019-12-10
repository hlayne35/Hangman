#include <iostream>
#include <string>
#include <algorithm>
#include <utility>
#include <chrono>
#include <thread>
#include <cstdlib>



// Clear the screen before printing more
void ClearAll()
{
#ifdef WIN32
    std::system("cls");
    std::system("cls");
#else
    // Assume POSIX
    std::system ("clear");
    std::system ("clear");
#endif
}

// Pause on this screen for x seconds
void Sleepy(int Time)
{
    std::this_thread::sleep_for(std::chrono::seconds(Time));
}

// Capitalize all letters in string
std::string MakeUp(std::string Input)
{
    transform(Input.begin(), Input.end(), Input.begin(), ::toupper);
    return Input;
}

// Print error message and get new input
std::string Invalid(std::string Input)
{
    std::cout << "\n" << "Input was invalid, please type valid input" << "\n" << std::endl;
    std::getline (std::cin, Input);
    std::cout << std::endl;
    Input = MakeUp(Input);
    return Input;
}

// Check if Master input any letters
bool HasAlpha(std::string Answer)
{
    for (char i: Answer)
    {
        if (i >= 'A' && i <= 'Z')
        {
            return true;
        }
        else
        {
            continue;
        }
    }
    return false;
}

// Turn user input into a single char
char GetLet()
{
    std::string NewChar;
    std::getline (std::cin, NewChar);
    NewChar = MakeUp(NewChar);
    while (NewChar.empty() || !(NewChar[0] >= 'A' && NewChar[0] <= 'Z'))
    {
        NewChar = Invalid(NewChar);
    }
    char Letter = NewChar[0];
    if(NewChar == "HELP")
    {
        Letter = '?';
    }
    return Letter;
}

// Print letters that haven't been guessed
void AlphaLeft(std::string Guesses)
{
    std::string Alphabet = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";
    for (char i: Guesses)
    {
        if (Alphabet.find(i) != std::string::npos)
        {
            std::replace (Alphabet.begin(), Alphabet.end(), i, '-');
        }
    }
    std::cout << Alphabet << "\n" << std::endl;
}

// Print word representing lost tries
void ChanceUsed(bool WoF, int Tries)
{
    std::string Losing;
    if (!WoF)
    {
        Losing = "HANGMAN";
    }
    if (WoF)
    {
        Losing = "WHEELOFFORTUNE";
    }
    for (int i = 0; i < Tries; i++)
    {
       std::cout << Losing[i];
    }
    for (int i = Tries; i < static_cast<int>(Losing.size()); i++)
    {
        std::cout << "*";
    }
    std::cout << std::endl;
}

// Create initial string of blank letters
std::string MakeBlanks(std::string Answer)
{
    std::string Blanks;
    for (char i: Answer)
    {
        if(i >= 'A' && i <= 'Z')
        {
            Blanks.push_back('_');
        }
        else
        {
            Blanks.push_back(i);
        }
    }
    return Blanks;
}

// Fill in blanks with correct letters
std::string FillBlanks(char Letter, std::string Answer, std::string Blanks)
{
    for (auto i = 0; i < static_cast<int>(Answer.size()); i++)
    {
        if(Answer[i] == Letter)
        {
            Blanks[i] = Letter;
        }
    }
    return Blanks;
}

// Display current game data for player
void DisplayAll(std::string Blanks, std::string Guesses, bool WoF, int Tries)
{
    ClearAll();
    AlphaLeft(Guesses);
    //std::cout << "\n" << Guesses << "\n" << std::endl;
    ChanceUsed(WoF, Tries);
    std::cout << "\n" << Blanks << "\n" << std::endl;
}

// Automatically fill some blanks first turn
std::pair<std::string, std::string> GetHelp(std::string Answer, std::string Guesses, std::string Blanks)
{
    std::string Choice;
    std::string Consonant = "RSTLN";
    std::string Vowel = "AEIOU";
    std::cout << "\n" << "Choose 'CONSONANT' or 'VOWEL'" << "\n" << std::endl;
    std::getline(std::cin, Choice);
    Choice = MakeUp(Choice);
    while (Choice[0] != 'C' && Choice[0] != 'V')
    {
        Choice = Invalid(Choice);
    }
    if (Choice[0] == 'C')
    {
        Choice = "RSTLN";
    }
    if (Choice[0] == 'V')
    {
        Choice = "AEIOU";
    }
    for (char i: Choice)
    {
        Guesses.push_back(i);
        Blanks = FillBlanks(i, Answer, Blanks);
    }
    return std::make_pair(Guesses, Blanks);
}



///////////     The actual game code!!!     ///////////
bool Hangman()
{
    ClearAll();
    std::string Answer;
    std::string Blanks;
    std::string Mode;
    bool WoF = false;
    char Letter = 0;
    std::string Guesses;
    int Max = 0;
    int Tries = 0;

    std::cout << "Master, enter your chosen word or phrase:" << "\n" << std::endl;
    std::getline (std::cin, Answer);
    Answer = MakeUp(Answer);
    while (Answer.empty() || !HasAlpha(Answer))
    {
        Answer = Invalid(Answer);
    }
    std::cout << "\n" << Answer << std::endl;

    Blanks = MakeBlanks(Answer);

    Sleepy(3);
    ClearAll();

    std::cout << "Player, choose game mode: type 'NORMAL' or 'FORTUNE'" << "\n" << std::endl;
    std::getline (std::cin, Mode);
    Mode = MakeUp(Mode);
    while ((Mode[0] != 'N') && (Mode[0] != 'F'))
    {
        Mode = Invalid(Mode);
    }
    if (Mode[0] == 'N')
    {
        Mode = "NORMAL";
        Max = 7;
    }
    if (Mode[0] == 'F')
    {
        Mode = "FORTUNE";
        Max = 14;
        WoF = true;
    }
    std::cout << "\n" << "You chose: " << Mode << "\n";
    std::cout << "You get " << Max << " attempts" << "\n" << std::endl;
    std::cout << "Note that if you enter more than one letter," << "\n";
    std::cout << "only the first letter will be your guess" << std::endl;
    std::cout << "\n" << "Type 'HELP' for help on first turn only" << std::endl;
    Sleepy(7);
    ClearAll();

    std::cout << Blanks << "\n" << std::endl;

    while (Tries <= Max)
    {
        DisplayAll(Blanks, Guesses, WoF, Tries);
        Letter = GetLet();

        if (Tries == 0 && Letter == '?')
        {
            std::pair <std::string, std::string> Autofill = GetHelp(Answer, Guesses, Blanks);
            Guesses = Autofill.first;
            Blanks = Autofill.second;
            Tries += 5;
            DisplayAll(Blanks, Guesses, WoF, Tries);
            continue;
        }

        while (Guesses.find(Letter) != std::string::npos)
        {
            std::cout << "Please enter a new letter" << "\n";
            Letter = GetLet();
        }

        Guesses.push_back(Letter);

        if (Answer.find(Letter) != std::string::npos)
        {
            Blanks = FillBlanks(Letter, Answer, Blanks);
        }
        else
        {
            Tries++;
        }
        if (Blanks == Answer)
        {
            DisplayAll(Blanks, Guesses, WoF, Tries);
            std::cout << Answer << " is the correct answer!" << "\n";
            return true;
        }
    }

    if (Blanks != Answer)
    {
        DisplayAll(Blanks, Guesses, WoF, Tries);
        std::cout << "Sorry, the answer was " << Answer << "\n";
        return false;
    }
    return false;
}
///////////         (end game code)         ///////////



// Print win or loss, ask to play again
bool Game()
{
    bool Results = Hangman();
    std::cout << "\n";
    if (Results)
    {
        std::cout << "\n" << "YOU WON" << "\n" << std::endl;
    }
    else
    {
        std::cout << "\n" << "YOU LOST" << "\n" << std::endl;
    }

    std::cout << "Do you want to play again: type 'YES' or 'NO'" << "\n" << std::endl;
    std::string Replay;
    std::getline (std::cin, Replay);
    Replay = MakeUp(Replay);
    while ((Replay[0] != 'Y') && (Replay[0] != 'N'))
    {
        Replay = Invalid(Replay);
    }
    if (Replay[0] == 'Y')
    {
        std::cout << std::endl;
        return true;
    }
    if (Replay[0] == 'N')
    {
        std::cout << std::endl;
        return false;
    }
    return false;
}

// Reset whole game loop until no replay
int main()
{
    bool Play = true;
    while (Play)
    {
        Play = Game();
    }
    return 0;
}


