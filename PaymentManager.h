#ifndef PAYMENT_MANAGER_H
#define PAYMENT_MANAGER_H

#include "ManagerBase.h"
#include "../model/Payment.h"

using namespace std;

static string getId_Payment(const Payment& x) {
    return x.to_csv()[0];
}

class PaymentManager : public ManagerBase<Payment> {
public:
    PaymentManager(const string& p)
        : ManagerBase<Payment>(p) {}

    Vec<string> toRow(const Payment& x) const override {
        return x.to_csv();
    }

    Payment fromRow(const Vec<string>& r) const override {
        return Payment::from_csv(r);
    }

    string IDof(const Payment& x) const override {
        return toRow(x)[0];
    }

    string (*IDgetter() const)(const Payment&) override {
        return getId_Payment;
    }
};

#endif
