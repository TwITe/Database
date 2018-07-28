#define CATCH_CONFIG_MAIN
#include "database.h"
#include "catch.hpp"
using namespace std;
int id = -1;

void preset_settings() {
#ifdef _WIN32
    set_path("c:\\users\\root\\documents\\visual studio 2017\\projects\\database\\data_files\\");
#endif
#ifdef __linux__
    set_path("/home/root/CLionProjects/Database/data_files/");
#endif
    set_data_file_size(20);
}

TEST_CASE("Get throw for invalid settings", "[general]") {
    set_path("");
    REQUIRE_THROWS(check_settings());
}

TEST_CASE("Do not get throw for valid settings", "[general]") {
    initialize_db("/home/root/CLionProjects/Database/data_files/", 20);
    REQUIRE(check_settings());
}

TEST_CASE("Get throw during a load of not existing data", "[data_load]") {
    REQUIRE_THROWS(load(55));
}

TEST_CASE("Get throw during a store of already existing data", "[data_store]") {
    store_helper(100, 5);
    REQUIRE_THROWS(store_helper(100, 76));
}

TEST_CASE("Data writing function working correctly", "[data_store]") {
    int arr[5]{0, 1, 2, 3, 4};
    id++;
    REQUIRE(write_data(id, arr, sizeof(int) * 5));
}

TEST_CASE("Able to write data of type <int>", "[data_store]") {
    id++;
    int data = 10;
    store_helper(id, data);
    int writed_data = load_helper<int>(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write data of type <double>", "[data_store]") {
    id++;
    double data = 10.105;
    store_helper(id, data);
    double writed_data = load_helper<double>(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write data of type <string>", "[data_store]") {
    id++;
    string data = "database is working perfectly (yes(no)) GG > WP";
    REQUIRE(store_helper(id, data));
    string writed_data = load_helper<string>(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write vector of type <int>", "[data_store]") {
    id++;
    vector<int> data;
    for (int i = 0; i < 10; i++) {
        data.push_back(i * 2);
    }
    REQUIRE(store_helper(id, data));
    vector<int> writed_data = load_int_vector_helper(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write vector of type <double>", "[data_store]") {
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
    id++;
    vector<string> data;
    string s = "data";
    for (char i = 'a'; i < 'g'; i++) {
        data.push_back(s + i);
    }
    REQUIRE(store_helper(id, data));
    vector<string> writed_data = load_string_vector_helper(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Get throw during loading deleted data", "[data_delete]") {
    id++;
    int data = 4341;
    store_helper(id, data);
    delete_id(id);
    REQUIRE_THROWS(load_helper<int>(id));
}

TEST_CASE("Able to store data with the same id after deleting old data", "[data_delete]") {
    id++;
    int data = 5525;
    store_helper(id, data);
    delete_id(id);
    data = 8134971;
    store_helper(id, data);
    int writed_data = load_helper<int>(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Load map from file", "[map_load]") {
    indexes.clear();
    load_map_from_file();
    get_deleted_indexes();
}

TEST_CASE("Able to write data of type <int> after loading map from file", "[data_load]") {
    id = 1;
    int data = 10;
    int writed_data = load_helper<int>(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write data of type <double> after loading map from file", "[data_load]") {
    id++;
    double data = 10.105;
    double writed_data = load_helper<double>(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write data of type <string> after loading map from file", "[data_load]") {
    id++;
    string data = "database is working perfectly (yes(no)) GG > WP";
    string writed_data = load_helper<string>(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write vector of type <int> after loading map from file", "[data_load]") {
    id++;
    vector<int> data;
    for (int i = 0; i < 10; i++) {
        data.push_back(i * 2);
    }
    vector<int> writed_data = load_int_vector_helper(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write vector of type <double> after loading map from file", "[data_load]") {
    id++;
    vector<double> data;
    for (int i = 10; i > 0; i--) {
        data.push_back(i / 3.22);
    }
    vector<double> writed_data = load_double_vector_helper(id);
    REQUIRE(data == writed_data);
}

TEST_CASE("Able to write vector of type <string> after loading map from file", "[data_load]") {
    id++;
    vector<string> data;
    string s = "data";
    for (char i = 'a'; i < 'g'; i++) {
        data.push_back(s + i);
    }
    vector<string> writed_data = load_string_vector_helper(id);
    REQUIRE(data == writed_data);
}
