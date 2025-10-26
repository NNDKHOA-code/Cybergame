#ifndef UTILS_H
#define UTILS_H

#include <ctime>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>
#include "MyVector.h"

using namespace std;

static inline int reFormatDate(const string &d)
{
    if (d.size() < 10) return 0;
    return (d[6] - '0') * 10000000 +
           (d[7] - '0') * 1000000 +
           (d[8] - '0') * 100000 +
           (d[9] - '0') * 10000 +
           (d[3] - '0') * 1000 +
           (d[4] - '0') * 100 +
           (d[0] - '0') * 10 +
           (d[1] - '0');
}

static inline string getTime()
{
    time_t t = time(0);
    char b[6];
    strftime(b, sizeof(b), "%H:%M", localtime(&t));
    return b;
}

static inline string getDate()
{
    time_t t = time(0);
    char b[16];
    strftime(b, 16, "%Y-%m-%d", localtime(&t));
    return b;
}

static inline string getTimeDate()
{
    time_t t = time(0);
    char b[20];
    strftime(b, 20, "%Y-%m-%d %H:%M", localtime(&t));
    return b;
}

static inline int StringToMinutes(const string &time)
{
    if (time.size() < 5)
        return 0;

    int h = (time[0] - '0') * 10 + (time[1] - '0');
    int m = (time[3] - '0') * 10 + (time[4] - '0');
    return h * 60 + m;
}

static inline Vec<string> makerow(initializer_list<string> lst)
{
    Vec<string> v;
    for (auto it = lst.begin(); it != lst.end(); ++it)
        v.PushBack(string(*it));
    return v;
}

static int getNumID(const string &ID)
{
    int pos = ID.find('-');
    return stoi(ID.substr(pos + 1));
}

template <typename T>
void merge(Vec<T> &arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    Vec<T> L, R;
    for (int i = 0; i < n1; i++)
        L.PushBack(arr[l + i]);
    for (int j = 0; j < n2; j++)
        R.PushBack(arr[m + 1 + j]);

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if (L[i].value >= R[j].value)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

template <typename T>
void mergeSort(Vec<T> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

static string nextIdAuto(const string& prefix, const string& path)
{
    ifstream fin(path);
    if (!fin.is_open())
        return prefix + "-1"; 

    string line;
    int maxId = 0;
    while (getline(fin, line))
    {
        if (line.empty()) continue;

        string id = line.substr(0, line.find(','));
        if (id.rfind(prefix + "-", 0) == 0)
        {
            size_t dash = id.find('-');
            if (dash != string::npos)
            {
                int num = stoi(id.substr(dash + 1));
                if (num > maxId) maxId = num;
            }
        }
    }

    fin.close();
    return prefix + "-" + to_string(maxId + 1);
}

static string autoIdFor(const string& prefix, const string& dir)
{
    string filename;
    if (prefix == "cus") filename = "customer";
    else if (prefix == "emp") filename = "employee";
    else if (prefix == "ser") filename = "service";
    else if (prefix == "room") filename = "room";
    else if (prefix == "pc") filename = "computer";
    else if (prefix == "emt") filename = "employee_type";
    else if (prefix == "ses") filename = "session";
    else if (prefix == "pay") filename = "payment";
    else if (prefix == "per") filename = "person";
    else if (prefix == "feed") filename = "feedback";
    else filename = prefix;

    string path = dir + "/" + filename + ".csv";
    return nextIdAuto(prefix, path);
}


#endif
