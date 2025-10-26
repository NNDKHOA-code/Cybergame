#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include "../core/MyVector.h"
#include "Person.h"

using namespace std;

class Employee : public Person
{
private:
    string employeeID, hiredate, adminID, typeID;
    int salary;

public:
    Employee() : salary(0), adminID("adm")
    {
        setRole("EMPLOYEE");
    }

    static Employee from_csv(const Vec<string> &c)
    {
        Employee x;
        if (c.getsize() > 0)
            x.employeeID = c[0];
        if (c.getsize() > 1)
            x.hiredate = c[1];
        if (c.getsize() > 2)
            x.salary = stoi(c[2]);
        if (c.getsize() > 3)
            x.setID(c[3]);
        if (c.getsize() > 4)
            x.adminID = c[4];
        if (c.getsize() > 5)
            x.typeID = c[5];
        return x;
    }

    Vec<string> to_csv() const
    {
        return makerow({employeeID, hiredate, to_string(salary), getID(), adminID, typeID});
    }

    const string &getEmployeeID() const
    {
        return employeeID;
    }
    const string &getHireDate() const
    {
        return hiredate;
    }
    int getSalary() const
    {
        return salary;
    }
    const string &getAdminID() const
    {
        return adminID;
    }
    const string &getTypeID() const
    {
        return typeID;
    }

    void setEmployeeID(const string &employeeID)
    {
        this->employeeID = employeeID;
    }
    void setHireDate(const string &hiredate)
    {
        this->hiredate = hiredate;
    }
    void setSalary(int salary)
    {
        this->salary = salary;
    }
    void setAdminID(const string &adminID)
    {
        this->adminID = adminID;
    }
    void setTypeID(const string &typeID)
    {
        this->typeID = typeID;
    }
};

#endif