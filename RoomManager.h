#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include "ManagerBase.h"
#include "../model/Room.h"

using namespace std;

static string getId_Room(const Room& x) {
    return x.to_csv()[0];
}

class RoomManager : public ManagerBase<Room> {
public:
    RoomManager(const string& p)
        : ManagerBase<Room>(p) {}

    Vec<string> toRow(const Room& x) const override {
        return x.to_csv();
    }

    Room fromRow(const Vec<string>& r) const override {
        return Room::from_csv(r);
    }

    string IDof(const Room& x) const override {
        return toRow(x)[0];
    }

    string (*IDgetter() const)(const Room&) override {
        return getId_Room;
    }
};

#endif
