#ifndef STOCKINFO_H
#define STOCKINFO_H

#include <iostream>
#include <vector>
#include <string>
#include "Calculation.h"

typedef std::vector<double> dVector;

class stockInfo {
private:
    dVector DailyPrice;
    dVector CumulativeDailyReturn;
    int GroupNum; // 0: Beat, 1: Meet, 2: Miss
    std::string ticker; // Ticker symbol
    std::string AnnouncementDate;
    std::string PeriodEnding;
    double estimatedEarning;
    double reportEarning;
    double surprise;
    double surprisePercentage;

public:
    stockInfo();
    stockInfo(const dVector &dailyPrice, const dVector &cumulativeDailyReturn, int groupNum,
              const std::string &ticker, const std::string &announcementDate, const std::string &periodEnding,
              double estimatedEarning, double reportEarning, double surprise, double surprisePercentage);

    const dVector& getDailyPrice() const;
    void setDailyPrice(const dVector &dailyPrice);

    const dVector& getCumulativeDailyReturn() const;
    void setCumulativeDailyReturn(const dVector &cumulativeDailyReturn);

    int getGroupNum() const;
    void setGroupNum(int groupNum);

    std::string getTicker() const;
    void setTicker(const std::string &newTicker);

    std::string getAnnouncementDate() const;
    void setAnnouncementDate(const std::string &announcementDate);

    std::string getPeriodEnding() const;
    void setPeriodEnding(const std::string &periodEnding);

    double getEstimatedEarning() const;
    void setEstimatedEarning(double estimatedEarning);

    double getReportEarning() const;
    void setReportEarning(double reportEarning);

    double getSurprise() const;
    void setSurprise(double newSurprise);

    double getSurprisePercentage() const;
    void setSurprisePercentage(double newSurprisePercentage);

    void printInfo() const;
};

#endif // STOCKINFO_H
