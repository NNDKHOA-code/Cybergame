#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "ManagerBase.h"
#include "../model/Session.h"

using namespace std;

static string getId_Session(const Session& x) { 
    return x.to_csv()[0];
}

class SessionManager : public ManagerBase<Session> {
public:
    SessionManager(const string& path)
        : ManagerBase<Session>(path) {}

    Vec<string> toRow(const Session& x) const override {
        return x.to_csv();
    }

    Session fromRow(const Vec<string>& r) const override {
        return Session::from_csv(r);
    }

    string IDof(const Session& x) const override {
        return toRow(x)[0];
    }

    string (*IDgetter() const)(const Session&) override {
        return getId_Session;
    }
};

#endif
