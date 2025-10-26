#ifndef SERVICE_SESSION_MANAGER_H
#define SERVICE_SESSION_MANAGER_H

#include "ManagerBase.h"
#include "../model/ServiceSession.h"

using namespace std;

static string getId_ServiceSession(const ServiceSession& x) {
    return x.to_csv()[0];
}

class ServiceSessionManager : public ManagerBase<ServiceSession> {
public:
    ServiceSessionManager(const string& p)
        : ManagerBase<ServiceSession>(p) {}

    Vec<string> toRow(const ServiceSession& x) const override {
        return x.to_csv();
    }

    ServiceSession fromRow(const Vec<string>& r) const override {
        return ServiceSession::from_csv(r);
    }

    string IDof(const ServiceSession& x) const override {
        return toRow(x)[0];
    }

    string (*IDgetter() const)(const ServiceSession&) override {
        return getId_ServiceSession;
    }
};

#endif
