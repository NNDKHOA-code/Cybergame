#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include "../core/MyVector.h"

using namespace std;

class Payment
{
private:
    string paymentID;
    string dateTime;
    string sessionID;
    string employeeID;
    int totalAmount;

public:
    Payment() : totalAmount(0) {}

    static Payment from_csv(const Vec<string> &c)
    {
        Payment x;
        if (c.getsize() > 0)
            x.paymentID = c[0];
        if (c.getsize() > 1)
            x.dateTime = c[1];
        if (c.getsize() > 2)
            x.totalAmount = stoi(c[2]);
        if (c.getsize() > 3)
            x.sessionID = c[3];
        if (c.getsize() > 4)
            x.employeeID = c[4];
        return x;
    }

    Vec<string> to_csv() const
    {
        return makerow({paymentID, dateTime, to_string(totalAmount), sessionID, employeeID});
    }

    const string &getPaymentID() const
    {
        return paymentID;
    }
    const string &getDateTime() const
    {
        return dateTime;
    }
    const string &getSessionID() const
    {
        return sessionID;
    }
    const string &getEmployeeID() const
    {
        return employeeID;
    }
    int getTotalAmount() const
    {
        return totalAmount;
    }

    void setPaymentID(const string &paymentID)
    {
        this->paymentID = paymentID;
    }
    void setDateTime(const string &dateTime)
    {
        this->dateTime = dateTime;
    }
    void setSessionID(const string &sessionID)
    {
        this->sessionID = sessionID;
    }
    void setEmployeeID(const string &employeeID)
    {
        this->employeeID = employeeID;
    }
    void setTotalAmount(int totalAmount)
    {
        this->totalAmount = totalAmount;
    }
};

#endif
