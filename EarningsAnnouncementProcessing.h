using namespace std;

// Function to convert the month abbreviation to a two-digit month number
std::string monthToNumber(const std::string& month);

// Function to convert a date from "DD-MMM-YY" to "YYYY-MM-DD"
std::string convertDate(const std::string& date);

// function to add or subtract days to "YYYY-MM-DD" formatted date
std::string addOrSubtractDays(const std::string& date, int businessDays);

void populateEarningsAnnouncementMapVec(map<string, string> &earningsDateMap, vector<pair<string, double>> &earningsSurprisePairVect, map<string, string> &periodEndingDateMap,  map<string, vector<double>>&nonComputationalData, string& minDate, string& maxDate);

void sortAscEarningsVec(vector<pair<string, double>> &earningsSurprisePairVect);