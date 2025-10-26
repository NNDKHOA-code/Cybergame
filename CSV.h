#ifndef CSV_H
#define CSV_H

#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include "MyVector.h"

using namespace std;

template <typename T>
using Vec = Vector<T>;

struct CSV
{
    static string trim(const string &s)
    {
        size_t start = 0;
        while (start < s.size() && isspace((unsigned char)s[start]))
            start++;
        size_t end = s.size();
        while (end > start && isspace((unsigned char)s[end - 1]))
            end--;
        return s.substr(start, end - start);
    }

    static Vec<Vec<string>> read(const string &path)
    {
        Vec<Vec<string>> rows;
        ifstream in(path.c_str());
        if (!in.is_open())
            return rows;
        string line;
        while (getline(in, line))
        {
            if (line.empty())
                continue;
            if (!line.empty() && line.back() == '\r')
                line.pop_back();

            Vec<string> cols;
            string cur;

            for (size_t i = 0; i < line.size(); i++)
            {
                char c = line[i];
                if(c == ',') {
                    cols.PushBack(trim(cur));
                    cur.clear();
                } else {
                    cur.push_back(c);
                }
            }
            cols.PushBack(trim(cur));
            rows.PushBack(cols);
        }
        return rows;
    }

    static void write(const string& path, const Vec<Vec<string>>& rows) {
        ofstream out(path.c_str(), ios::trunc);
        if(!out.is_open()) return;

        for(int i = 0; i<rows.getsize();i++) {
            for(int j=0;j<rows[i].getsize();j++){
                if(j) out << ",";
                out << rows[i][j];
            }
            out <<"\n";
        }
        out.close();
    }

};

#endif
