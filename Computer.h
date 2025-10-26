
#ifndef COMPUTER_H
#define COMPUTER_H

#include <string>
#include "../core/MyVector.h"

using namespace std;

class Computer
{
private:
    string ComputerID;
    string name;
    string conf;
    string status;
    string roomID;
    string adminID;

public:
    Computer() : status("Ready"), adminID("adm") {}
    
    static Computer from_csv(const Vec<string> &c)
    {
        Computer x;
        if (c.getsize() > 0)
            x.ComputerID = c[0];
        if (c.getsize() > 1)
            x.name = c[1];
        if (c.getsize() > 2)
            x.conf = c[2];
        if (c.getsize() > 3)
            x.status = c[3];
        if (c.getsize() > 4)
            x.roomID = c[4];
        if (c.getsize() > 5)
            x.adminID = c[5];
        return x;
    }
    Vec<string> to_csv() const
    {
        return makerow({ComputerID, name, conf, status, roomID, adminID});
    }

    const string &getComputerID() const
    {
        return ComputerID;
    }
    const string &getName() const
    {
        return name;
    }
    const string &getConf() const
    {
        return conf;
    }
    const string &getStatus() const
    {
        return status;
    }
    const string &getRoomID() const
    {
        return roomID;
    }
    const string &getAdminID() const
    {
        return adminID;
    }

    void setComputerID(const string &ComputerID)
    {
        this->ComputerID = ComputerID;
    }
    void setName(const string &name)
    {
        this->name = name;
    }
    void setConf(const string &conf)
    {
        this->conf = conf;
    }
    void setRoomID(const string &roomID)
    {
        this->roomID = roomID;
    }
    void setAdminID(const string &adminID)
    {
        this->adminID = adminID;
    }
    void setStatus(const string &status)
    {
        this->status = status;
    }
};

#endif
