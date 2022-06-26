//
// Created by DrMark on 10/2/2017.
//

#include "Room.h"
#include "wordwrap.h"
#include <iostream>
using std::string;
using namespace std;

/**
 * Stores a static list of all rooms.
 */
std::list<Room*> Room::rooms;

/**
 * Room default constructor.
 * @param _name Room's name.
 * @param _desc Room's description.
 */
Room::Room(const string* _name, const string *_desc) :
        name(_name), description(_desc), north(nullptr), east(nullptr), south(nullptr), west(nullptr), objs(){};

/**
 * Remove destroyed rooms from the static list.
 */
Room::~Room() {
    Room::rooms.remove(this);
}

/**
 * Prints the description of a room (the name and long description)
 */
void Room::describe() const {
    int count = 1;

    std::cout << "\n";
    std::cout << "-------------------------------------------------------------\n";
    wrapOut(this->name);
    wrapEndPara();
    std::cout << "-------------------------------------------------------------\n";
    wrapOut(this->description);
    wrapEndPara();


    std::cout << "-------------------------------------------------------------\n";
    cout << "Objects found: \n";
    std::cout << "\n";
    for(const auto& objects : this->objs)
    {
        cout << "Object Number: " << count <<  " -- ";
        std::cout << objects->getName() + " -- Keyword: " + objects->getKeyword();
        wrapEndPara();
        count++;
    }
    std::cout << "-------------------------------------------------------------\n";
}

/**
 * Statically creates a room and then adds it to the global list.
 * @param _name Name for the new room.
 * @param _desc Description for the new room.
 * @return A pointer to the newly created room.
 */
Room* Room::addRoom(const string* _name, const string *_desc) {
    auto *newRoom = new Room(_name, _desc);
    Room::rooms.push_back(newRoom);
    return newRoom;
}

/**
 * Adds an existing room to the static list.
 * @param room Pointer to the room to add.
 * @return
 */
void Room::addRoom(Room* room) {
    Room::rooms.push_back(room);
}


void Room::addObj(GameObject* obj){
    this->objs.push_back(obj);
}


Room* Room::getNorth() const {
    return this->north;
}
Room* Room::getEast() const {
    return this->east;
}
Room* Room::getWest() const {
    return this->west;
}
Room* Room::getSouth() const {
    return this->south;
}

void Room::setNorth(Room* _north) {
    this->north = _north;
}
void Room::setEast(Room* _east) {
    this->east = _east;
}
void Room::setSouth(Room* _south) {
    this->south = _south;
}
void Room::setWest(Room* _west) {
    this->west = _west;
}


list<GameObject*> Room::getObjs(){
    return this->objs;
}

void Room::RemoveObject(GameObject *obj) {
   this->objs.remove(obj);

}

Room* Room::getRoom(int i) {
    int count = 0;
    Room* roomd;
    for(auto& rooms1 : Room::rooms){
        count++;
        if(i == count){
            roomd = rooms1;
            break;
        }
    }


    return roomd;
}



