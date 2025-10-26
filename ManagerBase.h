#ifndef MANAGER_BASE_H
#define MANAGER_BASE_H

#include <string>
#include "../core/MyVector.h"
#include "../core/MyHashMap.h"
#include "../core/CSV.h"

using namespace std;

template <typename T>
class ManagerBase {
protected:
    string path;
public:
    Vec<T> items;
    HashMap<string, int> ID;
    
    ManagerBase(const string& p ="") : path(p) {}
    
    virtual ~ManagerBase() {}
    virtual Vec<string> toRow(const T& x) const = 0;
    virtual T fromRow(const Vec<string>& r) const = 0;
    virtual string IDof(const T& x) const = 0;
    virtual string(*IDgetter() const)(const T&) = 0;

    void reindex(string (*getid)(const T&)) {
        ID.clear();
        for(int i= 0;i<items.getsize();i++) {
            ID[getid(items[i])] = i;
        }
    }

    T* find(const string& id) {
        typename HashMap<string, int> ::iterator it = ID.find(id);
        if(it == ID.end()) return nullptr;
        int idx = it.operator->()->value;
        return &items[idx];
    }

    bool eraseID(const string& id) {
        typename HashMap<string, int>::iterator it = ID.find(id);
        if(it == ID.end()) return false;
        int idx = it.operator->()->value;
        items.removeAt(idx);
        reindex(IDgetter());
        return true;
    }

    void load() {
        items.clear();
        ID.clear();
        auto rows = CSV::read(path);
        for(int i=0;i<rows.getsize();i++) {
            items.PushBack(fromRow(rows[i]));
        }
        reindex(IDgetter());
    }

    void save() {
        Vec<Vec<string>> rows;
        for(int i=0;i<items.getsize();i++) {
            rows.PushBack(toRow(items[i]));
        }
        CSV::write(path, rows);
    }

    void upsert(const string& id, const T& val) {
        typename HashMap<string, int>::iterator it = ID.find(id);
        if(it == ID.end()) {
            items.PushBack(val);
            ID[id] = items.getsize() - 1;
        } else {
            int idx = it.operator->()->value;
            items[idx] = val;
        }
    }
};

#endif
