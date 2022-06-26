//
// Created by DrMark on 10/2/2017.
//

#include "State.h"
#include <iostream>
#include <sstream>

using std::string;
using namespace std;


/**
 * Current state of the game.
 */

std::list<GameObject*> State::inventory;
/**
 * Display the description of the room the player is in. */

void State::announceLoc() const {
    this->currentRoom->describe();
}

/**
 * Constructor.
 * @param startRoom Pointer to the room to start in.
 */
State::State(Room *startRoom) : currentRoom(startRoom) {};

/**
 * Move to a specified room and print its description.
 * @param target Pointer to the room to move to.
 */
void State::goTo(Room *target) {
    this->currentRoom = target;
    this->announceLoc();
}

/**
 * Return a pointer to the current room.
 * @return Pointer to the current room.
 */
Room* State::getCurrentRoom() const {
    return this->currentRoom;
}

void State::addInvObj(GameObject* obj){
    inventory.push_back(obj);

}

void State::removeInvObj(GameObject* obj){
    inventory.remove(obj);
}

void State::getInventoryNames() {
    cout << "Player Inventory: ";
    for (const auto& invObjs : inventory){
        cout << "\n";
        cout << "Object " +  invObjs->getName() + "\n";
    }
}

list<GameObject*> State::getObjs(){
    return inventory;
}



int State::getRoomNumber() {
    int count = 0;
    for(auto& rooms1 : Room::rooms){
        count ++;
        if (rooms1 == getCurrentRoom()){
            break;
        }
    }
    return count;
}


