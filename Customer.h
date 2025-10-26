
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "../core/MyVector.h"
#include "Person.h"

using namespace std;

class Customer
{
private:
    string customerID, personID, vipLevel;
    int totalDeposit;
    int balance;

public:
    Customer() : balance(0), totalDeposit(0), vipLevel("Bronze") {}

    static Customer from_csv(const Vec<string> &r)
    {
        Customer c;
        if (r.getsize() > 0)
            c.customerID = r[0];
        if (r.getsize() > 1)
            c.balance = stoi(r[1]);
        if (r.getsize() > 2)
            c.totalDeposit = stoi(r[2]);
        if (r.getsize() > 3)
            c.vipLevel = r[3];
        if (r.getsize() > 4)
            c.personID = r[4];
        return c;
    }

    Vec<string> to_csv() const
    {
        return makerow({customerID, to_string(balance), to_string(totalDeposit), vipLevel, personID});
    }

    void upgradeVip()
    {
        if (totalDeposit >= 2000000)
            vipLevel = "Diamond";
        else if (totalDeposit >= 100000)
            vipLevel = "Gold";
        else if (totalDeposit >= 50000)
            vipLevel = "Silver";
        else
            vipLevel = "Bronze";
    }

    int getDiscount() const
    {
        if (vipLevel == "Diamond")
            return 15;
        if (vipLevel == "Gold")
            return 10;
        if (vipLevel == "Silver")
            return 5;
        return 0;
    }

    const string &getCustomerID() const
    {
        return customerID;
    }
    const string &getVipLevel() const
    {
        return vipLevel;
    }
    int getBalance() const
    {
        return balance;
    }
    int gettotalDeposit() const
    {
        return totalDeposit;
    }

    void setCustomerID(const string &customerID)
    {
        this->customerID = customerID;
    }
    void setVipLevel(const string &vipLevel)
    {
        this->vipLevel = vipLevel;
    }
    void setBalance(int balance)
    {
        this->balance = balance;
    }
    void settotalDeposit(int totalDeposit)
    {
        this->totalDeposit = totalDeposit;
    }
};

#endif
