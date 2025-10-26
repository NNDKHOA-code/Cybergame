#ifndef ROOM_H
#define ROOM_H

#include <string>
#include "../core/MyVector.h"

using namespace std;

class Room
{
private:
    string RoomID;
    string name;
    string type;
    string status;
    string adminID;
    int capacity;
    int pricePerHour;

public:
    Room()
        : status("Active"), adminID(""), capacity(0), pricePerHour(0) {}


    static Room from_csv(const Vec<string> &c)
    {
        Room x;
        if (c.getsize() > 0)
            x.RoomID = c[0];
        if (c.getsize() > 1)
            x.name = c[1];
        if (c.getsize() > 2)
            x.type = c[2];
        if (c.getsize() > 3)
            x.capacity = stoi(c[3]);
        if (c.getsize() > 4)
            x.pricePerHour = stoi(c[4]);
        if (c.getsize() > 5)
            x.status = c[5];
        if (c.getsize() > 6)
            x.adminID = c[6];
        return x;
    }

    Vec<string> to_csv() const
    {
        return makerow({RoomID, name, type, to_string(capacity), to_string(pricePerHour), status, adminID});
    }

    const string &getRoomID() const
    {
        return RoomID;
    }
    const string &getName() const
    {
        return name;
    }
    const string &getType() const
    {
        return type;
    }
    const string &getStatus() const
    {
        return status;
    }
    const string &getAdminID() const
    {
        return adminID;
    }
    int getCapacity() const
    {
        return capacity;
    }
    int getPricePerHour() const
    {
        return pricePerHour;
    }

    void setRoomID(const string &RoomID)
    {
        this->RoomID = RoomID;
    }
    void setName(const string &name)
    {
        this->name = name;
    }
    void setType(const string &type)
    {
        this->type = type;
    }
    void setCapacity(int capacity)
    {
        this->capacity = capacity;
    }
    void setPricePerHour(int pricePerHour)
    {
        this->pricePerHour = pricePerHour;
    }
    void setAdminID(const string &adminID)
    {
        this->adminID = adminID;
    }
    void setStatus(const string &s)
    {
        this->status = status;
    }
};

#endif
