#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <cmath>
#include <cstdlib>
#include <set>

using namespace std;

const int NOT_FOUND = -1;
const int NUMBER_OF_ENTRY_POINTED_TO_NAME_OF_THE_FILES_FOLDER = 1, NUMBER_OF_MONTH_DAYS = 30;
const string EMPLOYEES_FILE_NAME = "employees.csv", WORKING_HOURS_FILE_NAME = "working_hours.csv",
             TEAMS_FILE_NAME = "teams.csv", SALARY_CONFIGS_FILE_NAME = "salary_configs.csv",
             CURRENT_DIRECTORY = "./";
const char SLASH = '/', COMMA = ',', DASH = '-', DOLLAR = '$';

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
    int get_day() { return day; }
    int calculate_lenght_of_working_invertal() { return (working_invertal.second - working_invertal.first); }
    WorkingHour(int day, pair<int, int> working_invertal);
};
class Team
{
    int team_id;
    int team_head_id;
    vector<int> member_ids;
    int bonus_percentage = 0;
    int bonus_min_working_hours;
    float bonus_working_hours_max_variance;

public:
    int get_team_id() { return team_id; }
    int get_bonus_percentage() { return bonus_percentage; }
    Team(int team_id, int team_head_id, vector<int> member_ids, int bonus_min_working_hours, float bonus_working_hours_max_variance);
    bool is_id_in_team(int id)
    {
        if (find(member_ids.begin(), member_ids.end(), id) != member_ids.end())
            return true;
        return false;
    }
};
class SalaryConfig /////////*********مرتب کنم
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
    // int calculate_total_earing(float salary, int bonus);
    int calculate_tax(float salary, int bonus);
    float calculate_salary(int working_hours);
    int get_tax_percentage() { return tax_percentage; }
};
class Employee
{
    int id;
    string name;
    int age;
    Level level;
    vector<WorkingHour> working_hours;
    float salary = 0.0;
    float bonus = 0.0;

public:
    int get_age() { return age; }
    int get_id() { return id; }
    string get_name() { return name; }
    Level get_level() { return level; }
    Employee(int id, string name, int age, Level level, vector<WorkingHour> working_hours);
    int calculate_total_working_hour();
    int calculate_absent_days();
    void report_salary_short(vector<SalaryConfig> &salary_configs);
    void preparing_for_report_salary(vector<Team> &teams, vector<SalaryConfig> &salary_configs);
    int calculate_total_earing(float tax);
    float set_salary(float _salary)
    {
        salary = _salary;
        return _salary;
    }
    float calculate_and_set_bonus(int bonus_percentage);
    int find_team_index(vector<Team> &teams);
    void report_salary_long(vector<SalaryConfig> &salary_configs, int team_id);
    string level_to_string(Level level);
    int give_team_id(vector<Team> teams);
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
    vector<string> fields = split_line_of_string_by_specific_char(str, DASH);
    return {stoi(fields[0]), stoi(fields[1])};
}
string delete_first_field_of_csv_line(string csv_line)
{
    return csv_line.substr(csv_line.find(COMMA) + 1);
}
WorkingHour convert_csv_line_to_working_hour(string csv_line)
{
    vector<string> csv_fields = split_line_of_string_by_specific_char(delete_first_field_of_csv_line(csv_line), COMMA);
    return WorkingHour(stoi(csv_fields[0]), convert_string_to_invertal(csv_fields[1]));
}
string first_word_of_csv_line(string line)
{
    return line.substr(0, line.find(COMMA));
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
    vector<string> csv_fields = split_line_of_string_by_specific_char(csv_line, COMMA);
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
    return CURRENT_DIRECTORY + folder_name + SLASH; /////////////***************
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
    vector<string> csv_fields = split_line_of_string_by_specific_char(csv_line, COMMA);
    vector<string> member_ids_str = split_line_of_string_by_specific_char(csv_fields[2], DOLLAR);
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
    vector<string> csv_fields = split_line_of_string_by_specific_char(csv_line, COMMA);
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

int SalaryConfig::calculate_tax(float salary, int bonus)
{
    salary += salary * bonus / 100;
    float tax = salary * tax_percentage;
    return round(tax);
}
// int SalaryConfig::calculate_total_earing(float salary, int bonus)
// {
//     salary += salary * bonus / 100;
//     salary -= salary * tax_percentage; //////////////////***************************************غلط
//     return round(salary);
// }

float Employee::calculate_and_set_bonus(int bonus_percentage)
{
    bonus = salary * bonus_percentage / 100;
    return bonus;
}

int Employee::calculate_total_earing(float tax)
{

    return round(salary + bonus - tax);
}

float SalaryConfig::calculate_salary(int total_working_hours)
{
    float salary = base_salary;
    if (total_working_hours >= official_working_hours)
    {
        int extra_hours = total_working_hours - official_working_hours;
        salary += official_working_hours * salary_per_hour + extra_hours * salary_per_extra_hour;
    }
    else
        salary += total_working_hours * salary_per_hour;
    return salary;
}

int Employee::calculate_total_working_hour()
{
    int total_working_hour = 0;
    for (auto working_hour : working_hours)
        total_working_hour += working_hour.calculate_lenght_of_working_invertal();
    return total_working_hour;
}

int Employee::find_team_index(vector<Team> &teams)
{
    for (int i = 0; i < teams.size(); i++)
        if (teams[i].is_id_in_team(id))
            return i;
    return NOT_FOUND;
}

int Employee::calculate_absent_days()
{
    // int absent_days = 0;
    // vector<int> days(30, 0);
    // for (auto working_hour : working_hours)
    //     days[working_hour.get_day() - 1] = 1;
    // for (auto num : days)
    //     if (num == 0)
    //         absent_days++;
    // return absent_days;
    set<int> unique_days;
    for (auto working_hour : working_hours)
        unique_days.insert(working_hour.get_day());
    return NUMBER_OF_MONTH_DAYS - unique_days.size();
}

void Employee::report_salary_short(vector<SalaryConfig> &salary_configs)
{
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Total Working Hours: " << calculate_total_working_hour() << endl;
    cout << "Total Earning: " << calculate_total_earing(salary_configs[level].calculate_tax(salary, bonus)) << endl;
    cout << "---" << endl;
}

// void report_salaries(const vector<Employee> &employees, vector<SalaryConfig> &salary_configs, const vector<Team> &teams)
// {
//     for (auto employee : employees)
//     {
//         int bonus = 0;
//         for (auto team : teams)
//         {
//             if (team.is_id_in_team(employee.get_id()))
//                 bonus = team.get_bonus();
//         }
//         cout << "ID : " << employee.get_id() << endl;
//         cout << "Name : " << employee.get_name() << endl;
//         cout << "Total Working Hours : " << employee.calculate_total_working_hour() << endl;
//         cout << "Total Earning : " << salary_configs[employee.get_level()].calculate_total_earing(salary_configs[employee.get_level()].calculate_salary(employee.calculate_total_working_hour()), bonus) << endl;
//         cout << "---" << endl;
//     }
// }

void report_salaries(const vector<Employee> &employees, vector<Team> &teams, vector<SalaryConfig> &salary_configs)
{
    for (auto employee : employees)
    {
        employee.preparing_for_report_salary(teams, salary_configs);
        employee.report_salary_short(salary_configs);
    }
}

void Employee::preparing_for_report_salary(vector<Team> &teams, vector<SalaryConfig> &salary_configs)
{
    salary = salary_configs[level].calculate_salary(calculate_total_working_hour());
    bonus = calculate_and_set_bonus(teams[find_team_index(teams)].get_bonus_percentage());
}

int find_employee_index(int id, vector<Employee> &employees)
{
    for (int i = 0; i < employees.size(); i++)
        if (employees[i].get_id() == id)
            return i;
    return NOT_FOUND;
}

string Employee::level_to_string(Level level)
{
    switch (level)
    {
    case 0:
        return "junior";
    case 1:
        return "expert";
    case 2:
        return "senior";
    case 3:
        return "team_lead";
    }
    exit(EXIT_FAILURE);
}

// void report_employee_salary(int employee_id, vector<Employee> &employees, vector<SalaryConfig> &salary_configs, vector<Team> &teams)
// {
//     int employee_index = find_employee_index(employee_id, employees);
//     int bonus = 0;
//     if (employee_index == NOT_FOUND)
//     {
//         cout << "EMPLOYEE_NOT_FOUND" << endl;
//         return;
//     }
//     int team_index = find_team_index(employee_id, teams);
//     cout << "ID : " << employees[employee_index].get_id() << endl;
//     cout << "Name : " << employees[employee_id].get_name() << endl;
//     cout << "Age : " << employees[employee_index].get_age() << endl;
//     cout << "Level : " << level_to_string(employees[employee_index].get_level()) << endl;
//     if (team_index == NOT_FOUND)
//         cout << "Team ID : "
//              << "N/A" << endl;
//     else
//     {
//         bonus = teams[team_index].get_bonus();
//         cout << "Team ID : " << teams[team_index].get_team_id() << endl;
//     }
//     cout << "Total Working Hours : " << employees[employee_index].calculate_total_working_hour() << endl;
//     cout << "Absent Days : " << employees[employee_index].calculate_absent_days() << endl;
//     cout << "Salary " << salary_configs[employees[employee_index].get_level()].calculate_salary(employees[employee_index].calculate_total_working_hour()) << endl;

//     cout << "Bonus : " << bonus << endl;
//     cout << "Tax : " << salary_configs[employees[employee_index].get_level()].calculate_tax(salary_configs[employees[employee_index].get_level()].calculate_salary(employees[employee_index].calculate_total_working_hour()), bonus) << endl;
//     cout << "Total Earning : " << salary_configs[employees[employee_index].get_level()].calculate_total_earing(employees[employee_index].calculate_total_working_hour(), bonus) << endl;
// }

void Employee::report_salary_long(vector<SalaryConfig> &salary_configs, int team_id)
{
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Level: " << level_to_string(level);
    if (team_id == NOT_FOUND)
        cout << "Team ID: " << "N/A" << endl;
    else
        cout << "Team ID: " << team_id << endl;
    cout << "Total Working Hours: " << calculate_total_working_hour() << endl;
    cout << "Absent Days: " << calculate_absent_days() << endl;
    cout << "Salary: " << salary << endl;
    cout << "Bonus: " << bonus << endl;
    float tax = salary_configs[level].calculate_tax(salary, bonus);
    cout << "Tax: " << tax << endl;
    cout << "Total Earning: " << calculate_total_earing(tax) << endl;
}

int Employee::give_team_id(vector<Team> teams)
{
    int team_index = find_team_index(teams);
    if (team_index == NOT_FOUND)
        return NOT_FOUND;
    else
        return teams[team_index].get_team_id();
}

void report_employee_salary(vector<Employee> &employees, vector<Team> &teams, vector<SalaryConfig> &salary_configs)
{
    int employee_id, team_id;
    cin >> employee_id;
    int employee_index = find_employee_index(employee_id, employees);
    if (employee_index == NOT_FOUND)
    {
        cout << "EMPLOYEE_NOT_FOUND" << endl;
        return;
    }
    employees[employee_index].preparing_for_report_salary(teams, salary_configs);
    employees[employee_index].report_salary_long(salary_configs, employees[employee_index].give_team_id(teams));
}

void get_order(vector<Employee> employees, vector<Team> teams, vector<SalaryConfig> salary_configs)
{
    string order;
    while (cin >> order)
        if (order == "report_salaries")
            report_salaries(employees, teams, salary_configs);
        else if (order == "report_employee_salary")
            report_employee_salary(employees, teams, salary_configs);
        else
            exit(EXIT_FAILURE); ////////////**********شاید باید پیغامی بدم
}

int main(int argc, char *argv[])
{
    const string files_directory = "./assets/"; // set_folder_of_files_directory(argv[NUMBER_OF_ENTRY_POINTED_TO_NAME_OF_THE_FILES_FOLDER]);
    vector<Employee> employees = get_employees_and_working_hours(files_directory + EMPLOYEES_FILE_NAME,
                                                                 files_directory + WORKING_HOURS_FILE_NAME);
    vector<Team> teams = get_teams(files_directory + TEAMS_FILE_NAME);
    vector<SalaryConfig> salary_configs = get_salary_configs(files_directory + SALARY_CONFIGS_FILE_NAME);
    get_order(employees, teams, salary_configs);
    return 0;
}