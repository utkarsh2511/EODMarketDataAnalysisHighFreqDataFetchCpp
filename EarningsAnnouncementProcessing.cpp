#include <cstring>
#include <string> 
#include <iostream>
#include <sstream>  
#include <vector>
#include <locale>
#include <map>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include "EarningsAnnouncementProcessing.h"

using namespace std;

const char* earningsAnnouncementFile = "Russell3000EarningsAnnouncements.csv";

// Function to convert the month abbreviation to a two-digit month number
std::string monthToNumber(const std::string& month) {
    static const std::map<std::string, std::string> monthMap = {
        {"JAN", "01"}, {"FEB", "02"}, {"MAR", "03"},
        {"APR", "04"}, {"MAY", "05"}, {"JUN", "06"},
        {"JUL", "07"}, {"AUG", "08"}, {"SEP", "09"},
        {"OCT", "10"}, {"NOV", "11"}, {"DEC", "12"}
    };
    auto it = monthMap.find(month);
    if (it != monthMap.end()) {
        return it->second;
    }
    return "00"; // Return "00" for an invalid month
}

// Function to convert a date from "DD-MMM-YY" to "YYYY-MM-DD"
std::string convertDate(const std::string& date) {
    // Extract day, month, and year components
    std::string day = date.substr(0, 2);
    std::string month = date.substr(3, 3);
    std::string year = date.substr(7, 2);

    // Convert the month string to uppercase
    for (char& c : month) {
        c = std::toupper(c);
    }

    // Convert two-digit year to four-digit year
    int yearNumber = std::stoi(year);
    // Assuming the year '00' to '99' corresponds to '2000' to '2099'
    if (yearNumber <= 99) {
        year = "20" + year;
    }

    // Convert month abbreviation to month number
    std::string monthNumber = monthToNumber(month);

    // Format the date in ISO standard format "YYYY-MM-DD"
    return year + "-" + monthNumber + "-" + day;
}

// function to add or subtract days to "YYYY-MM-DD" formatted date
// std::string addOrSubtractDays(const std::string& date, int days) {
//     std::tm tm = {};
//     std::istringstream ss(date);

//     // Parse the date assuming the format YYYY-MM-DD
//     ss >> std::get_time(&tm, "%Y-%m-%d");

//     // Check if parsing was successful
//     if (ss.fail()) {
//         // std::cerr << "Failed to parse date" << std::endl;
//         return "";
//     }

//     // Convert tm to time_t, then add or subtract days
//     std::time_t time = std::mktime(&tm);
//     const int secondsPerDay = 86400;
//     std::time_t newTime = time + days * secondsPerDay;

//     // Convert the time_t back to tm struct
//     std::tm* newTm = std::localtime(&newTime);

//     // Create a new stringstream to format the resulting date
//     std::ostringstream result;
//     result << std::put_time(newTm, "%Y-%m-%d");

//     return result.str();
// }

bool isStockMarketHoliday(const std::tm* date) {
    // List of fixed-date holidays (month, day)
    std::vector<std::pair<int, int>> fixedHolidays = {
        {1, 1},   // New Year's Day
        {7, 4},   // Independence Day
        {12, 25}  // Christmas Day
    };

    for (const auto& holiday : fixedHolidays) {
        if (date->tm_mon + 1 == holiday.first && date->tm_mday == holiday.second) {
            return true;
        }
    }

    // Add more complex holidays like Thanksgiving (4th Thursday of November)
    if (date->tm_mon == 10) { // November is month 10 (0-based)
        if (date->tm_wday == 4) { // Thursday
            int weekdayOfFirst = (1 + 7 - date->tm_wday) % 7;  // Day of week for Nov 1
            if ((date->tm_mday - weekdayOfFirst + 6) / 7 == 3) { // 4th Thursday check
                return true;
            }
        }
    }

    return false;
}

// Function to add or subtract business days to "YYYY-MM-DD" formatted date,
// considering weekends and stock market holidays
std::string addOrSubtractDays(const std::string& date, int businessDays) {
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail()) {
        return "";  // Early exit if date parsing fails
    }

    std::time_t time = std::mktime(&tm);
    int direction = (businessDays >= 0) ? 1 : -1;
    int remainingDays = std::abs(businessDays);

    while (remainingDays > 0) {
        time += direction * 86400; // Add/subtract a day in seconds
        std::tm* newTm = std::localtime(&time);

        // Check if the new day is a weekend or a holiday
        if (newTm->tm_wday != 0 && newTm->tm_wday != 6 && !isStockMarketHoliday(newTm)) {
            remainingDays--;  // Only decrement remaining days on valid business days
        }
    }

    std::tm* newTm = std::localtime(&time);
    std::ostringstream result;
    result << std::put_time(newTm, "%Y-%m-%d");
    return result.str();
}

void populateEarningsAnnouncementMapVec(map<string, string> &earningsDateMap, vector<pair<string, double>> &earningsSurprisePairVect, map<string, string> &periodEndingDateMap,  map<string, vector<double>>&nonComputationalData, string& minDate, string& maxDate)
{
	ifstream fin;
	fin.open(earningsAnnouncementFile, ios::in);
	
	string line, date, ticker, surprisePercent, waste;
	
	// discard the first line of earnings report
	getline(fin, line);
	
    bool firstTime = true;
	while (!fin.eof())
	{
		getline(fin, line);
		stringstream sin(line);
		
		getline(sin, ticker, ',');
		getline(sin, date, ',');
		earningsDateMap[ticker] = convertDate(date);
		// save min and max benchmark dates
		if(!firstTime){
		    if (earningsDateMap[ticker].compare(minDate) < 0) {
                minDate = earningsDateMap[ticker];
            }
            if (earningsDateMap[ticker].compare(maxDate) > 0) {
                maxDate = earningsDateMap[ticker];
            }
		}else{
		    minDate = earningsDateMap[ticker];
		    maxDate = earningsDateMap[ticker];
		    firstTime = false;
		}
		
		// calling the field where we don't do any computation a waste
		getline(sin, waste, ',');
	    periodEndingDateMap[ticker] = waste;
		getline(sin, waste, ',');
		nonComputationalData[ticker].push_back(stod(waste));
		getline(sin, waste, ',');
		nonComputationalData[ticker].push_back(stod(waste));
		getline(sin, waste, ',');
		nonComputationalData[ticker].push_back(stod(waste));
		
		getline(sin, surprisePercent, ',');
		earningsSurprisePairVect.emplace_back(make_pair(ticker, stod(surprisePercent)));
	}
	fin.close();
}

void sortAscEarningsVec(vector<pair<string, double>> &earningsSurprisePairVect)
{
	sort(earningsSurprisePairVect.begin(), earningsSurprisePairVect.end(), 
	[] (const pair<string, double>& a, const pair<string, double>& b)
		{
        	return a.second < b.second;
    	});
}
