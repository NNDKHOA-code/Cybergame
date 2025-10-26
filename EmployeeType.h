#ifndef EMPLOYEE_TYPE_H
#define EMPLOYEE_TYPE_H

#include <string>
#include "../core/MyVector.h"

using namespace std;

class EmployeeType
{
private:
    string TypeID, name, shift, start, end;
    int baseSalary;

public:
    EmployeeType() : baseSalary(0.0) {}

    static EmployeeType from_csv(const Vec<string> &c)
    {
        EmployeeType x;
        if (c.getsize() > 0)
            x.TypeID = c[0];
        if (c.getsize() > 1)
            x.name = c[1];
        if (c.getsize() > 2)
            x.shift = c[2];
        if (c.getsize() > 3)
            x.start = c[3];
        if (c.getsize() > 4)
            x.end = c[4];
        if (c.getsize() > 5)
            x.baseSalary = stoi(c[5]);
        return x;
    }

    Vec<string> to_csv() const
    {
        return makerow({TypeID, name, shift, start, end, to_string(baseSalary)});
    }

    const string &getTypeID() const
    {
        return TypeID;
    }
    const string &getName() const
    {
        return name;
    }
    const string &getShift() const
    {
        return shift;
    }
    const string &getStart() const
    {
        return start;
    }
    const string &getEnd() const
    {
        return end;
    }
    int getBaseSalary() const
    {
        return baseSalary;
    }

    void setTypeID(const string &TypeID)
    {
        this->TypeID = TypeID;
    }
    void setName(const string &name)
    {
        this->name = name;
    }
    void setShift(const string &shift)
    {
        this->shift = shift;
    }
    
    void setBaseSalary(int baseSalary)
    {
        this->baseSalary = baseSalary;
    }
};

#endif
