#ifndef COMPUTER_MANAGER_H
#define COMPUTER_MANAGER_H

#include "ManagerBase.h"
#include "../model/Computer.h"

using namespace std;

static string getId_Computer(const Computer& x) {
    return x.to_csv()[0];  
}

class ComputerManager : public ManagerBase<Computer> {
public:
    ComputerManager(const string& p)
        : ManagerBase<Computer>(p) {}

    Vec<string> toRow(const Computer& x) const override {
        return x.to_csv();  
    }

    Computer fromRow(const Vec<string>& r) const override {
        return Computer::from_csv(r);
    }

    string IDof(const Computer& x) const override {
        return toRow(x)[0];
    }

    string (*IDgetter() const)(const Computer&) override {
        return getId_Computer;
    }
};

#endif
