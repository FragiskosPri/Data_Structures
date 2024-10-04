#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;


class LOF
{
public:
    int position; // The position it's at on the file
    LOF *next;    // Pointer for the next position

    // Constructor
    LOF(int position)
    {
        this->position = position;
        this->next = nullptr;
    }
};

// Define a custom LOW class to represent nodes in the linked list
class LOW
{
public:
    string word; // The word this node contains
    LOF *head;   // Pointer to the first node in the LOF
    int host;    // Number of times this word is spotted
    LOW *next;   // Pointer to the next word

    // Constructor
    LOW(string word, int i)
    {
        this->word = word;
        this->host = 1;
        this->next = nullptr;
        this->head = nullptr;
        add(i);
    }

    void add(int page)
    {
        LOF *newLOF = new LOF(page); // Create a new LOF node with the given page

        if (head == nullptr)
        {
            // If the  LOF is empty, set the new node as the head of the LOF
            head = newLOF;
        }
        else if (page < head->position)
        {
            // If the given page is smaller than the position of the head node,
            // insert the new node at the beginning of the LOF
            newLOF->next = head;
            head = newLOF;
        }
        else
        {
            LOF *current = head;
            while (current->next != nullptr && current->next->position < page)
            {
                // Find the position in the LOF where the new node should be inserted
                current = current->next;
            }
            // Insert the new node after the current node in the LOF
            newLOF->next = current->next;
            current->next = newLOF;
        }
    }

    // Check if a page is present in the LOF
    bool containsPage(int page)
    {
        LOF *current = head;
        while (current != nullptr) // While there is another node available
        {
            if (current->position == page)
            {
                return true; // Page found in the LOF
            }
            current = current->next;
        }
        return false; // Page not found in the LOF
    }

    // Print all the pages in the LOF in ascending order
    void printAllPages()
    {
        LOF *current = head;
        cout << "The word \"" << word << "\" appears in " << host << " documents. These documents are" << endl;
        while (current != nullptr)
        {
            cout << current->position << " ";
            current = current->next;
        }
        cout << endl;
    }
};

// Define a custom LinkedList class to represent the linked list
class LinkedList // This is used to simulate a list
{
public:
    // Constructor
    LinkedList()
    {
        head = nullptr;
    }

    // Add a new node with the given word, host, and LOF to the end of the list
    void add(string word, int i)
    {
        LOW *newNode = new LOW(word, i); // Dynamically allocate memory for a new node
        if (head == nullptr)
        {
            head = newNode; // If the list is empty, set the new node as the head
        }
        else
        {
            LOW *current = head;
            while (current->next != nullptr)
            {
                current = current->next; // Go to the end of the list
            }
            current->next = newNode; // Add the new node to the end of the list
        }
    }

    // Check if a word is present in the list and return a pointer to the node
    LOW *contains(string targetWord)
    {
        LOW *current = head;
        while (current != nullptr)
        {
            if (current->word == targetWord)
            {
                return current; // Return pointer to the node containing the word
            }                   // Returns object so as not to use a search twice later
            current = current->next;
        }
        return nullptr; // Word not found, return nullptr
    }

private:
    LOW *head; // Pointer to the first node in the list
};

//---------------------------------------------------------------------------------------------------------------------------
void processWords(const string &content, int &i, LinkedList *myList)
{
    istringstream iss(content);
    string word;
    while (iss >> word) // Extract each word from the string stream
    {
        LOW *foundNode = myList->contains(word);
        if (foundNode == nullptr)
        {
            myList->add(word, i); // Creates a new word node
        }
        else 
        {
            if (!foundNode->containsPage(i)) // If it hasn't been spotted on this page yet
            {
                foundNode->host += 1; // Increase the amount of times it appears by one
                foundNode->add(i);    // Add the page to the LOF
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------
// Function to get the contents from the files
string getFileContents(int orderOfFiles[], int &index)
{
    string filename = "input/" + to_string(orderOfFiles[index]) + ".txt"; // Puts the name of the file location into a string variable
    ifstream file(filename);                                              // Opens file
    if (file.is_open())                                                   // If the file is open
    {
        string contents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()); // Put the string into a variable
        file.close();                                                                    // Closes the file
        return contents;                                                                 // And returns the content
    }
    else
    {
        return "Couldn't open file";
    }
}

//--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--/=/--//

int main()
{
    LinkedList *myList = new LinkedList(); // Create a new list
   
    int orderOfFiles[] = {26, 94, 22, 40, 43, 24, 87, 44, 31, 45, 6, 49, 14, 53, 58, 33, 74, 13, 19, 93, 55, 71, 39, 29, 10, 80, 86, 77, 79, 82, 9, 92, 17, 96, 50, 2, 65, 90, 89, 11, 57, 51, 68, 83, 66, 54, 27, 95, 48, 81, 70, 25, 4, 60, 21, 76, 75, 61, 15, 37, 30, 62, 34, 56, 88, 59, 1, 32, 3, 73, 78, 42, 16, 36, 7, 47, 12, 67, 85, 64, 28, 52, 84, 98, 69, 41, 63, 72, 46, 20, 18, 100, 97, 99, 23, 38, 35, 91, 8, 5};
    int numFiles = sizeof(orderOfFiles) / sizeof(orderOfFiles[0]); // Get the number of files in the array

    for (int i = 0; i < numFiles; i++)
    {
        string content = getFileContents(orderOfFiles, i); // Get the contents of the file
        processWords(content, orderOfFiles[i], myList);    // Process the words in the content and update the list
    }

    string search;
    cout << "Give me the word you want to search: ";
    cin >> search;                        // Inputs the word you want to search
    LOW *Node = myList->contains(search); // Finds the word that contains the word

    if (Node != nullptr)
    {
        Node->printAllPages(); // Prints the location of the words
    }
    else
    {
        cout << endl << "This word was not found in any files!";
    }


    // This stops the terminal from ending automatically
    for (int i = 0; i < 5; i++)
    {
        cout << endl;
    }
    cout << "Enter anything to end terminal: ";
    string dummy; 
    cin >> dummy;
    return 0;
}