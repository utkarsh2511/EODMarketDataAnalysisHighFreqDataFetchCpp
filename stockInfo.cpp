#include "stockInfo.h"

stockInfo::stockInfo()
    : GroupNum(0), estimatedEarning(0.0), reportEarning(0.0), surprise(0.0), surprisePercentage(0.0) {}

stockInfo::stockInfo(const dVector &dailyPrice, const dVector &cumulativeDailyReturn, int groupNum,
          const std::string &ticker, const std::string &announcementDate, const std::string &periodEnding,
          double estimatedEarning, double reportEarning, double surprise, double surprisePercentage)
    : DailyPrice(dailyPrice), CumulativeDailyReturn(cumulativeDailyReturn),
      GroupNum(groupNum), ticker(ticker), AnnouncementDate(announcementDate), PeriodEnding(periodEnding),
      estimatedEarning(estimatedEarning), reportEarning(reportEarning),
      surprise(surprise), surprisePercentage(surprisePercentage) {}

const dVector& stockInfo::getDailyPrice() const {
    return DailyPrice;
}

void stockInfo::setDailyPrice(const dVector &dailyPrice) {
    DailyPrice = dailyPrice;
}

const dVector& stockInfo::getCumulativeDailyReturn() const {
    return CumulativeDailyReturn;
}

void stockInfo::setCumulativeDailyReturn(const dVector &cumulativeDailyReturn) {
    CumulativeDailyReturn = cumulativeDailyReturn;
}

int stockInfo::getGroupNum() const {
    return GroupNum;
}

void stockInfo::setGroupNum(int groupNum) {
    GroupNum = groupNum;
}

std::string stockInfo::getTicker() const {
    return ticker;
}

void stockInfo::setTicker(const std::string &newTicker) {
    ticker = newTicker;
}

std::string stockInfo::getAnnouncementDate() const {
    return AnnouncementDate;
}

void stockInfo::setAnnouncementDate(const std::string &announcementDate) {
    AnnouncementDate = announcementDate;
}

std::string stockInfo::getPeriodEnding() const {
    return PeriodEnding;
}

void stockInfo::setPeriodEnding(const std::string &periodEnding) {
    PeriodEnding = periodEnding;
}

double stockInfo::getEstimatedEarning() const {
    return estimatedEarning;
}

void stockInfo::setEstimatedEarning(double estimatedEarning) {
    this->estimatedEarning = estimatedEarning;
}

double stockInfo::getReportEarning() const {
    return reportEarning;
}

void stockInfo::setReportEarning(double reportEarning) {
    this->reportEarning = reportEarning;
}

double stockInfo::getSurprise() const {
    return surprise;
}

void stockInfo::setSurprise(double newSurprise) {
    surprise = newSurprise;
}

double stockInfo::getSurprisePercentage() const {
    return surprisePercentage;
}

void stockInfo::setSurprisePercentage(double newSurprisePercentage) {
    surprisePercentage = newSurprisePercentage;
}

void stockInfo::printInfo() const {
    std::cout << "Ticker: " << ticker << std::endl;
    std::cout << "Announcement Date: " << AnnouncementDate << std::endl;
    std::cout << "Period Ending: " << PeriodEnding << std::endl;
    if(GroupNum == 0){
        std::cout << "Group: Miss" << std::endl;
    } else if(GroupNum == 1){
        std::cout << "Group: Meet" << std::endl;
    } else{
        std::cout << "Group: Beat" << std::endl;
    }

    std::cout << "Estimated Earning: $" << estimatedEarning << std::endl;
    std::cout << "Reported Earning: $" << reportEarning << std::endl;
    std::cout << "Surprise: $" << surprise << std::endl;
    std::cout << "Surprise Percentage: " << surprisePercentage << "%" << std::endl;
    std::cout << "Stock Daily Price" << std::endl;
    displayVector(DailyPrice);
    std::cout << "Stock Cumulative Daily Return" << std::endl;
    displayVector(CumulativeDailyReturn);
}
