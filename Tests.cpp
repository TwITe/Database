#define CATCH_CONFIG_MAIN
#include "database_lib.h"
#include "catch.hpp"
using namespace std;

TEST_CASE("Check settings", "[general]") {
    set_path("");
    REQUIRE_THROWS(check_settings());
}

TEST_CASE("Data was succesfully writed to files", "[data_store]") {
    set_path("/home/twite/CLionProjects/Database/Data_files/");
    set_data_file_size(500);
    int arr[5]{ 0, 1, 2, 3, 4 };
    int id = 1;
    REQUIRE(write_data(1, arr, 20));
}

TEST_CASE("Created datafiles exists", "[data_store]") {
    set_path("/home/twite/CLionProjects/Database/Data_files/");
    set_data_file_size(500);
    int arr[5]{ 0, 1, 2, 3, 4 };
    int id = 1;
    REQUIRE(write_data(id, arr, 20));
    ifstream current_data_file;
    for (const auto& current_file_name : indexes[id].file_names) {
        current_data_file.open(current_file_name);
        REQUIRE(current_data_file.is_open());
    }
}

TEST_CASE("Able to load existing data") {
    set_path("/home/twite/CLionProjects/Database/Data_files/");
    set_data_file_size(500);
    const int array_length = 5;
    int arr[array_length]{ 0, 1, 2, 3, 4 };
    int id = 1;
    write_data(1, arr, 20);
    void* returned_data_num1 = load(1);
	int* casted_data = static_cast<int*>(returned_data_num1);
	for (int i = 0; i < 5; i++) {
		arr[i] = casted_data[i];
	}
}

TEST_CASE("Data was succesfully deleted", "[data_delete]") {
    set_path("/home/twite/CLionProjects/Database/Data_files/");
    set_data_file_size(500);
    int arr[5]{ 0, 1, 2, 3, 4 };
    int id = 1;
    write_data(id, arr, 20);
    delete_data(id);
    REQUIRE(indexes[id].deleted);
}