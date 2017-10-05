//It's database library
//Use the function "set_data_filesize" to set a default size of the data file
//Use the function "set_path" to set path for saving data files
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
using namespace std;

struct index {
	vector <string> file_names;
	vector <int> start_reading_positions;
	vector <int> end_reading_positions;
	int data_size;
};

string data_filename;
string user_path;
const int default_file_size = 8388608;
int data_file_number = -1;
unsigned int data_file_size = default_file_size;
unordered_map <int, index> indexes;
int map_size = 0;

void check_path() {
	if (user_path.empty()) {
		throw runtime_error("path is invalid");
	}
}

void check_settings() {
	check_path();
}

void set_data_file_size(int user_data_size) {
	data_file_size = user_data_size;
}

void set_path(const string &saving_path) {
	user_path = saving_path;
}

string get_new_file_path() {
	data_file_number++;
	data_filename = "data" + to_string(data_file_number);
	string path = user_path + data_filename + ".txt";
	return path;
}

void save_current_data_size(int id, int current_data_size){
	indexes[id].data_size = current_data_size;
}

void save_file_name_and_reading_position(int id, const string& current_filename, int start_position, int end_position) {
	indexes[id].file_names.push_back(current_filename);
	indexes[id].start_reading_positions.push_back(start_position);
	indexes[id].end_reading_positions.push_back(end_position);
}

unsigned int get_file_free_space(const string& current_file_name) {
	ifstream current_file(current_file_name, ifstream::binary);
	//unsigned int current_file_size = static_cast<unsigned int>(current_file.tellp());
	//unsigned int current_file_size = (current_file.tellp());
	current_file.seekg(0, ios::end);
	if (static_cast<int>(current_file.tellg()) == 0) {
		return data_file_size;
	}
	size_t current_data_file_size = current_file.tellg();
	current_file.seekg(0, ios::beg);
	current_file.close();
	return data_file_size - current_data_file_size;
}

void store(int id, void* data, int array_length) {
	check_settings();
	unsigned int current_data_size = array_length * sizeof(data);
	save_current_data_size(id, current_data_size);
	FILE * data_file = NULL;
	int last_written_byte_position = 0;
	string current_file;		
	while (current_data_size != 0) {
		if (data_file == NULL) {
			current_file = get_new_file_path();
		}
		data_file = fopen(current_file.c_str(), "a+b");
		unsigned int free_space = get_file_free_space(current_file);
		if (free_space == 0) {
			current_file = get_new_file_path();
			data_file = fopen(current_file.c_str(), "a+b");
		}
		if (current_data_size > free_space) {
			long int start_position = ftell(data_file);
			fwrite(((void*)((char*)data + last_written_byte_position)), 1, free_space, data_file);
			long int end_position = ftell(data_file);
			save_file_name_and_reading_position(id, current_file, start_position, end_position);
			last_written_byte_position += free_space;
			current_data_size -= free_space;
		}
		else {
			long int start_position = ftell(data_file);
			fwrite(((void*)((char*)data + last_written_byte_position)), 1, current_data_size, data_file);
			long int end_position = ftell(data_file);
			save_file_name_and_reading_position(id, current_file, start_position, end_position);
			current_data_size -= current_data_size;
		}
		fclose(data_file);
		data_file = NULL;
	}
}

void* load(int id) {
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