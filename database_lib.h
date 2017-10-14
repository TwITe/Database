//It's database library
//Call function "set_data_filesize" to set the default size of the data file
//Call function "set_path" to set the path for saving data files
//Call function "set_map_size" to set a size of the map that can be stored in memory before writing to file
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <cstring>
using namespace std;

struct index {
	vector <string> file_names;
	vector <int> start_reading_positions;
	vector <int> end_reading_positions;
	int data_size;
	bool deleted;
};

string user_path;
int data_file_number = -1;
unsigned int data_file_size = 8388608;
unsigned int map_memory_size;
unordered_map <int, index> indexes;

bool check_path() {
	if (user_path.empty()) {
		throw runtime_error("path is invalid");
	}
}

bool check_settings() {
	check_path();
	return true;
}

void set_data_file_size(int user_data_size) {
	data_file_size = user_data_size;
}

void set_map_size(int user_map_size) {
	map_memory_size = user_map_size;
}

void set_path(const string &saving_path) {
	user_path = saving_path;
}

string get_new_file_path() {
	data_file_number++;
	string data_filename = "data" + to_string(data_file_number);
	string path = user_path + data_filename + ".dat";
	return path;
}

bool save_current_data_size(int id, int current_data_size){
	indexes[id].data_size = current_data_size;
	return true;
}

bool save_file_name_and_reading_positions(int id, const string& current_filename, int start_position, int end_position) {
	indexes[id].file_names.push_back(current_filename);
	indexes[id].start_reading_positions.push_back(start_position);
	indexes[id].end_reading_positions.push_back(end_position);
	return true;
}

unsigned int get_file_free_space(const string& current_file_name) {
	ifstream current_file(current_file_name, ifstream::binary);
	current_file.seekg(0, ios::end);
	if (static_cast<int>(current_file.tellg()) == 0) {
		return data_file_size;
	}
	size_t current_data_file_size = current_file.tellg();
	current_file.seekg(0, ios::beg);
	current_file.close();
	return data_file_size - current_data_file_size;
}

bool check_if_current_id_was_deleted(int id) {
	if (indexes[id].deleted == true) {
		return true;
	}
}

bool check_if_current_id_is_already_exists(int id) {
	if (indexes.count(id) == 1) {
		return true;
	}
	return false;
}

void is_data_size_invalid(int current_data_size) {
	if (current_data_size < 0) {
		cerr << "The data size is invalid, please check it";
	}
}

void write_data_to_file(void* data, FILE * data_file, int last_written_byte_position, int bytes_number_to_write, const string& current_file, int id) {
	long int start_position = ftell(data_file);
	fwrite(((void*)((char*)data + last_written_byte_position)), 1, bytes_number_to_write, data_file);
	long int end_position = ftell(data_file);
	save_file_name_and_reading_positions(id, current_file, start_position, end_position);
}

void delete_data(int id) {
	indexes.erase(id);
	indexes[id].deleted = true;
}

bool write_data(int id, void* data, int current_data_size) {
	FILE * data_file = NULL;
	string current_file;
	int last_written_byte_position = 0;
	while (current_data_size != 0) {
		if (data_file == NULL) {
			current_file = get_new_file_path();
		}
		data_file = fopen(current_file.c_str(), "a+b");
		unsigned int file_free_space = get_file_free_space(current_file);
		if (file_free_space == 0) {
			current_file = get_new_file_path();
			data_file = fopen(current_file.c_str(), "a+b");
		}
		int bytes_number_to_write;
		if (current_data_size > file_free_space) {
			bytes_number_to_write = file_free_space;
			write_data_to_file(data, data_file, last_written_byte_position, bytes_number_to_write, current_file, id);
			last_written_byte_position += bytes_number_to_write;
		}
		else {
			bytes_number_to_write = current_data_size;
			write_data_to_file(data, data_file, last_written_byte_position, bytes_number_to_write, current_file, id);
		}
		current_data_size -= bytes_number_to_write;
		is_data_size_invalid(current_data_size);
		fclose(data_file);
		data_file = NULL;
	}
	return true;
}

bool storage(int id, void* data, int array_length) {
	if (check_if_current_id_is_already_exists(id)) {
		delete_data(id);
	}
	unsigned int current_data_size = array_length * 4;
	save_current_data_size(id, current_data_size);
	write_data(id, data, current_data_size);
	indexes[id].deleted = false;
	return true;
}

bool store(int id, void* data, int array_length) {
	check_settings();
	storage(id, data, array_length);
	return true;
}

bool store_array_helper() {
	return true;
}

void* load(int id) {
	if (check_if_current_id_was_deleted(id)) {
		cerr << "Current data was deleted and no longer exists!";
		return 0;
	}
	FILE * data_file;
	int current_data_size = indexes[id].data_size;
	void* return_data = new void*[current_data_size];
	for (int i = 0; i < indexes[id].file_names.size(); i++) {
		const char* current_filename = indexes[id].file_names[i].c_str();
		int start_reading_position = indexes[id].start_reading_positions[i];
		int end_reading_position = indexes[id].end_reading_positions[i];
		int reading_bytes_number = end_reading_position - start_reading_position;
		data_file = fopen(current_filename, "a+b");
		void* current_read_data = new void*[reading_bytes_number];
		fseek(data_file, start_reading_position, 0);
		fread(current_read_data, 1, reading_bytes_number, data_file);
        memcpy((void*)((char*)return_data + (reading_bytes_number*i)), current_read_data, reading_bytes_number);
		delete[] current_read_data;
		fclose(data_file);
	}
	return return_data;
}