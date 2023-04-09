#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <map>
#include <string>

using namespace std;

const int number_of_entry_pointed_to_name_of_the_files_folder = 1;
const string employees_file_name = "employees.csv", working_hours_file_name = "working_hours.csv",
             teams_file_name = "teams.csv", salary_configs_file_name = "salary_configs.csv",
             current_directory = "./";
const char slash = '/', comma = ',', dash = '-';

enum Level
{
    JUNIOR,
    EXPERT,
    SENIOR,
    TEAM_LEAD
};

map<string, Level> convert_string_to_Level = {{"junior", JUNIOR},
                                              {"expert", EXPERT},
                                              {"senior", SENIOR},
                                              {"team_lead", TEAM_LEAD}};

class WorkingHour
{
    int day;
    pair<int, int> working_invertal;

public:
    WorkingHour(int day, pair<int, int> working_invertal);
};
class Employee
{
    int id;
    string name;
    int age;
    Level level;
    vector<WorkingHour> working_hours;

public:
    Employee(int id, string name, int age, Level level, vector<WorkingHour> working_hours);
};
class Team
{
    int team_id, team_head_id;
    vector<int> member_ids;
    int bonus_min_working_hours;
    float bonus_working_hours_max_variance;

public:
};
class SalaryConfig
{
    Level level;
    int base_salary, salary_per_hour, salary_per_extra_hour, official_working_hours, tax_percentage;

public:
};

Employee::Employee(int _id, string _name, int _age, Level _level, vector<WorkingHour> _working_hours)
    : id(_id), name(_name), age(_age), level(_level), working_hours(_working_hours) {}

WorkingHour::WorkingHour(int _day, pair<int, int> _working_invertal)
    : day(_day), working_invertal(_working_invertal) {}

pair<int, int> split_string_by_dash_and_convert_to_invertal(string str)
{
    return {stoi(str.substr(0, str.find(dash))), stoi(str.substr(str.find(dash) + 1))};
}

string delete_bachslash_r_if_it_was_at_the_end_of_the_word(string word)
{
    return (word.back() == '\r') ? word.substr(0, word.size() - 1) : word;
}

vector<string> split_line_of_string_by_commas(string line_string)
{
    istringstream line_stream(line_string);
    vector<string> fields;
    string word;
    while (getline(line_stream, word, ','))
        fields.push_back(word);
    fields.back() = delete_bachslash_r_if_it_was_at_the_end_of_the_word(fields.back());
    return fields;
}

string delete_first_field_of_csv_line(string csv_line)
{
    return csv_line.substr(csv_line.find(comma) + 1);
}

WorkingHour convert_csv_line_to_working_hour(string line)
{
    vector<string> csv_fields = split_line_of_string_by_commas(delete_first_field_of_csv_line(line));
    return WorkingHour(stoi(csv_fields[0]), split_string_by_dash_and_convert_to_invertal(csv_fields[1]));
}

string first_word_of_csv_line(string line)
{
    return line.substr(0, line.find(comma));
}

void skip_header_row_of_csv(ifstream &csv_file)
{
    string line;
    getline(csv_file, line);
}

vector<WorkingHour> get_working_hours(string file_directory, int employee_id)
{
    vector<WorkingHour> working_hours;
    ifstream working_hours_csv(file_directory);
    skip_header_row_of_csv(working_hours_csv);
    string line;
    while (getline(working_hours_csv, line))
        if (employee_id == stoi(first_word_of_csv_line(line)))
            working_hours.push_back(convert_csv_line_to_working_hour(line));
    working_hours_csv.close();
    return working_hours;
}

Employee convert_csv_line_to_employee(string line_of_csv, string file_directory)
{
    vector<string> csv_fields = split_line_of_string_by_commas(line_of_csv);
    int id = stoi(csv_fields[0]);
    return Employee(id, csv_fields[1], stoi(csv_fields[2]),
                    convert_string_to_Level[csv_fields[3]],
                    get_working_hours(file_directory, id));
}

vector<Employee> get_employees_and_working_hours(const string employees_file_directory,
                                                 const string working_hours_file_directory)
{
    vector<Employee> employees;
    ifstream employees_csv(employees_file_directory);
    skip_header_row_of_csv(employees_csv);
    string line;
    while (getline(employees_csv, line))
        employees.push_back(convert_csv_line_to_employee(line, working_hours_file_directory));
    employees_csv.close();
    return employees;
}

string set_folder_of_files_directory(string folder_name)
{
    return current_directory + folder_name + slash; /////////////***************
}

int main(int argc, char *argv[])
{
    const string files_directory = "./assets/"; // set_folder_of_files_directory(argv[number_of_entry_pointed_to_name_of_the_files_folder]);
    vector<Employee> employees = get_employees_and_working_hours(files_directory + employees_file_name,
                                                                 files_directory + working_hours_file_name);
    vector<Team> teams;
    vector<SalaryConfig> salary_configs(sizeof(Level));
}