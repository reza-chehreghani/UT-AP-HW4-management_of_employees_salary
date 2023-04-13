#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <cmath>

using namespace std;

const int NOT_FOUND = -1;
const int number_of_entry_pointed_to_name_of_the_files_folder = 1;
const string employees_file_name = "employees.csv", working_hours_file_name = "working_hours.csv",
             teams_file_name = "teams.csv", salary_configs_file_name = "salary_configs.csv",
             current_directory = "./";
const char slash = '/', comma = ',', dash = '-', dollar = '$';

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
    int get_day(){return day;}
    pair <int, int> get_working_invertal(){return working_invertal;}
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
    int get_age(){return age;}
    int get_id(){return id;}
    string get_name(){return name;}
    Level get_level(){return level;}
    Employee(int id, string name, int age, Level level, vector<WorkingHour> working_hours);
    int find_total_working_hour();
    int find_absent_days();
};
class Team
{
    int team_id;
    int team_head_id;
    vector<int> member_ids;
    int bonus = 0;
    int bonus_min_working_hours;
    float bonus_working_hours_max_variance;

public:
    int get_team_id(){return team_id;}
    int get_bonus(){return bonus;}
    Team(int team_id, int team_head_id, vector<int> member_ids, int bonus_min_working_hours, float bonus_working_hours_max_variance);
    bool is_id_in_team(int id)
    {
        if(find(member_ids.begin(), member_ids.end(), id) != member_ids.end())
            return true;
        return false;
        }
};
class SalaryConfig
{
    Level level;
    int base_salary;
    int salary_per_hour;
    int salary_per_extra_hour;
    int official_working_hours;
    int tax_percentage;

public:
    SalaryConfig(Level level, int base_salary, int salary_per_hour, int salaty_per_extra_hour,
                 int official_working_hours, int tax_percentage);
    int find_total_salary(float salary, int bonus);
    int find_tax(float salary, int bonus);
    float find_salary(int working_hours);
};

Employee::Employee(int _id, string _name, int _age, Level _level, vector<WorkingHour> _working_hours)
    : id(_id), name(_name), age(_age), level(_level), working_hours(_working_hours) {}

WorkingHour::WorkingHour(int _day, pair<int, int> _working_invertal)
    : day(_day), working_invertal(_working_invertal) {}

Team::Team(int _team_id, int _team_head_id, vector<int> _member_ids,
           int _bonus_min_working_hours, float _bonus_working_hours_max_variance)
    : team_id(_team_id), team_head_id(_team_head_id), member_ids(_member_ids),
      bonus_min_working_hours(_bonus_min_working_hours), bonus_working_hours_max_variance(_bonus_working_hours_max_variance) {}

SalaryConfig::SalaryConfig(Level _level, int _base_salary, int _salary_per_hour, int _salaty_per_extra_hour,
                           int _official_working_hours, int _tax_percentage)
    : level(_level), base_salary(_base_salary), salary_per_hour(_salary_per_hour), salary_per_extra_hour(_salaty_per_extra_hour),
      official_working_hours(_official_working_hours), tax_percentage(_tax_percentage) {}

string delete_bachslash_r_if_it_was_at_the_end_of_the_word(string word)
{
    return (word.back() == '\r') ? word.substr(0, word.size() - 1) : word;
}
vector<string> split_line_of_string_by_specific_char(string line_string, char specific_char)
{
    istringstream line_stream(line_string);
    vector<string> fields;
    string word;
    while (getline(line_stream, word, specific_char))
        fields.push_back(word);
    fields.back() = delete_bachslash_r_if_it_was_at_the_end_of_the_word(fields.back());
    return fields;
}
pair<int, int> convert_string_to_invertal(string str)
{
    vector<string> fields = split_line_of_string_by_specific_char(str, dash);
    return {stoi(fields[0]), stoi(fields[1])};
}
string delete_first_field_of_csv_line(string csv_line)
{
    return csv_line.substr(csv_line.find(comma) + 1);
}
WorkingHour convert_csv_line_to_working_hour(string csv_line)
{
    vector<string> csv_fields = split_line_of_string_by_specific_char(delete_first_field_of_csv_line(csv_line), comma);
    return WorkingHour(stoi(csv_fields[0]), convert_string_to_invertal(csv_fields[1]));
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
Employee convert_csv_line_to_employee(string csv_line, string file_directory)
{
    vector<string> csv_fields = split_line_of_string_by_specific_char(csv_line, comma);
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
vector<int> convert_vector_string_to_vector_int(vector<string> numbers_str)
{
    int size_vector = numbers_str.size();
    vector<int> numbers(size_vector);
    for (int i = 0; i < size_vector; i++)
        numbers[i] = stoi(numbers_str[i]);
    return numbers;
}
Team convert_csv_line_to_team(string csv_line)
{
    vector<string> csv_fields = split_line_of_string_by_specific_char(csv_line, comma);
    vector<string> member_ids_str = split_line_of_string_by_specific_char(csv_fields[2], dollar);
    return Team(stoi(csv_fields[0]), stoi(csv_fields[1]), convert_vector_string_to_vector_int(member_ids_str),
                stoi(csv_fields[3]), stof(csv_fields[4]));
}
vector<Team> get_teams(const string teams_file_directory)
{
    vector<Team> teams;
    ifstream teams_csv(teams_file_directory);
    skip_header_row_of_csv(teams_csv);
    string line;
    while (getline(teams_csv, line))
        teams.push_back(convert_csv_line_to_team(line));
    teams_csv.close();
    return teams;
}
SalaryConfig convert_csv_line_to_salary_config(string csv_line)
{
    vector<string> csv_fields = split_line_of_string_by_specific_char(csv_line, comma);
    return SalaryConfig(convert_string_to_Level[csv_fields[0]], stoi(csv_fields[1]), stoi(csv_fields[2]),
                        stoi(csv_fields[3]), stoi(csv_fields[4]), stoi(csv_fields[5]));
}
vector<SalaryConfig> get_salary_configs(const string salary_configs_file_directory)
{
    vector<SalaryConfig> salary_configs;
    ifstream salary_configs_csv(salary_configs_file_directory);
    skip_header_row_of_csv(salary_configs_csv);
    string line;
    while (getline(salary_configs_csv, line))
        salary_configs.push_back(convert_csv_line_to_salary_config(line));
    salary_configs_csv.close();
    return salary_configs;
}

int SalaryConfig :: find_tax(float salary, int bonus)
{
    // float salary = 0;
    // salary += base_salary;
    // if (working_hours >= official_working_hours)
    // {
    //     int extra_hours = working_hours - official_working_hours;
    //     salary += official_working_hours * salary_per_hour + extra_hours * salary_per_extra_hour;
    // }
    // else
    //     salary += working_hours * salary_per_hour;
    salary += salary * (float)bonus / 100;
    float tax = salary * tax_percentage;
    tax = round(tax);
    return tax;
}
int SalaryConfig :: find_total_salary(float salary, int bonus)
{
    // float salary = 0;
    // salary += base_salary;
    // if (working_hours >= official_working_hours)
    // {
    //     int extra_hours = working_hours - official_working_hours;
    //     salary += official_working_hours * salary_per_hour + extra_hours * salary_per_extra_hour;
    // }
    // else
    //     salary += working_hours * salary_per_hour;
    salary += salary * (float)bonus / 100;
    salary -= salary * tax_percentage;
    salary = round(salary);
    return salary;
}

float SalaryConfig :: find_salary(int working_hours)
    {
        float salary = 0;
        salary += base_salary;
        if (working_hours >= official_working_hours)
        {
            int extra_hours = working_hours - official_working_hours;
            salary += official_working_hours * salary_per_hour + extra_hours * salary_per_extra_hour;
        }
        else
            salary += working_hours * salary_per_hour;
        return salary;
    }

int Employee :: find_total_working_hour()
{
    int total_working_hour = 0;
    for(auto working_hour : working_hours)
    {
        total_working_hour += working_hour.get_working_invertal().second - working_hour.get_working_invertal().first;
    }
    return total_working_hour;
}

int find_team_index(int employee_id, vector<Team>& teams)
{
    for (int i = 0; i < teams.size(); i++)
        if (teams[i].is_id_in_team(employee_id))
            return i;
    return NOT_FOUND;
}

int Employee :: find_absent_days()
{
    int absent_days = 0;
    vector <int> days(30, 0);
    for (auto working_hour : working_hours)
        days[working_hour.get_day() - 1] = 1;
    for (auto num : days)
        if (num == 0)
            absent_days++;
    return absent_days;

}

void report_salaries(const vector<Employee>& employees, vector<SalaryConfig>& salary_configs, const vector<Team>& teams)
{
    for (auto employee : employees)
    {
        int bonus = 0;
        for (auto team :teams)
        {
            if (team.is_id_in_team(employee.get_id()))
                bonus = team.get_bonus();
        }
        cout << "ID : " << employee.get_id() << endl;
        cout << "Name : " << employee.get_name() << endl;
        cout << "Total Working Hours : " << employee.find_total_working_hour() << endl;
        cout << "Total Earning : " << salary_configs[employee.get_level()].find_total_salary(salary_configs[employee.get_level()].find_salary(employee.find_total_working_hour()), bonus) << endl;
        cout << "---" << endl;
    }
}
int find_employee_index(int id, vector<Employee>& employees)
{
    for (int i = 0; i < employees.size(); i++)
        if (employees[i].get_id() == id)
            return i;
    return NOT_FOUND;
}

string level_to_string(Level level)
{
    if (level == 0)
        return "junior";
    else if (level == 1)
        return "expert";
    else if (level == 2)
        return "senior";
    else if (level == 3)
        return "team_lead";
}


void report_employee_salary(int employee_id, vector<Employee>& employees, vector<SalaryConfig>& salary_configs, vector<Team>& teams)
{
    int employee_index = find_employee_index(employee_id, employees);
    int bonus = 0;
    if(employee_index == NOT_FOUND)
    {
        cout << "EMPLOYEE_NOT_FOUND" << endl;
        return;
    }
    int team_index = find_team_index(employee_id, teams);
    cout << "ID : " << employees[employee_index].get_id() << endl;
    cout << "Name : " << employees[employee_id].get_name() << endl;
    cout << "Age : " << employees[employee_index].get_age() << endl;
    cout << "Level : " << level_to_string(employees[employee_index].get_level()) << endl;
    if (team_index == NOT_FOUND)
        cout << "Team ID : " << "N/A" << endl;
    else
    { 
        bonus = teams[team_index].get_bonus();
        cout << "Team ID : " << teams[team_index].get_team_id() << endl;
    }
    cout << "Total Working Hours : " << employees[employee_index].find_total_working_hour() << endl;
    cout << "Absent Days : " << employees[employee_index].find_absent_days() << endl;
    cout << "Salary " << salary_configs[employees[employee_index].get_level()].find_salary(employees[employee_index].find_total_working_hour()) << endl;;
    cout << "Bonus : " << bonus << endl; 
    cout << "Tax : " << salary_configs[employees[employee_index].get_level()].find_tax(salary_configs[employees[employee_index].get_level()].find_salary(employees[employee_index].find_total_working_hour()), bonus) << endl;
    cout << "Total Earning : " << salary_configs[employees[employee_index].get_level()].find_total_salary(employees[employee_index].find_total_working_hour(), bonus) << endl;
}

int main(int argc, char *argv[])
{
    const string files_directory = "./assets/"; // set_folder_of_files_directory(argv[number_of_entry_pointed_to_name_of_the_files_folder]);
    vector<Employee> employees = get_employees_and_working_hours(files_directory + employees_file_name,
                                                                 files_directory + working_hours_file_name);
    vector<Team> teams = get_teams(files_directory + teams_file_name);
    vector<SalaryConfig> salary_configs = get_salary_configs(files_directory + salary_configs_file_name);
}