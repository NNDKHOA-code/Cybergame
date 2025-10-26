#ifndef SERVICE_SESSION_H
#define SERVICE_SESSION_H

#include <string>
#include "../core/MyVector.h"

using namespace std;

class ServiceSession
{
private:
    string sessionID;
    string serviceID;
    int qty;
    int totalPrice;

public:
    ServiceSession() : qty(0), totalPrice(0.0) {}

    static ServiceSession from_csv(const Vec<string> &c)
    {
        ServiceSession x;
        if (c.getsize() > 0)
            x.sessionID = c[0];
        if (c.getsize() > 1)
            x.serviceID = c[1];
        if (c.getsize() > 2)
            x.qty = stoi(c[2]);
        if (c.getsize() > 3)
            x.totalPrice = stoi(c[3]);
        return x;
    }

    Vec<string> to_csv() const
    {
        return makerow({sessionID, serviceID, to_string(qty), to_string(totalPrice)});
    }

    const string &getSessionID() const
    {
        return sessionID;
    }
    const string &getServiceID() const
    {
        return serviceID;
    }
    int getQty() const
    {
        return qty;
    }
    int getTotalPrice() const
    {
        return totalPrice;
    }

    void setSessionID(const string &sessionID)
    {
        this->sessionID = sessionID;
    }
    void setServiceID(const string &serviceID)
    {
        this->serviceID = serviceID;
    }
    void setQty(int qty)
    {
        this->qty = qty;
    }
    void setTotalPrice(int totalPrice)
    {
        this->totalPrice = totalPrice;
    }
};

#endif
