#define CATCH_CONFIG_MAIN
#include "database.h"
#include "catch.hpp"
using namespace std;
int id = -1;

void preset_settings() {
#ifdef _WIN32
    set_path("c:\\users\\twite\\documents\\visual studio 2017\\projects\\database\\data_files\\");
#endif
#ifdef __linux__
    set_path("/home/twite/CLionProjects/Database/data_files/");
#endif
    set_data_file_size(20);
}

TEST_CASE("Check settings", "[general]") {
    set_path("");
    REQUIRE_THROWS(check_settings());
}

TEST_CASE("Data writing function working correctly", "[data_store]") {
    preset_settings();
    int arr[5]{0, 1, 2, 3, 4};
    id++;
    REQUIRE(write_data(id, arr, sizeof(int) * 5));
}

TEST_CASE("Able to write data of type <int>", "[data_store]") {
    preset_settings();
    int data = 10;
    id++;
    REQUIRE(store_helper(id, data));
    int writed_data = load_helper<int>(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write data of type <double>", "[data_store]") {
    preset_settings();
    double data = 10.105;
    id++;
    REQUIRE(store_helper(id, data));
    double writed_data = load_helper<double>(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write data of type <string>", "[data_store]") {
    preset_settings();
    string data = "database is working perfectly (yes(no)) GG > WP";
    id++;
    REQUIRE(store_helper(id, data));
    string writed_data = load_helper<string>(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write vector of type <int>", "[data_store]") {
    preset_settings();
    vector<int> data;
    id++;
    for (int i = 0; i < 10; i++) {
        data.push_back(i * 2);
    }
    REQUIRE(store_helper(id, data));
    vector<int> writed_data = load_int_vector_helper(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write vector of type <double>", "[data_store]") {
    preset_settings();
    id++;
    vector<double> data;
    for (int i = 10; i > 0; i--) {
        data.push_back(i / 3.22);
    }
    REQUIRE(store_helper(id, data));
    vector<double> writed_data = load_double_vector_helper(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write vector of type <string>", "[data_store]") {
    preset_settings();
    id++;
    vector<string> data(0);
    string s = "data";
    for (char i = 'a'; i < 'g'; i++) {
        data.push_back(s + i);
    }
    REQUIRE(store_helper(id, data));
    vector<string> writed_data = load_string_vector_helper(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Created datafiles exists", "[data_store]") {
    preset_settings();
    id++;
    int arr[5]{ 0, 1, 2, 3, 4 };
    write_data(id, arr, 20);
    ifstream current_data_file;
    for (const auto& current_file_name : indexes[id].file_names) {
        current_data_file.open(current_file_name);
        REQUIRE(current_data_file.is_open());
    }
}