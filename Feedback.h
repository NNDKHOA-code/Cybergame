#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <string>
#include "../core/MyVector.h"
#include "../core/Utils.h"
using namespace std;

class Feedback
{
private:
    string id;
    string customerID;
    string date;
    string message;

public:
    Feedback() {}

    static Feedback from_csv(const Vec<string> &c)
    {
        Feedback x;
        if (c.getsize() > 0)
            x.id = c[0];
        if (c.getsize() > 1)
            x.customerID = c[1];
        if (c.getsize() > 2)
            x.date = c[2];
        if (c.getsize() > 3)
            x.message = c[3];
        return x;
    }

    Vec<string> to_csv() const
    {
        return makerow({id, customerID, date, message});
    }

    string getID() const { return id; }
    string getCustomerID() const { return customerID; }
    string getMessage() const { return message; }
};
#endif
