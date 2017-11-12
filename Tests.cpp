#define CATCH_CONFIG_MAIN
#include "database_lib.h"
#include "catch.hpp"
using namespace std;
int id = -1;

void preset_settings() {
    set_path("/home/twite/CLionProjects/Database/Data_files/");
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
    void* returned_data = load(id);
    int* casted_data = static_cast<int*>(returned_data);
    REQUIRE(data == *casted_data);
}

TEST_CASE("Able to write data of type <double>", "[data_store]") {
    preset_settings();
    double data = 10.105;
    id++;
    store_helper(id, data);
    void* returned_data = load(id);
    double* casted_data = static_cast<double*>(returned_data);
    REQUIRE(data == *casted_data);
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