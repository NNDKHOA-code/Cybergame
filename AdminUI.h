#ifndef ADMIN_UI_H
#define ADMIN_UI_H

#include "../core/Ui.h"
#include "../core/Utils.h"
#include "../manager/ServiceManager.h"
#include "../manager/CustomerManager.h"
#include "../manager/RoomManager.h"
#include "../manager/ComputerManager.h"
#include "../manager/EmployeeTypeManager.h"
#include "../manager/EmployeeManager.h"
#include "../manager/SessionManager.h"
#include "../manager/ServiceSessionManager.h"
#include "../manager/PaymentManager.h"
#include "../manager/PersonManager.h"
#include "../manager/FeedbackManager.h"
#include "../ui/MenuBase.h"
#include "../statistics/Statistics.h"
#include <algorithm>

class AdminUI : public MenuBase
{
private:
    string dir;

public:
    AdminUI(const string &dir) : dir(dir) {}

    void menuService()
    {
        ManagerBase<Service> *m = new ServiceManager(dir + "/service.csv");
        m->load();

        genericCrud("QUẢN LÝ DỊCH VỤ", *m, [](Service *x, bool hdr) -> Vec<string>
                    {
            if (hdr)
                return makerow({"ServiceID", "Tên dịch vụ", "Giá (VNĐ)", "Số lượng", "Trạng thái"});
            return x->to_csv(); },

                    [this]() -> Vec<string>
                    {
            string id = autoIdFor("ser", dir);
            cout << "Tự động tạo ServiceID: " << id << endl;

            string name, status = "Còn hàng";
            int price, qty;

            cin.ignore();
            cout << "Tên dịch vụ: "; getline(cin, name);
            cout << "Giá: "; cin >> price;
            cout << "Số lượng: "; cin >> qty;

            if (qty <= 0) status = "Hết hàng";

            return makerow({id, name, to_string(price), to_string(qty), status}); },

                    [](Service &x) -> Vec<string>
                    {
            auto r = x.to_csv();
            string name, status;
            int price, qty;

            cin.ignore();
            cout << "Cập nhật tên: ";
            getline(cin, name);
            name = r[1];

            cout << "Cập nhật giá: ";
            cin >> price;
            price = stoi(r[2]);

            cout << "Cập nhật số lượng: ";
            cin >> qty;
            if (qty < 0) qty = stoi(r[3]);

            if (qty == 0)
                status = "Hết hàng";
            else
                status = "Còn hàng";

            return makerow({r[0], name, to_string(price), to_string(qty), status}); });

        delete m;
    }

    void menuCustomer()
    {
        ManagerBase<Customer> *m = new CustomerManager(dir + "/customer.csv");
        m->load();

        genericCrud("KHÁCH HÀNG", *m, [&](Customer *x, bool hdr) -> Vec<string>
                    {
                if (hdr)
                    return makerow({"CustomerID", "Số dư (VNĐ)", "Số tiền nạp tích lũy (VNĐ)","Hạng VIP", "Tên khách"});
                auto r = x->to_csv();

                PersonManager pm(dir + "/person.csv"); pm.load();

                string cusName;
                if (Person* p = pm.find(r[4]))
                    cusName = p->getFullName() + " (" + r[4] + ")";
                
                return makerow({r[0], r[1], r[2], r[3], cusName}); }, [this]() -> Vec<string>
                    {
                string id = autoIdFor("cus", dir);
                string pid = autoIdFor("per", dir);
                cout << "Tự động tạo CustomerID: " << id << endl;
                cout << "Tự động tạo PersonID: " << pid << endl;

                int balance, totalDeposit; 
                string vip, name;
                string username = id;

                username.erase(remove(username.begin(), username.end(), '-'), username.end());
                string pass = "123";
                cout << "Số dư: "; cin >> balance;
                totalDeposit = balance;
                cout << "Số tiền nạp tích lúy: " << totalDeposit;
                Customer* c = new Customer();
                c->settotalDeposit(totalDeposit);
                c->upgradeVip();
                vip = c->getVipLevel();
                cout << "Hạng VIP: " << vip << endl;
                cin.ignore();
                cout << "Tên khách hàng: "; getline(cin, name);
                cout << "Tên đăng nhập: " << username << endl;
                cout << "Mật khẩu: " << pass << endl;

                PersonManager pm(dir + "/person.csv");
                pm.load();
                pm.items.PushBack(Person::from_csv(makerow({pid, name, username, pass, "CUSTOMER"})));
                pm.save();

                okMsg("Đã tạo Person mới cho khách hàng");
                return makerow({id, to_string(balance), vip, pid}); }, [](Customer &x) -> Vec<string>
                    {
                auto r = x.to_csv();
                int balance, totalDeposit; 
                string vip;
                cout << "Cập nhật số dư: "; cin >> balance;
                cout << "Cập nhật số tiền tích lũy: "; cin >> totalDeposit;
                cout << "Cập nhật hạng VIP: "; cin >> vip;
                return makerow({r[0], to_string(balance), to_string(totalDeposit), vip, r[3]}); });

        delete m;
    }

    void menuRoom()
    {
        ManagerBase<Room> *m = new RoomManager(dir + "/room.csv");
        m->load();

        genericCrud("PHÒNG MÁY", *m, [](Room *x, bool hdr) -> Vec<string>
                    {
                if (hdr)
                    return makerow({"RoomID", "Tên phòng", "Loại", "Số lượng máy", "Giá/giờ (VNĐ)", "Trạng thái"});
                return x->to_csv(); }, [this]() -> Vec<string>
                    {
                string id = autoIdFor("room", dir);
                cout << "Tự động tạo RoomID: " << id << endl;
                string name, type; int cap, price;
                cin.ignore();
                cout << "Tên phòng: "; getline(cin, name);
                cout << "Loại phòng: "; getline(cin, type);
                cout << "Sức chứa: "; cin >> cap;
                cout << "Giá/giờ: "; cin >> price;
                return makerow({id, name, type, to_string(cap), to_string(price), "Đang mở"}); }, [](Room &x) -> Vec<string>
                    {
                auto r = x.to_csv();
                string name, type; int cap, price;
                cin.ignore();
                cout << "Cập nhật tên phòng: "; getline(cin, name);
                cout << "Cập nhật loại phòng: "; getline(cin, type);
                cout << "Cập nhật sức chứa: "; cin >> cap;
                cout << "Cập nhật giá/giờ: "; cin >> price;
                return makerow({r[0], type, to_string(cap), to_string(price)}); });

        delete m;
    }

    void menuComputer()
    {
        ManagerBase<Computer> *m = new ComputerManager(dir + "/computer.csv");
        m->load();

        genericCrud("MÁY TÍNH", *m, [&](Computer *x, bool hdr) -> Vec<string>
                    {
                if (hdr)
                    return makerow({"ComputerID", "Tên máy", "Cấu hình", "Trạng thái", "Phòng"});
                auto r = x->to_csv();

                RoomManager rm(dir + "/room.csv"); rm.load();
                string roomName;
                if (Room* room = rm.find(r[4]))
                    roomName = room->getName();

                return makerow({r[0], r[1], r[2], r[3], roomName}); }, [this]() -> Vec<string>
                    {
                string id = autoIdFor("pc", dir);
                cout << "Tự động tạo ComputerID: " << id << endl;
                string name, conf, room;
                cin.ignore();
                cout << "Tên máy: "; getline(cin, name);
                cout << "Cấu hình: "; getline(cin, conf);
                cout << "Phòng (RoomID): "; getline(cin, room);
                return makerow({id, name, conf, "Đang trống", room}); }, [](Computer &x) -> Vec<string>
                    {
                auto r = x.to_csv();
                string name, conf, status, room;
                cin.ignore();
                cout << "Cập nhật tên máy: "; getline(cin, name);
                cout << "Cập nhật cấu hình: "; getline(cin, conf);
                cout << "Cập nhật trạng thái: "; getline(cin, status);
                cout << "Cập nhật phòng (RoomID): "; getline(cin, room);
                return makerow({r[0], name, conf, status, room}); });

        delete m;
    }

    void menuEmployeeType()
    {
        ManagerBase<EmployeeType> *m = new EmployeeTypeManager(dir + "/employee_type.csv");
        m->load();

        genericCrud("CHỨC VỤ NHÂN VIÊN", *m, [](EmployeeType *x, bool hdr) -> Vec<string>
                    {
                if (hdr)
                    return makerow({"TypeID", "Tên chức vụ", "Ca", "Giờ bắt đầu","Giờ kết thúc","Lương cơ bản/giờ (VNĐ)"});
                return x->to_csv(); }, [this]() -> Vec<string>
                    {
                string id = autoIdFor("emt", dir);
                cout << "Tự động tạo TypeID: " << id << endl;
                string name, shift, start, end; 
                int base;
                cin.ignore();
                cout << "Tên chức vụ: "; getline(cin, name);
                cout << "Ca: "; getline(cin, shift);
                cout << "Giờ ắt đầu: "; getline(cin, start);
                cout << "Giờ kết thúc: "; getline(cin, end);
                cout << "Lương cơ bản: "; cin >> base;
                return makerow({id, name, shift, to_string(base)}); }, [](EmployeeType &x) -> Vec<string>
                    {
                auto r = x.to_csv();
                string name, shift, start, end; 
                int base;
                cin.ignore();
                cout << "Cập nhật tên chức vụ: "; getline(cin, name);
                cout << "Cập nhật ca: "; getline(cin, shift);
                cout << "Cập nhật giờ bắt đầu: "; getline(cin, start);
                cout << "Cập nhật giờ kết thúc: "; getline(cin, end);
                cout << "Cập nhật lương cơ bản/giờ: "; cin >> base;
                return makerow({r[0], name, shift, to_string(base)}); });

        delete m;
    }

    void menuEmployee()
    {
        ManagerBase<Employee> *m = new EmployeeManager(dir + "/employee.csv");
        m->load();

        genericCrud("NHÂN VIÊN", *m, [&](Employee *x, bool hdr) -> Vec<string>
                    {
            if (hdr)
                return makerow({"EmployeeID", "Ngày thuê", "Lương (VNĐ)", "Tên nhân viên", "Chức vụ / Ca"});

            auto r = x->to_csv();
            PersonManager pm(dir + "/person.csv"); pm.load();
            EmployeeTypeManager etm(dir + "/employee_type.csv"); etm.load();

            string personName, typeInfo;

            if (Person *p = pm.find(r[3]))
                personName = p->getFullName() + " (" + r[3] + ")";

            if (EmployeeType *t = etm.find(r[5]))
                typeInfo = t->getName() + " - " + t->getShift() +
                           " (" + t->getStart() + "-" + t->getEnd() + ")";

            return makerow({r[0], r[1], r[2], personName, typeInfo}); }, [this]() -> Vec<string>
                    {
                string id = autoIdFor("emp", dir);
                string pid = autoIdFor("per", dir);
                cout << "Tự động tạo EmployeeID: " << id << endl;
                cout << "Tự động tạo PersonID: " << pid << endl;
                string hire, tid, name, username, pass;
                int sal;
                cout << "Ngày thuê (YYYY-MM-DD): "; cin >> hire;
                cout << "Lương: "; cin >> sal;
                cout << "TypeID: "; cin >> tid;
                cin.ignore();
                cout << "Tên nhân viên: "; getline(cin, name);
                cout << "Tên đăng nhập: "; getline(cin, username);
                cout << "Mật khẩu: "; getline(cin, pass);

                EmployeeTypeManager etm(dir + "/employee_type.csv");
                etm.load();
                EmployeeType *t = etm.find(tid);

                if (!t)
                {
                    errMsg("Không tìm thấy loại nhân viên");
                }
                else
                {
                    sal = t->getBaseSalary() * 8; 
                    okMsg("Lương cơ bản/ giờ: " + to_string((int)sal) + " VNĐ");
                    cout << "Chức vụ: " << t->getName() << " | "
                        << t->getShift() << " (" << t->getStart()
                        << "-" << t->getEnd() << ")" << endl;
                }

                PersonManager pm(dir + "/person.csv");
                pm.load();
                pm.items.PushBack(Person::from_csv(makerow({pid, name, username, pass, "EMPLOYEE"})));
                pm.save();

                okMsg("Đã tạo Person mới cho nhân viên");
                return makerow({id, hire, to_string(sal), pid, "adm", tid}); }, [](Employee &x) -> Vec<string>
                    {
                auto r = x.to_csv();
                string hire, tid;
                int sal;
                cout << "Cập nhật ngày thuê (DD-MM-YYYY): "; cin >> hire;
                cout << "Cập nhật lương: "; cin >> sal;
                cout << "Cập nhật chức vụ (TypeID): "; cin >> tid;
                return makerow({r[0], hire, to_string(sal), r[3], r[4], tid}); });

        delete m;
    }

    void menuSession()
    {
        ManagerBase<Session> *m = new SessionManager(dir + "/session.csv");
        m->load();

        genericCrud("PHIÊN CHƠI", *m, [&](Session *x, bool hdr) -> Vec<string>
                    {
            if (hdr)
                return makerow({"SessionID", "Ngày", "Giờ bắt đầu", "Giờ kết thúc", "Tổng tiền (VNĐ)", "Khách hàng", "Nhân viên", "Máy tính", "Phòng"});

            auto r = x->to_csv(); 

            CustomerManager cm(dir + "/customer.csv"); cm.load();
            EmployeeManager em(dir + "/employee.csv"); em.load();
            ComputerManager cpm(dir + "/computer.csv"); cpm.load();
            RoomManager rm(dir + "/room.csv"); rm.load();
            PersonManager pm(dir + "/person.csv"); pm.load();

            string cusName, empName, compName, roomName;

            if (Customer* c = cm.find(r[5])) {
                auto cr = c->to_csv();
                if (Person* p = pm.find(cr[4]))
                    cusName = p->getFullName() + " (" + r[5] + ")";
            }

            if (Employee* e = em.find(r[6])) {
                auto er = e->to_csv();
                if (Person* pe = pm.find(er[3]))
                    empName = pe->getFullName() + " (" + r[6] + ")";
            }

            if (Computer* pc = cpm.find(r[7]))
                compName = pc->getName() + " (" + r[7] + ")";

            if (Room* room = rm.find(r[8]))
                roomName = room->getName() + " (" + r[8] + ")";

            return makerow({r[0], r[1], r[2], r[3], r[4], cusName, empName, compName, roomName}); }, [this]() -> Vec<string>
                    {
            string id = autoIdFor("ses", dir);
            cout << "Tự động tạo SessionID: " << id << endl;

            string date = getDate(); 
            string start = getTime();
            string end = "", cusid, empid, compid, roomid;
            double total = 0;
            cin.ignore();
            cout << "Ngày (DD-MM-YYYY): " << date << endl;
            cout << "Giờ bắt đầu (HH:MM): " << start << endl;
            cout << "Giờ kết thúc (HH:MM): " << end << endl;
            cout << "Khách hàng (CustomerID): "; getline(cin, cusid);
            cout << "Nhân viên (EmployeeID): "; getline(cin, empid);
            cout << "Máy tính (ComputerID): "; getline(cin, compid);
            cout << "Phòng (RoomID): "; getline(cin, roomid);

            return makerow({id, date, start, end, to_string(total), cusid, empid, compid, roomid}); }, [](Session &x) -> Vec<string>
                    {
                        auto r = x.to_csv();

                        cout << "1. Kết thúc phiên\n";
                        cout << "2. Chỉnh sửa phiên\n";
                        cout << "Nhập lựa chọn: ";
                        int c;
                        cin >> c;
                        cin.ignore();
                        if (c == 1)
                        {
                            if (stoi(r[4]))
                            {
                                errMsg("Phiên này đã kết thúc trước đó");
                                return r;
                            }
                            string end = getTime();
                            okMsg("Phiên chơi kết thúc - đã ghi giờ kết thúc tự động");
                            r[3] = end;
                        }
                        else if (c == 2)
                        {
                            string date, start, cusid, empid, compid, roomid;
                            cout << "Cập nhật ngày (DD-MM-YYYY): " << date << endl;
                            cout << "Cập nhật giờ bắt đầu (HH:MM): " << start << endl;
                            cout << "Cập nhật khách hàng (CustomerID): ";
                            getline(cin, cusid);
                            cout << "Cập nhật nhân viên (EmployeeID): ";
                            getline(cin, empid);
                            cout << "Cập nhật máy tính (ComputerID): ";
                            getline(cin, compid);
                            cout << "Cập nhật phòng (RoomID): ";
                            getline(cin, roomid);
                        } else 
                            errMsg("Lựa chọn không hợp lệ");
                        return r; });

        delete m;
    }

    void menuServiceSession()
    {
        ManagerBase<ServiceSession> *m = new ServiceSessionManager(dir + "/service_session.csv");
        m->load();

        genericCrud("DỊCH VỤ SỬ DỤNG TRONG PHIÊN)", *m, [&](ServiceSession *x, bool hdr) -> Vec<string>
                    {
            if (hdr)
                return makerow({"SessionID", "Dịch vụ", "Số lượng", "Thành tiền (VNĐ)"});

            auto r = x->to_csv(); 
            ServiceManager sv(dir + "/service.csv"); sv.load();

            string serName;
            if (Service* s = sv.find(r[1]))
                serName = s->getName() + " (" + r[1] + ")";

            return makerow({r[0], serName, r[2], r[3]}); }, [this]() -> Vec<string>
                    {
            string sesid, serid;
            int qty;
            cin.ignore();
            cout << "SessionID: "; getline(cin, sesid);
            cout << "ServiceID: "; getline(cin, serid);
            cout << "Số lượng: "; cin >> qty;

            ServiceManager sm(dir + "/service.csv");
            sm.load();
            Service* s = sm.find(serid);
            if (!s) { errMsg("Service không tồn tại"); return {}; }

            int price = stoi(s->to_csv()[2]);
            int total = price * qty;
            return makerow({sesid, serid, to_string(qty), to_string(total)}); }, [](ServiceSession &x) -> Vec<string>
                    {
            auto r = x.to_csv();
            int qty;
            cout << "Cập nhật số lượng: "; cin >> qty;
            int oldQty = stoi(r[2]);
            int price = stoi(r[3]) / oldQty;
            int total = qty * price;
            return makerow({r[0], r[1], to_string(qty), to_string(total)}); });

        delete m;
    }

    void menuPayment()
    {
        ManagerBase<Payment> *m = new PaymentManager(dir + "/payment.csv");
        m->load();

        genericCrud("THANH TOÁN", *m, [&](Payment *x, bool hdr) -> Vec<string>
                    {
            if (hdr)
                return makerow({"PaymentID", "Ngày giờ thanh toán", "Tổng tiền (VNĐ)", "Phiên chơi", "Khách hàng", "Nhân viên"});

            auto r = x->to_csv(); 

            SessionManager sm(dir + "/session.csv"); sm.load();
            CustomerManager cm(dir + "/customer.csv"); cm.load();
            EmployeeManager em(dir + "/employee.csv"); em.load();
            PersonManager pm(dir + "/person.csv"); pm.load();

            string sessionID = r[3];
            string cusName, empName;

            if (Session* s = sm.find(sessionID)) {
                auto sr = s->to_csv();
                string custID = sr[5];
                if (Customer* c = cm.find(custID)) {
                    auto cr = c->to_csv();
                    if (Person* p = pm.find(cr[3]))
                        cusName = p->getFullName() + " (" + custID + ")";
                }
            }

            if (Employee* e = em.find(r[4])) {
                auto er = e->to_csv();
                if (Person* pe = pm.find(er[3]))
                    empName = pe->getFullName() + " (" + r[4] + ")";
            }

            return makerow({r[0], r[1], r[2], sessionID, cusName, empName}); }, [this]() -> Vec<string>
                    {
            string sesid, date, empid;
            double total;
            cin.ignore();
            cout << "SessionID: "; getline(cin, sesid);

            string id = "pay-" + sesid;
            cout << "Tự động tạo PaymentID: " << id << endl;

            cout << "Ngày giờ (DD-MM-YYYY hh:mm): "; getline(cin, date);
            cout << "Tổng tiền: "; cin >> total;
            cout << "Nhân viên (EmployeeID): "; cin >> empid;

            return makerow({id, date, to_string(total), sesid, empid}); }, [](Payment &x) -> Vec<string>
                    {
            auto r = x.to_csv();
            string date, empid;
            double total;
            cin.ignore();
            cout << "Cập nhật ngày giờ (DD-MM-YYYY hh:mm): "; getline(cin, date);
            cout << "Cập nhật tổng tiền: "; cin >> total;
            cout << "Cập nhật nhân viên (EmployeeID): "; cin >> empid;
            return makerow({r[0], date, to_string(total), r[3], empid}); });

        delete m;
    }

    void adminStatistics()
    {
        using namespace ansi;
        while (true)
        {
            boxTitle("THỐNG KÊ DOANH THU");
            cout << BOLD << "1." << RESET << " Theo Dịch vụ\n"
                 << BOLD << "2." << RESET << " Theo Khách hàng\n"
                 << BOLD << "3." << RESET << " Theo Phòng\n"
                 << BOLD << "4." << RESET << " Theo Máy tính\n"
                 << BOLD << "5." << RESET << " Theo Nhân viên\n"
                 << BOLD << "0." << RESET << " Quay lại\n→ ";
            int c;
            if (!(cin >> c))
                return;
            if (c == 0)
                return;
            if (c == 1)
                Statistics::byService(dir);
            else if (c == 2)
                Statistics::byCustomer(dir);
            else if (c == 3)
                Statistics::byRoom(dir);
            else if (c == 4)
                Statistics::byComputer(dir);
            else if (c == 5)
                Statistics::byEmployee(dir);
        }
    }

    void menuFeedback()
    {
        FeedbackManager fm(dir + "/feedback.csv");
        fm.load();
        CustomerManager cm(dir + "/customer.csv");
        cm.load();
        PersonManager pm(dir + "/person.csv");
        pm.load();

        Vec<Vec<string>> table;
        table.PushBack(makerow({"FeedbackID", "Khách hàng", "Ngày giờ", "Nội dung"}));

        for (int i = 0; i < fm.items.getsize(); i++)
        {
            auto r = fm.items[i].to_csv();
            string cusName;
            if (Customer *c = cm.find(r[1]))
            {
                auto col = c->to_csv();
                if (Person *p = pm.find(col[4]))
                {
                    cusName = p->getFullName() + " (" + col[4] + ")";
                }
            }
            table.PushBack(makerow({r[0], cusName, r[2], r[3]}));
        }

        boxTitle("DANH SÁCH PHẢN HỒI KHÁCH HÀNG");
        printTable(table);
    }

    void run()
    {
        using namespace ansi;
        while (true)
        {
            boxTitle("MENU QUẢN TRỊ VIÊN");
            cout << BOLD << "1." << RESET << " Quản lý dịch vụ\n"
                 << BOLD << "2." << RESET << " Quản lý khách hàng\n"
                 << BOLD << "3." << RESET << " Quản lý phòng máy\n"
                 << BOLD << "4." << RESET << " Quản lý máy tính\n"
                 << BOLD << "5." << RESET << " Quản lý loại nhân viên\n"
                 << BOLD << "6." << RESET << " Quản lý nhân viên\n"
                 << BOLD << "7." << RESET << " Quản lý phiên chơi\n"
                 << BOLD << "8." << RESET << " Quản lý dịch vụ sử dụng\n"
                 << BOLD << "9." << RESET << " Quản lý thanh toán\n"
                 << BOLD << "10." << RESET << " Thống kê & Báo cáo\n"
                 << BOLD << "11." << RESET << " Xem phản hồi khách hàng\n"
                 << BOLD << "0." << RESET << " Đăng xuất\n→ ";

            int c;
            if (!(cin >> c))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                errMsg("Nhập sai, vui lòng nhập số");
                continue;
            }

            if (c == 1)
                menuService();
            else if (c == 2)
                menuCustomer();
            else if (c == 3)
                menuRoom();
            else if (c == 4)
                menuComputer();
            else if (c == 5)
                menuEmployeeType();
            else if (c == 6)
                menuEmployee();
            else if (c == 7)
                menuSession();
            else if (c == 8)
                menuServiceSession();
            else if (c == 9)
                menuPayment();
            else if (c == 10)
                adminStatistics();
            else if (c == 11)
                menuFeedback();
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
