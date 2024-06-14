#include<iostream>
#include <vector>
#include "MovieHashTable.hpp"
using namespace std;

// Constructor for MovieHashTable with default size
/* hash table constructor sets table size to 2000
creates a new movie node table of the given size
set each movie node in table to nullptr
set # of collisions to 0
*/
MovieHashTable::MovieHashTable() {
    table_size = 2000;
    table = new MovieNode*[table_size];
    for(int i = 0; i < table_size; i++) table[i] = nullptr;
    n_collisions = 0;
}

// Constructor for MovieHashTable with given size
/* hash table constructor takes size as parameter. Sets table size to parameter size
creates a new movie node table of the given size
set each movie node in table to nullptr
set # of collisions to 0
*/
MovieHashTable::MovieHashTable(int s) {
    table_size = s;
    table = new MovieNode*[table_size];
    for(int i = 0; i < table_size; i++) table[i] = nullptr;
    n_collisions = 0;
}

// Destructor for MovieHashTable that deletes all nodes in the hash table
/* uses two movie nodes and a for loop to perform linked list destruction method for each index in the hash table to delete each movie node
then delete hash table
*/
MovieHashTable::~MovieHashTable() {
    MovieNode* prev;
    MovieNode *curr;
    for(int i = 0; i < table_size; i++){
        curr = table[i];
        prev = nullptr;
        while(curr != nullptr){
            prev = curr->next;
            delete curr;
            curr=prev;
        }
    }

    delete[] table;
}

// Hash function for MovieHashTable that returns an index in the hash table for a given movie title.
// hash: Students must use their identikey to come up with a creative hash function that minimizes collisions
// for the given IMDB-Movie database to ensure efficient insertion and retrieval of movie nodes.
/*function takes the movie title as parameter. Calculates integer value for movie title and integer value for student indentiKey
two values are multiplied and the mod of the product and the hash table size is taken to calculate the hash table index.
result is returned
*/
int MovieHashTable::hash(string title) {
    int hash = 0;
    for(int i = 0; i < title.length(); i++){
        hash += title[i];
    }
    int idNum = 0;
    string identiKey = "nico9956";
    for(int i = 0; i < identiKey.length(); i++){
        idNum += identiKey[i];
    }
    hash = hash * idNum;
    hash = hash % table_size;
    return hash;
}

// Inserts a movie node into the hash table with the specified title
/* insert: Takes movie node and movie title as parameters
First search hash table for movie by title. If movie already exists, (with same director to make sure it is the same movie and not just a different movie with the same name),
delete new movie node and end function (return).
Calculate the new movie's index in the hash table. If there is already a head movie node at that index, increment collisions.
Set new movie node's next to head, and then make the new movie node the head at the index.
void funciton, so no return value
*/
void MovieHashTable::insert(string title, MovieNode* movie) {
    MovieNode* tmp = search(title);
    if(tmp != nullptr && tmp->director == movie->director){
        /*cout << title << " - ";
        cerr << "Movie already in database." << endl;*/
        delete movie;
        return;
    }
    MovieNode* head = table[hash(title)];
    if(head != nullptr) setCollisions();
    movie->next = head;
    table[hash(title)] = movie;
}

// Searches for a node in the hash table with the specified title
/* search: takes movie title as parameter
calculates k: the hash index for the title. Starting from the head node at index k in the hash table,
perform linked list traversal until a movie with the title is found (return that movie).
If no movie with the same title is found, return nullptr
*/
MovieNode* MovieHashTable::search(string title) {
    int k = hash(title);
    if(table[k] == nullptr) return nullptr;

    MovieNode* curr = table[k];
    while(curr != nullptr){
        if(curr->title == title) return curr;
        curr = curr->next;
    }
    return nullptr;
}

// Returns the number of collisions that have occurred during insertion into the hash table
// getCollisions has no parameters and simply returns private integer n_collisions
int MovieHashTable::getCollisions() {
    return n_collisions;
}

// Increments the number of collisions that have occurred during insertion into the hash table
// sertCollisions has no parameters and simply increments private variable n_collisions
void MovieHashTable::setCollisions() {
    n_collisions++;
}
