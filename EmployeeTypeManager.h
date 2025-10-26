#ifndef EMPLOYEETYPE_MANAGER_H
#define EMPLOYEETYPE_MANAGER_H

#include "ManagerBase.h"
#include "../model/EmployeeType.h"

using namespace std;

static string getId_EmployeeType(const EmployeeType& x) {
    return x.to_csv()[0];
}

class EmployeeTypeManager : public ManagerBase<EmployeeType> {
public:
    EmployeeTypeManager(const string& p)
        : ManagerBase<EmployeeType>(p) {}

    Vec<string> toRow(const EmployeeType& x) const override {
        return x.to_csv();
    }

    EmployeeType fromRow(const Vec<string>& r) const override {
        return EmployeeType::from_csv(r);
    }

    string IDof(const EmployeeType& x) const override {
        return toRow(x)[0];
    }

    string (*IDgetter() const)(const EmployeeType&) override {
        return getId_EmployeeType;
    }

};

#endif
