#define CATCH_CONFIG_MAIN
#include "database.h"
#include "catch.hpp"
using namespace std;
int id = -1;

void preset_settings() {
    set_path("/home/twite/CLionProjects/Database/data_files/");
    set_data_file_size(20);
}

TEST_CASE("Check settings", "[general]") {
    set_path("");
    REQUIRE_THROWS(check_settings());
}

TEST_CASE("Data was succesfully writed to files", "[data_store]") {
    preset_settings();
    int arr[5]{0, 1, 2, 3, 4};
    id++;
    REQUIRE(write_data(id, arr, sizeof(int) * 5));
}

TEST_CASE("Able to write data of type <int>", "[data_store]") {
    preset_settings();
    int data = 10;
    id++;
    store_helper(id, data);
    int* returned_data = static_cast<int*>(get_writed_data(id));
    REQUIRE(data == *returned_data);
}

TEST_CASE("Able to write data of type <double>", "[data_store]") {
    preset_settings();
    double data = 10.105;
    id++;
    store_helper(id, data);
    double* returned_data = static_cast<double*>(get_writed_data(id));
    REQUIRE(data == *returned_data);
}

TEST_CASE("Able to write data of type <string>", "[data_store]") {
    preset_settings();
    string data = "database is working perfectly y f s";
    id++;
    store_helper(id, data);
    char* returned_data = static_cast<char*>(get_writed_data(id));
    string converted_string;
    for (unsigned int i = 0; i < data.size() * sizeof(char); i++) {
        converted_string.push_back(*(returned_data + i));
    }
    REQUIRE(data == converted_string);
}

TEST_CASE("Able to write vector of type <int>", "[data_store]") {
    preset_settings();
    vector <int> data;
    for (int i = 0; i < 10; i++) {
        data.push_back(i * 2);
    }
    store_helper(id, data);
    int* casted_data = static_cast<int*>(get_writed_data(id));
    vector <int> converted_int_type_vector;
    for (int i = 0; i < data.size(); i++) {
        converted_int_type_vector.push_back(*(casted_data + i));
    }
    REQUIRE(data == converted_int_type_vector);
}

TEST_CASE("Able to write vector of type <double>", "[data_store]") {
    preset_settings();
    vector <double> data;
    for (int i = 10; i > 0; i--) {
        data.push_back(i / 3.22);
    }
    store_helper(id, data);
    double* casted_data = static_cast<double*>(get_writed_data(id));
    vector <double> converted_double_type_vector;
    for (int i = 0; i < data.size(); i++) {
        converted_double_type_vector.push_back(*(casted_data + i));
    }
    REQUIRE(data == converted_double_type_vector);
}

TEST_CASE("Able to write vector of type <string>", "[data_store]") {
    preset_settings();
    vector <string> data;
    string s = "data";
    for (char i = 'a'; i < 'g'; i++) {
        data.push_back(s + i);
    }
    store_helper(id, data);
    char* casted_data = static_cast<char*>(get_writed_data(id));
    vector <string> converted_string_type_vector(data.size());
    int j = 0;
    for (int i = 0; i < data.size(); i++) {
        for (int ii = 0; ii < data[i].size(); ii++) {
            converted_string_type_vector[i].push_back(*(casted_data + j));
            j++;
        }
    }
    REQUIRE(data == converted_string_type_vector);
}

TEST_CASE("Created datafiles exists", "[data_store]") {
    preset_settings();
    int arr[5]{ 0, 1, 2, 3, 4 };
    id++;
    write_data(id, arr, 20);
    ifstream current_data_file;
    for (const auto& current_file_name : indexes[id].file_names) {
        current_data_file.open(current_file_name);
        REQUIRE(current_data_file.is_open());
    }
}

TEST_CASE("Data was succesfully deleted", "[data_delete]") {
    preset_settings();
    int arr[5]{ 0, 1, 2, 3, 4 };
    id++;
    write_data(id, arr, 20);
    delete_data(id);
    REQUIRE(indexes[id].deleted);
}

TEST_CASE("Able to load existing data") {
    preset_settings();
    const int array_length = 5;
    int arr[array_length]{0, 1, 2, 3, 4};
    id++;
    write_data(id, arr, 20);
    void* returned_data_num1 = load(id);
    int* casted_data = static_cast<int*>(returned_data_num1);
    for (int i = 0; i < 5; i++) {
        arr[i] = casted_data[i];
    }
}