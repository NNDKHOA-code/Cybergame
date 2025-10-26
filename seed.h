#ifndef SEED_H
#define SEED_H

#include <filesystem>
#include "../core/Utils.h"
#include "../manager/PersonManager.h"
#include "../manager/CustomerManager.h"
#include "../manager/ServiceManager.h"
#include "../manager/RoomManager.h"
#include "../manager/ComputerManager.h"
#include "../manager/EmployeeTypeManager.h"
#include "../manager/EmployeeManager.h"
#include "../manager/SessionManager.h"
#include "../manager/ServiceSessionManager.h"
#include "../manager/PaymentManager.h"
#include "../manager/FeedbackManager.h"

using namespace std;

static void ensureSeed(const string &dir)
{
    filesystem::create_directories(dir);

    ifstream check(dir + "/person.csv");
    if (check && check.peek() != EOF)
        return;

    PersonManager pm(dir + "/person.csv");
    pm.items.PushBack(Person::from_csv(makerow({"per-1", "Nguyễn Ngọc Đăng Khoa", "admin", "123", "ADMIN"})));
    pm.items.PushBack(Person::from_csv(makerow({"per-2", "Trịnh Quốc Khánh", "emp1", "123", "EMPLOYEE"})));
    pm.items.PushBack(Person::from_csv(makerow({"per-3", "Dương Danh Quốc", "emp2", "123", "EMPLOYEE"})));
    pm.items.PushBack(Person::from_csv(makerow({"per-4", "Huỳnh Văn Gia Huy", "emp3", "123", "EMPLOYEE"})));
    pm.items.PushBack(Person::from_csv(makerow({"per-5", "Nguyễn Vũ Hoàng", "emp4", "123", "EMPLOYEE"})));
    pm.items.PushBack(Person::from_csv(makerow({"per-6", "Lê Ý Thiên", "cus1", "123", "CUSTOMER"})));
    pm.items.PushBack(Person::from_csv(makerow({"per-7", "Lê Bá Khang", "cus2", "123", "CUSTOMER"})));
    pm.items.PushBack(Person::from_csv(makerow({"per-8", "Nguyễn Tiến Nhân", "cus3", "123", "CUSTOMER"})));
    pm.save();

    CustomerManager cm(dir + "/customer.csv");
    cm.items.PushBack(Customer::from_csv(makerow({"cus-1", "50000", "50000", "Bronze", "per-6"})));
    cm.items.PushBack(Customer::from_csv(makerow({"cus-2", "600000", "600000", "Silver", "per-7"})));
    cm.items.PushBack(Customer::from_csv(makerow({"cus-3", "1200000", "1200000", "Silver", "per-8"})));
    cm.save();

    EmployeeTypeManager etm(dir + "/employee_type.csv");
    etm.items.PushBack(EmployeeType::from_csv(makerow({"emt-1", "Thu Ngân", "Ca sáng", "06:00", "14:00", "20000"})));
    etm.items.PushBack(EmployeeType::from_csv(makerow({"emt-2", "Thu Ngân", "Ca chiều", "14:00", "22:00", "22000"})));
    etm.items.PushBack(EmployeeType::from_csv(makerow({"emt-3", "Thu Ngân", "Ca đêm", "22:00", "06:00", "25000"})));
    etm.items.PushBack(EmployeeType::from_csv(makerow({"emt-4", "Kỹ thuật viên", "Ca sáng", "06:00", "14:00", "20000"})));
    etm.items.PushBack(EmployeeType::from_csv(makerow({"emt-5", "Kỹ thuật viên", "Ca chiều", "14:00", "22:00", "22000"})));
    etm.items.PushBack(EmployeeType::from_csv(makerow({"emt-6", "Kỹ thuật viên", "Ca đêm", "22:00", "06:00", "25000"})));
    etm.items.PushBack(EmployeeType::from_csv(makerow({"emt-7", "Bếp", "Ca sáng", "06:00", "14:00", "20000"})));
    etm.items.PushBack(EmployeeType::from_csv(makerow({"emt-8", "Bếp", "Ca chiều", "14:00", "22:00", "22000"})));
    etm.items.PushBack(EmployeeType::from_csv(makerow({"emt-9", "Bếp", "Ca đêm", "22:00", "06:00", "25000"})));
    etm.save();

    EmployeeManager em(dir + "/employee.csv");
    em.items.PushBack(Employee::from_csv(makerow({"emp-1", "01-01-2025", "160000", "per-2", "adm-1", "emt-1"})));
    em.items.PushBack(Employee::from_csv(makerow({"emp-2", "01-01-2025", "200000", "per-3", "adm-1", "emt-5"})));
    em.items.PushBack(Employee::from_csv(makerow({"emp-3", "01-01-2025", "250000", "per-4", "adm-1", "emt-9"})));
    em.save();

    ServiceManager sm(dir + "/service.csv");
    sm.items.PushBack(Service::from_csv(makerow({"ser-1", "Nước suối", "10000", "100", "Active", "adm-1"})));
    sm.items.PushBack(Service::from_csv(makerow({"ser-2", "Mì ly", "15000", "80", "Active", "adm-1"})));
    sm.items.PushBack(Service::from_csv(makerow({"ser-3", "Cà phê", "20000", "60", "Active", "adm-1"})));
    sm.items.PushBack(Service::from_csv(makerow({"ser-4", "Snack", "10000", "120", "Active", "adm-1"})));
    sm.save();

    RoomManager rm(dir + "/room.csv");
    rm.items.PushBack(Room::from_csv(makerow({"room-1", "Phòng VIP", "VIP", "5", "30000", "Active", "adm-1"})));
    rm.items.PushBack(Room::from_csv(makerow({"room-2", "Phòng Thường", "Standard", "5", "20000", "Active", "adm-1"})));
    rm.items.PushBack(Room::from_csv(makerow({"room-3", "Phòng Thi Đấu", "Training", "3", "50000", "Active", "adm-1"})));
    rm.save();

    ComputerManager pc(dir + "/computer.csv");
    pc.items.PushBack(Computer::from_csv(makerow({"pc-1", "Asus ROG Strix G15", "i9/32G/4080", "Ready", "room-1", "adm-1"})));
    pc.items.PushBack(Computer::from_csv(makerow({"pc-2", "Dell Optiplex 5090", "i5/16G/3060", "Ready", "room-2", "adm-1"})));
    pc.items.PushBack(Computer::from_csv(makerow({"pc-3", "Lenovo Legion T7", "i7/32G/4070", "Ready", "room-3", "adm-1"})));
    pc.save();

    SessionManager ses(dir + "/session.csv");
    ses.items.PushBack(Session::from_csv(makerow({"ses-1", "10-10-2025", "08:00", "09:15", "77500", "cus-1", "emp-1", "pc-1", "room-1"})));
    ses.items.PushBack(Session::from_csv(makerow({"ses-2", "02-10-2025", "10:00", "11:30", "45000", "cus-2", "emp-1", "pc-2", "room-2"})));
    ses.items.PushBack(Session::from_csv(makerow({"ses-3", "02-10-2025", "00:00", "05:30", "275000", "cus-3", "emp-2", "pc-3", "room-3"})));
    ses.save();

    ServiceSessionManager ssm(dir + "/service_session.csv");
    ssm.items.PushBack(ServiceSession::from_csv(makerow({"ses-1", "ser-1", "2", "20000"})));
    ssm.items.PushBack(ServiceSession::from_csv(makerow({"ses-1", "ser-3", "1", "20000"})));
    ssm.items.PushBack(ServiceSession::from_csv(makerow({"ses-2", "ser-2", "1", "15000"})));
    ssm.save();

    PaymentManager pay(dir + "/payment.csv");
    pay.items.PushBack(Payment::from_csv(makerow({"pay-ses-1", "10-10-2025 09:20", "77500", "ses-1", "emp-1"})));
    pay.items.PushBack(Payment::from_csv(makerow({"pay-ses-2", "02-10-2025 11:40", "45000", "ses-2", "emp-1"})));
    pay.items.PushBack(Payment::from_csv(makerow({"pay-ses-3", "02-10-2025 05:45", "275000", "ses-3", "emp-2"})));
    pay.save();

    FeedbackManager fm(dir + "/feedback.csv");
    fm.items.PushBack(Feedback::from_csv(makerow({"fb-1", "cus-1", "20-10-2025", "Máy số 4 bị lag"})));
    fm.save();
}

#endif
