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

string filename = "data0";
string user_path;
const char* full_path;
int num = -1;
int default_filesize = 1;
unordered_map <int, vector <string> > users;

void set_path(const string &saving_path) {
	user_path = saving_path;
}

char get_full_path() {
	num++;
	filename = "data" + to_string(num);
	user_path = user_path + filename + ".txt";
	full_path = user_path.c_str();
}

void set_filesize(int user_size) {
	default_filesize = user_size;
}

void check_path() {	
	if (user_path.empty()) {
		throw runtime_error("path is invalid");
	}
}

//bool is_datafile_full(const string& filename) {
//	ifstream current_file("tes	t.txt", std::ifstream::binary);
//	if (current_file) {
//		// get length of file:
//		current_file.seekg(0, current_file.end);
//		int length = current_file.tellg();
//		current_file.seekg(0, current_file.beg);
//		return true;
//	}
//}

void write_id(int id) {

}

int file_free_space() {
	ifstream current_file("test.txt", std::ifstream::binary);
	current_file.seekg(0, current_file.end);
	int length = current_file.tellg();
	current_file.seekg(0, current_file.beg);
	return default_filesize - length;
}

void write_data(void* data, int array_length) {
	int free_space = file_free_space();
	get_full_path();
	FILE * pFile;
	if ()
	pFile = fopen(full_path, "rw");
	fwrite(&data, sizeof(data), free_space / sizeof(data), pFile);
	fclose(pFile);
	int current_data_size = array_length * sizeof(data);
	if (current_data_size <= free_space) {
		
	}
}

void store(int id, void* data, int array_length) {
	write_id(id);
	write_data(data, array_length);
}

void load(int id) {
	
}