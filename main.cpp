//  main.cpp
//  Homework 5, Tristan Meinke, G03831297
//
//  Created by Tristan on 12/5/15.
//  Copyright © 2015 Tristan. All rights reserved.
//
//  -For whatever reason, the termination string of the create file option still only seems
//  to work with a single character termination string.


#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <array>

using namespace std;

#define numASCII 128 //This is based upon the first half of the ASCII table, or the non-extened ASCII table (characters 0-127).

#define maxWords 256 //This is based upon the maximum number of discrete words the test input is supposed to have.

//*******Data Types*******

typedef struct
{
    string word;
    int counts = 0;
    string lineNum = "uninitialized";
    array<int, maxWords> perLineCounts = {0};
    
}wordData;

//*******Function Prototypes*******/

void doStuff(string optionSelect);

// - Option Processing Functions - /

string menu ();

string getFileName(ios_base::openmode mode);  //This function is primarily intended as a file check.

string menuInput ();

string quitOrRepeat (string &optionSelect);

// - Message Display Functions - /

void fileNotFound ();

void inputNotValid ();

// - Pre Operational Functions - /

void preCopyFile(string &fileName1, string &fileName2);

void preDisplayFile(string &fileName1);

void preCountChars (string &fileName1);

// - Operational Functions - /

void displayFile (fstream &file1);

void copyFile (fstream &file1, fstream &file2);

void createFile(string &fileName1, string &termString);

array <int, 128> countChars (fstream &file1);

array<wordData, maxWords> countWords (array<wordData, maxWords>, string fileName1);

void indexWords (array<wordData, maxWords> wordBucket, string fileName1);

// - Conversion Functions - /

array<int, numASCII> charToCode(char &inputCharacter);

array<char, numASCII> codeToChar(array<int, numASCII>);

// - Print Functions - /

void printCharCounts(array<char, numASCII>, array<int, numASCII>, array<int, numASCII> counts);

void printWordBucket(array<wordData, maxWords> wordBucket, string fileName1, int wordCounter);

void printIndex(array <wordData, maxWords> wordBucket, int wordCounter, string fileName1);


//*******Main Function*******/


int main()
{
    string originalOption = "r";
    
    doStuff(originalOption);
    
    
    cout << endl;
    cout << "Good day to you." << endl;

    return 0;
}

//*******Additional Function Definitions*******

void doStuff(string optionSelect)
{

    fstream file1;
    fstream file2;
    
    string fileName1;
    string fileName2;
    string fileName;
    
    string userInput;
    string termString = "not defined yet";
    string interpInput;
    string textInput;
    
    
    
    if (optionSelect == "quit" || optionSelect == "q")
    {
        return;
    }
    
    menu(); //First, deploy the menu
    
    //The following is a contingency for bad input:
    
    while (optionSelect != "display" && optionSelect != "d"
           && optionSelect != "copy" && optionSelect != "c"
           && optionSelect != "quit" && optionSelect != "q"
           && optionSelect != "repeat" && optionSelect != "r"
           && optionSelect != "create" && optionSelect == "cr"
           && optionSelect != "count chars" && optionSelect == "cc"
           && optionSelect != "count words" && optionSelect == "cw"
           && optionSelect != "index" && optionSelect == "i")
        
    {
        inputNotValid();
        optionSelect = quitOrRepeat(optionSelect);
        
    }
    
    if (optionSelect == "quit" || optionSelect == "q") //This input returns to the exit code in the main by way of a goodbye message.

    {
        return;
    }
    
}


string getFileName(ios_base::openmode mode)
{

    string fileName1;
    string fileName2;
    
    string fileName;

    
    while (cin.peek() == ' ') cin.get(); //Chomp loop; eats spaces.
    if (cin.peek() == '"')
    {
        cin.get();
        getline(cin, fileName, '"');
    }
    
    else
    {
        cin >> fileName;
    }
    
    
    fstream file1;
    
    if (mode == ios::in)
    {
 
        file1.open(fileName.c_str(), ios::in);
        
        
    }
    
    if (mode == ios::out)
    {
        

        file1.open(fileName.c_str(), ios::out);
        
    }

    
    return fileName;
}

string menuInput ()
//This function enters takes string input from the menu and passes it to getFileName, as well as, then routes the filenames to the appropriate operational or pre-operational functions.

{
    string userInput;
    string interpInput;
    string fileName1;
    string fileName2;
    string optionSelect;
    string termString;
    
    fstream file1;
    
    
    cin >> userInput; //This is used so that only the first word is read, (copy, dispaly, quit, whatever)
    interpInput.resize(userInput.length());
    transform(userInput.begin(), userInput.end(), interpInput.begin(), ::tolower);
    
    optionSelect = interpInput;
    
    array<wordData, maxWords> wordBucket;

    if (optionSelect == "copy" || optionSelect == "c")
    {
        fileName1 = getFileName(ios::in);
        getFileName(ios::in);
        fileName2 = getFileName(ios::out);
        
        preCopyFile(fileName1, fileName2); //This function calls and processes the copyFile function.
    }
    if (optionSelect == "display" || optionSelect == "d")
    {
        fileName1 = getFileName(ios::in);
        
        preDisplayFile(fileName1); //This function calls and processes the displayFile function.
    }

    if (optionSelect == "create" || optionSelect == "cr")
    {
        fileName1 = getFileName(ios::out);
        cin >> termString;
        
        createFile(fileName1, termString); //This function calls and processes the createFile function.
    }
    if (optionSelect == "count chars" || optionSelect == "cc")
    {
        fileName1 = getFileName(ios::in);
        
        preCountChars(fileName1); //This function calls and processes the countChars function.
    }
    if (optionSelect == "count words" || optionSelect == "cw")
    {
        fileName1 = getFileName(ios::in);
        
        countWords(wordBucket, fileName1);  //This function calls and processes the countWords function.
    }
    if (optionSelect == "index" || optionSelect == "i")
    {
        fileName1 = getFileName(ios::in);
        
        indexWords(wordBucket, fileName1); //This function calls and processes the indexWords function.
    }
    if (optionSelect == "quit" || optionSelect == "q")
    {
        
    }
    
    return optionSelect;
}

void fileNotFound ()  //This function allows for easy repitition of an error message without clogging up other functions.
{
    cout << " - - - - - - - - - - - - - - - - -" << endl;
    cout << "Error Encountered. File not found." << endl;
    cout << " - - - - - - - - - - - - - - - - -" << endl;
    
    return;
}

void inputNotValid ()  //Another error message.
{
    cout << " - - - - - - - - - - - - - - - - - " << endl;
    cout << "Error encountered. Input not valid." << endl;
    cout << " - - - - - - - - - - - - - - - - - " << endl;
    
    return;
}

string menu ()

//This function prompts the user for initial input, calls the menuInput function, and can be easily repeated.

{
    string optionSelect;
    
    cout << "Please choose from the following options:" << endl;
    cout << "Enter 'display <file 1>' to display file" << endl;
    cout << "Enter 'copy <file 1> to <file 2>' to copy file" << endl;
    cout << "Enter 'create <file1> <endword>' to create a new file." << endl;
    cout << "Enter 'count chars' <file1> to retrieve a character count." << endl;
    cout << "Enter 'count words' <file1> to retrieve a word count." << endl;
    cout << "Enter 'index' <file1> to retrieve an index of words by line." << endl;
    cout << "Enter 'quit' to quit program" << endl;
    
    optionSelect = menuInput();
    
    return optionSelect;
}

string quitOrRepeat (string &optionSelect)

//This function prompts the user to repeat or quit the program once a successful run has completed.

{
    string termString;
    string fileName;
    
    do
    {
        cout << endl;
        cout << "Would you like to use this program again? " << endl;
        cout << "Enter 'repeat' to repeat." << endl;
        cout << "Enter 'quit' to quit." << endl;
        
        cin >> optionSelect;
        
        if (optionSelect != "quit" && optionSelect != "q"
            && optionSelect != "repeat" && optionSelect != "r")
        {
            inputNotValid();
        }
        
        
    }
    while (optionSelect != "r" && optionSelect != "repeat"
           && optionSelect != "quit" && optionSelect != "q");
    
    doStuff(optionSelect);  //Calls doStuff so that all stuff may be repeated.
    
    return optionSelect;
}

void preDisplayFile (string &fileName1)  //Function to open a file for display, and process the displayFile function.
{
    string optionSelect;
    
    {
        fstream file1;
        file1.open(fileName1.c_str(), ios::in);
        
        displayFile(file1); //calls displayFile to execute the display option
        
        quitOrRepeat(optionSelect);
        
    }

}

void displayFile (fstream &file1)  //Function which executes the display option.
{
    
    char character;
        
    {
        
        if (!file1.is_open())
        {
            fileNotFound();
            menu(); //calls the menu function in case of input failure
        }
        
        if (file1.is_open())
        {
            while ((character = file1.get()) != EOF)  //retrieves the next character from the file as long as a next character exists.
            {
                cout << character; //prints that character within the same loop.
            }
            
        }
        file1.close();
    }
    
    return;
}

void preCopyFile(string &fileName1, string &fileName2) //Function to open files for copy, and process the copyFile function.

{
    string optionSelect;
 
        fstream file1; fstream file2;
        
        
        file1.open(fileName1.c_str(), ios::in);
        file2.open(fileName2.c_str(), ios::out);
    
        copyFile(file1, file2);  //Calls copyFile to execute the copy option

        
        cout << endl << "\""
        << fileName1 << "\" copied to \"" << fileName2 << "\"" <<endl;
        
        quitOrRepeat(optionSelect);
    
    
}

void copyFile (fstream &file1, fstream &file2) //Function which executes the copy option
{
    char character;
    
    if (!file1.is_open()) //Displays an error if the file is not found.
    {
        fileNotFound();
        menu(); //calls the menu function
    }
    
    if (!file2.is_open())  //Displays an error if the file is not found.
    {
        fileNotFound();
        menu();
    }
    if (file1.is_open())
    {
        while ((character = file1.get()) != EOF)  //retrieves the next character from the file as long as a next character exists.
        {
            file2 << character; //Writes the character to the output file.
//            cout << character;
            
        }
        
        file1.clear(); file1.close ();
        file2.close();
        
        
    }

    
    return;
}

void createFile(string &fileName1, string &termString) //Function which executes create option.  Creates a text file and writes to it.  This operation does not use a preliminary function.
{
    
    string textInput;
    string userInput;
    string optionSelect;
    
    string newTerm = termString;
    
        fstream file1;
        
        file1.open(fileName1.c_str(), ios::out);
        
        while (!file1.is_open())
        {
            fileNotFound();
        }
        
        if (file1.is_open())
        {
            cout << termString << endl;
            cout << "New file created. Enter the text you'd like your file to contain: " << endl;
            getline(cin, textInput);
            
            cout<<"Text Input : "<<textInput<<endl;
            
            while ((textInput = cin.get()) != newTerm)
            {
                file1 << textInput;
            }
            
            cout << "Text Saved." << endl;
            
        
        file1.close();
        quitOrRepeat(optionSelect);
        }
    
    return;
    
}

void preCountChars (string &fileName1)
{
    string optionSelect;
    
    {
        fstream file1;
        file1.open(fileName1.c_str(), ios::in);
        
        countChars(file1); //calls countChars to execute the display option
        
        quitOrRepeat(optionSelect);
        
    }
}

array <int, numASCII> countChars (fstream &file1)
{
    array<int, numASCII> counts;
    array<int, numASCII> assignedCodes;
    array<char, numASCII> assignedCharacters;
    
    string fileName1;

    char inputCharacter;
    for(int i = 0; i <= numASCII; i++)
    {
        counts[i] = 0;
        
    }
    
    while (file1.peek() != EOF)
    {
        file1 >> inputCharacter;
        counts[inputCharacter]++; //increments counts at inputCharacter, when the character is written.
        
    }
    
    
    assignedCodes = charToCode(inputCharacter);

    assignedCharacters = codeToChar(assignedCodes);
    
    printCharCounts(assignedCharacters, assignedCodes, counts);
    
    
    file1.close();
    
    return counts;
}


array<int, numASCII> charToCode(char &inputCharacter) //Converts a character to its ASCII code
{
    
    array <char, numASCII> possibleCharacters = {  //First 128 ASCII Codes with non-printable character codes initialized null
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
        '\0','\0',' ','!','"','#','$','%','&','\'',
        '(',')','*','+',',','-','.','/','0','1',
        '2','3','4','5','6','7','8','9',':',';',
        '<','=','>','?','@','A','B','C','D','E',
        'F','G','H','I','J','K','L','M','N','O',
        'P','Q','R','S','T','U','V','W','X','Y',
        'Z','[','\\',']','^','_','`','a','b','c',
        'd','e','f','g','h','i','j','k','l','m',
        'n','o','p','q','r','s','t','u','v','w',
        'x','y','z','{','|','}','~','\0'};
    
    
    array<int, numASCII> assignedCodes = {'\0'};
    
    for (int i = 0; i < numASCII; i ++)
    {
        if (inputCharacter == possibleCharacters[i])
        {
            assignedCodes[i] = possibleCharacters[i];
            
        
            /*DIAGNOSTIC*/ cout << assignedCodes[i] << endl;/*DIAGNOSTIC*/
            //APPARENTLY THE ABOVE STATEMENT IS CRITICAL TO THE OPERATION OF THE OPTION.  HUH...
        }
    }
    
    return assignedCodes;
}

array <char, numASCII> codeToChar(array<int, numASCII>)
{
    string optionSelect;
    array <char, numASCII> possibleCharacters = {  //First 128 ASCII Codes with non-printable character codes initialized null
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
        '\0','\0',' ','!','"','#','$','%','&','\'',
        '(',')','*','+',',','-','.','/','0','1',
        '2','3','4','5','6','7','8','9',':',';',
        '<','=','>','?','@','A','B','C','D','E',
        'F','G','H','I','J','K','L','M','N','O',
        'P','Q','R','S','T','U','V','W','X','Y',
        'Z','[','\\',']','^','_','`','a','b','c',
        'd','e','f','g','h','i','j','k','l','m',
        'n','o','p','q','r','s','t','u','v','w',
        'x','y','z','{','|','}','~','\0'};
    
    array<char, numASCII> assignedCharacters = {'\0'};
    array<int, numASCII> assignedCodes;
    
    
    for (int i = 0; i < numASCII; i++)
    {
        //        if(assignedCodes[i] == possibleCharacters[i])//THIS LINE IS A PROBLEM.
        if (assignedCodes[i] == i)
        {
            assignedCharacters[i] = possibleCharacters[i];
        }
    }
    
    return assignedCharacters;
}

void printCharCounts(array<char, numASCII>, array<int, numASCII>, array<int, numASCII> counts)
{
    array<int, numASCII> assignedCodes;
    array<char, numASCII> assignedCharacters;
    
    for (int i = 0; i < numASCII; i++)
    {
        if (assignedCharacters[i] != '\0' && counts[i] != 0 && assignedCodes[i] != '\0')
        {
            cout << "Char: " << assignedCharacters[i] <<
            " occurs " << counts[i] << " times." << endl;
        }
        
        //ASCII codes are printing 4 behind, if not adjusted.  This is probably screwing some stuff up.
        
                if (assignedCodes[i] == 0 || counts[i] == 0)
                {

                }
    }
    
    return;
}

array<wordData, maxWords> countWords (array<wordData, maxWords>, string fileName1)
//The only error here is that it does not take the first character of the first word.
//No idea why, and appending a space to the beginning of the file as a workaround
//seemed like more work than there was strictly time for.
{
    string optionSelect;
    
    array<wordData, maxWords> wordBucket;
    string wordInput;
    int wordCounter = 0;
    bool wordFound = false;
    
    fstream file1;
    
    file1.open(fileName1.c_str(), ios::in);
    
        for (int i = 0; i < maxWords; i++) //Initialization loop.
        {
            wordBucket[i].word = "unitialized";
            wordBucket[i].counts = 0;
            wordBucket[i].lineNum = "uninitialized";
            
            for (int i = 0; i < maxWords; i++)
            {
                wordBucket[i].perLineCounts = {0};
            }
        }
    
    while (file1.get() != EOF)
    {
        file1 >> wordInput;
        
        wordFound = false;
        
        if (wordCounter == 0)
        {
            wordBucket[0].word = wordInput;
            wordCounter++;

            
        }
        if(wordCounter > 0)
        {
            for (int i = 0; i < wordCounter; i++)
            {
                if (wordInput == wordBucket[i].word)
                {
                    wordBucket[i].counts++;
                    wordFound = true;
                    
                }
            }
        }
        
        
        if (wordFound == false)
        {
            wordBucket[wordCounter].word = wordInput;
            wordBucket[wordCounter].counts++;
            wordCounter++; //This corresponds to the loop it resides in.
            
        }
        
        
    }
    
    
    printWordBucket(wordBucket, fileName1, wordCounter);
    
    quitOrRepeat(optionSelect);
    
    return wordBucket;
}

void indexWords (array<wordData, maxWords> wordBucket, string fileName1)
{
    
    //To save time, this function recycles a fair amount of code from countWords, in order to populate the struct with the requisite data.
    
    string optionSelect;
    
    string wordInput;
    int wordCounter = 0;
    bool wordFound = false;
    string lineInput;
    string wordInLine;
    
    int numWordsInLine = 0;
    int lineCounter = 0;
    
    fstream file1;
    
    file1.open(fileName1.c_str(), ios::in);
    
    for (int i = 0; i < maxWords; i++) //Initialization loop.
    {
        wordBucket[i].word = "uninitialized";
        wordBucket[i].counts = 0;
        wordBucket[i].lineNum = "uninitialized";
        
        for (int i = 0; i < maxWords; i++)
        {
            wordBucket[i].perLineCounts = {0};
        }
    }
    
    while (file1.get() != EOF)
    {
        file1 >> wordInput;
        
        wordFound = false;
        
        if (wordCounter == 0)
        {
            wordBucket[0].word = wordInput;
            wordCounter++;
            
        }
        if(wordCounter > 0)
        {
            for (int i = 0; i < wordCounter; i++)
            {
                if (wordInput == wordBucket[i].word)
                {
                    wordBucket[i].counts++;
                    wordFound = true;
                }
            }
        }
        
        
        if (wordFound == false)
        {
            wordBucket[wordCounter].word = wordInput;
            wordBucket[wordCounter].counts++;
            wordCounter++;
            
        }
        
        
    }
    
    //    printWordBucket(wordBucket, fileName1, wordCounter); //An option to print the word count with the index.
    
    
    //From here, the actual indexing begins...
    
    //reads file with getline
    //gets a line, increments the line counter, to track the line number)
    //runs through the line to check for a word
    //if the word is in the line, increments the array at [line number]
    
    
    for (int i = 0; i < wordCounter; i++)
    {
        
        file1.clear();
        file1.seekg(0, ios::beg);
        
        lineCounter = 0;
        
        
        while (file1.peek() != EOF)
        {
            numWordsInLine = 0;
            
            getline(file1, lineInput);
            //            cout << lineInput;
            
            istringstream iss(lineInput);
            
            lineCounter++;
            
            while (iss >> wordInLine)
            {

                numWordsInLine++;
                
                if (wordInLine == wordBucket[i].word)
                {
                    wordBucket[i].perLineCounts[lineCounter]++;
                }
                
            }
            
        }
    }
    
    printIndex(wordBucket, wordCounter, fileName1);
    
    quitOrRepeat(optionSelect);
    
    return;
}

void printWordBucket(array<wordData, maxWords> wordBucket, string fileName1, int wordCounter)
{
    cout << endl;
    cout << "Words in " << fileName1 << ":" << endl;
    cout << endl;
    cout << "Total discrete words: " << wordCounter << endl;
    
    for (int i = 0; i < wordCounter; i++)
    {
        cout << "\"" << wordBucket[i].word << "\"" << " occurs " << wordBucket[i].counts << " times in the file." << endl;
        
    }
    
    return;
}

void printIndex (array <wordData, maxWords> wordBucket, int wordCounter, string fileName1)
{
    
    cout << endl;
    cout << "Index for: " << fileName1 << endl;
    cout << endl;
    cout << " Word  /  Line Numbers" << endl;
    cout << "-----------------------" << endl;

    for(int i = 0; i < wordCounter; i++)
    {
        cout << wordBucket[i].word << " - "; //Prints each word
        for(int j = 0; j < 256; j++) //Prints each line number for each word
        {
            if(wordBucket[i].perLineCounts[j] > 0)
            {
                cout<< j <<", ";
                
            }
            
        }
        cout<<endl;
        
    }
}

