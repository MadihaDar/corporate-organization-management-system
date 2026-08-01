#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <fstream>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class JSONHandler {
public:
    template<typename T>
    static bool save(const string& filename, const T& obj) {
        ofstream out(filename);
        if (!out) {
            cout << "File does not open" << filename << endl;
            return false;
        }
        out << obj.serializeJSON();
        out.close();
        return true;
    }

    template<typename T>
    static bool load(const string& filename, T& obj) {
        ifstream in(filename);
        if (!in) {
            cout << "File does not open: " << filename << endl;
            return false;
        }

        string jsonStr;
        string line;
        while (getline(in, line)) {
            jsonStr += line + "\n";
        }
        in.close();
        obj.deserializeJSON(jsonStr);     
        return true;

    }
};

#endif
