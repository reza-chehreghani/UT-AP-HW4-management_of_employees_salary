#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

const int number_of_entry_pointed_to_name_of_the_files_folder = 1;
const string employees_file_name = "employees.csv", working_hours_file_name = "working_hours.csv",
             teams_file_name = "teams.csv", salary_configs_file_name = "salary_configs.csv",
             current_directory = "./";
const char slash = '/';

enum Level {JUNIOR, EXPERT, SENIOR, TEAM_LEAD};

map<string, Level> convert_string_to_Level = {{"junior", JUNIOR}, {"expert", EXPERT},
                                              {"senior", SENIOR}, {"team_lead", TEAM_LEAD}};

class WorkingHour {
    int day;
    pair<int, int> working_invertal;
public:
};
class Employee {
    int id;
    string name;
    int age;
    Level level;
    vector<WorkingHour> working_hours;
public:
    Employee(int id, string name, int age, Level level);
};
class Team {
    int team_id, team_head_id;
    vector<int> member_ids;
    int bonus_min_working_hours;
    float bonus_working_hours_max_variance;
public:
};
class SalaryConfig {
    Level level;
    int base_salary, salary_per_hour, salary_per_extra_hour, official_working_hours, tax_percentage;
public:
};

Employee::Employee(int id, string name,int age, Level level) : id(id), name(name), age(age), level(level) {}

string delete_bachslash_r_if_it_was_at_the_end_of_the_word(string word) {
    return (word.back() == '\r') ? word.substr(0, word.size() - 1) : word;
}

vector<string> split_line_of_string_by_commas(string line_string) {
    istringstream line_stream(line_string);
    vector<string> fields;
    string word;
    while(getline(line_stream, word, ','))
        fields.push_back(word);
    fields.back() = delete_bachslash_r_if_it_was_at_the_end_of_the_word(fields.back());
    return fields;
}

Employee convert_line_of_csv_to_employee(string line_of_csv) {
    vector<string> csv_fields = split_line_of_string_by_commas(line_of_csv);
    // Employee a(stoi(csv_fields[0]), csv_fields[1], stoi(csv_fields[2]), convert_string_to_Level[csv_fields[3]]);
    // return a;
    return Employee(stoi(csv_fields[0]), csv_fields[1], stoi(csv_fields[2]), convert_string_to_Level[csv_fields[3]]);
}

void skip_header_row_of_csv(ifstream &csv_file) {
    string line;
    getline(csv_file, line);
}

vector<Employee> get_employees(string file_directory) {
    vector<Employee> employees;
    ifstream employees_csv(file_directory);
    skip_header_row_of_csv(employees_csv);
    string line;
    while(getline(employees_csv, line))
        employees.push_back(convert_line_of_csv_to_employee(line));
    employees_csv.close();
    return employees;
}

string set_folder_of_files_directory(string folder_name) {
    return current_directory + folder_name + slash; /////////////***************
}

int main(int argc, char *argv[]) {
    const string folder_of_files_directory = "./assets/"; // set_folder_of_files_directory(argv[number_of_entry_pointed_to_name_of_the_files_folder]);
    vector<Employee> employees = get_employees(folder_of_files_directory + employees_file_name);
    vector<Team> teams;
    vector<SalaryConfig> salary_configs (sizeof(Level));
}