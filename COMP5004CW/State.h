//
// Created by DrMark on 10/2/2017.
//

#ifndef TEXTADV_STATE_H
#define TEXTADV_STATE_H

#include <iostream>
#include <fstream>
#include "Room.h"
#include "GameObject.h"

class State {
    Room *currentRoom;
public:
    explicit State(Room *startRoom);

    void goTo(Room *target);
    void announceLoc() const;
    Room* getCurrentRoom() const;
    int getRoomNumber();

    static std::list<GameObject*> inventory;
    static void getInventoryNames() ;
    static std::list<GameObject*> getObjs();
    static void addInvObj(GameObject* obj);
    static void removeInvObj(GameObject* obj);




};


#endif //TEXTADV_STATE_H
