#ifndef STATISTICS_H
#define STATISTICS_H

#include "../core/Ui.h"
#include "../core/Utils.h"
#include "../core/MyVector.h"
#include "../core/MyHashMap.h"

#include "../manager/PaymentManager.h"
#include "../manager/ServiceSessionManager.h"
#include "../manager/SessionManager.h"

using namespace std;

struct KeyValue
{
    string key;
    int value;
};

class Statistics
{
private:
    static void sortAndPrint(HashMap<string, int> &sum, const string &label, int total)
    {
        Vec<string> hdr = makerow({label, "Doanh thu"});
        Vec<Vec<string>> rows;

        Vec<KeyValue> vec;
        for (auto it = sum.begin(); it != sum.end(); ++it)
        {
            KeyValue kv;
            kv.key = it.operator->()->key;
            kv.value = it.operator->()->value;
            vec.PushBack(kv);
        }

        if (vec.getsize() > 1)
            mergeSort(vec, 0, vec.getsize() - 1);

        for (int i = 0; i < vec.getsize(); i++)
            rows.PushBack(makerow({vec[i].key, to_string(vec[i].value)}));

        printTable(hdr, rows);
        okMsg("Tổng doanh thu: " + to_string(total) + " VND");
    }

public:
    static void byService(const string &dir)
    {
        ManagerBase<Session> *sm = new SessionManager(dir + "/session.csv");
        ManagerBase<ServiceSession> *ssm = new ServiceSessionManager(dir + "/service_session.csv");
        sm->load();
        ssm->load();

        string from, to;
        cout << "Từ ngày (DD-MM-YYYY): ";
        cin >> from;
        cout << "Đến ngày (DD-MM-YYYY): ";
        cin >> to;
        int F = reFormatDate(from), T = reFormatDate(to);
        int total = 0;
        HashMap<string, int> sum;

        for (int i = 0; i < ssm->items.getsize(); i++)
        {
            auto r = ssm->items[i].to_csv();

            string sessionID = r[0];
            string serviceID = r[1];
            int quantity = stoi(r[2]);
            int price = stoi(r[3]);
            int cost = quantity * price;

            string date;
            for (int j = 0; j < sm->items.getsize(); j++)
            {
                if (sm->items[j].getSessionID() == sessionID)
                {
                    auto row = sm->items[j].to_csv();
                    date = row[1];
                    break;
                }
            }
            if (date.empty())
                continue;
            int D = reFormatDate(date);
            if (D < F || D > T)
                continue;
            sum[serviceID] += cost;
            total += cost;
        }

        sortAndPrint(sum, "ServiceID", total);
        delete sm;
        delete ssm;
    }

    static void byCustomer(const string &dir)
    {
        ManagerBase<Session> *sm = new SessionManager(dir + "/session.csv");
        sm->load();

        string from, to;
        cout << "Từ ngày (DD-MM-YYYY): ";
        cin >> from;
        cout << "Đến ngày (DD-MM-YYYY): ";
        cin >> to;
        int F = reFormatDate(from), T = reFormatDate(to);

        HashMap<string, int> sum;
        int total = 0;

        for (int i = 0; i < sm->items.getsize(); i++)
        {
            auto r = sm->items[i].to_csv();
            int D = reFormatDate(r[1]);
            if (D < F || D > T)
                continue;
            sum[r[5]] += stoi(r[4]);
            total += stoi(r[4]);
        }

        sortAndPrint(sum, "CustomerID", total);
        delete sm;
    }

    static void byRoom(const string &dir)
    {
        ManagerBase<Room> *rm = new RoomManager(dir + "/room.csv");
        ManagerBase<Session> *sm = new SessionManager(dir + "/session.csv");
        rm->load();
        sm->load();

        string from, to;
        cout << "Từ ngày (DD-MM-YYYY): ";
        cin >> from;
        cout << "Đến ngày (DD-MM-YYYY): ";
        cin >> to;
        int F = reFormatDate(from), T = reFormatDate(to);

        HashMap<string, int> sum;
        int total = 0;

        for (int i = 0; i < sm->items.getsize(); i++)
        {
            auto r = sm->items[i].to_csv();
            int D = reFormatDate(r[1]);
            if (D < F || D > T)
                continue;
            if(stoi(r[4]) == 0) continue;
            string roomID = r[8];
            string start = r[2];
            string end = r[3];
            int pricePerHour = 0;
            for (int j = 0; j < rm->items.getsize(); j++)
            {
                if (rm->items[j].getRoomID() == roomID)
                {
                    auto row = rm->items[j].to_csv();
                    pricePerHour = stoi(row[4]);
                    break;
                }
            }
            int mins = StringToMinutes(end) - StringToMinutes(start);
            if (mins < 0)
                mins = 0;
            int hourCost = pricePerHour * (mins / 60.0);
            sum[roomID] += hourCost;
            total += hourCost;
        }

        sortAndPrint(sum, "RoomID", total);
        delete rm;
        delete sm;
    }

    static void byComputer(const string &dir)
    {
        ManagerBase<Room> *rm = new RoomManager(dir + "/room.csv");
        ManagerBase<Session> *sm = new SessionManager(dir + "/session.csv");
        rm->load();
        sm->load();

        string from, to;
        cout << "Từ ngày (DD-MM-YYYY): ";
        cin >> from;
        cout << "Đến ngày (DD-MM-YYYY): ";
        cin >> to;
        int F = reFormatDate(from), T = reFormatDate(to);

        HashMap<string, int> sum;
        int total = 0;

        for (int i = 0; i < sm->items.getsize(); i++)
        {
            auto r = sm->items[i].to_csv();
            int D = reFormatDate(r[1]);
            if (D < F || D > T)
                continue;
            if(stoi(r[4]) == 0) continue;
            string computerID = r[7];
            string roomID = r[8];
            string start = r[2];
            string end = r[3];
            int pricePerHour = 0;
            for (int j = 0; j < rm->items.getsize(); j++)
            {
                if (rm->items[j].getRoomID() == roomID)
                {
                    auto row = rm->items[j].to_csv();
                    pricePerHour = stoi(row[4]);
                    break;
                }
            }
            int mins = StringToMinutes(end) - StringToMinutes(start);
            if (mins < 0)
                mins = 0;
            int hourCost = pricePerHour * (mins / 60.0);
            sum[computerID] += hourCost;
            total += hourCost;
        }

        sortAndPrint(sum, "ComputerID", total);
        delete rm;
        delete sm;
    }

    static void byEmployee(const string &dir)
    {
        ManagerBase<Session> *sm = new SessionManager(dir + "/session.csv");
        sm->load();

        string from, to;
        cout << "Từ ngày (DD-MM-YYYY): ";
        cin >> from;
        cout << "Đến ngày (DD-MM-YYYY): ";
        cin >> to;
        int F = reFormatDate(from), T = reFormatDate(to);

        HashMap<string, int> sum;
        int total = 0;

        for (int i = 0; i < sm->items.getsize(); i++)
        {
            auto r = sm->items[i].to_csv();
            int D = reFormatDate(r[1]);
            if (D < F || D > T)
                continue;
            sum[r[6]] += stoi(r[4]);
            total += stoi(r[4]);
        }

        sortAndPrint(sum, "EmployeeID", total);
        delete sm;
    }
};

#endif
