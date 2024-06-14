#include<iostream>
#include <vector>
#include "DirectorSkipList.hpp"
using namespace std;

// Constructor for DirectorSkipList with default capacity and levels
// sets default capacity to 2000 and levels to 10
// size always starts at 0 and the head of the skiplist initialized with empty director and max level of nullptr next vector
DirectorSkipList::DirectorSkipList() {
    levels = 10;
    capacity = 2000;
    size = 0;
    head = new DirectorSLNode(" ", levels);
}

// Constructor for DirectorSkipList with given capacity and levels
//takes capacity and levels as parameters, and sets the values to those parameters
// size always starts at 0 and the head of the skiplist initialized with empty director and max level of nullptr next vector
DirectorSkipList::DirectorSkipList(int _cap, int _levels) {
    capacity = _cap;
    levels = _levels;
    size = 0;
    head = new DirectorSLNode(" ", levels);
}

// Destructor for DirectorSkipList that deletes all nodes in the skip list (but not the MovieNodes, which are shared with other data structures)
/* perform linked list - style destruction to traverse through skip list and delete every director node
stay on bottom level to avoid skipping any nodes
*/
DirectorSkipList::~DirectorSkipList() {
    DirectorSLNode* prev;
    DirectorSLNode* curr = head;
    while(curr != nullptr){
        prev = curr;
        curr = curr->next[0];
        delete prev;
    }
}

/* Inserts a movie node into the skip list with the specified director and movie node pointer as parameter
// First, create a vector if DirectorSLNode* for the nodes whos next will have to updated
// if the director is already in the skiplist, add the movie to its movie vector and do not add the director again (return)
// calculate the number of levels that the new director will reach
// update the nodes in the change vector within the new directors level to point to the new director, and the new director to point to where the change nodes went
 add the movie to the new directors movie vector and increment size */
void DirectorSkipList::insert(string director, MovieNode* _movie) {
    DirectorSLNode* tmp = head;
    vector<DirectorSLNode*> change(levels, nullptr);
    for(int i = levels - 1; i >= 0; i--){
        while(tmp->next[i] != nullptr && tmp->next[i]->director < director){
            tmp = tmp->next[i];
        }
        change[i] = tmp;
    }
    tmp = tmp->next[0];
    if(tmp != nullptr && tmp->director == director){
        //cout << "director already exists" << endl;
        tmp->addMovie(_movie);
        return;
    }
    int n_levels = 1;
    while(n_levels < levels && rand() % 2 == 0){
        n_levels++;
    }
    //cout << n_levels << endl;
    DirectorSLNode* new_node = new DirectorSLNode(director, n_levels);
    for(int i = 0; i < n_levels; i++){
        new_node->next[i] = change[i]->next[i];
        change[i]->next[i] = new_node;
    }
    new_node->addMovie(_movie);
    size++;
}

/* Searches for a node in the skip list with the specified director as function parameter
// Start from the head at the top level and keep traversing while the next node's director is still less than what we are searching for
//If the next node is greater, then drop a level and perform the same process
// once search algorithm is completed: if the node we have has the same director name we are searching for, return node
Otherwise: director we are searching for does not exist in skip list. return nullptr. */
DirectorSLNode *DirectorSkipList::search(string director) {
    DirectorSLNode *curr = head;
    for(int i = levels - 1; i >= 0; i--){
        while(curr->next[i] != nullptr && curr->next[i]->director < director){
            curr = curr->next[i];
            //if(curr->director == director) cout << "found director" << endl; return curr;
        }
    }
    //return curr->next[0];
    curr = curr->next[0];
    if(curr->director == director) return curr;
    else return nullptr;
}

/* Pretty-prints the skip list
For each level in the skip list, print level and traverse (linked-list style) throught that level and print each director
*/
void DirectorSkipList::prettyPrint() {
    DirectorSLNode* curr = head;
    for(int i = levels; i >= 0; i--){
        curr = head;
        cout << "Level " << i << ": ";
        while(curr != nullptr){
            cout << curr->director << "-> ";
            curr = curr->next[i];
        }
        cout << endl;
    }
}
