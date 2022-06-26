
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "Room.h"
#include "wordwrap.h"
#include "State.h"
#include "strings.h"

#include <cctype>


using namespace std;
using std::string;
using std::unique_ptr;


string commandBuffer;
State *currentState;
std::vector<GameObject*> objs;
std::string splitString(string basicString);
void SaveGame();
bool is_file_exist(const string& fileName);
void LoadGame(const string& filename, bool state);

/**
 * Print out the command prompt then read a command into the provided string buffer.
 * @param buffer Pointer to the string buffer to use.
 */
void inputCommand(string *buffer) {
    buffer->clear();
    std::cout << "> ";
    std::getline(std::cin, *buffer);
}

/**
 * Sets up the map.
 */
void initRooms(bool flag) {
    auto * o1 = new GameObject(&o1name, &o1desc, &Keys[0]);
    auto * o2 = new GameObject(&o2name, &o2desc, &Keys[1]);
    auto * o3 = new GameObject(&o3name, &o3desc, &Keys[2]);
    objs.push_back(o1);
    objs.push_back(o2);
    objs.push_back(o3);

    auto * r1 = Room::addRoom(&r1name, &r1desc);
    auto * r2 = Room::addRoom(&r2name, &r2desc);
    auto * r3 = Room::addRoom(&r3name, &r3desc);
    r1->setNorth(r2);
    r2->setSouth(r1);
    r2->setWest(r3);
    r3->setEast(r2);

    if (flag == false){
        r2->addObj(o1);
        r2->addObj(o2);
        r2->addObj(o3);
        r3->addObj(o3);
    } else{
        //do nothing
    }

}

/**
 * Sets up the game state.
 */
void initState() {
    currentState = new State(Room::rooms.front());
}

/**
 * The main game loop.
 */
void gameLoop() {
    bool gameOver = false;
    while (!gameOver) {
        /* Ask for a command. */
        bool commandOk = false;

        inputCommand(&commandBuffer);


        auto endOfVerb = static_cast<uint8_t>(commandBuffer.find(' '));

        if (commandBuffer.find(' ') != string::npos) {
            commandOk = true;

            auto key = splitString(commandBuffer);
            auto Roomkeys = currentState->getCurrentRoom()->getObjs();
            auto InventoryKeys = State::getObjs();

            GameObject *RoomKeyMatch;
            GameObject *InventoryKeyMatch;

            auto Roomflag = 0;
            auto InvFlag = 0;


            for (auto &object : Roomkeys) {
                if (key == object->getKeyword()) {
                    RoomKeyMatch = object;
                    Roomflag = 1;
                    break;
                } else {
                    Roomflag = 0;
                }
            }
            if (Roomflag == 1) {
                std::cout << "\nKey entered matches an object in the room\n";
            } else {
                std::cout << "\nKey does not match an object in the room\n";
            }


            for (auto &object : InventoryKeys) {
                if (key == object->getKeyword()) {
                    InventoryKeyMatch = object;
                    InvFlag = 1;
                    break;
                } else {
                    InvFlag = 0;
                }
            }
            if (InvFlag == 1) {
                std::cout << "Key entered matches an object in your inventory\n";
            } else {
                std::cout << "Key does not match an object in your inventory\n";
            }


            if ((commandBuffer.compare(0, endOfVerb, "get") == 0) ||
                (commandBuffer.compare(0, endOfVerb, "g") == 0)) {
                if ((InvFlag == 0) && (Roomflag == 1)) {

                    State::addInvObj(RoomKeyMatch);
                    currentState->getCurrentRoom()->RemoveObject(RoomKeyMatch);
                    std::cout << RoomKeyMatch->getName() + " added to your inventory!\n";
                    std::cout << RoomKeyMatch->getName() + " Removed from current room\n";
                    currentState->announceLoc();

                } else {
                    std::cout
                            << "The key you entered doesn't match an object in the room, or you already hold the object in your inventory!\n";
                }

            } else if ((commandBuffer.compare(0, endOfVerb, "drop") == 0) ||
                       (commandBuffer.compare(0, endOfVerb, "d") == 0)) {

                if ((InvFlag == 1) && (Roomflag == 0)) {
                    currentState->getCurrentRoom()->addObj(InventoryKeyMatch);
                    State::removeInvObj(InventoryKeyMatch);
                    std::cout << InventoryKeyMatch->getName() + " removed from your inventory!\n";
                    std::cout << InventoryKeyMatch->getName() + " Added to the current room!\n";
                    currentState->announceLoc();


                } else {
                    std::cout
                            << "The key you entered doesn't match an object in your inventory, or you already hold the object in your inventory!\n";
                }


            } else if (commandBuffer.compare(0, endOfVerb, "examine") == 0) {
                if (InvFlag == 1) {
                    std::cout << "Object " + InventoryKeyMatch->getName() + "\n" + "Object Description: " +
                                 InventoryKeyMatch->getDesc() + "\n" + "Object Keyword: " +
                                 InventoryKeyMatch->getKeyword() + "\n";
                } else if (Roomflag == 1) {
                    std::cout
                            << "Object " + RoomKeyMatch->getName() + "\n" + "Object " + RoomKeyMatch->getDesc() + "\n" +
                               "Object Keyword: " + RoomKeyMatch->getKeyword() + "\n";
                }

            } else {
                std::cout << "I didn't recognise that command! \n";
                currentState->announceLoc();
            }


        } else {
            /* We could copy the verb to another string but there's no reason to, we'll just compare it in place. */
            /* Command to go north. */
            if ((commandBuffer.compare(0, endOfVerb, "north") == 0) ||
                (commandBuffer.compare(0, endOfVerb, "n") == 0)) {
                commandOk = true; /* Confirm command has been handled */
                /* See if there's a north exit */
                Room *northRoom = currentState->getCurrentRoom()->getNorth();
                if (northRoom == nullptr) { /* there isn't */
                    wrapOut(&badExit);      /* Output the "can't go there" message */
                    wrapEndPara();
                } else {                    /* There is */
                    currentState->goTo(northRoom); /* Update state to that room - this will also describe it */
                }
            } else if ((commandBuffer.compare(0, endOfVerb, "west") == 0) ||
                       (commandBuffer.compare(0, endOfVerb, "w") == 0)) {
                commandOk = true;
                Room *westRoom = currentState->getCurrentRoom()->getWest();
                if (westRoom == nullptr) { /* there isn't */
                    wrapOut(&badExit);      /* Output the "can't go there" message */
                    wrapEndPara();
                } else {                    /* There is */
                    currentState->goTo(westRoom); /* Update state to that room - this will also describe it */
                }

            } else if ((commandBuffer.compare(0, endOfVerb, "south") == 0) ||
                       (commandBuffer.compare(0, endOfVerb, "s") == 0)) {
                commandOk = true;
                Room *southRoom = currentState->getCurrentRoom()->getSouth();
                if (southRoom == nullptr) { /* there isn't */
                    wrapOut(&badExit);      /* Output the "can't go there" message */
                    wrapEndPara();
                } else {                    /* There is */
                    currentState->goTo(southRoom); /* Update state to that room - this will also describe it */
                }

            } else if ((commandBuffer.compare(0, endOfVerb, "east") == 0) ||
                       (commandBuffer.compare(0, endOfVerb, "e") == 0)) {
                commandOk = true;
                Room *eastRoom = currentState->getCurrentRoom()->getEast();
                if (eastRoom == nullptr) { /* there isn't */
                    wrapOut(&badExit);      /* Output the "can't go there" message */
                    wrapEndPara();
                } else {                    /* There is */
                    currentState->goTo(eastRoom); /* Update state to that room - this will also describe it */
                }

            } else if ((commandBuffer.compare(0, endOfVerb, "inventory") == 0) ||
                       (commandBuffer.compare(0, endOfVerb, "i") == 0)) {
                commandOk = true;
                State::getInventoryNames();
            } else if ((commandBuffer.compare(0, endOfVerb, "save") == 0)) {
                commandOk = true;
                SaveGame();
            } else if ((commandBuffer.compare(0, endOfVerb, "load") == 0)) {
                bool flag = false;
                commandOk = true;

                while (flag == false) {
                    std::cout << "Enter Filename: ";
                    std::cout << "\n";
                    inputCommand(&commandBuffer);
                    flag = is_file_exist(commandBuffer);
                }

                LoadGame(commandBuffer, true);

            }


            /* Quit command */
            if ((commandBuffer.compare(0, endOfVerb, "quit") == 0)) {
                commandOk = true;
                gameOver = true;
            }


            /* If commandOk hasn't been set, command wasn't understood, display error message */

        }
        if (!commandOk) {
            wrapOut(&badCommand);
            wrapEndPara();
        }
        }
    }

void SaveGame() {
    ofstream myfile;
    bool valid = false;

    while(valid == false){

        std::cout << "Enter Filename (must use .txt file extension): ";
        std::cout << "\n";

        inputCommand(&commandBuffer);

        auto temp = commandBuffer;

        if (is_file_exist(temp) == true){
            system(("attrib -R " + temp).c_str());
        }

        if(commandBuffer.substr( commandBuffer.length() - 4 ) == ".txt"){
        myfile.open(commandBuffer, std::ofstream::trunc);

        myfile << "Location: ";
        myfile << currentState->getRoomNumber();

        for(auto& rooms1 : Room::rooms){
            myfile << "\n";
            for(auto & objs1: rooms1->getObjs()){
                myfile << objs1->getKeyword() + " ";
            }

        }
        myfile << "\n";
        myfile << "Inventory: " << "\n";
        for(auto & objs1: State::getObjs()){
            myfile << objs1->getKeyword() + " ";
        }
        std::cout << "File successfully saved! \n";
        system(("attrib +R " + temp).c_str());
        valid = true;



        }
        else{
            std::cout << "That was an invalid input! \n";
            valid = false;
    }
}
    myfile.close();

}

bool is_file_exist(const string& fileName) {
    std::ifstream infile(fileName);

    if (infile.good() == true){
        std::cout <<  "Loading file...\n";
    }else{
        std::cout << "File does not exist!\n";
    }

    return infile.good();
}




std::string splitString(string basicString) {
    std::string s = std::move(basicString);
    std::string delimiter = " ";
    size_t pos = 0;


    while ((pos = s.find(delimiter)) != std::string::npos) {
        s.erase(0, pos + delimiter.length());
    }
    return s;
}

vector<string> split(string str, const string& token) {
    vector<string>result;
    while(!str.empty()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

void LoadGame(const string& filename, bool state) {
    ifstream myfile (filename);
    string line;
    string s;
    int count = 0;

    if(state == true) {

        for (auto &destruction : Room::rooms) {
            for (auto &objz : destruction->getObjs()) {
                destruction->RemoveObject(objz);
            }
        }

        for (auto &InvDestruction : State::getObjs()){
            State::removeInvObj(InvDestruction);
        }
    }

    if (myfile.is_open())
    {
        while (getline(myfile,line)) {

            if (count == 0){
                s = splitString(line);
                int i_s = std::stoi(s);
                auto roomstart = Room::getRoom(i_s);
                currentState = new State(roomstart);


            } else if(count >= 1 && count <= Room::rooms.size()){

                auto RoomObjectlist = split(line, " ");
                auto temp = Room::getRoom(count);

                        for(auto &objects : objs){
                            if (std::find(RoomObjectlist.begin(), RoomObjectlist.end(), objects->getKeyword()) != RoomObjectlist.end())
                            {
                                temp->addObj(objects);
                            }
                    }

                } else if(count == Room::rooms.size() + 2){
                auto Inventorylist = split(line, " ");

                for(auto &objects : objs){
                    if (std::find(Inventorylist.begin(), Inventorylist.end(), objects->getKeyword()) != Inventorylist.end())
                    {
                        State::addInvObj(objects);
                    }
                }

            }
            count++;
                }

            }

    myfile.close();

    currentState->announceLoc();

    gameLoop();



        }





int main() {
    initWordWrap();
    std::cout << "-------------------------------------------------------------\n";
    std::cout << "Menu\n";
    std::cout << "1) New Game\n";
    std::cout << "2) Load Game\n";
    std::cout << "-------------------------------------------------------------\n";
    std::cout << "\n";

    inputCommand(&commandBuffer);

    if (commandBuffer == "1"){
        initRooms(false);
        initState();
        currentState->announceLoc();
        gameLoop();
    }else if (commandBuffer ==  "2"){
        initRooms(true);
        bool flag = false;

        while(flag == false) {
            std::cout << "Enter Filename: ";
            std::cout << "\n";
            inputCommand(&commandBuffer);
            flag = is_file_exist(commandBuffer);
        }

        LoadGame(commandBuffer, false);

    }



    return 0;
}
//Text File Format
/**
 * Location: 1   -Room the player is in when game is saved
 * H10 H12 H01   -room 1
 * H10           -room 2
 * H12 H10       -room 3
 * Inventory:
 * H10 H12       -objects in inventory
 */

