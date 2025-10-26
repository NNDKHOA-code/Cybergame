
#ifndef PERSON_MANAGER_H
#define PERSON_MANAGER_H

#include "ManagerBase.h"
#include "../model/Person.h"

using namespace std;

static string getId_Person(const Person &x)
{
    return x.to_csv()[0];
}

class PersonManager : public ManagerBase<Person> {
public:
    PersonManager(const string& p): ManagerBase<Person>(p) {}

    Vec<string> toRow(const Person& x) const override {
        return x.to_csv();
    }

    Person fromRow(const Vec<string>& r) const override {
        return Person::from_csv(r);
    }

    string IDof(const Person& x) const override {
        return toRow(x)[0];
    }

    string (*IDgetter() const)(const Person&) override {
        return getId_Person;
    }

    Person* findByUsername(const string& u) {
        for (int i = 0; i < items.getsize(); ++i)
            if (items[i].getUsername() == u)
                return &items[i];
        return nullptr;
    }
};


#endif
