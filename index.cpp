#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <limits>
using namespace std;
 
struct sWord
{
   char data[30];
   sWord *next;
};


struct dWord
{
    char data[30];
    sWord* synonym,*antonym;
    dWord* next, *previous;
};



struct Dictionary
{
    dWord* head,* tail;
};


 
Dictionary dictionary;
 

 
// Function to parse the dictionary file and create a doubly linked list of words
 
Dictionary parseInputFile(const string& filename) {
    Dictionary dict;
    dict.head=nullptr;
    dict.tail=nullptr;
    ifstream inputFile(filename);
    if(!inputFile.is_open()){
        cerr<<"Unable to open file: "<<filename<<endl;
        return dict;
    }
string line;
while (getline(inputFile,line)){
    stringstream ss(line);
    string word;
    dWord* newWord=new dWord();
    newWord->synonym=nullptr;
    int colonPos=line.find(':');
    if (colonPos!=string::npos)
    {
        string mainWord=line.substr(0, colonPos);
        strcpy(newWord->data, mainWord.c_str());
string synonyms=line.substr(colonPos + 1,line.find('#')-colonPos-1);
stringstream synonymStream(synonyms);
while (getline(synonymStream,word,':')){
    sWord* synonym=new sWord;
    strcpy(synonym->data,word.c_str());//$$
    synonym->next=newWord->synonym;
    newWord->synonym=synonym;
}


    }
    
string antonyms = line.substr(line.find('#') + 1);
stringstream antonymStream(antonyms);
while (getline(antonymStream, word, '#'))
{
    sWord *antonym= new sWord();
    strcpy (antonym->data, word.c_str());
    antonym->next = newWord->antonym;
    newWord->antonym = antonym;
}if (dict.head==nullptr)
{
  dict.head=newWord;
  dict.tail=newWord;
}
else{
    dict.tail->next=newWord;
    newWord->previous=dict.tail;
    dict.tail=newWord;
}
}
inputFile.close();
return dict;

 

}
// function to display the dictionary
 void displayDictionary(const Dictionary& dict) {
dWord* currWord = dict.head;
while (currWord!=nullptr)
{
    cout<<"Word: "<<currWord->data<<endl;
    sWord* currSynonym = currWord->synonym;
    if (currSynonym != nullptr)
    {
      cout<<"synonyms";
while (currSynonym != nullptr)
{
   cout<<currSynonym->data << " ";
   currSynonym=currSynonym->next;
}
cout<<endl;

    }
    sWord*currAntonym = currWord->antonym;
    if (currAntonym != nullptr)
    {
        cout<<"Antonyms:";
        while (currAntonym != nullptr)
        {
            cout<<currAntonym->data << " ";
            currAntonym=currAntonym->next;
        }
        cout<<endl;
    }
    cout<<endl;
    currWord=currWord->next;
    
}}


    void writeDictionaryToFile(const Dictionary& dict, const string& filename) {
    ofstream outputFile(filename); // open the file
    if (!outputFile.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        return;
    }

    dWord* currWord = dict.head; // start from the head of the dict
    while (currWord != nullptr) {
        outputFile << currWord->data; // write the current word to the file

        // Writing synonyms for the curr word
        sWord* currSynonym = currWord->synonym;
        while (currSynonym != nullptr) {
            outputFile << ":" << currSynonym->data; // Write each synonym for the current word separated by ':'
            currSynonym = currSynonym->next;
        }

        // Writing antonyms for the curr word
        sWord* currAntonym = currWord->antonym;
        while (currAntonym != nullptr) {
            outputFile << "#" << currAntonym->data; // Write each antonym for the current word separated by '#'
            currAntonym = currAntonym->next;
        }

        outputFile << endl;
        currWord = currWord->next; // move to the next word in the dict
    }

    outputFile.close(); // close the file
}

void sortDictionary(Dictionary& dict) {
    bool swapped; // to track if any swapping occurs in a pass
    dWord* ptr1; // pointer to traverse the dictionary
    dWord* lptr = nullptr; // last checked ptr to optimize the sorting

    // Check if the dictionary is empty or has only one element
    if (dict.head == nullptr || dict.head->next == nullptr) {
        return; // if so, there's no need to sort
    }

    do {
        swapped = false; // Reset the swapped flag for each pass
        ptr1 = dict.head; // Start from the head of the dictionary

        while (ptr1->next != lptr) {
            // Compare current node with next node
            if (strcmp(ptr1->data, ptr1->next->data) > 0) {
                // If the current word is greater than the next word alphabetically,
                // swap their data (words), synonyms, and antonyms
                swap(ptr1->data, ptr1->next->data);
                swap(ptr1->synonym, ptr1->next->synonym);
                swap(ptr1->antonym, ptr1->next->antonym);
                swapped = true; // Set swapped flag to true
            }
            ptr1 = ptr1->next; // Move to the next word
        }
        lptr = ptr1; // Update the last checked pointer
    } while (swapped); // Continue until no more swapping is needed
}
    

void addWordToDictionary(Dictionary& dict, const char* newWord) {
    // Check if the dictionary is empty
    if (dict.head == nullptr) { // If the dictionary is empty, create a new entry for the new word
        dWord* newEntry = new dWord();
        strcpy(newEntry->data, newWord);
        newEntry->synonym = nullptr;
        newEntry->antonym = nullptr;
        newEntry->next = nullptr;
        newEntry->previous = nullptr;
        dict.head = newEntry; // Set the new entry as both head and tail
        dict.tail = newEntry;
        return;
    }

    dWord* currWord = dict.head;
    while (currWord != nullptr) {
        int compareResult = strcmp(currWord->data, newWord);

        if (compareResult == 0) { // If the new word already exists in the dictionary, do not add it again
            cout << "Word already exists in the dictionary." << endl;
            return; // Word already exists, do not add
        }
        else if (compareResult > 0) { // If the new word should be inserted before the current word in order
            dWord* newEntry = new dWord();
            strcpy(newEntry->data, newWord);
            newEntry->synonym = nullptr;
            newEntry->antonym = nullptr;
            newEntry->next = currWord;
            newEntry->previous = currWord->previous;

            if (currWord->previous != nullptr) {
                currWord->previous->next = newEntry;
            }
            else {
                dict.head = newEntry; // Update the head if the new word becomes the first
            }
            currWord->previous = newEntry;
            return;
        }

        if (currWord->next == nullptr) {
            // If the new word should be inserted at the end of the dictionary
            dWord* newEntry = new dWord();
            strcpy(newEntry->data, newWord);
            newEntry->synonym = nullptr;
            newEntry->antonym = nullptr;
            newEntry->next = nullptr;
            newEntry->previous = currWord;
            currWord->next = newEntry;
            dict.tail = newEntry; // Update the tail to the new entry
            return;
        }

        currWord = currWord->next; // Move to the next word
    }
}


void addSynonymOrAntonym(Dictionary& dict, const string& word, const string& newWord, bool isSynonym,const string& filename) {
    dWord* currWord = dict.head;
    dWord* existing = nullptr;

    while (currWord != nullptr) { // Find the word in the dictionary
        if (strcmp(currWord->data, word.c_str()) == 0) {
            existing = currWord;
            break;
        }
        currWord = currWord->next;
    }

    if (existing == nullptr) { // If the existing word is not found in the dictionary
        cout << "Existing word not found in the dictionary." << endl;
        return;
    }

    ofstream outputFile(filename, ios::app); // Open the file for appending
    if (!outputFile.is_open()) {
        cerr << "Unable to open file for appending." << endl;
        return;
    }

    if (isSynonym) { // If adding a synonym
        sWord* newSynonym = new sWord();
        strcpy(newSynonym->data, newWord.c_str());
        newSynonym->next = existing->synonym;
        existing->synonym = newSynonym;

        outputFile << ":" << newWord; // Update file with the added synonym
        cout << "Synonym added successfully!" << endl;
    } else { // If adding an antonym
        sWord* newAntonym = new sWord();
        strcpy(newAntonym->data, newWord.c_str());
        newAntonym->next = existing->antonym;
        existing->antonym = newAntonym;

        outputFile << "#" << newWord; // Update the file with the added antonym
        cout << "Antonym added successfully!" << endl;
    }

    outputFile.close(); // Close the file
}

 

 
 
// deletes words contanining these letters
 
void deleteWordsWithLetters(Dictionary& dict, const char* letters) {
    dWord* currWord = dict.head;
    dWord* prevWord = nullptr;
 
    while (currWord != nullptr) {
        bool containsLetter = false;
        for (size_t i = 0; i < strlen(letters); ++i) {
            if (strchr(currWord->data, letters[i]) != nullptr) {
                containsLetter = true;
                break;
            }
        }
 
        if (containsLetter) {
            if (prevWord == nullptr) {
                dict.head = currWord->next;
            } else {
                prevWord->next = currWord->next;
            }
 
            if (currWord->next != nullptr) {
                currWord->next->previous = prevWord;
            } else {
                dict.tail = prevWord;
            }
 
            delete currWord;
            currWord = prevWord == nullptr ? dict.head : prevWord->next;
        } else {
            prevWord = currWord;
            currWord = currWord->next;
        }
    }
}

 
// search in the dictionary for words starting with
 
void searchWordsStartingWith(Dictionary& dict, const char* startingLetters) {
    dWord* currWord = dict.head;
 
    while (currWord != nullptr) {
        if (strncmp(currWord->data, startingLetters, strlen(startingLetters)) == 0) {
            cout << "Word starting with '" << startingLetters << "': " << currWord->data << endl;
        }
        currWord = currWord->next;
    }
}
 
// function that searches the dictionary for all the synonyms and antonyms of a given word
 
void searchSynonymsAndAntonyms(Dictionary& dict, const char* word) {
    dWord* currWord = dict.head;
    bool found = false;

    while (currWord != nullptr) { // loop through each word
        if (strcmp(currWord->data, word) == 0) {
            found = true; // if the word is found in the dic
            // Display synonyms
            sWord* synonym = currWord->synonym;
            if (synonym != nullptr) {
                cout << "Synonyms of '" << word << "': " << endl;
                while (synonym != nullptr) {
                    cout << synonym->data << endl;
                    synonym = synonym->next;
                }
            } else {
                cout << "No synonyms found for '" << word << "'" << endl;
            }

            // Display antonyms
            sWord* antonym = currWord->antonym;
            if (antonym != nullptr) {
                cout << "Antonyms of '" << word << "': " << endl;
                while (antonym != nullptr) {
                    cout << antonym->data << endl;
                    antonym = antonym->next;
                }
            } else {
                cout << "No antonyms found for '" << word << "'" << endl;
            }

            break;
        }
        currWord = currWord->next; // move to the next word
    }

    if (!found) { // if the word is not found
        cout << "Word '" << word << "' not found in the dictionary." << endl;
    }
}

 
// function to delete all the words in the dictionary that start with a given set of letters
 
void deleteWordsStartingWith(Dictionary& dict, const char* startingLetters) {
    dWord* currWord = dict.head;
    dWord* prevWord = nullptr;

    while (currWord != nullptr) { // loop through each word
        if (strncmp(currWord->data, startingLetters, strlen(startingLetters)) == 0) { // If the current word starts with the specified sequence of letters
            // Remove the word from the dictionary
            if (prevWord == nullptr) {
                dict.head = currWord->next;
            } else {
                prevWord->next = currWord->next;
            }

            if (currWord->next != nullptr) {
                currWord->next->previous = prevWord;
            } else {
                dict.tail = prevWord;
            }

            dWord* temp = currWord;
            currWord = currWord->next; // move to the next word
            delete temp; // Delete the word from memory
        } else {
            prevWord = currWord;
            currWord = currWord->next;
        }
    }
}

 
int main() {
    Dictionary dic = parseInputFile("dictionary.txt");

    while (true) {
        cout << endl << "~~~~~~~~~~~~~~~~~~ Welcome to the Dictionary ! ~~~~~~~~~~~~~~~~~~ " << endl << endl;
        cout << "Please choose a command:" << endl;
        cout << "1. Display dictionary" << endl;
        cout << "2. Write contents of the dictionary to a new text file" << endl;
        cout << "3. Sort the dictionary in alphabetical order" << endl;
        cout << "4. Add a word to the dictionary" << endl;
        cout << "5. Add a new word as a synonym or antonym" << endl;
        cout << "6. Delete words containing a letter" << endl;
        cout << "7. Search in the dictionary for words starting with a letter" << endl;
        cout << "8. Search for synonyms and antonyms of a given word" << endl;
        cout << "9. Delete all words in the dictionary starting with a given set of letters" << endl;
        cout << "10. Quit" << endl;
        cout << endl << "Enter your choice : ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        switch (choice) {
            case 1:
                displayDictionary(dic);
                break;
            case 2:
                writeDictionaryToFile(dic, "dictionary.txt");
                cout << endl << "Contents of the dictionary written to newfile.txt successfully!" << endl;
                break;
            case 3:
                sortDictionary(dic);
                cout << endl << "Dictionary sorted in alphabetical order successfully!" << endl;
                break;
            case 4: {
                string word;
                cout << endl << "Enter the word to add: ";
                getline(cin, word);
                addWordToDictionary(dic, word.c_str());
                cout << "Word added successfully !" << endl;
                break;
            }
            case 5: {
    string word, newWord;
    cout << endl << "Enter the word you want to add a synonym or antonym for: ";
    getline(cin, word);

    dWord* currWord = dic.head;
    bool wordExists = false;
    while (currWord != nullptr) {
        if (strcmp(currWord->data, word.c_str()) == 0) {
            wordExists = true;
            break;
        }
        currWord = currWord->next;
    }

    if (!wordExists) {
        cout << endl << "Word doesn't exist in the dictionary. Try adding the word first." << endl;
        break;
    }

    cout << "Do you want to add a synonym or an antonym for this word? " << endl;
    cout << "1- Synonym " << endl;
    cout << "2- Antonym " << endl;
    cout << "Enter your choice : ";
    int option;
    cin >> option;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

    do {
        if (option == 1) {
            cout << endl << "Enter the synonym: ";
            getline(cin, newWord);
            addSynonymOrAntonym(dic, word, newWord, true,"dictionary.txt");
            cout << endl << "Synonym added successfully !";
        } else if (option == 2) {
            cout << endl << "Enter the antonym: ";
            getline(cin, newWord);
            addSynonymOrAntonym(dic, word, newWord, false,"dictionary.txt");
            cout << endl << "Antonym added successfully !";
        } else {
            cout << endl << "Invalid choice. Please enter 1 for Synonym or 2 for Antonym." << endl;
        }
    } while (option != 1 && option != 2);
    break;
}

    case 6: {
    string letters;
    cout << endl << "Enter letters to delete words containing those letters: ";
    getline(cin, letters);
    deleteWordsWithLetters(dic, letters.c_str());
    cout << endl << "Words deleted successfully !";
    break;
}

case 7: {
    string startingLetters;
    cout << endl << "Enter letters to search for words starting with those letters: ";
    getline(cin, startingLetters);
    searchWordsStartingWith(dic, startingLetters.c_str());
    break;
}

case 8: {
    string word;
    cout << endl << "Enter the word to search for its synonyms and antonyms: ";
    getline(cin, word);
    searchSynonymsAndAntonyms(dic, word.c_str());
    break;
}

case 9: {
    string startingLetters;
    cout << endl << "Enter letters to delete words starting with these letters: ";
    getline(cin, startingLetters);
    deleteWordsStartingWith(dic, startingLetters.c_str());
    cout << endl << "Words deleted successfully !";
    break;
}

case 10:
    cout << endl << "Exiting the program..." << endl;
    return 0;
default:
    std::cout << endl << "Invalid choice. Please enter a valid number." << endl;
    break;
}

        
    }

    return 0;
}
