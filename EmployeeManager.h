#ifndef EMPLOYEE_MANAGER_H
#define EMPLOYEE_MANAGER_H

#include "ManagerBase.h"
#include "../model/Employee.h"

using namespace std;

static string getId_Employee(const Employee& x) {
    return x.to_csv()[0];
}

class EmployeeManager : public ManagerBase<Employee> {
public:
    EmployeeManager(const string& p)
        : ManagerBase<Employee>(p) {}

    Vec<string> toRow(const Employee& x) const override {
        return x.to_csv(); 
    }

    Employee fromRow(const Vec<string>& r) const override {
        return Employee::from_csv(r);
    }

    string IDof(const Employee& x) const override {
        return toRow(x)[0];
    }

    string (*IDgetter() const)(const Employee&) override {
        return getId_Employee;
    }
};

#endif
