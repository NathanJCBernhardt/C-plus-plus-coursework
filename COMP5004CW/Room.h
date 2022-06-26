//
// Created by DrMark on 10/2/2017.
//


#ifndef TEXTADV_ROOM_H
#define TEXTADV_ROOM_H

#include <string>
#include <forward_list>
#include <list>
#include <vector>
#include "GameObject.h"

using std::string;

/**
 * Represents a room (accessible location in the game).
 */
class Room {
    /**
     * Short name used as a header.
     */
    const string* name;
    /**
     * Full description of the room.
     */
    const string* description;
    /**
     * Pointer to room that is north of this one.
     */
    Room* north;
    Room* east;
    Room* south;
    Room* west;
    //objs list declared in constructor
    std::list<GameObject*> objs;

public:

    Room(const string *_name, const string *_desc);
    ~Room();


    void describe() const;

    static std::list<Room*> rooms;

    static Room* addRoom(const string* _name, const string* _desc);
    static void addRoom(Room* room);


    void addObj(GameObject* obj);

    Room* getNorth() const;
    Room* getEast() const;
    Room* getSouth() const;
    Room* getWest() const;
    std::list<GameObject*> getObjs();

    void RemoveObject(GameObject* obj);

    void setNorth(Room* _north);
    void setSouth(Room* _south);
    void setWest(Room* _west);
    void setEast(Room* _east);

    static Room* getRoom(int i);



};

#endif //TEXTADV_ROOM_H
