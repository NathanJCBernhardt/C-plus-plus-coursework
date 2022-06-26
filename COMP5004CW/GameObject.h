//
// Created by Nathan on 21/11/2020.
//

#ifndef TEXTADV_GAMEOBJECT_H
#define TEXTADV_GAMEOBJECT_H

#include <string>

using std::string;

class GameObject {
    const string* name;

    const string* description;
    string* keyword;



public:

    GameObject(const string* _name, const string* _description, string* _keyword);


    const string getName() {return *name;};
    const string getDesc() {return *description;};

    string getKeyword() {return *keyword;};

};


#endif //TEXTADV_GAMEOBJECT_H
