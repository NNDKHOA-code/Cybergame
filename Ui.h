#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include "MyVector.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define _HAS_STD_BYTE 0
#include <windows.h>
#endif

using namespace std;

namespace ansi
{
    static const char *DIM = "\x1b[2m";
    static const char *RESET = "\x1b[0m";
    static const char *BOLD = "\x1b[1m";
    static const char *RED = "\x1b[31m";
    static const char *GREEN = "\x1b[32m";
    static const char *YELLOW = "\x1b[33m";
    static const char *BLUE = "\x1b[34m";
    static const char *CYAN = "\x1b[36m";

    static void enable()
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE)
            return;
        DWORD mode = 0;
        if (!GetConsoleMode(hOut, &mode))
            return;
        mode |= 0x0004;
        SetConsoleMode(hOut, mode);
        SetConsoleOutputCP(65001);
    }
}

static inline int visibleWidth(const string &s)
{
    int count = 0;
    for (unsigned char c : s)
        if ((c & 0xC0) != 0x80)
            count++;
    return count;
}

static inline void hr(int w)
{
    for (int i = 0; i < w; ++i)
        cout << "═";
}

static inline void boxTitle(const string &title, int w = 60)
{
    using namespace ansi;
    int tlen = visibleWidth(title);
    if (tlen + 6 > w)
        w = tlen + 6;
    cout << BLUE << "╔";
    hr(w - 2);
    cout << "╗\n";
    int pad = w - 2 - tlen;
    if (pad < 0)
        pad = 0;
    int L = pad / 2, R = pad - L;
    cout << "║" << BOLD << string(L, ' ') << title
         << string(R, ' ') << RESET << "║\n";
    cout << "╚";
    hr(w - 2);
    cout << "╝" << RESET << "\n";
}

static inline void okMsg(const string &s) { cout << ansi::GREEN << s << ansi::RESET << "\n"; }
static inline void errMsg(const string &s) { cout << ansi::RED << s << ansi::RESET << "\n"; }
static inline void warnMsg(const string &s) { cout << ansi::YELLOW << s << ansi::RESET << "\n"; }

static inline void drawTop(const Vec<string> &headers, const Vec<int> &W)
{
    using namespace ansi;
    cout << CYAN << "╔";
    for (int i = 0; i < headers.getsize(); ++i)
    {
        for (int k = 0; k < W[i] + 2; ++k)
            cout << "═";
        cout << (i + 1 < headers.getsize() ? "╦" : "╗");
    }
    cout << RESET << "\n";
}

static inline void drawSep(const Vec<string> &headers, const Vec<int> &W)
{
    using namespace ansi;
    cout << CYAN << "╠";
    for (int i = 0; i < headers.getsize(); ++i)
    {
        for (int k = 0; k < W[i] + 2; ++k)
            cout << "═";
        cout << (i + 1 < headers.getsize() ? "╬" : "╣");
    }
    cout << RESET << "\n";
}

static inline void drawBot(const Vec<string> &headers, const Vec<int> &W)
{
    using namespace ansi;
    cout << CYAN << "╚";
    for (int i = 0; i < headers.getsize(); ++i)
    {
        for (int k = 0; k < W[i] + 2; ++k)
            cout << "═";
        cout << (i + 1 < headers.getsize() ? "╩" : "╝");
    }
    cout << RESET << "\n";
}

static inline void printTable(const Vec<string> &headers, const Vec<Vec<string>> &rows)
{
    using namespace ansi;
    if (headers.getsize() == 0)
    {
        cout << "Không có dữ liệu\n";
        return;
    }

    Vec<int> W;
    for (int i = 0; i < headers.getsize(); ++i)
    {
        int ww = visibleWidth(headers[i]);
        W.PushBack(ww < 3 ? 3 : ww);
    }
    for (int r = 0; r < rows.getsize(); ++r)
        for (int c = 0; c < headers.getsize() && c < rows[r].getsize(); ++c)
        {
            int sz = visibleWidth(rows[r][c]);
            if (sz > W[c])
                W[c] = sz;
        }

    drawTop(headers, W);

    cout << CYAN << "║" << RESET;
    for (int i = 0; i < headers.getsize(); ++i)
    {
        int pad = W[i] - visibleWidth(headers[i]);
        int L = pad / 2, R = pad - L;
        cout << " " << ansi::BOLD << string(L, ' ') << headers[i]
             << string(R, ' ') << ansi::RESET << " " << CYAN << "║" << RESET;
    }
    cout << "\n";

    drawSep(headers, W);

    for (int r = 0; r < rows.getsize(); ++r)
    {
        cout << CYAN << "║" << RESET;
        for (int c = 0; c < headers.getsize(); ++c)
        {
            string v = (c < rows[r].getsize() ? rows[r][c] : "");
            int pad = W[c] - visibleWidth(v);
            if (pad < 0)
                pad = 0;
            cout << " " << v << string(pad, ' ') << " " << CYAN << "║" << RESET;
        }
        cout << "\n";
    }

    drawBot(headers, W);
}

static inline void printTable(const Vec<Vec<string>> &table)
{
    if (table.getsize() == 0)
    {
        cout << "Không có dữ liệu\n";
        return;
    }
    Vec<string> hdr = table[0];
    Vec<Vec<string>> rows;
    for (int i = 1; i < table.getsize(); ++i)
        rows.PushBack(table[i]);
    printTable(hdr, rows);
}

template <typename MAN, typename GETCOLS, typename BUILDER, typename UPDATER>
static void genericCrud(const string &title, MAN &mng, GETCOLS getCols, BUILDER builder, UPDATER updater)
{
    using namespace ansi;
    mng.load();
    while (true)
    {
        boxTitle(title);
        cout << BOLD << "1." << RESET << " Xem\n"
             << BOLD << "2." << RESET << " Thêm\n"
             << BOLD << "3." << RESET << " Cập nhật\n"
             << BOLD << "4." << RESET << " Xóa\n"
             << BOLD << "5." << RESET << " Tìm kiếm\n"
             << BOLD << "0." << RESET << " Quay lại\n→ ";

        int c;
        if (!(cin >> c))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            errMsg("Nhập sai, vui lòng nhập số!");
            continue;
        }

        cin.ignore(10000, '\n');

        if (c == 1)
        {
            auto hdr = getCols(nullptr, true);
            Vec<Vec<string>> rows;
            for (int i = 0; i < mng.items.getsize(); ++i)
            {
                auto r = getCols(&mng.items[i], false);
                if (r.getsize() > 0)
                    rows.PushBack(r);
            }
            printTable(hdr, rows);
        }
        else if (c == 2)
        {
            auto nv = builder();
            mng.upsert(nv[0], mng.fromRow(nv));
            mng.save();
            okMsg("Đã thêm");
        }
        else if (c == 3)
        {
            string id;
            cout << "Nhập ID: ";
            getline(cin, id);
            if (id.empty())
            {
                warnMsg("ID không được để trống");
                continue;
            }

            auto *p = mng.find(id);
            if (!p)
            {
                errMsg("Không tồn tại");
                continue;
            }
            auto nv = updater(*p);
            mng.upsert(id, mng.fromRow(nv));
            mng.save();
            okMsg("Đã cập nhật");
        }
        else if (c == 4)
        {
            string id;
            cout << "Nhập ID: ";
            getline(cin, id);
            if (id.empty())
            {
                warnMsg("ID không được để trống");
                continue;
            }

            if (mng.eraseID(id))
            {
                mng.save();
                okMsg("Đã xóa");
            }
            else
                errMsg("Không tồn tại");
        }
        else if (c == 5)
        {
            string ID;
            cout << "Nhập ID tìm kiếm: ";
            getline(cin, ID);
            if (ID.empty())
            {
                warnMsg("Bạn chưa nhập ID tìm kiếm");
                continue;
            }

            auto hdr = getCols(nullptr, true);
            Vec<Vec<string>> rows;
            for (int i = 0; i < mng.items.getsize(); ++i)
            {
                auto r = getCols(&mng.items[i], false);
                bool ok = false;
                for (int j = 0; j < r.getsize(); ++j)
                    if (r[j] == ID)
                    {
                        ok = true;
                        break;
                    }
                if (ok)
                    rows.PushBack(r);
            }
            if (rows.getsize() > 0)
                printTable(hdr, rows);
            else
                errMsg("Không tìm thấy kết quả nào khớp");
        }
        else if (c == 0)
        {
            okMsg("Quay lại thành công");
            return;
        }
        else
            errMsg("Lựa chọn không hợp lệ");
    }
}

#endif
