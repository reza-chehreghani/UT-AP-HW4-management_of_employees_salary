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
#include <iomanip>

using namespace std;

const int START_OF_MONTH = 1;
const int MONTH_DAYS_COUNT = 30;
const int END_OF_MONTH = 30;
const int START_OF_DAY = 0;
const int END_OF_DAY = 24;
const int NOT_FOUND = -1;
const int NUMBER_OF_ENTRY_POINTED_TO_NAME_OF_THE_FILES_FOLDER = 1, NUMBER_OF_MONTH_DAYS = 30, NUMBER_KINDS_LEVEL = 4;
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

class Employee;
class SalaryConfig;

class WorkingHour
{
    int day;
    pair<int, int> working_interval;

public:
    WorkingHour(int day, pair<int, int> working_invertal);
    int get_day() { return day; }
    int calculate_lenght_of_working_invertal() { return (working_interval.second - working_interval.first); }
    bool is_time_between_invertal(int time)
    {
        if (time >= working_interval.first && time < working_interval.second)
            return true;
        return false;
    }
    bool is_day_this_day(int _day)
    {
        if (_day == day)
            return true;
        return false;
    }
    bool does_working_interval_interfere(int _day, int _start_period, int _finish_period)
    {
        if (day == _day)
            if ((_start_period >= working_interval.first && _start_period < working_interval.second) ||
                (_finish_period <= working_interval.second && _finish_period > working_interval.first) ||
                (_start_period < working_interval.first && _finish_period > working_interval.second))
                return true;
        return false;
    }
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
    Team(int team_id, int team_head_id, vector<int> member_ids, int bonus_min_working_hours, float bonus_working_hours_max_variance);
    int get_number_of_team_members() { return member_ids.size(); }
    void show_members_total_earning(const vector<Employee> &employees, vector<Team> &teams, vector<SalaryConfig> &salary_configs);
    int find_total_working_hours(const vector<Employee> &employees);
    bool is_id_this_team_id(int id)
    {
        if (id == team_id)
            return true;
        else
            return false;
    }
    string find_head_name(const vector<Employee> &employees);
    int get_team_id() { return team_id; }
    int get_bonus_percentage() { return bonus_percentage; }
    bool is_id_in_team(int id)
    {
        if (find(member_ids.begin(), member_ids.end(), id) != member_ids.end())
            return true;
        return false;
    }
    void report_salary(vector<Employee> &employees, vector<Team> &teams, vector<SalaryConfig> &salary_configs);
    void update_team_bonus(int new_bonus_percentage) { bonus_percentage = new_bonus_percentage; }
    pair<int, int> check_for_bonus(vector<Employee> &employees);
    vector<int> calcutlat_members_total_working_hours(vector<Employee> &employees);
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
    SalaryConfig(){};
    SalaryConfig(Level level, int base_salary, int salary_per_hour, int salaty_per_extra_hour,
                 int official_working_hours, int tax_percentage);
    float calculate_tax(float salary, float bonus);
    float calculate_salary(int working_hours);
    int get_tax_percentage() { return tax_percentage; }
    void show_details();
    void update_arguments();
    void change_base_salary(int new_base_salary) { base_salary = new_base_salary; }
    void change_salary_per_hour(int new_salary_per_hour) { salary_per_hour = new_salary_per_hour; }
    void change_salary_per_extra_hour(int new_salary_per_extra_hour) { salary_per_extra_hour = new_salary_per_extra_hour; }
    void change_official_working_hour(int new_official_working_hour) { official_working_hours = new_official_working_hour; }
    void change_tax(int new_tax) { tax_percentage = new_tax; }
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
    Employee(int id, string name, int age, Level level, vector<WorkingHour> working_hours);
    bool is_id_for_this_employee(int employee_id);
    int get_age() { return age; }
    int get_id() { return id; }
    string get_name() { return name; }
    Level get_level() { return level; }
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
    float calculate_bonus(int bonus_percentage);
    int find_emoloyee_team_index(vector<Team> &teams);
    void report_salary_long(vector<SalaryConfig> &salary_configs, int team_id);
    string level_to_string(Level level);
    int give_team_id(vector<Team> &teams);
    int find_working_hours_in_a_day(int day);
    int cout_days_work_in_a_period(int start_of_period);
    void add_working_hours(int day, int start_period, int finish_period);
    bool is_new_working_interval_invalid(int day, int start_period, int finish_period);
    void report_salary_as_team_member(vector<Team> &teams, vector<SalaryConfig> &salary_configs);
    void delete_working_hours(int day);
};

Employee::Employee(int _id, string _name, int _age, Level _level, vector<WorkingHour> _working_hours)
    : id(_id), name(_name), age(_age), level(_level), working_hours(_working_hours) {}

WorkingHour::WorkingHour(int _day, pair<int, int> _working_invertal)
    : day(_day), working_interval(_working_invertal) {}

Team::Team(int _team_id, int _team_head_id, vector<int> _member_ids,
           int _bonus_min_working_hours, float _bonus_working_hours_max_variance)
    : team_id(_team_id), team_head_id(_team_head_id), member_ids(_member_ids),
      bonus_min_working_hours(_bonus_min_working_hours), bonus_working_hours_max_variance(_bonus_working_hours_max_variance) {}

SalaryConfig::SalaryConfig(Level _level, int _base_salary, int _salary_per_hour, int _salaty_per_extra_hour,
                           int _official_working_hours, int _tax_percentage)
    : level(_level), base_salary(_base_salary), salary_per_hour(_salary_per_hour), salary_per_extra_hour(_salaty_per_extra_hour),
      official_working_hours(_official_working_hours), tax_percentage(_tax_percentage) {}

vector<string> split_line_of_string_by_specific_char(string line_string, char specific_char)
{
    istringstream line_stream(line_string);
    vector<string> fields;
    string word;
    while (getline(line_stream, word, specific_char))
        fields.push_back(word);
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
    sort(employees.begin(), employees.end(),
         [](Employee &left, Employee &right)
         { return (left.get_id() < right.get_id()); });
    return employees;
}
string set_folder_of_files_directory(string folder_name)
{
    return CURRENT_DIRECTORY + folder_name + SLASH;
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
    vector<int> member_ids = convert_vector_string_to_vector_int(split_line_of_string_by_specific_char(csv_fields[2], DOLLAR));
    sort(member_ids.begin(), member_ids.end());
    return Team(stoi(csv_fields[0]), stoi(csv_fields[1]), member_ids,
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
    sort(teams.begin(), teams.end(),
         [](Team &left, Team &right)
         { return (left.get_team_id() < right.get_team_id()); });
    return teams;
}
SalaryConfig convert_csv_line_to_salary_config(string csv_line)
{
    vector<string> csv_fields = split_line_of_string_by_specific_char(csv_line, COMMA);
    return SalaryConfig(convert_string_to_Level[csv_fields[0]], stoi(csv_fields[1]), stoi(csv_fields[2]),
                        stoi(csv_fields[3]), stoi(csv_fields[4]), stoi(csv_fields[5]));
}
Level find_level_of_line(string csv_line)
{
    return convert_string_to_Level[first_word_of_csv_line(csv_line)];
}
vector<SalaryConfig> get_salary_configs(const string salary_configs_file_directory)
{
    vector<SalaryConfig> salary_configs(NUMBER_KINDS_LEVEL);
    ifstream salary_configs_csv(salary_configs_file_directory);
    skip_header_row_of_csv(salary_configs_csv);
    string line;
    while (getline(salary_configs_csv, line))
        salary_configs[find_level_of_line(line)] = convert_csv_line_to_salary_config(line);
    salary_configs_csv.close();
    return salary_configs;
}

float SalaryConfig::calculate_tax(float salary, float bonus)
{
    return (round(salary) + round(bonus)) * tax_percentage / 100;
}

float Employee::calculate_bonus(int bonus_percentage)
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

int Employee::find_emoloyee_team_index(vector<Team> &teams)
{
    for (int i = 0; i < teams.size(); i++)
        if (teams[i].is_id_in_team(id))
            return i;
    return NOT_FOUND;
}

int Employee::calculate_absent_days()
{
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
    bonus = calculate_bonus(teams[find_emoloyee_team_index(teams)].get_bonus_percentage());
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

void Employee::report_salary_long(vector<SalaryConfig> &salary_configs, int team_id)
{
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Level: " << level_to_string(level) << endl;
    ;
    if (team_id == NOT_FOUND)
        cout << "Team ID: "
             << "N/A" << endl;
    else
        cout << "Team ID: " << team_id << endl;
    cout << "Total Working Hours: " << calculate_total_working_hour() << endl;
    cout << "Absent Days: " << calculate_absent_days() << endl;
    cout << "Salary: " << (int)round(salary) << endl;
    cout << "Bonus: " << (int)round(bonus) << endl;
    float tax = salary_configs[level].calculate_tax(salary, bonus);
    cout << "Tax: " << (int)round(tax) << endl;
    cout << "Total Earning: " << calculate_total_earing(tax) << endl;
}

int Employee::give_team_id(vector<Team> &teams)
{
    int team_index = find_emoloyee_team_index(teams);
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

bool Employee::is_id_for_this_employee(int employee_id)
{
    if (id == employee_id)
        return true;
    else
        return false;
}

string Team::find_head_name(const vector<Employee> &employees)
{
    for (auto employee : employees)
        if (employee.is_id_for_this_employee(team_head_id))
            return employee.get_name();
    exit(EXIT_FAILURE);
}

int find_team_index(int team_id, vector<Team> &teams)
{
    for (int i = 0; i < teams.size(); i++)
        if (teams[i].is_id_this_team_id(team_id))
            return i;
    return NOT_FOUND;
}

int Team::find_total_working_hours(const vector<Employee> &employees)
{
    int team_total_working_hours = 0;
    for (auto employee : employees)
        if (is_id_in_team(employee.get_id()))
            team_total_working_hours += employee.calculate_total_working_hour();
    return team_total_working_hours;
}

void Team ::show_members_total_earning(const vector<Employee> &employees, vector<Team> &teams, vector<SalaryConfig> &salary_configs)
{
    for (auto employee : employees)
        if (is_id_in_team(employee.get_id()))
            employee.report_salary_as_team_member(teams, salary_configs);
}

void Employee::report_salary_as_team_member(vector<Team> &teams, vector<SalaryConfig> &salary_configs)
{
    cout << "Member ID: " << id << endl;
    preparing_for_report_salary(teams, salary_configs);
    cout << "Total Earning: " << calculate_total_earing(salary_configs[level].calculate_tax(salary, bonus)) << endl;
    cout << "---" << endl;
}

void report_team_salary(vector<Employee> &employees, vector<SalaryConfig> &salary_configs, vector<Team> &teams)
{
    int team_id;
    cin >> team_id;
    int team_index = find_team_index(team_id, teams);
    if (team_index == NOT_FOUND)
    {
        cout << "TEAM_NOT_FOUND" << endl;
        return;
    }
    teams[team_index].report_salary(employees, teams, salary_configs);
}

void show_round_to_one_decimal_place(float number)
{
    int round_10_x_number = round(10 * number);
    cout << fixed;
    cout << setprecision(1);
    cout << (float)round_10_x_number / 10;
}

void Team::report_salary(vector<Employee> &employees, vector<Team> &teams, vector<SalaryConfig> &salary_configs)
{
    cout << "ID: " << team_id << endl;
    cout << "Head ID: " << team_head_id << endl;
    cout << "Head Name: " << find_head_name(employees) << endl;
    int total_working_hours = find_total_working_hours(employees);
    cout << "Team Total Working Hours: " << total_working_hours << endl;
    cout << "Average Member Working Hours: ";
    show_round_to_one_decimal_place((float)total_working_hours / get_number_of_team_members());
    cout << endl;
    cout << "Bonus: " << bonus_percentage << endl;
    cout << "---" << endl;
    show_members_total_earning(employees, teams, salary_configs);
}

int Employee ::find_working_hours_in_a_day(int day)
{
    int total_hours = 0;
    for (auto working_hour : working_hours)
        if (working_hour.is_day_this_day(day))
            total_hours += working_hour.calculate_lenght_of_working_invertal();
    return total_hours;
}

vector<int> calcute_days_total_working_hours(int start_day, int finish_day, const vector<Employee> &employees)
{
    vector<int> days_total_hours(finish_day - start_day + 1, 0);
    for (auto employee : employees)
        for (int day = start_day; day <= finish_day; day++)
            days_total_hours[day - start_day] += employee.find_working_hours_in_a_day(day);
    return days_total_hours;
}

void show_total_hours_per_day(int satart_day, vector<int> days_total_hours)
{
    for (int i = 0; i < days_total_hours.size(); i++)
        cout << "Day #" << satart_day + i << ": " << days_total_hours[i] << endl;
    cout << "---" << endl;
}

vector<int> find_max_numbers(int start_number, vector<int> numbers)
{
    vector<int> max_numbers = {start_number};
    for (int number = (start_number + 1); number < (start_number + numbers.size()); number++)
        if (numbers[max_numbers[0] - start_number] == numbers[number - start_number])
            max_numbers.push_back(number);
        else if (numbers[max_numbers[0] - start_number] < numbers[number - start_number])
        {
            max_numbers.clear();
            max_numbers.push_back(number);
        }
    return max_numbers;
}

vector<int> find_min_numbers(int start_number, vector<int> numbers)
{
    vector<int> min_numbers = {start_number};
    for (int number = (start_number + 1); number < (start_number + numbers.size()); number++)
        if (numbers[min_numbers[0] - start_number] == numbers[number - start_number])
            min_numbers.push_back(number);
        else if (numbers[min_numbers[0] - start_number] > numbers[number - start_number])
        {
            min_numbers.clear();
            min_numbers.push_back(number);
        }
    return min_numbers;
}

bool is_start_and_finish_day_of_month_invalid(int start_day, int finish_day)
{
    if (start_day < START_OF_MONTH || finish_day > END_OF_MONTH || start_day > finish_day)
        return true;
    return false;
}

void show_max_and_min_days_working_hours(vector<int> max_days_working_hours, vector<int> min_days_working_hours)
{
    cout << "Day(s) with Max Working Hours:";
    for (auto max_day_working_hours : max_days_working_hours)
        cout << " " << max_day_working_hours;
    cout << endl;
    cout << "Day(s) with Min Working Hours:";
    for (auto min_day_working_hours : min_days_working_hours)
        cout << " " << min_day_working_hours;
    cout << endl;
}

void report_total_hours_per_day(const vector<Employee> &employees)
{
    int start_day, finish_day;
    cin >> start_day >> finish_day;
    if (is_start_and_finish_day_of_month_invalid(start_day, finish_day))
    {
        cout << "INVALID_ARGUMENTS" << endl;
        return;
    }
    vector<int> day_total_working_hours = calcute_days_total_working_hours(start_day, finish_day, employees);
    show_total_hours_per_day(start_day, day_total_working_hours);
    vector<int> days_with_max_working_hours = find_max_numbers(start_day, day_total_working_hours);
    vector<int> days_with_min_working_hours = find_min_numbers(start_day, day_total_working_hours);
    show_max_and_min_days_working_hours(days_with_max_working_hours, days_with_min_working_hours);
}

int Employee ::cout_days_work_in_a_period(int start_of_period)
{
    int day_count = 0;
    for (auto working_hour : working_hours)
        if (working_hour.is_time_between_invertal(start_of_period))
            day_count++;
    return day_count;
}

vector<pair<int, float>> print_avg_working_employee(int start_hour, int finish_hour, vector<Employee> &employees)
{
    vector<pair<int, float>> each_hour_avg;
    for (int start_period = start_hour; start_period < finish_hour; start_period++)
    {
        pair<int, float> this_hour;
        float total_working_days = 0;
        for (auto employee : employees)
            total_working_days += employee.cout_days_work_in_a_period(start_period);
        this_hour.first = start_period;
        this_hour.second = total_working_days / MONTH_DAYS_COUNT;
        each_hour_avg.push_back(this_hour);
        cout << std::fixed;
        cout << std::setprecision(1);
        cout << start_period << "-" << start_period + 1 << ": " << this_hour.second << endl;
    }
    cout << "---" << endl;
    return each_hour_avg;
}

vector<int> calculate_employee_per_hour(int start_hour, int finish_hour, const vector<Employee> &employees)
{
    vector<int> periods_total_hours(finish_hour - start_hour, 0);
    for (auto employee : employees)
        for (int start_period = start_hour; start_period < finish_hour; start_period++)
            periods_total_hours[start_period - start_hour] += employee.cout_days_work_in_a_period(start_period);
    return periods_total_hours;
}

bool is_start_and_finish_hour_invalid(int start_hour, int finish_hour)
{
    if (start_hour < START_OF_DAY || finish_hour > END_OF_DAY || start_hour >= finish_hour)
        return true;
    return false;
}

void show_employee_per_hour(int start_hour, vector<int> periods_total_hours)
{
    for (int start_period = start_hour; start_period < (periods_total_hours.size() + start_hour); start_period++)
    {
        cout << start_period << "-" << start_period + 1 << ": ";
        show_round_to_one_decimal_place((float)periods_total_hours[start_period - start_hour] / MONTH_DAYS_COUNT);
        cout << endl;
    }
    cout << "---" << endl;
}

void show_max_and_min_working_employees_periods(vector<int> periods_with_max_working_employees, vector<int> periods_with_min_working_employees)
{
    cout << "Period(s) with Max Working Employees:";
    for (auto max_period : periods_with_max_working_employees)
        cout << " " << max_period << "-" << max_period + 1;
    cout << endl;
    cout << "Period(s) with Min Working Employees:";
    for (auto min_period : periods_with_min_working_employees)
        cout << " " << min_period << "-" << min_period + 1;
    cout << endl;
}

vector<int> divide_vector(vector<int> divideds, int divisor)
{
    vector<int> ans_numbers(divideds.size());
    for (int index = 0; index < divideds.size(); index++)
        ans_numbers[index] = (int)round((float)divideds[index] / divisor);
    return ans_numbers;
}

void report_employee_per_hour(const vector<Employee> &employees)
{
    int start_hour, finish_hour;
    cin >> start_hour >> finish_hour;
    if (is_start_and_finish_hour_invalid(start_hour, finish_hour))
    {
        cout << "INVALID_ARGUMENTS" << endl;
        return;
    }
    vector<int> periods_total_hours = calculate_employee_per_hour(start_hour, finish_hour, employees);
    show_employee_per_hour(start_hour, periods_total_hours);
    vector<int> average_periods_total_hours = divide_vector(periods_total_hours, MONTH_DAYS_COUNT);
    vector<int> periods_with_max_working_employees = find_max_numbers(start_hour, average_periods_total_hours);
    vector<int> periods_with_min_working_employees = find_min_numbers(start_hour, average_periods_total_hours);
    show_max_and_min_working_employees_periods(periods_with_max_working_employees, periods_with_min_working_employees);
}

void SalaryConfig ::show_details()
{
    cout << "Base Salary: " << base_salary << endl;
    cout << "Salary Per Hour: " << salary_per_hour << endl;
    cout << "Salary Per Extra Hour: " << salary_per_extra_hour << endl;
    cout << "Official Working Hours: " << official_working_hours << endl;
    cout << "Tax: " << tax_percentage << "%" << endl;
}

bool is_level_valid(string level)
{
    if (level == "junior" || level == "expert" || level == "senior" || level == "team_lead")
        return true;
    return false;
}

void show_salary_config(vector<SalaryConfig> &salary_configs)
{
    string str_level;
    cin >> str_level;
    if (!(is_level_valid(str_level)))
    {
        cout << "INVALID_LEVEL" << endl;
        return;
    }
    Level level = convert_string_to_Level[str_level];
    salary_configs[level].show_details();
}

bool should_be_changed(string argument)
{
    if (argument != "-")
        return true;
    return false;
}

void SalaryConfig::update_arguments()
{
    string new_base_salary, new_salary_per_hour, new_salary_per_extra_hours, new_official_working_hours, new_tax_percentage;
    cin >> new_base_salary >> new_salary_per_hour >> new_salary_per_extra_hours >> new_official_working_hours >> new_tax_percentage;
    if (should_be_changed(new_base_salary))
        base_salary = stoi(new_base_salary);
    if (should_be_changed(new_salary_per_hour))
        salary_per_hour = stoi(new_salary_per_hour);
    if (should_be_changed(new_salary_per_extra_hours))
        salary_per_extra_hour = stoi(new_salary_per_extra_hours);
    if (should_be_changed(new_official_working_hours))
        official_working_hours = stoi(new_official_working_hours);
    if (should_be_changed(new_tax_percentage))
        tax_percentage = stoi(new_tax_percentage);
}

void update_salary_config(vector<SalaryConfig> &salary_configs)
{
    string str_level;
    cin >> str_level;
    if (!(is_level_valid(str_level)))
    {
        cout << "INVALID_LEVEL" << endl;
        return;
    }
    Level level = convert_string_to_Level[str_level];
    salary_configs[level].update_arguments();
    cout << "OK" << endl;
}

bool Employee ::is_new_working_interval_invalid(int day, int start_period, int finish_period)
{
    for (auto working_hour : working_hours)
        if (working_hour.does_working_interval_interfere(day, start_period, finish_period))
            return true;
    return false;
}

bool is_day_valid(int day)
{
    if (day >= START_OF_MONTH && day <= END_OF_MONTH)
        return true;
    return false;
}

void Employee ::add_working_hours(int day, int start_period, int finish_period)
{
    if (is_new_working_interval_invalid(day, start_period, finish_period))
    {
        cout << "INVALID_INTERVAL" << endl;
        return;
    }
    working_hours.push_back(WorkingHour(day, {start_period, finish_period}));
    cout << "OK" << endl;
}

void add_working_hours(vector<Employee> &employees)
{
    int employee_id, day, start_period, finish_period;
    cin >> employee_id >> day >> start_period >> finish_period;
    int employee_index = find_employee_index(employee_id, employees);
    if (employee_index == NOT_FOUND)
    {
        cout << "EMPLOYEE_NOT_FOUND" << endl;
        return;
    }
    if (!is_day_valid(day) || is_start_and_finish_hour_invalid(start_period, finish_period))
    {
        cout << "INVALID_ARGUMENTS" << endl;
        return;
    }
    employees[employee_index].add_working_hours(day, start_period, finish_period);
}

void Employee::delete_working_hours(int day)
{
    for (int working_hour_index = 0; working_hour_index < working_hours.size(); working_hour_index++)
        if (working_hours[working_hour_index].is_day_this_day(day))
        {
            working_hours.erase(working_hours.begin() + working_hour_index);
            working_hour_index--;
        }
}

void delete_working_hours(vector<Employee> &employees)
{
    int employee_id, day;
    cin >> employee_id >> day;
    int employee_index = find_employee_index(employee_id, employees);
    if (employee_index == NOT_FOUND)
    {
        cout << "EMPLOYEE_NOT_FOUND" << endl;
        return;
    }
    if (!is_day_valid(day))
    {
        cout << "INVALID_ARGUMENTS" << endl;
        return;
    }
    employees[employee_index].delete_working_hours(day);
    cout << "OK" << endl;
}

void update_team_bonus(vector<Team> &teams)
{
    int team_id, bonus_percentage;
    cin >> team_id >> bonus_percentage;
    int team_index = find_team_index(team_id, teams);
    if (team_index == NOT_FOUND)
    {
        cout << "TEAM_NOT_FOUND" << endl;
        return;
    }
    if (bonus_percentage < 0 || bonus_percentage > 100)
    {
        cout << "INVALID_ARGUMENTS" << endl;
        return;
    }
    teams[team_index].update_team_bonus(bonus_percentage);
    cout << "OK" << endl;
}

vector<int> Team::calcutlat_members_total_working_hours(vector<Employee> &employees)
{
    vector<int> members_total_working_hours;
    for (auto employee : employees)
        if (is_id_in_team(employee.get_id()))
            members_total_working_hours.push_back(employee.calculate_total_working_hour());
    return members_total_working_hours;
}

int sum_vector(vector<int> numbers)
{
    int sum = 0;
    for (auto number : numbers)
        sum += number;
    return sum;
}

float variance_vector(vector<int> numbers, float average)
{
    float sum_power_two = 0;
    for (int i = 0; i < numbers.size(); i++)
        sum_power_two += (numbers[i] - average) * (numbers[i] - average);
    return sum_power_two / numbers.size();
}

pair<int, int> Team::check_for_bonus(vector<Employee> &employees)
{
    vector<int> members_working_hours = calcutlat_members_total_working_hours(employees);
    int sum_members_working_hours = sum_vector(members_working_hours);
    float variance_members_working_hours = variance_vector(members_working_hours, (float)sum_members_working_hours / get_number_of_team_members());
    if (sum_members_working_hours > bonus_min_working_hours && variance_members_working_hours < bonus_working_hours_max_variance)
        return {team_id, sum_members_working_hours};
    return {NOT_FOUND, NOT_FOUND};
}

void show_teams_for_bonus(vector<pair<int, int>> teams_for_bonus)
{
    for (auto team_for_bonus : teams_for_bonus)
        cout << "Team ID: " << team_for_bonus.first << endl;
}

void find_teams_for_bonus(vector<Employee> &employees, vector<Team> &teams)
{
    vector<pair<int, int>> teams_for_bonus;
    for (auto team : teams)
    {
        pair<int, int> team_for_bonus = team.check_for_bonus(employees);
        if (team_for_bonus.first != NOT_FOUND)
            teams_for_bonus.push_back(team_for_bonus);
    }
    sort(teams_for_bonus.begin(), teams_for_bonus.end(),
         [](const pair<int, int> &left, const pair<int, int> &right)
         { return (left.second < right.second); });
    show_teams_for_bonus(teams_for_bonus);
}

void get_order(vector<Employee> &employees, vector<Team> &teams, vector<SalaryConfig> &salary_configs)
{
    string order;
    while (cin >> order)
        if (order == "report_salaries")
            report_salaries(employees, teams, salary_configs);
        else if (order == "report_employee_salary")
            report_employee_salary(employees, teams, salary_configs);
        else if (order == "report_team_salary")
            report_team_salary(employees, salary_configs, teams);
        else if (order == "report_total_hours_per_day")
            report_total_hours_per_day(employees);
        else if (order == "report_employee_per_hour")
            report_employee_per_hour(employees);
        else if (order == "show_salary_config")
            show_salary_config(salary_configs);
        else if (order == "update_salary_config")
            update_salary_config(salary_configs);
        else if (order == "add_working_hours")
            add_working_hours(employees);
        else if (order == "delete_working_hours")
            delete_working_hours(employees);
        else if (order == "update_team_bonus")
            update_team_bonus(teams);
        else if (order == "find_teams_for_bonus")
            find_teams_for_bonus(employees, teams);
        else
            exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    const string files_directory = set_folder_of_files_directory(argv[NUMBER_OF_ENTRY_POINTED_TO_NAME_OF_THE_FILES_FOLDER]);
    vector<Employee> employees = get_employees_and_working_hours(files_directory + EMPLOYEES_FILE_NAME,
                                                                 files_directory + WORKING_HOURS_FILE_NAME);
    vector<Team> teams = get_teams(files_directory + TEAMS_FILE_NAME);
    vector<SalaryConfig> salary_configs = get_salary_configs(files_directory + SALARY_CONFIGS_FILE_NAME);
    get_order(employees, teams, salary_configs);
    return 0;
}