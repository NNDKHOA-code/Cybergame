#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <filesystem>

#include "core/Ui.h"
#include "core/Csv.h"
#include "core/Utils.h"

#include "model/Person.h"
#include "model/Customer.h"
#include "model/Service.h"
#include "model/Room.h"
#include "model/Computer.h"
#include "model/EmployeeType.h"
#include "model/Employee.h"
#include "model/Session.h"
#include "model/ServiceSession.h"
#include "model/Payment.h"

#include "manager/PersonManager.h"
#include "manager/CustomerManager.h"
#include "manager/ServiceManager.h"
#include "manager/RoomManager.h"
#include "manager/ComputerManager.h"
#include "manager/EmployeeTypeManager.h"
#include "manager/EmployeeManager.h"
#include "manager/SessionManager.h"
#include "manager/ServiceSessionManager.h"
#include "manager/PaymentManager.h"

#include "ui/MenuBase.h"
#include "ui/AdminUI.h"
#include "ui/EmployeeUI.h"
#include "ui/CustomerUI.h"

#include "seed/seed.h"

#include "statistics/Statistics.h"

using namespace std;

struct AuthResult
{
    bool ok = false;
    string role;
    bool exit = false;
    string personID;
};

static AuthResult login(PersonManager &pm)
{
    using namespace ansi;
    boxTitle("ĐĂNG NHẬP HỆ THỐNG");
    okMsg("Nhấn phím q kết thúc chương trình");
    string u;
    string p;
    cout << ansi::BOLD << "Tài khoản" << ansi::RESET << ": ";
    cin >> u;

    if (u == "q")
    {
        cout << YELLOW << "\nĐã chọn thoát chương trình\n"
             << RESET;
        AuthResult r;
        r.exit = true;
        return r;
    }

    cout << ansi::BOLD << "Mật khẩu" << ansi::RESET << ": ";
    cin >> p;

    Person *per = pm.findByUsername(u);
    if (per && per->getPassword() == p)
    {
        okMsg("Đăng nhập thành công - Vai trò " + per->getRole());
        AuthResult r;
        r.ok = true;
        r.role = per->getRole();
        r.personID = per->getID();
        return r;
    }

    errMsg("Sai thông tin đăng nhập");
    cin.clear();
    cin.ignore(10000, '\n');
    return AuthResult();
}

int main()
{

    ansi::enable();
    string dataDir = "data";

    cout
        << ansi::BOLD << ansi::GREEN
        << "\n╔════════════════════════════════════════════╗\n"
        << "║          CYBERGAME MANAGEMENT SYSTEM       ║\n"
        << "╠════════════════════════════════════════════╣\n"
        << "║   Developed by:                            ║\n"
        << "║   Nguyễn Ngọc Đăng Khoa & Trịnh Quốc Khánh ║\n"
        << "╚════════════════════════════════════════════╝\n"
        << ansi::RESET;

    ensureSeed(dataDir);

    while (true)
    {
        PersonManager pm(dataDir + "/person.csv");
        pm.load();
        AuthResult ar = login(pm);

        if (ar.exit)
            return 0;

        if (!ar.ok)
            continue;

        MenuBase *ui = nullptr;

        if (ar.role == "ADMIN")
        {
            ui = new AdminUI(dataDir);
        }
        else if (ar.role == "EMPLOYEE")
        {
            EmployeeManager em(dataDir + "/employee.csv");
            em.load();

            string empID = "";
            string typeID;
            for (int i = 0; i < em.items.getsize(); i++)
            {
                auto r = em.items[i].to_csv();
                if (r[3] == ar.personID)
                {
                    empID = r[0];
                    typeID = r[5];
                    break;
                }
            }

            if (empID == "")
            {
                cout << ansi::RED << "Không tìm thấy Employee tương ứng\n"
                     << ansi::RESET;
                continue;
            }

            EmployeeTypeManager etm(dataDir + "/employee_type.csv");
            etm.load();

            string typeName = "";
            for (int i = 0; i < etm.items.getsize(); i++)
            {
                auto r = etm.items[i].to_csv();
                if (r[0] == typeID)
                {
                    typeName = r[1];
                    break;
                }
            }

            if (typeName != "Thu Ngân")
            {
                errMsg("Chỉ nhân viên [THU NGÂN] được phép đăng nhập vào giao diện nhân viên");
                continue;
            }

            ui = new EmployeeUI(dataDir, empID);
        }
        else if (ar.role == "CUSTOMER")
        {
            CustomerManager cm(dataDir + "/customer.csv");
            cm.load();

            string cusID = "";
            for (int i = 0; i < cm.items.getsize(); ++i)
            {
                auto r = cm.items[i].to_csv();
                if (r[4] == ar.personID)
                {
                    cusID = r[0];
                    break;
                }
            }

            if (cusID.empty())
            {
                errMsg("Không tìm thấy thông tin khách hàng");
            }
            else
            {
                ui = new CustomerUI(dataDir, cusID);
            }
        }

        if (ui)
        {
            ui->run();
            delete ui;
        }
    }
    cout << ansi::DIM << "\nTạm biệt!\n"
         << ansi::RESET;
    return 0;
}
