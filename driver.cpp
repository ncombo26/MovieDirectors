#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MovieHashTable.hpp"
#include "DirectorSkipList.hpp"

using namespace std;

// Function prototypes
MovieNode* parseMovieLine(string line);
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList);
void display_menu();


// Main function
int main(int argc, char* argv[]) {
    if(argc < 4){
        cout << "Invalid number of arguments." << endl;
        cout << "Usage: ./<program name> <csv file> <hashTable size> <skiplist size>" << endl;
    }
    string fileName = argv[1];
    int hash_size = stoi(argv[2]);
    int skip_capacity = stoi(argv[3]);
    int skip_levels = 10;

    MovieHashTable movieTable(hash_size);
    DirectorSkipList directorList(skip_capacity, skip_levels);
    readMovieCSV(fileName, movieTable, directorList);
    //directorList.prettyPrint();

    cout << "Number of collisions: " << movieTable.getCollisions() << endl;
    int option = 0;
    bool quit = false;
    while(!quit){
        display_menu();
        cin >> option;
        if(option == 1){
            string movieName;
            cout << "Enter movie name: ";
            getline(cin, movieName);
            getline(cin, movieName);
            MovieNode* movie = movieTable.search(movieName);
            if(movie == nullptr) cout << "Movie Not found!" << endl;
            else cout << "The director of " << movieName << " is " << movie->director << endl;
            cout << endl;
        }else if(option == 2){
            string directorName;
            cout << "Enter director name: ";
            getline(cin, directorName);
            getline(cin, directorName);
            DirectorSLNode* director = directorList.search(directorName);
            if(director == nullptr) cout << "Director not found!" << endl;
            else cout << directorName << " directed " << director->movies.size() << " movies" << endl;
            cout << endl;
        }else if(option == 3){
            string movieName;
            cout << "Enter movie name: ";
            getline(cin, movieName);
            getline(cin, movieName);
            MovieNode* movie = movieTable.search(movieName);
            if(movie == nullptr) cout << "Movie not found!" << endl;
            else{
                cout << "Summary: " << movieName << " (" << movie->year <<   ") is a (" << movie->genre << ") film featuring: (";
                cout << movie->actors << ")." << endl;
                cout << "Plot: " << movie->description << endl;
            }
            cout << endl;
        }else if(option == 4){
            string directorName;
            cout << "Enter director name: ";
            getline(cin, directorName);
            getline(cin, directorName);
            DirectorSLNode* director = directorList.search(directorName);
            if(director == nullptr) cout << "Director not found!" << endl;
            else{
                cout << directorName << " directed the following movies: " << endl;
                for(int i = 0; i < director->movies.size(); i++){
                    cout << i << ": " << director->movies[i]->title << endl;
                }
            }
            cout << endl;
        }else if(option == 5){
            quit = true;
        }else{
            cout << "Invalid input. Please select an option 1-5." << endl;
        }
    }

    return 0;
}

// Function to parse a CSV line into a MovieNode object
MovieNode* parseMovieLine(string line) {
    stringstream ss(line);
    vector<string> fields;
    string field;

    // Loop through the characters in the line
    bool in_quotes = false;
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        if (c == '\"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            // add field to vector and reset for next field
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
            // handle double quotes inside a quoted field
            if (in_quotes && c == '\"' && i < line.length() - 1 && line[i+1] == '\"') {
                field += '\"'; // add the second double quote and skip it
                ++i;
            }
        }
    }
    fields.push_back(field);

    if (fields.size() != 12) {
        cerr << "Error: Invalid movie line format" << line << endl;
        return nullptr;
    }

    int rank = stoi(fields[0]);
    string title = fields[1];
    string genre = fields[2];
    string description = fields[3];
    string director = fields[4];
    string actors = fields[5];
    int year = stoi(fields[6]);
    int runtime = stoi(fields[7]);
    float rating = stof(fields[8]);
    int votes = stoi(fields[9]);
    float revenue = stof(fields[10]);
    int metascore = stoi(fields[11]);

    // Create a new MovieNode object with the parsed fields
    MovieNode* movie = new MovieNode(rank, title, genre, description, director, actors, year, runtime, rating, votes, revenue, metascore);
    return movie;
}

// Function to read a CSV file into a vector of MovieNode objects
/* readMovieCSV: takes file name, movie hash table and director skip list as parameters (movieTable and directorList are passed by reference)
read through the file and use the parseMovieLine function to create a movie node for each line
use the new movie node to perform insertion on movie hash table and director skip list
*/
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList) {
    ifstream file;
    file.open(filename);
    string line;
    getline(file, line);
    while(getline(file, line)){
        MovieNode* newMovie = parseMovieLine(line);
        movieTable.insert(newMovie->title, newMovie);
        directorList.insert(newMovie->director, newMovie);
    }
}

// Function to display the menu options
void display_menu() {
    cout << "Please select an option:" << endl;
    cout << "1. Find the director of a movie" << endl;
    cout << "2. Find the number of movies by a director" << endl;
    cout << "3. Find the description of a movie" << endl;
    cout << "4. List the movies by a director" << endl;
    cout << "5. Quit" << endl << endl;
    cout << "Enter an option: ";
}
