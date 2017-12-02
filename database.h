//It's database library
//Call function "set_data_filesize" to set the default size of the data file
//Call function "set_path" to set the path for saving data files
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <sstream>

using namespace std;

struct index_data {
	vector <string> file_names;
	vector <streamoff> start_reading_positions;
	vector <streamoff> end_reading_positions;
	size_t data_size = 0;
    bool deleted = false;
};

string path;
int data_file_number = -1;
unsigned int data_file_size = 8388608;
unordered_map <int,index_data> indexes;

bool check_path() {
    if (path.empty()) {
        throw runtime_error("path is invalid");
    }
    return true;
}

bool check_settings() {
    if (!check_path()) {
        return false;
    }
    return true;
}

void set_data_file_size(unsigned int user_data_size) {
    data_file_size = user_data_size;
}

void set_path(const string &saving_path) {
	path = saving_path;
}

string get_new_file_path() {
	data_file_number++;
	string data_filename = "data" + to_string(data_file_number);
	string current_path = path + data_filename + ".dat";
	return current_path;
}

bool save_current_data_size(int id, size_t current_data_size) {
    index_data obj;
    obj.data_size = current_data_size;
	indexes[id] = obj;
	return true;
}

bool save_file_name_and_reading_positions(int id, const string& current_filename, streamoff start_position, streamoff end_position) {
	indexes[id].file_names.push_back(current_filename);
	indexes[id].start_reading_positions.push_back(start_position);
	indexes[id].end_reading_positions.push_back(end_position);
	return true;
}

size_t get_file_free_space(FILE* &data_file) {
	fseek(data_file, 0, SEEK_END);
	long int last_written_byte_in_current_file_position = ftell(data_file);
    return static_cast<size_t>(data_file_size - last_written_byte_in_current_file_position);
}

bool check_if_current_id_does_not_exists(int id) {
	return (indexes.count(id) == 0);
}

bool check_if_current_id_is_already_exists(int id) {
    return indexes.count(id) == 1;
}

bool check_if_current_id_was_deleted(int id) {
    return indexes[id].deleted;
}

bool delete_id(int id) {
    indexes[id].deleted = true;
    string file_name = path + "deleted_indexes.dat";
    ofstream file;
    file.open(file_name, ios::out | ios::app);
    file << id << "\n";
}

void is_data_size_invalid(size_t current_data_size) {
    if (current_data_size < 0) {
        cerr << "The data size is invalid, please check it";
    }
}

void write_data_to_file(void* data, FILE* data_file, int last_written_byte_position, size_t bytes_number_to_write, const string& current_file,  streamoff cur_pos_in_the_file, int id) {
    streamoff start_position = cur_pos_in_the_file;
	data = ((static_cast<char*>(data))) + last_written_byte_position;
    fwrite(data, 1, bytes_number_to_write, data_file);
    streamoff end_position = ftell(data_file);
    save_file_name_and_reading_positions(id, current_file, start_position, end_position);
}

bool write_data(int id, void* data, size_t current_data_size) {
    FILE* data_file;
    string current_file;
    int last_written_byte_position = 0;
    while (current_data_size != 0) {
        current_file = get_new_file_path();
        data_file = fopen(current_file.c_str(), "a+b");
        size_t file_free_space = get_file_free_space(data_file);
        while (file_free_space == 0) {
            fclose(data_file);
            current_file = get_new_file_path();
            data_file = fopen(current_file.c_str(), "a+b");
            file_free_space = get_file_free_space(data_file);
        }
        long int cur_pos_in_the_file = data_file_size - file_free_space;
        fseek(data_file, cur_pos_in_the_file, SEEK_SET);
        size_t bytes_number_to_write;
        if (current_data_size > file_free_space) {
            bytes_number_to_write = file_free_space;
            write_data_to_file(data, data_file, last_written_byte_position, bytes_number_to_write, current_file,
                               cur_pos_in_the_file, id);
            last_written_byte_position += bytes_number_to_write;
        }
        else {
            bytes_number_to_write = current_data_size;
            write_data_to_file(data, data_file, last_written_byte_position, bytes_number_to_write, current_file,
                               cur_pos_in_the_file, id);
        }
        current_data_size -= bytes_number_to_write;
        is_data_size_invalid(current_data_size);
        fclose(data_file);
    }
    return true;
}

void save_current_id_in_map_to_file(int id) {
    ofstream file;
    string file_name = path + "map.dat";
    file.open(file_name, ios::out | ios::app);
    file << "id" << "\n" << id << "\n" << "--" << "\n";
    file << "file_names" << "\n";
    for (const auto& current_filename: indexes[id].file_names) {
        file << current_filename << "\n";
    }
    file << "--" << "\n";
    file << "start_reading_positions" << "\n";
    for (const auto& current_start_read_pos: indexes[id].start_reading_positions) {
        file << current_start_read_pos << "\n";
    }
    file << "--" "\n";
    file << "end_reading_positions" << "\n";
    for (const auto& current_end_read_pos: indexes[id].end_reading_positions) {
        file << current_end_read_pos << "\n";
    }
    file << "--" "\n";
    file << "data_size" << "\n";
    file << indexes[id].data_size << "\n";
    file << "--" << "\n";
}

bool load_map_from_file() {
    int id;
    index_data current_id;
    ifstream file;
    string file_name = path + "map.dat";
    file.open(file_name, ios::in);
    string s;
    int a;
    streamoff b;
    while (getline (file, s)) {
        if (s == "id") {
            getline (file, s);
            istringstream iss(s);
            iss >> a;
            id = a;
        }
        if (s == "file_names") {
            while (getline(file, s)) {
                if (s == "--") {
                    break;
                }
                current_id.file_names.push_back(s);
            }
        }
        if (s == "start_reading_positions") {
            while (getline(file, s)) {
                if (s == "--") {
                    break;
                }
                istringstream iss(s);
                iss >> b;
                current_id.start_reading_positions.push_back(b);
            }
        }
        if (s == "end_reading_positions") {
            while (getline(file, s)) {
                if (s == "--") {
                    break;
                }
                istringstream iss(s);
                iss >> b;
                current_id.end_reading_positions.push_back(b);
            }
        }
        if (s == "data_size") {
            getline(file, s);
            istringstream iss(s);
            iss >> a;
            current_id.data_size = a;
            indexes[id] = current_id;
            current_id.file_names.clear();
            current_id.start_reading_positions.clear();
            current_id.end_reading_positions.clear();
        }
    }
    return true;
}

void get_deleted_indexes() {
    ifstream file;
    string file_name = path + "deleted_indexes.dat";
    file.open(file_name, ios::in);
    int id;
    while (file >> id) {
        indexes[id].deleted = true;
    }
}

bool store(int id, void* data, size_t data_size) {
    check_settings();
    if (check_if_current_id_is_already_exists(id)) {
        throw runtime_error("Current id is already in use");
    }
    save_current_data_size(id, data_size);
    write_data(id, data, data_size);
    save_current_id_in_map_to_file(id);
    return true;
}

bool store_helper(int id, int data) {
    size_t data_size = sizeof(int);
    return store(id, &data, data_size);
}

bool store_helper(int id, double data) {
    size_t data_size = sizeof(double);
    return store(id, &data, data_size);
}

bool store_helper(int id, const string &data) {
	size_t data_size = data.size();
	char* casted_string = new char[data_size + 1];
	strcpy(casted_string, data.c_str());
	casted_string[data_size] = '\0';
    bool store_result = store(id, casted_string, data_size + 1);
    delete[] casted_string;
    return store_result;
}

bool store_helper(int id, const vector <int> &data) {
    size_t data_size = 40;
    data_size += sizeof(int) * data.size();
    int* casted_vector = new int[data_size];
    memset(&casted_vector[0], 0, 10);
    int data_size_copy = static_cast<int>(data_size);
    for (int i = 9; i >= 0; i--) {
        casted_vector[i] = data_size_copy % 10;
        data_size_copy /= 10;
    }
    for (unsigned int i = 0; i < data.size(); i++) {
        casted_vector[10 + i] = data[i];
    }
    bool store_result = store(id, casted_vector, data_size);
	delete[] casted_vector;
    return store_result;
}

bool store_helper(int id, const vector <double> &data) {
    size_t data_size = 80;
    data_size += sizeof(double) * data.size();
    double* casted_vector = new double[data_size];
    memset(&casted_vector[0], 0, 10);
    size_t data_size_copy = data_size;
    for (int i = 9; i >= 0; i--) {
        casted_vector[i] = data_size_copy % 10;
        data_size_copy /= 10;
    }
    for (unsigned int i = 0; i < data.size(); i++) {
        casted_vector[10 + i] = data[i];
    }
    bool store_result = store(id, casted_vector, data_size);
	delete[] casted_vector;
    return store_result;
}

bool store_helper(int id, const vector <string> &data) {
    size_t data_size = 10;
    vector <int> string_sizes;
    string_sizes.push_back(data.size());
    for (const auto &str: data) {
        data_size += str.size();
        string_sizes.push_back(str.size());
    }
    int meta_data_id = rand();
    store_helper(meta_data_id, string_sizes);
    string temp = to_string(meta_data_id);
	const char *charnum = temp.c_str();
    char number[10];
    memset(&number[0], '/', 10);
    for (size_t i = 0; i < temp.size(); i++) {
        number[i] = *(charnum + i);
    }
    char* casted_vector = new char[data_size + 1];
    memset(&casted_vector[0], 0, data_size);
    for (int i = 0; i < 10; i++) {
        casted_vector[i] = number[i];
    }
    int i = 10;
    for (const auto &current_string : data) {
        for (char current_char : current_string) {
            casted_vector[i] = current_char;
            i++;
        }
    }
	casted_vector[data_size] = '\0';
    bool store_result = store(id, casted_vector, data_size + 1);
	delete[] casted_vector;
    return store_result;
}

void* load(int id) {
    if (check_if_current_id_does_not_exists(id)) {
		throw runtime_error("Current data does not exists!");
    }
    if (check_if_current_id_was_deleted(id)) {
        throw runtime_error("Current data was deleted!");
    }
    FILE* data_file;
    size_t current_data_size = indexes[id].data_size;
    void* return_data = malloc(current_data_size);
    int last_written_byte_position_in_main_buffer = 0;
    for (unsigned int i = 0; i < indexes[id].file_names.size(); i++) {
        const char* current_filename = indexes[id].file_names[i].c_str();
        streamoff start_reading_position = indexes[id].start_reading_positions[i];
		streamoff end_reading_position = indexes[id].end_reading_positions[i];
        size_t reading_bytes_number = static_cast<size_t>(end_reading_position - start_reading_position);
        data_file = fopen(current_filename, "a+b");
        void* current_read_data = malloc(reading_bytes_number);
        fseek(data_file, static_cast<long int>(start_reading_position), SEEK_SET);
        fread(current_read_data, 1, reading_bytes_number, data_file);
        char* cur_pos = static_cast<char*>(return_data) + last_written_byte_position_in_main_buffer;
        memcpy(cur_pos, current_read_data, reading_bytes_number);
        last_written_byte_position_in_main_buffer += reading_bytes_number;
        free(current_read_data);
		fclose(data_file);
    }
    return return_data;
}

template <class T>
T load_helper(int id) {
    throw runtime_error("not implemented");
}

template <>
int load_helper<int>(int id) {
    int* loaded_data =  static_cast<int*>(load(id));
    int return_data = *loaded_data;
    delete[] loaded_data;
    return return_data;
}

template <>
double load_helper<double>(int id) {
    double* loaded_data =  static_cast<double*>(load(id));
    double return_data = *loaded_data;
    delete[] loaded_data;
    return return_data;
}

template <>
string load_helper<string>(int id) {
    char* loaded_data = static_cast<char*>(load(id));
    string return_data(loaded_data);
    delete[] loaded_data;
    return return_data;
}

vector <int> load_int_vector_helper(int id) {
    int* loaded_data = static_cast<int*>(load(id));
    int data_size = 0;
    for (int i = 0; i < 10; i++) {
        data_size *= 10;
        data_size += loaded_data[i];
    }
    data_size -= 40;
    vector <int> return_data;
    for (unsigned int i = 0; i < data_size / sizeof(int); i++) {
        return_data.push_back(loaded_data[10 + i]);
    }
    delete[] loaded_data;
    return return_data;
}

vector <double> load_double_vector_helper(int id) {
    double* loaded_data = static_cast<double*>(load(id));
    int data_size = 0;
    for (int i = 0; i < 10; i++) {
        data_size *= 10;
        data_size += static_cast<int>(loaded_data[i]);
    }
    data_size -= 80;
    vector <double> return_data;
    for (unsigned int i = 0; i < data_size / sizeof(double); i++) {
        return_data.push_back(loaded_data[10 + i]);
    }
    delete[] loaded_data;
    return return_data;
}

vector <string> load_string_vector_helper(int id) {
    char* main_data = static_cast<char*>(load(id));
    char not_casted_meta_data_id[10];
    for (int i = 0; i < 10; i++) {
        if (main_data[i] == '/') {
            break;
        }
        not_casted_meta_data_id[i] = main_data[i];
    }
    int meta_data_id = stoi(not_casted_meta_data_id);
    int* meta_data = (static_cast<int*>(load(meta_data_id)));
    int strings_number = meta_data[10];
    vector <int> string_sizes;
    for (int i = 1; i <= strings_number; i++) {
        string_sizes.push_back(meta_data[10 + i]);
    }
    char* iter = main_data + 10;
    vector <string> return_data(strings_number);
    for (int i = 0; i < strings_number; i++) {
        for (int ii = 0; ii < string_sizes[i]; ii++) {
            return_data[i].push_back(*iter);
            iter++;
        }
    }
    delete[] main_data;
    delete[] meta_data;
    return return_data;
}