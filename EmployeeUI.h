#ifndef EMPLOYEE_UI_H
#define EMPLOYEE_UI_H

#include "../core/Ui.h"
#include "../core/Utils.h"
#include "../ui/MenuBase.h"

#include "../manager/ServiceSessionManager.h"
#include "../manager/PaymentManager.h"
#include "../manager/ServiceManager.h"
#include "../manager/CustomerManager.h"
#include "../manager/SessionManager.h"
#include "../manager/RoomManager.h"
#include "../manager/ComputerManager.h"
#include "../manager/EmployeeManager.h"
#include "../manager/PersonManager.h"

#include <iomanip>
#include <algorithm>
using namespace std;

class EmployeeUI : public MenuBase {
private:
    string dir;
    string empID;

public:
    EmployeeUI(const string& d, const string& eid)
        : dir(d), empID(eid) {}

    void menuCustomer() {
        ManagerBase<Customer>* m = new CustomerManager(dir + "/customer.csv");
        m->load();

        genericCrud("KHÁCH HÀNG", *m,
            [&](Customer* x, bool hdr)->Vec<string> {
                if (hdr)
                    return makerow({"CustomerID", "Số dư (VNĐ)", "Tổng nạp (VNĐ)", "VIP", "Tên khách"});
                auto r = x->to_csv();

                PersonManager pm(dir + "/person.csv");
                pm.load();

                string cusName;
                if (Person* p = pm.find(r[4]))
                    cusName = p->getFullName() + " (" + r[4] + ")";

                return makerow({r[0], r[1], r[2], r[3], cusName});
            },

            [this]() -> Vec<string> {
                string id = autoIdFor("cus", dir);
                string pid = autoIdFor("per", dir);
                cout << "Tự động tạo CustomerID: " << id << endl;
                cout << "Tự động tạo PersonID: " << pid << endl;

                int balance;
                cout << "Số dư ban đầu: "; cin >> balance;

                Customer c;
                c.setBalance(balance);
                c.settotalDeposit(balance);
                c.upgradeVip();
                string vip = c.getVipLevel();
                cout << "Hạng VIP: " << vip << endl;

                cin.ignore();
                string name;
                cout << "Tên khách hàng: "; getline(cin, name);

                string username = id; 
                username.erase(remove(username.begin(), username.end(), '-'), username.end());
                string pass = "123";

                PersonManager pm(dir + "/person.csv");
                pm.load();
                pm.items.PushBack(Person::from_csv(makerow({pid, name, username, pass, "CUSTOMER"})));
                pm.save();

                okMsg("Đã tạo Person mới cho khách hàng");
                return makerow({id, to_string(balance), to_string(balance), vip, pid});
            },

            [](Customer& x)->Vec<string> {
                auto r = x.to_csv();
                int balance, total;
                cout << "Cập nhật số dư: "; cin >> balance;
                cout << "Cập nhật tổng nạp: "; cin >> total;
                Customer c = Customer::from_csv(r);
                c.setBalance(balance);
                c.settotalDeposit(total);
                c.upgradeVip();
                return makerow({r[0], to_string(c.getBalance()), to_string(c.gettotalDeposit()), c.getVipLevel(), r[4]});
            }
        );

        delete m;
    }

    void menuSession() {
        ManagerBase<Session>* m = new SessionManager(dir + "/session.csv");
        m->load();

        genericCrud("PHIÊN CHƠI", *m,
            [&](Session* x, bool hdr)->Vec<string> {
                if (hdr)
                    return makerow({"SessionID", "Ngày", "Giờ bắt đầu", "Giờ kết thúc", "Tổng (VNĐ)", "Khách hàng", "Máy tính", "Phòng"});

                auto r = x->to_csv();
                if (r[6] != empID) return {}; 

                CustomerManager cm(dir + "/customer.csv"); cm.load();
                ComputerManager cpm(dir + "/computer.csv"); cpm.load();
                RoomManager rm(dir + "/room.csv"); rm.load();
                PersonManager pm(dir + "/person.csv"); pm.load();

                string cusName, compName, roomName;

                if (Customer* c = cm.find(r[5])) {
                    auto cr = c->to_csv();
                    if (Person* p = pm.find(cr[4]))
                        cusName = p->getFullName() + " (" + r[5] + ")";
                }
                if (Computer* c = cpm.find(r[7]))
                    compName = c->getName() + " (" + r[7] + ")";
                if (Room* room = rm.find(r[8]))
                    roomName = room->getName() + " (" + r[8] + ")";

                return makerow({r[0], r[1], r[2], r[3], r[4], cusName, compName, roomName});
            },

            [this]() -> Vec<string> {
                string id = autoIdFor("ses", dir);
                string date = getDate();
                string start = getTime();
                string end = "";
                double total = 0;
                string cusid, compid, roomid;

                cout << "Tự động tạo SessionID: " << id << endl;
                cout << "Ngày (DD-MM-YYYY): " << date << endl;
                cout << "Giờ bắt đầu (HH:MM): " << start << endl;
                cout << "CustomerID: "; cin >> cusid;
                cout << "ComputerID: "; cin >> compid;
                cout << "RoomID: "; cin >> roomid;

                return makerow({id, date, start, end, to_string(total), cusid, empID, compid, roomid});
            },

            [this](Session& x)->Vec<string> {
                auto r = x.to_csv();
                if (r[6] != empID) {
                    errMsg("Bạn không có quyền sửa phiên của người khác!");
                    return r;
                }

                cout << "1. Kết thúc phiên\n2. Chỉnh sửa thông tin\n→ ";
                int c; cin >> c;

                if (c == 1) {
                    if (!r[3].empty()) {
                        errMsg("Phiên đã kết thúc!");
                        return r;
                    }
                    r[3] = getTime();
                    okMsg("Đã ghi giờ kết thúc tự động!");
                } else if (c == 2) {
                    string date, start, cusid, compid, roomid;
                    cout << "Ngày: "; cin >> date;
                    cout << "Giờ bắt đầu: "; cin >> start;
                    cout << "Khách hàng (CustomerID): "; cin >> cusid;
                    cout << "Máy tính (ComputerID): "; cin >> compid;
                    cout << "Phòng (RoomID): "; cin >> roomid;
                    if (!date.empty()) r[1] = date;
                    if (!start.empty()) r[2] = start;
                    if (!cusid.empty()) r[5] = cusid;
                    if (!compid.empty()) r[7] = compid;
                    if (!roomid.empty()) r[8] = roomid;
                } else {
                    errMsg("Lựa chọn không hợp lệ!");
                }

                return r;
            }
        );

        delete m;
    }

    void menuServiceSession() {
        ManagerBase<ServiceSession>* m = new ServiceSessionManager(dir + "/service_session.csv");
        m->load();

        genericCrud("DỊCH VỤ TRONG PHIÊN", *m,
            [&](ServiceSession* x, bool hdr)->Vec<string> {
                if (hdr)
                    return makerow({"SessionID", "Dịch vụ", "Số lượng", "Thành tiền (VNĐ)"});
                auto r = x->to_csv();

                ServiceManager sv(dir + "/service.csv"); sv.load();
                string sname;
                if (Service* s = sv.find(r[1]))
                    sname = s->getName() + " (" + r[1] + ")";
                return makerow({r[0], sname, r[2], r[3]});
            },

            // ➕ Thêm
            [this]() -> Vec<string> {
                string sesid, serid;
                int qty;
                cout << "SessionID: "; cin >> sesid;
                cout << "ServiceID: "; cin >> serid;
                cout << "Số lượng: "; cin >> qty;
                ServiceManager sm(dir + "/service.csv"); sm.load();
                Service* s = sm.find(serid);
                if (!s) { errMsg("Không tìm thấy dịch vụ!"); return {}; }
                int price = stoi(s->to_csv()[2]);
                int total = price * qty;
                return makerow({sesid, serid, to_string(qty), to_string(total)});
            },

            [](ServiceSession& x)->Vec<string> {
                auto r = x.to_csv();
                int qty;
                cout << "Cập nhật số lượng: "; cin >> qty;
                int price = stoi(r[3]) / stoi(r[2]);
                int total = qty * price;
                return makerow({r[0], r[1], to_string(qty), to_string(total)});
            }
        );

        delete m;
    }

    void menuPayment() {
        PaymentManager pay(dir + "/payment.csv"); pay.load();
        SessionManager sm(dir + "/session.csv"); sm.load();
        ServiceSessionManager ssm(dir + "/service_session.csv"); ssm.load();
        ServiceManager sv(dir + "/service.csv"); sv.load();
        RoomManager rm(dir + "/room.csv"); rm.load();
        ComputerManager cm(dir + "/computer.csv"); cm.load();
        PersonManager pm(dir + "/person.csv"); pm.load();
        CustomerManager cusm(dir + "/customer.csv"); cusm.load();
        EmployeeManager em(dir + "/employee.csv"); em.load();

        string sid;
        cout << "Nhập SessionID cần thanh toán: ";
        cin >> sid;

        Session* s = sm.find(sid);
        if (!s) { errMsg("Không tồn tại phiên chơi"); return; }
        if (s->to_csv()[6] != empID) { errMsg("Phiên này không thuộc quyền của bạn"); return; }

        string payid = "pay-" + sid;
        if (pay.find(payid)) { warnMsg("Phiên này đã được thanh toán!"); return; }

        auto sr = s->to_csv();
        string custID = sr[5], roomID = sr[8], compID = sr[7];
        string start = sr[2], end = sr[3];

        int serviceCost = 0;
        for (int i = 0; i < ssm.items.getsize(); i++) {
            auto r = ssm.items[i].to_csv();
            if (r[0] == sid) serviceCost += stoi(r[3]);
        }

        Room* room = rm.find(roomID);
        int rate = room ? stoi(room->to_csv()[4]) : 0;
        int mins = StringToMinutes(end) - StringToMinutes(start);
        if (mins < 0) mins = 0;
        int hourCost = rate * (mins / 60.0);
        int subtotal = hourCost + serviceCost;

        Customer* c = cusm.find(custID);
        if (!c) { errMsg("Không tìm thấy khách hàng"); return; }

        c->upgradeVip();
        int discountPercent = c->getDiscount();
        int discountAmt = subtotal * discountPercent / 100;
        int total = subtotal - discountAmt;

        if (c->getBalance() < total) { errMsg("Số dư không đủ!"); return; }

        c->setBalance(c->getBalance() - total);
        cusm.save();

        string time = getTimeDate();
        Vec<string> pr = makerow({payid, time, to_string(total), sid, empID});
        pay.upsert(payid, Payment::from_csv(pr));
        pay.save();
        s->setTotal(total);
        sm.save();

        string cusName, empName, compName, roomName;
        if (Person* p = pm.find(c->to_csv()[4])) cusName = p->getFullName();
        if (Employee* e = em.find(empID)) {
            auto er = e->to_csv();
            if (Person* p = pm.find(er[3])) empName = p->getFullName();
        }
        if (Computer* cp = cm.find(compID)) compName = cp->getName();
        if (room) roomName = room->getName();

        boxTitle("HÓA ĐƠN THANH TOÁN");
        cout << ansi::BOLD << "Ngày: " << ansi::RESET << time << "\n"
             << "Khách hàng: " << cusName << " (" << custID << ")\n"
             << "Nhân viên: " << empName << " (" << empID << ")\n"
             << "Máy / Phòng: " << compName << " / " << roomName << "\n"
             << "---------------------------------------------\n"
             << "Tiền giờ chơi: " << hourCost << "\n"
             << "Tiền dịch vụ : " << serviceCost << "\n"
             << "Giảm giá (" << discountPercent << "%): -" << discountAmt << "\n"
             << "---------------------------------------------\n"
             << "TỔNG THANH TOÁN: " << ansi::BOLD << total << ansi::RESET << " VNĐ\n"
             << "Số dư còn lại : " << c->getBalance() << " VNĐ\n";
        okMsg("Đã thanh toán thành công!");
    }

    void menuViewPayments() {
        PaymentManager pay(dir + "/payment.csv"); pay.load();
        SessionManager sm(dir + "/session.csv"); sm.load();
        CustomerManager cm(dir + "/customer.csv"); cm.load();
        PersonManager pm(dir + "/person.csv"); pm.load();

        Vec<Vec<string>> table;
        table.PushBack(makerow({"PaymentID", "Ngày", "Tổng tiền", "Phiên", "Khách hàng"}));

        for (int i = 0; i < pay.items.getsize(); i++) {
            auto p = pay.items[i].to_csv();
            if (p[4] != empID) continue;
            Session* s = sm.find(p[3]);
            if (!s) continue;
            string cus;
            if (Customer* c = cm.find(s->to_csv()[5])) {
                auto cr = c->to_csv();
                if (Person* per = pm.find(cr[4])) cus = per->getFullName();
            }
            table.PushBack(makerow({p[0], p[1], p[2], p[3], cus}));
        }
        printTable(table);
    }

    void menuRecharge() {
        CustomerManager cm(dir + "/customer.csv"); cm.load();
        PersonManager pm(dir + "/person.csv"); pm.load();

        string id; int money;
        cout << "CustomerID: "; cin >> id;
        Customer* c = cm.find(id);
        if (!c) { errMsg("Không tìm thấy khách hàng!"); return; }
        cout << "Số tiền nạp: "; cin >> money;

        c->setBalance(c->getBalance() + money);
        c->settotalDeposit(c->gettotalDeposit() + money);
        c->upgradeVip();
        cm.save();

        string name;
        if (Person* p = pm.find(c->to_csv()[4])) name = p->getFullName();
        okMsg("Đã nạp " + to_string(money) + " VNĐ cho " + name);
        cout << "Hạng VIP mới: " << ansi::YELLOW << c->getVipLevel() << ansi::RESET << endl;
    }

    void run() override {
        using namespace ansi;
        while (true) {
            boxTitle("EMPLOYEE MENU");
            cout << BOLD << "1." << RESET << " Quản lý khách hàng\n"
                 << BOLD << "2." << RESET << " Quản lý phiên chơi\n"
                 << BOLD << "3." << RESET << " Quản lý dịch vụ trong phiên\n"
                 << BOLD << "4." << RESET << " Thanh toán phiên chơi\n"
                 << BOLD << "5." << RESET << " Xem danh sách thanh toán\n"
                 << BOLD << "6." << RESET << " Nạp tiền cho khách hàng\n"
                 << BOLD << "0." << RESET << " Đăng xuất\n→ ";
            int c;
            if (!(cin >> c))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                errMsg("Nhập sai, vui lòng nhập số");
                continue;
            }
            if (c == 1) menuCustomer();
            else if (c == 2) menuSession();
            else if (c == 3) menuServiceSession();
            else if (c == 4) menuPayment();
            else if (c == 5) menuViewPayments();
            else if (c == 6) menuRecharge();
            else if (c == 0)
            {
                okMsg("Đăng xuất thành công");
                return;
            }
            else
                errMsg("Lựa chọn không hợp lệ");
        }
    }
};

#endif
