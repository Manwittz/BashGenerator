#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <set>
#include <getopt.h>
#include <cstdlib>
#include <string>
#include <regex>
#include <filesystem>

using namespace std;

bool isValidDate(const string& date) {
    // Step 1: Check length
    if (date.length() != 10) return false;
    
    // Step 2: Check format using regex
    regex date_pattern("([0-9]{4})-([0-9]{2})-([0-9]{2})");
    if (!regex_match(date, date_pattern)) return false;

    // Step 3: Validate components
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    // Validate month
    if (month < 1 || month > 12) return false;

    // Validate day based on month
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        days_in_month[2] = 29;
    }

    if (day < 1 || day > days_in_month[month]) return false;

    return true;
}

string getRandomDate(const string& start_date, const string& end_date) {
    struct tm start_tm = {}, end_tm = {};
    strptime(start_date.c_str(), "%Y-%m-%d", &start_tm);
    strptime(end_date.c_str(), "%Y-%m-%d", &end_tm);
    
    time_t start_time = mktime(&start_tm);
    time_t end_time = mktime(&end_tm);

    // Generate a random time between start and end
    time_t random_time = start_time + rand() % (end_time - start_time + 1);

    struct tm *random_tm;
    do {
        random_time = start_time + rand() % (end_time - start_time + 1);
        random_tm = localtime(&random_time);
    } while (random_tm->tm_wday == 0 || random_tm->tm_wday == 7); // Skip Sunday and Saturday

    // Generate a random time between 7:00 AM and 11:59 PM
    random_tm->tm_hour = 7 + rand() % 17;
    random_tm->tm_min = rand() % 60;
    random_tm->tm_sec = rand() % 60;

    char buffer[20];
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", random_tm);

    return string(buffer);
}


// void createBashFile(const string& date, const string& commit_message, int index) {
//     stringstream filename;
//     filename << "commit_" << index << ".sh";

//     ofstream file(filename.str());
//     file << "git add .\n";
//     file << "export GIT_AUTHOR_DATE=\"" << date << "\"\n";
//     file << "export GIT_COMMITTER_DATE=\"" << date << "\"\n";
//     file << "git commit -m \\\"$1\\\"\n";
//     file.close();
// }

void createBashFile(const string& date, const string& start_date, const string& end_date, int X, int index) {
    stringstream dir_name;
    dir_name << "commit_" << start_date << "_" << end_date << "_" << X;

    // Create the directory if it doesn't exist
    std::filesystem::create_directory(dir_name.str());

    stringstream filename;
    filename << dir_name.str() << "/commit_" << index << ".sh";

    ofstream file(filename.str());
    file << "#!/bin/bash\n";
    file << "if [ -z \"$1\" ]; then\n";
    file << "  echo \"Please provide a commit message as the first argument.\"\n";
    file << "  exit 1\n";
    file << "fi\n";
    file << "git add .\n";
    file << "export GIT_AUTHOR_DATE=\"" << date << "\"\n";
    file << "export GIT_COMMITTER_DATE=\"" << date << "\"\n";
    file << "git commit -m \\\"$1\\\"\n";
    file << "rm -- \"$0\"\n";  // Remove the file after execution
    file.close();
}

void saveDatesToFile(const set<string>& unique_dates, const string& dir_name) {
    stringstream filename;
    filename << dir_name << "/generated_dates.txt";

    ofstream file(filename.str());
    for (const string& date : unique_dates) {
        file << date << "\n";
    }
    file.close();
}

int main(int argc, char *argv[]) {

    // Seed the random number generator
    srand(time(0));
    
    string start_date, end_date, commit_message;
    int X;

    int opt;
    static struct option long_options[] = {
        {"start", required_argument, 0, 's'},
        {"end", required_argument, 0, 'e'},
        {"commit", required_argument, 0, 'c'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "s:e:c:", long_options, NULL)) != -1) {
        switch (opt) {
            case 's':
                start_date = optarg;
                break;
            case 'e':
                end_date = optarg;
                break;
            case 'c':
                X = atoi(optarg);
                break;
            default:
                cerr << "Invalid option" << endl;
                return 1;
        }
    }

    if (!isValidDate(start_date) || !isValidDate(end_date)) {
        cerr << "Invalid date format" << endl;
        return 1;
    }

    set<string> unique_dates;
    while (unique_dates.size() < X) {
        string date = getRandomDate(start_date, end_date);
        unique_dates.insert(date);
    }

    stringstream dir_name;
    dir_name << "commit_" << start_date << "_" << end_date << "_" << X;

    // Create the directory if it doesn't exist
    std::filesystem::create_directory(dir_name.str());

    int index = 0;
    for (const string& date : unique_dates) {
        createBashFile(date, start_date, end_date, X, index++);
    }

    // Save the generated dates to a text file in the same directory
    saveDatesToFile(unique_dates, dir_name.str());

    return 0;
}
