#ifndef SESSION_H
#define SESSION_H

#include <string>
#include "../core/MyVector.h"

using namespace std;

class Session
{
private:
    string SessionID;
    string date;
    string start;
    string end;
    string customerID;
    string employeeID;
    string computerID;
    string roomID;
    int totalCost;

public:
    Session() : totalCost(0) {}

    static Session from_csv(const Vec<string> &c)
    {
        Session x;
        if (c.getsize() > 0)
            x.SessionID = c[0];
        if (c.getsize() > 1)
            x.date = c[1];
        if (c.getsize() > 2)
            x.start = c[2];
        if (c.getsize() > 3)
            x.end = c[3];
        if (c.getsize() > 4)
            x.totalCost = stoi(c[4]);
        if (c.getsize() > 5)
            x.customerID = c[5];
        if (c.getsize() > 6)
            x.employeeID = c[6];
        if (c.getsize() > 7)
            x.computerID = c[7];
        if (c.getsize() > 8)
            x.roomID = c[8];
        return x;
    }

    Vec<string> to_csv() const
    {
        return makerow({SessionID, date, start, end, to_string(totalCost), customerID, employeeID, computerID, roomID});
    }

    const string &getSessionID() const
    {
        return SessionID;
    }
    const string &getDate() const
    {
        return date;
    }
    const string &getStart() const
    {
        return start;
    }
    const string &getEnd() const
    {
        return end;
    }
    const string &getCustomerID() const
    {
        return customerID;
    }
    const string &getEmployeeID() const
    {
        return employeeID;
    }
    const string &getComputerID() const
    {
        return computerID;
    }
    const string &getRoomID() const
    {
        return roomID;
    }
    int getTotal() const
    {
        return totalCost;
    }

    void setSessionID(const string &id)
    {
        this->SessionID = SessionID;
    }
    void setDate(const string &date)
    {
        this->date = date;
    }
    void setStart(const string &start)
    {
        this->start = start;
    }
    void setEnd(const string &end)
    {
        this->end = end;
    }
    void setCustomerID(const string &customerID)
    {
        this->customerID = customerID;
    }
    void setEmployeeID(const string &employeeID)
    {
        this->employeeID = employeeID;
    }
    void setComputerID(const string &computerID)
    {
        this->computerID = computerID;
    }
    void setRoomID(const string &roomID)
    {
        this->roomID = roomID;
    }
    void setTotal(int totalCost)
    {
        this->totalCost = totalCost;
    }
};

#endif
