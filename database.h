//It's database library
//Call function "set_data_filesize" to set the default size of the data file
//Call function "set_path" to set the path for saving data files
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstring>
using namespace std;

struct index_data {
	vector <string> file_names;
	vector <long long> start_reading_positions;
	vector <long long> end_reading_positions;
	size_t data_size = 1;
	bool deleted = false;
};

string user_path;
int data_file_number = -1;
unsigned int data_file_size = 8388608;
unordered_map <int,index_data> indexes;

void check_path() {
    if (user_path.empty()) {
        throw runtime_error("path is invalid");
    }
}

void check_settings() {
    check_path();
}

void set_data_file_size(unsigned int user_data_size) {
    data_file_size = user_data_size;
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

bool save_current_data_size(int id, size_t current_data_size) {
    index_data obj;
    obj.data_size = current_data_size;
	indexes[id] = obj;
	return true;
}

bool save_file_name_and_reading_positions(int id, const string& current_filename, long long start_position, long long end_position) {
	indexes[id].file_names.push_back(current_filename);
	indexes[id].start_reading_positions.push_back(start_position);
	indexes[id].end_reading_positions.push_back(end_position);
	return true;
}

size_t get_file_free_space(const string& current_file_name) {
    ifstream current_file(current_file_name, ifstream::binary);
    current_file.seekg(0, current_file.end);
    if (static_cast<int>(current_file.tellg()) == 0) {
        return data_file_size;
    }
    int current_data_file_size = current_file.tellg();
    current_file.close();
    return data_file_size - current_data_file_size;
}

bool check_if_current_id_was_deleted(int id) {
	return indexes[id].deleted;
}

bool check_if_current_id_is_already_exists(int id) {
    return indexes.count(id) == 1;
}

void is_data_size_invalid(size_t current_data_size) {
    if (current_data_size < 0) {
        cerr << "The data size is invalid, please check it";
    }
}

void write_data_to_file(void* &data, FILE* &data_file, int last_written_byte_position, size_t bytes_number_to_write, const string& current_file,  streamoff cur_pos_in_the_file, int id) {
    long long start_position = cur_pos_in_the_file;
    fwrite(static_cast<void*>(static_cast<char*>(data + last_written_byte_position)), 1, bytes_number_to_write, data_file);
    long long end_position = ftell(data_file);
    save_file_name_and_reading_positions(id, current_file, start_position, end_position);
}

void delete_data(int id) {
    indexes.erase(id);
    indexes[id].deleted = true;
}

bool write_data(int id, void* data, size_t current_data_size) {
    FILE* data_file;
    string current_file;
    int last_written_byte_position = 0;
    while (current_data_size != 0) {
        current_file = get_new_file_path();
        data_file = fopen(current_file.c_str(), "a+b");
        size_t file_free_space = get_file_free_space(current_file);
        fseek (data_file, data_file_size - file_free_space, SEEK_SET);
        while (file_free_space == 0) {
            current_file = get_new_file_path();
            data_file = fopen(current_file.c_str(), "a+b");
            file_free_space = get_file_free_space(current_file);
        }
        streamoff cur_pos_in_the_file = data_file_size - file_free_space;
        size_t bytes_number_to_write;
        if (current_data_size > file_free_space) {
            bytes_number_to_write = file_free_space;
            write_data_to_file(data, data_file, last_written_byte_position, bytes_number_to_write, current_file, cur_pos_in_the_file, id);
            last_written_byte_position += bytes_number_to_write;
        }
        else {
            bytes_number_to_write = current_data_size;
            write_data_to_file(data, data_file, last_written_byte_position, bytes_number_to_write, current_file, cur_pos_in_the_file, id);
        }
        current_data_size -= bytes_number_to_write;
        is_data_size_invalid(current_data_size);
        fclose(data_file);
    }
    return true;
}

bool store(int id, size_t data_size) {
    if (check_if_current_id_is_already_exists(id)) {
        delete_data(id);
    }
    save_current_data_size(id, data_size);
    indexes[id].deleted = false;
    return true;
}

bool store_helper(int id, int data) {
    check_settings();
    size_t data_size = sizeof(int);
    store(id, data_size);
    write_data(id, &data, data_size);
    return true;
}

bool store_helper(int id, double data) {
    check_settings();
    size_t data_size = sizeof(double);
    store(id, data_size);
    write_data(id, &data, data_size);
    return true;
}

bool store_helper(int id, const string &data) {
    check_settings();
    string s = &data[0];
    size_t data_size = s.size() * sizeof(char);
    char casted_string[data_size];
    strcpy(casted_string, &data[0]);
    store(id, data_size);
    write_data(id, casted_string, data_size);
    return true;
}

bool store_helper(int id, const vector <int> &data) {
    size_t data_size = sizeof(int) * data.size();
    store(id, data_size);
    int casted_vector[data.size()];
    for (int i = 0; i < data.size(); i++) {
        casted_vector[i] = data[i];
    }
    write_data(id, casted_vector, data_size);
    return true;
}

bool store_helper(int id, const vector <double> &data) {
    size_t data_size = sizeof(double) * data.size();
    store(id, data_size);
    double casted_vector[data.size()];
    for (int i = 0; i < data.size(); i++) {
        casted_vector[i] = data[i];
    }
    write_data(id, casted_vector, data_size);
    return true;
}

bool store_helper(int id, const vector <string> &data) {
    size_t data_size = 0;
    for (const auto &str: data) {
        data_size += str.size();
    }
    store(id, data_size);
    char casted_vector[data_size];
    int j = 0;
    for (int i = 0; i < data.size(); i++) {
        for (auto ch: data[i]) {
            casted_vector[j]= ch;
            j++;
        }
    }
    write_data(id, casted_vector, data_size);
    return true;
}

void* load(int id) {
    if (check_if_current_id_was_deleted(id)) {
        cerr << "Current data was deleted and no longer exists!";
        return nullptr;
    }
    FILE* data_file;
    size_t current_data_size = indexes[id].data_size;
    //char buf[current_data_size];
    void* return_data = malloc(current_data_size);
    int last_written_byte_position_in_main_buffer = 0;
    for (unsigned int i = 0; i < indexes[id].file_names.size(); i++) {
        const char* current_filename = indexes[id].file_names[i].c_str();
        long long start_reading_position = indexes[id].start_reading_positions[i];
        long long end_reading_position = indexes[id].end_reading_positions[i];
        size_t reading_bytes_number = end_reading_position - start_reading_position;
        data_file = fopen(current_filename, "a+b");
        void* current_read_data = malloc(reading_bytes_number);
        fseek(data_file, start_reading_position, 0);
        fread(current_read_data, 1, reading_bytes_number, data_file);
        memcpy(static_cast<void*>(static_cast<char*>(return_data + last_written_byte_position_in_main_buffer)), current_read_data, reading_bytes_number);
        last_written_byte_position_in_main_buffer += reading_bytes_number;
        free(current_read_data);
        fclose(data_file);
    }
    return return_data;
}

void* get_writed_data(int id) {
    void *data = load(id);
    return data;
}