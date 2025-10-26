#ifndef PERSON_H
#define PERSON_H

#include <string>
#include "../core/MyVector.h"

using namespace std;

class Person
{
protected:
    string id;
    string fullname;
    string username;
    string password;
    string role;

public:
    Person(){};

    virtual ~Person() {}

    const string& getID() const {
        return id;
    }
    const string& getFullName() const {
        return fullname;
    }
    const string& getUsername() const {
        return username;
    }
    const string& getPassword() const {
        return password;
    }
    const string& getRole() const {
        return role;
    }

    void setID(const string& id) {
        this->id = id;
    }
    void setFullName(const string& fullname) {
        this->fullname = fullname;
    }
    void setUsername(const string& username) {
        this->username = username;
    }
    void setPassword(const string& password) {
        this->password = password;
    }
    void setRole(const string& role) {
        this->role = role;
    }

    static Person from_csv(const Vec<string>& c) {
        Person x;
        if (c.getsize() > 0) x.id = c[0];
        if (c.getsize() > 1) x.fullname = c[1];
        if (c.getsize() > 2) x.username = c[2];
        if (c.getsize() > 3) x.password = c[3];
        if (c.getsize() > 4) x.role = c[4];
        return x;
    }

    Vec<string> to_csv() const {
        return makerow({id, fullname, username, password, role});
    }
};

class Admin : public Person {
public:
    using Person::Person;
};

#endif
