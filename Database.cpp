#include "/Users/TwITe/Documents/Visual Studio 2017/Projects/Database/database_lib.h"

int main() {
	set_path("C:/Users/TwITe/Documents/Visual Studio 2017/Projects/Database/Data_files/");
	set_data_file_size(1);
	const int array_length = 10;
	int arr[array_length];
	for (int i = 0; i < 10; i++) {
		arr[i] = i;
	}
	store(1, arr, array_length); 
	void* returned_data = load(1);
	int* casted_data = static_cast<int*>(returned_data);
	for (int i = 0; i < array_length; i++) {
		cout << *(casted_data + i) << " ";
	}
	system("PAUSE");
	return 0;
}