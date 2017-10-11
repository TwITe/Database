#define CATCH_CONFIG_MAIN
#include "/Users/TwITe/Documents/Visual Studio 2017/Projects/Database/database_lib.h"
#include "/Users/TwITe/Documents/Visual Studio 2017/Projects/catch.hpp"

TEST_CASE("Data was succesfully writed to files", "[data_store]") {
	set_path("C:/Users/TwITe/Documents/Visual Studio 2017/Projects/Database/Data_files/");
	set_data_file_size(5);
	int arr[5]{ 0, 1, 2, 3, 4 };
	REQUIRE(write_data(1, arr, 20) == true);
}

TEST_CASE("Data was succesfully deleted", "[data_delete]") {
	set_path("C:/Users/TwITe/Documents/Visual Studio 2017/Projects/Database/Data_files/");
	set_data_file_size(5);
	int arr[5]{ 0, 1, 2, 3, 4 };
	int id = 1;
	write_data(id, arr, 20);
	delete_data(id);
	REQUIRE(indexes[id].deleted == true);
}



//int main() {
//	set_path("C:/Users/TwITe/Documents/Visual Studio 2017/Projects/Database/Data_files/");
//	set_data_file_size(5);
//	int test_array_num1[10];
//	for (int i = 0; i < 10; i++) {
//		test_array_num1[i] = i;
//	}
//	store(1, test_array_num1, 10);
//	void* returned_data_num1 = load(1);
//	int* casted_data_num1 = static_cast<int*>(returned_data_num1);
//	cout << "First array: ";
//	for (int i = 0; i < 10; i++) {
//		cout << *(casted_data_num1 + i) << " ";
//	}
//	int test_array_num2[15];
//	for (int i = 0; i < 15; i++) {
//		test_array_num2[i] = i * 2;
//	}	
//	store(1, test_array_num2, 15);
//	void* returned_data_num2 = load(1);
//	int* casted_data_num2 = static_cast<int*>(returned_data_num2);
//	cout << endl << "Second array with identifiable id: ";
//	for (int i = 0; i < 15; i++) {
//		cout << *(casted_data_num2 + i) << " ";
//	}
//	system("PAUSE");
//	return 0;
//}