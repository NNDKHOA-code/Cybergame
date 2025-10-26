#ifndef CUSTOMER_UI_H
#define CUSTOMER_UI_H

#include "../core/Ui.h"
#include "../core/Utils.h"
#include "../ui/MenuBase.h"

#include "../manager/SessionManager.h"
#include "../manager/ServiceSessionManager.h"
#include "../manager/ServiceManager.h"
#include "../manager/PaymentManager.h"
#include "../manager/RoomManager.h"
#include "../manager/ComputerManager.h"
#include "../manager/CustomerManager.h"
#include "../manager/PersonManager.h"

#include <iomanip>
using namespace std;

class CustomerUI : public MenuBase
{
private:
    string dir;
    string cusID;

public:
    CustomerUI(const string &d, const string &cid)
        : dir(d), cusID(cid) {}

    void menuProfile()
    {
        using namespace ansi;
        CustomerManager cm(dir + "/customer.csv");
        cm.load();
        PersonManager pm(dir + "/person.csv");
        pm.load();

        Customer *c = cm.find(cusID);
        if (!c)
        {
            errMsg("Không tìm thấy tài khoản khách hàng");
            return;
        }

        auto r = c->to_csv();

        Person *p = pm.find(r[4]);
        string name = p->getFullName();

        boxTitle("THÔNG TIN CÁ NHÂN");
        cout << YELLOW << "Họ tên: " << YELLOW << name << "\n"
             << YELLOW << "CustomerID: " << YELLOW << r[0] << "\n"
             << YELLOW << "Số dư: " << YELLOW << r[1] << YELLOW << " VNĐ\n"
             << YELLOW << "Số tiền tích lũy: " << YELLOW << r[2] << YELLOW << " VNĐ\n"
             << YELLOW << "Hạng VIP: " << YELLOW << r[3] << "\n";
    }

    void menuChangePassword()
    {
        CustomerManager cm(dir + "/customer.csv");
        cm.load();
        PersonManager pm(dir + "/person.csv");
        pm.load();

        Customer *c = cm.find(cusID);
        string pid = c->to_csv()[4];
        Person *p = pm.find(pid);

        string oldpass, newpass;
        cout << "Nhập mật khẩu cũ: ";
        cin.ignore();
        getline(cin, oldpass);
        if (p->getPassword() != oldpass)
        {
            errMsg("Sai nhật khẩu");
            return;
        }
        cout << "Nhập mật khẩu mới: ";
        getline(cin, newpass);
        p->setPassword(newpass);
        pm.save();
        okMsg("Đổi mật khẩu thành công");
    }

    void menuSessionHistory()
    {
        SessionManager sm(dir + "/session.csv");
        sm.load();
        CustomerManager cm(dir + "/customer.csv");
        cm.load();
        EmployeeManager em(dir + "/employee.csv");
        em.load();
        ComputerManager cpm(dir + "/computer.csv");
        cpm.load();
        RoomManager rm(dir + "/room.csv");
        rm.load();
        PersonManager pm(dir + "/person.csv");
        pm.load();

        Vec<Vec<string>> table;
        table.PushBack(makerow({"SessionID", "Ngày", "Giờ bắt đầu", "Giờ kết thúc", "Tổng tiền (VNĐ)", "Nhân viên", "Máy tính", "Phòng"}));

        for (int i = 0; i < sm.items.getsize(); i++)
        {
            auto r = sm.items[i].to_csv();
            if (r[5] != cusID)
                continue;

            string empName = "(?)", compName, roomName;

            if (Employee *e = em.find(r[6]))
            {
                auto er = e->to_csv();
                if (Person *pe = pm.find(er[3]))
                    empName = pe->getFullName() + " (" + r[6] + ")";
            }

            if (Computer *pc = cpm.find(r[7]))
                compName = pc->getName() + " (" + r[7] + ")";

            if (Room *room = rm.find(r[8]))
                roomName = room->getName() + " (" + r[8] + ")";

            table.PushBack(makerow({r[0], r[1], r[2], r[3], r[4], empName, compName, roomName}));
        }

        boxTitle("LỊCH SỬ PHIÊN CHƠI CỦA BẠN");
        printTable(table);
    }

    void menuUsedServices()
    {
        ServiceSessionManager ssm(dir + "/service_session.csv");
        ssm.load();
        SessionManager sm(dir + "/session.csv");
        sm.load();
        ServiceManager sv(dir + "/service.csv");
        sv.load();

        Vec<Vec<string>> table;
        table.PushBack(makerow({"SessionID", "Tên dịch vụ", "Số lượng", "Thành tiền (VNĐ)"}));

        for (int i = 0; i < ssm.items.getsize(); i++)
        {
            auto r = ssm.items[i].to_csv();
            Session *s = sm.find(r[0]);
            if (!s || s->to_csv()[5] != cusID)
                continue;

            string sname;
            if (Service *svObj = sv.find(r[1]))
                sname = svObj->getName();
            table.PushBack(makerow({r[0], sname, r[2], r[3]}));
        }

        boxTitle("DỊCH VỤ BẠN ĐÃ SỬ DỤNG");
        printTable(table);
    }

    void menuPaymentHistory()
    {
        PaymentManager pmn(dir + "/payment.csv");
        pmn.load();
        SessionManager sm(dir + "/session.csv");
        sm.load();
        EmployeeManager em(dir + "/employee.csv");
        em.load();
        PersonManager pm(dir + "/person.csv");
        pm.load();

        Vec<Vec<string>> table;
        table.PushBack(makerow({"PaymentID", "Ngày giờ thanh toán", "Tổng tiền (VNĐ)", "Phiên chơi", "Nhân viên xử lý"}));

        for (int i = 0; i < pmn.items.getsize(); i++)
        {
            auto r = pmn.items[i].to_csv();

            Session *s = sm.find(r[3]);
            if (!s)
                continue;
            auto sr = s->to_csv();

            if (sr[5] != cusID)
                continue;

            string empName;
            if (Employee *e = em.find(r[4]))
            {
                auto er = e->to_csv();
                if (Person *pe = pm.find(er[3]))
                    empName = pe->getFullName() + " (" + r[4] + ")";
            }

            table.PushBack(makerow({r[0], r[1], r[2], sr[0], empName}));
        }

        boxTitle("LỊCH SỬ THANH TOÁN CỦA BẠN");
        printTable(table);
    }

    void menuFeedback()
    {
        FeedbackManager fm(dir + "/feedback.csv");
        fm.load();

        cin.ignore();
        string content;
        cout << "Nhập nội dung phản hồi/góp ý: ";
        getline(cin, content);

        string fid = autoIdFor("fb", dir);
        string date = getTimeDate();

        fm.items.PushBack(Feedback::from_csv(makerow({fid, cusID, date, content})));
        fm.save();

        okMsg("Cảm ơn bạn đã gửi phản hồi");
    }

    void run() override
    {
        using namespace ansi;
        while (true)
        {
            boxTitle("MENU KHÁCH HÀNG");
            cout << BOLD << "1." << RESET << " Xem thông tin cá nhân\n";
            cout << BOLD << "2." << RESET << " Đổi mật khẩu\n";
            cout << BOLD << "3." << RESET << " Lịch sử phiên chơi\n";
            cout << BOLD << "4." << RESET << " Dịch vụ đã sử dụng\n";
            cout << BOLD << "5." << RESET << " Lịch sử thanh toán\n";
            cout << BOLD << "6." << RESET << " Gửi phản hồi / góp ý\n";
            cout << BOLD << "0." << RESET << " Đăng xuất\n";
            cout << "Nhập lựa chọn: ";

            int c;
            if (!(cin >> c))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                errMsg("Vui lòng nhập số");
                continue;
            }

            if (c == 1)
                menuProfile();
            else if (c == 2)
                menuChangePassword();
            else if (c == 3)
                menuSessionHistory();
            else if (c == 4)
                menuUsedServices();
            else if (c == 5)
                menuPaymentHistory();
            else if (c == 6)
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
