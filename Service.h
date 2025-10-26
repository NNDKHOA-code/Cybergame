#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include "../core/MyVector.h"

using namespace std;

class Service
{
private:
    string ServiceID;
    string name;
    string status;
    string adminID;
    int price;
    int qty;

public:
    Service()
        : price(0), qty(0), status("Active"), adminID("") {}

    static Service from_csv(const Vec<string> &c)
    {
        Service x;
        if (c.getsize() > 0)
            x.ServiceID = c[0];
        if (c.getsize() > 1)
            x.name = c[1];
        if (c.getsize() > 2)
            x.price = stoi(c[2]);
        if (c.getsize() > 3)
            x.qty = stoi(c[3]);
        if (c.getsize() > 4)
            x.status = c[4];
        if (c.getsize() > 5)
            x.adminID = c[5];
        return x;
    }

    Vec<string> to_csv() const
    {
        return makerow({ServiceID, name, to_string(price), to_string(qty), status, adminID});
    }
    const string &getServiceID() const
    {
        return ServiceID;
    }
    const string &getName() const
    {
        return name;
    }
    const string &getStatus() const
    {
        return status;
    }
    const string &getAdminID() const
    {
        return adminID;
    }
    int getPrice() const
    {
        return price;
    }
    int getQty() const
    {
        return qty;
    }

    void setServiceID(const string &id)
    {
        this->ServiceID = ServiceID;
    }
    void setName(const string &name)
    {
        this->name = name;
    }
    void setStatus(const string &status)
    {
        this->status = status;
    }
    void setPrice(int price)
    {
        this->price = price;
    }
    void setQty(int qty)
    {
        this->qty = qty;
    }
    void setAdminID(const string &adminID)
    {
        this->adminID = adminID;
    }
};

#endif
