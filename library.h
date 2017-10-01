//It's database library
//Use the function "set_filesize" to set a default size of the data file
//Use the function "set_path" to set path for saving data files
//#ifndef DATABASE_LIBRARY_H
//#define DATABASE_LIBRARY_H
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
using namespace std;

string filename = "data0";
string path;
int num = 0;
int default_filesize = 1;
//unordered_map <int, <vector <string>>> users;

void set_path(const string &saving_path) {
    path = saving_path;
}

void set_filesize(int user_size) {
    default_filesize = user_size;
}

void check_path() {
    if (path.empty()) {
        throw std::runtime_error("path is invalid");
    }
}

bool is_datafile_full(int i) {
    if (i == default_filesize) {
        num++;
        filename = "data" + to_string(num);
        return true;
    }
    return false;
}

template <typename T>
void store(int id, void* data) {
    ;
    void* byte_array = data;
    check_path();
    users[id].push_back(filename);
    ofstream datafile;
    datafile.open(path + filename + ".txt");
    users[id].push_back(filename);
    for (int i= 0; i < sizeof(T); i++) {
        datafile << bytes[i];
        if ((is_datafile_full(i))) {
            datafile.close();
            users[id].push_back(filename);
            datafile.open(path + filename + ".txt");
        }
    }
    datafile.close();
}

template <typename T>
T load(int id) {
    bitset <sizeof(T)> return_value;
    ofstream datafile;
    for (auto filename: users[id]) {
        datafile.open(path + filename + ".txt");

    }
}
//#endif