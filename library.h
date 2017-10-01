//It's database library
//Use the function "set_filesize" to set a default size of the data file
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

string id_filename = "index";
string data_filename;
string user_path;
const char* full_path;
const int default_file_size = 8388608;
int data_file_number = -1;
int data_file_size = default_file_size;
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

struct index {
	vector <string> file_names;
	vector <int> start_reading_positions;
	vector <int> end_reading_positions;
};

void set_filesize(int user_size) {
	data_file_size = user_size;
}

void set_path(const string &saving_path) {
	user_path = saving_path;
}

void create_new_file() {
	data_file_number++;
	data_filename = "data" + to_string(data_file_number);
	string path = user_path + data_filename + ".txt";
	full_path = path.c_str();
}

void write_id(int id) {

}

void save_file_name_and_reading_position(int id, int start_position, int end_position) {
	indexes[id].file_names.push_back(data_filename);
	indexes[id].start_reading_positions.push_back(start_position);
	indexes[id].end_reading_positions(end_position);
	
}

int get_file_free_space() {
	ifstream current_file(full_path, ifstream::binary);
	current_file.seekg(0, current_file.end);
	int current_file_size = current_file.tellg();
	current_file.seekg(0, current_file.beg);
	return data_file_size - current_file_size;
}

void store(int id, void* data, int array_length) {
	check_settings();
	int free_space;
	int current_data_size = array_length * sizeof(data);
	FILE * data_file;
	int last_written_byte_position = 0;
	while (current_data_size != 0) {
		create_new_file();
		free_space = get_file_free_space();
		data_file = fopen(full_path, "rw");
		if (current_data_size > free_space) {
			long int start_position = ftell(data_file);
			fwrite(&data + last_written_byte_position, 1, free_space, data_file);
			long int end_position = ftell(data_file);
			save_file_name_and_reading_position(id, start_position, end_position);
			last_written_byte_position += free_space;
			current_data_size -= free_space;
		}
		else {
			long int start_position = ftell(data_file);
			fwrite(&data + last_written_byte_position, 1, current_data_size, data_file);
			long int end_position = ftell(data_file);
			save_file_name_and_reading_position(id, start_position, end_position);
			current_data_size -= current_data_size;
		}
		fclose(data_file);
	}
}

void* load(int id) {
	FILE * data_file;
	void* return_data = (void*) malloc(0);
	for (int i = 0; i < indexes[id].file_names.size(); i++) {
		const char* current_filename = indexes[id].file_names[i].c_str();
		int start_reading_position = indexes[id].start_reading_positions[i];
		int end_reading_position = indexes[id].end_reading_positions[i];
		int reading_elements_number = end_reading_position - start_reading_position;
		data_file = fopen(current_filename, "rw");
		void* current_read_data = (void*) malloc(reading_elements_number);
		fseek(data_file, start_reading_position, SEEK_SET);
		fread(current_read_data, 1, reading_elements_number, data_file);
		memcpy(return_data, current_read_data, reading_elements_number);
		free (current_read_data);
	}
	return return_data;
}