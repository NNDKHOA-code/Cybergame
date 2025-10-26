#ifndef SERVICE_MANAGER_H
#define SERVICE_MANAGER_H

#include "ManagerBase.h"
#include "../model/Service.h"

using namespace std;

static string getId_Service(const Service& x) {
    return x.to_csv()[0];  
}

class ServiceManager : public ManagerBase<Service> {
public:
    ServiceManager(const string& p)
        : ManagerBase<Service>(p) {}

    Vec<string> toRow(const Service& x) const override {
        return x.to_csv();   
    }

    Service fromRow(const Vec<string>& r) const override {
        return Service::from_csv(r);
    }

    string IDof(const Service& x) const override {
        return toRow(x)[0];
    }

    string (*IDgetter() const)(const Service&) override {
        return getId_Service;
    }
};

#endif
