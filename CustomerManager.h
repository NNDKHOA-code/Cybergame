#ifndef CUSTOMER_MANAGER_H
#define CUSTOMER_MANAGER_H

#include "ManagerBase.h"
#include "../model/Customer.h"

using namespace std;

static string getId_Customer(const Customer& x) {
    return x.to_csv()[0];  
}

class CustomerManager : public ManagerBase<Customer> {
public:
    CustomerManager(const string& p) : ManagerBase<Customer>(p) {}

    Vec<string> toRow(const Customer& x) const override {
        return x.to_csv();  
    }

    Customer fromRow(const Vec<string>& r) const override {
        return Customer::from_csv(r);
    }

    string IDof(const Customer& x) const override {
        return toRow(x)[0];
    }

    string (*IDgetter() const)(const Customer&) override {
        return getId_Customer;
    }

};

#endif
