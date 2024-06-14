#include <cstring>
#include <string> 
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "curl/curl.h"
#include <iostream>
#include <algorithm> // for std::copy
#include <cmath>
#include <set>
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <cstdio>
#include "EarningsAnnouncementProcessing.h"
#include "gnuplot.h"
#include "Overload.h"
#include "stockInfo.h"
#include "Calculation.h"

using namespace std;

typedef std::vector<double> dVector;
typedef std::vector<std::vector<double>> Matrix;


struct MemoryStruct {
	char* memory;
	size_t size;
};

void* myrealloc(void* ptr, size_t size)
{
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}

int write_data2(void* ptr, size_t size, size_t nmemb, void* data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)data;
	mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

int getData2(vector<map<string, double>>& datesAndPricesMapVec, vector<string> symbols, vector<string> start_date, vector<string> end_date){
	int still_running;
	// initialize curl multi handle
	CURL *multi = curl_multi_init();
	int numHandles = symbols.size();
	
	CURL **handles = new CURL*[numHandles];
	
	for(int i = 0; i <numHandles; i++){
		handles[i] = curl_easy_init();
	}
	

	// set up the program environment that libcurl needs
	curl_global_init(CURL_GLOBAL_ALL);
	
	string url_common = "https://eodhistoricaldata.com/api/eod/";
	string api_token = "6627fdcbc41cf2.06270794";  // You must replace this API token with yours  662a9df348cae5.15199329
	struct MemoryStruct* data = new MemoryStruct[numHandles];
	
	for(int i = 0; i <numHandles; i++){
		data[i].memory = NULL;
		data[i].size = 0;
		
		string url_request = url_common + symbols[i] + ".US?" + "from=" + start_date[i] + "&to=" + end_date[i] + "&api_token=" + api_token + "&period=d";
		curl_easy_setopt(handles[i], CURLOPT_URL, url_request.c_str());
		
		//adding a user agent
		curl_easy_setopt(handles[i], CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
		curl_easy_setopt(handles[i], CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(handles[i], CURLOPT_SSL_VERIFYHOST, 0);
	
		curl_easy_setopt(handles[i], CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handles[i], CURLOPT_WRITEDATA, (void*)&data[i]);
		curl_multi_add_handle(multi, handles[i]);
	}
	
    do {
      CURLMcode mc = curl_multi_perform(multi, &still_running);
 
      if(!mc && still_running)
        /* wait for activity, timeout or "nothing" */
        mc = curl_multi_poll(multi, NULL, 0, 1000, NULL);
 
      if(mc) {
        fprintf(stderr, "curl_multi_poll() failed, code %d.\n", (int)mc);
        break;
      }
 
    /* if there are still transfers, loop! */
    } while(still_running);

    stringstream* sData = new stringstream[numHandles];
	string sAdjCloseValue, sDate;
	double dAdjCloseValue = 0.0;
	string line;
	
	for(int i = 0; i < numHandles; i++){
		sData[i].str(data[i].memory);
		map<string, double> datePriceMap;
		
		while (getline(sData[i], line)) {
			
			size_t found = line.find('-');
			if (found != std::string::npos)
			{
				sDate = line.substr(0, line.find_first_of(','));
				
				line.erase(line.find_last_of(','));
				
				sAdjCloseValue = line.substr(line.find_last_of(',') + 1);
				
				dAdjCloseValue = strtod(sAdjCloseValue.c_str(), NULL);
		
				datePriceMap[sDate] = dAdjCloseValue;
				
			}
			
		}
		
		if (datePriceMap.size() < 110) {
	    std::cout << "Stock " << symbols[i] << " does not have enough data points. Skipping this stock." << std::endl;
	}

		
		datesAndPricesMapVec.push_back(datePriceMap);
		
		free(data[i].memory);
		data[i].size = 0;
			
		// cleanup what is created by curl_easy_init
		curl_easy_cleanup(handles[i]);
	}
	delete[] sData;
	delete[] data;
	delete[] handles;
	curl_multi_cleanup(multi);
	// release resources acquired by curl_global_init()
	curl_global_cleanup();
	
	if(numHandles > 1){
		for(int i = 0; i < 700000000; i++){
		continue;
	}
	}
	
	return numHandles;
}

int main()
{
	// Seed the random number generator
    srand(time(nullptr));
    
	// stl vector of pairs to hold ticker and surprise%
	vector<pair<string, double>> earningsSurprisePairVect; 
	
	// map to hold other earnings data not needed for computation
	map<string, vector<double>> nonComputationalData;
	map<string, string> periodEndingDateMap;
	
	// stl map to hold ticker and earnings announcement date
	map<string, string> earningsDateMap;
	
	string minDateForBenchMark, maxDateForBenchmark;
	
	populateEarningsAnnouncementMapVec(earningsDateMap, earningsSurprisePairVect, periodEndingDateMap, nonComputationalData, minDateForBenchMark, maxDateForBenchmark);

	// sort the vector of pairs based on surprise % ascending
	sortAscEarningsVec(earningsSurprisePairVect);
	
	// Divide them in 3 approximately equal groups
	// Calculate the size of each part
    size_t part_size = earningsSurprisePairVect.size() / 3;

    // Determine the starting points for each slice
    auto miss = earningsSurprisePairVect.begin();
    auto meet = next(miss, part_size);
    auto beat = next(meet, part_size);
    
	// do menu things here
	int choice;
	int daysAround;

	// ------------------------------------menu
    bool running = true;

	vector<Matrix> ResultofSimulation(3);
	
    while (running) {
    	
    	// vector of size 1 containing a map to store entire data for benchmark
		vector<map<string, double>> benchmarkDataMapVec;
		vector<string> bsymbols, bstart_dates, bend_dates;
		
		// converting into format that getData2 accepts
		bsymbols.push_back("IWV");
		map<string, double> benchmarkDataMap;
		
		int numStocksInGroup = 30;
		vector<double> AverageCAARAcrossSimulationsGroup1, AverageCAARAcrossSimulationsGroup2, AverageCAARAcrossSimulationsGroup3;
					
		Matrix AARMissGroup, AARMeetGroup, AARBeatGroup;
		Matrix CAARMissGroup, CAARMeetGroup, CAARBeatGroup;
		
		map<string, map<string, double>> pulledStockDataMap;
		vector<dVector> AbnormalReturnsEntireGroup1;
		
		vector<double> AARSTDMissGroup ;
		vector<double> AARSTDMeetGroup ;
		vector<double> AARSTDBeatGroup ;
		vector<double> CAARSTDMissGroup ;
		vector<double> CAARSTDMeetGroup ;
		vector<double> CAARSTDBeatGroup ;
		
		vector<double> AverageAARAcrossSimulationsGroup1, AverageAARAcrossSimulationsGroup2, AverageAARAcrossSimulationsGroup3;
		vector<double> missAvg, meetAvg, beatAvg;
		
		//case 2
		string stock;
		vector<map<string, double>> stockDataMapVec;
		vector<string> ssymbols, sstart_dates, send_dates;
		map<string, double> stockData;
		dVector prices;
		dVector logReturn;
		dVector cumLogReturns;
		int indexofstock;
		int group;
		string annDate;
		string periodEndDate;
		double estimated;
		double reportedEarnings;
		double surprise;
		double surprisePercent;
		
		stockInfo stockInformation(prices, cumLogReturns, group, stock, annDate, periodEndDate, estimated, reportedEarnings, surprise, surprisePercent);
		//case 3
		Matrix res ;
		dVector AvgAAR;
		dVector AARSTD ;
		dVector AvgCAAR ;
		dVector CAARSTD ;
		//case 4
		Matrix mat;
    	
        cout << "\nMenu:\n";
        cout << "1 - Enter N to retrieve 2N+1 days of historical price data for all stocks.\n";
        cout << "2 - Pull information for one stock from one group.\n";
        cout << "3 - Show AAR, AAR-STD, CAAR, CAAR-STD for one group.\n";
        cout << "4 - Plot Avg CAAR for all 3 groups.\n";
        cout << "5 - Exit.\n";
        cout << "Enter your choice and press return: ";
        cin >> choice;

        switch (choice) {
            case 1:
                
                cout << "Enter N (N must be between 60 and 90): \n";
                cin >> daysAround;
                while(daysAround > 90 || daysAround < 60){
                	cout << "N must be between 60 and 90" << endl;
                	cout << "Enter N (N must be between 60 and 90): \n";
                	cin >> daysAround;
                }
				
				bstart_dates.push_back(addOrSubtractDays(minDateForBenchMark, -1*daysAround));
				bend_dates.push_back(addOrSubtractDays(maxDateForBenchmark, daysAround));
				getData2(benchmarkDataMapVec, bsymbols, bstart_dates, bend_dates);
				benchmarkDataMap = benchmarkDataMapVec[0];
				
				cout << "Simulation Number: ";
				for(int numSimulations = 0; numSimulations < 40; numSimulations++){
					vector<map<string, double>> stockPriceDataMapVec;
					vector<string> symbols, start_dates, end_dates;
					
					// push the 3 groups stocks inside
					vector<pair<string, double>>::iterator itr;
					// include random 30 tickers  and corresponding dates from miss group
					for(int i = 0; i < numStocksInGroup; i++){
						itr = miss + (rand() % part_size);
						symbols.push_back(itr->first);
						start_dates.push_back(addOrSubtractDays(earningsDateMap[itr->first], -1*daysAround));
						end_dates.push_back(addOrSubtractDays(earningsDateMap[itr->first], daysAround));
					}
					
					// include random 30 tickers and corresponding dates from meet group
					for(int i = 0; i < numStocksInGroup; i++){
						itr = meet + (rand() % part_size);
						symbols.push_back(itr->first);
						start_dates.push_back(addOrSubtractDays(earningsDateMap[itr->first], -1*daysAround));
						end_dates.push_back(addOrSubtractDays(earningsDateMap[itr->first], daysAround));
					}
					
					// include  random 30 tickers and corresponding dates from beat group
					for(int i = 0; i < numStocksInGroup; i++){
						itr = beat + (rand() % (part_size));
						symbols.push_back(itr->first);
						start_dates.push_back(addOrSubtractDays(earningsDateMap[itr->first], -1*daysAround));
						end_dates.push_back(addOrSubtractDays(earningsDateMap[itr->first], daysAround));
					}
					
					getData2(stockPriceDataMapVec, symbols, start_dates, end_dates);
					//cout << "Number of tickers accessed in one simulation: " << stockPriceDataMapVec.size() << endl;
			
					for(size_t i = 0; i <symbols.size(); i++){
						pulledStockDataMap[symbols[i]] = stockPriceDataMapVec[i];
					}
					// Check if got all the data of 90 stocks
					/*for(int m=0; m < 3*numStocksInGroup; m++ ){
						cout << stockPriceDataMapVec[m].size() << " ";
					}
					cout << endl;*/
					
					// get daily abnormal returns for group1
					vector<dVector> AbnormalReturnsEntireGroup1;
					dVector AARtGroup1;
					bool firstTime = true;
					for(int k = 0; k < numStocksInGroup; k++){
						dVector stockPrices, stockLogReturns;
						dVector benchMarkPrices, benchMarkLogReturns;
						
						// get stock prices and corresponding benchmark prices
						for (auto it = stockPriceDataMapVec[k].begin(); it != stockPriceDataMapVec[k].end(); ++it) { //it -> first is the date
							stockPrices.push_back(stockPriceDataMapVec[k][it->first]); // it->first is the date
							benchMarkPrices.push_back(benchmarkDataMap[it->first]);
						}
						// the data for the stock was unavailable skip over it
						if(stockPrices.size() < 1.9*daysAround){
							continue;
						}
						calculateLogReturns(stockPrices, stockLogReturns);
						calculateLogReturns(benchMarkPrices, benchMarkLogReturns);
						if(firstTime){
							firstTime = false;
							AARtGroup1.resize(stockLogReturns.size());
						}
						AARtGroup1 = AARtGroup1 + (stockLogReturns - benchMarkLogReturns);
						AbnormalReturnsEntireGroup1.push_back(stockLogReturns - benchMarkLogReturns);
					}
					// diving by number of stocks in the group
					AARtGroup1 = AARtGroup1/(AbnormalReturnsEntireGroup1.size());
					AARMissGroup.push_back(AARtGroup1);
					dVector CAARtGroup1 = calculateCAARt(AARtGroup1);
					CAARMissGroup.push_back(CAARtGroup1);
					
					vector<dVector> AbnormalReturnsEntireGroup2;
					dVector  AARtGroup2;
					firstTime = true;
					for(int k = numStocksInGroup; k < 2*numStocksInGroup; k++){
						dVector stockPrices, stockLogReturns;
						dVector benchMarkPrices, benchMarkLogReturns;
						// get stock prices and corresponding benchmark prices
						for (auto it = stockPriceDataMapVec[k].begin(); it != stockPriceDataMapVec[k].end(); ++it) { //it -> first is the date
							stockPrices.push_back(stockPriceDataMapVec[k][it->first]);
							benchMarkPrices.push_back(benchmarkDataMap[it->first]);
						}
						// the data for the stock was unavailable skip over it
						// if(stockPrices.size() == 0){
						if(stockPrices.size() < 1.9*daysAround){
							continue;
						}
						calculateLogReturns(stockPrices, stockLogReturns);
						calculateLogReturns(benchMarkPrices, benchMarkLogReturns);
						if(firstTime){
							firstTime = false;
							AARtGroup2.resize(stockLogReturns.size());
						}
						AARtGroup2 = AARtGroup2 + (stockLogReturns - benchMarkLogReturns);
						AbnormalReturnsEntireGroup2.push_back(stockLogReturns - benchMarkLogReturns);
					}
					// diving by number of stocks in the group
					AARtGroup2 = AARtGroup2/(AbnormalReturnsEntireGroup2.size());
					AARMeetGroup.push_back(AARtGroup2);
					dVector CAARtGroup2 = calculateCAARt(AARtGroup2);
					CAARMeetGroup.push_back(CAARtGroup2);
					
					// get daily abnormal returns for group1
					vector<dVector> AbnormalReturnsEntireGroup3;
					dVector  AARtGroup3;
					firstTime = true;
					for(int k = 2*numStocksInGroup; k < 3*numStocksInGroup; k++){
						dVector stockPrices, stockLogReturns;
						dVector benchMarkPrices, benchMarkLogReturns;
						// get stock prices and corresponding benchmark prices
						for (auto it = stockPriceDataMapVec[k].begin(); it != stockPriceDataMapVec[k].end(); ++it) { //it -> first is the date
							stockPrices.push_back(stockPriceDataMapVec[k][it->first]);
							benchMarkPrices.push_back(benchmarkDataMap[it->first]);
						}
						// the data for the stock was unavailable skip over it
						// if(stockPrices.size() == 0){
						if(stockPrices.size() < 1.9*daysAround){
							continue;
						}
						calculateLogReturns(stockPrices, stockLogReturns);
						calculateLogReturns(benchMarkPrices, benchMarkLogReturns);
						if(firstTime){
							firstTime = false;
							AARtGroup3.resize(stockLogReturns.size());
						}
						AARtGroup3 = AARtGroup3 + (stockLogReturns - benchMarkLogReturns);
						AbnormalReturnsEntireGroup3.push_back(stockLogReturns - benchMarkLogReturns);
					}
					// diving by number of stocks in the group
					AARtGroup3 = AARtGroup3/AbnormalReturnsEntireGroup3.size();
					AARBeatGroup.push_back(AARtGroup3);
					dVector CAARtGroup3 = calculateCAARt(AARtGroup3);
					CAARBeatGroup.push_back(CAARtGroup3);
					
					if(numSimulations == 0){
						AverageCAARAcrossSimulationsGroup1.resize(CAARtGroup1.size());
						AverageCAARAcrossSimulationsGroup2.resize(CAARtGroup2.size());
						AverageCAARAcrossSimulationsGroup3.resize(CAARtGroup3.size());
					}
					AverageCAARAcrossSimulationsGroup1 = AverageCAARAcrossSimulationsGroup1 + CAARtGroup1;
					AverageCAARAcrossSimulationsGroup2 = AverageCAARAcrossSimulationsGroup2 + CAARtGroup2;
					AverageCAARAcrossSimulationsGroup3 = AverageCAARAcrossSimulationsGroup3 + CAARtGroup3;
					cout << numSimulations << " " << std::flush;
				}
				
				AARSTDMissGroup = calculateStdDev(AARMissGroup);
				AARSTDMeetGroup = calculateStdDev(AARMeetGroup);
				AARSTDBeatGroup = calculateStdDev(AARBeatGroup);
				CAARSTDMissGroup = calculateStdDev(CAARMissGroup);
				CAARSTDMeetGroup = calculateStdDev(CAARMeetGroup);
				CAARSTDBeatGroup = calculateStdDev(CAARBeatGroup);
				
				AverageAARAcrossSimulationsGroup1 = calculateMean(AARMissGroup);
				AverageAARAcrossSimulationsGroup2 = calculateMean(AARMeetGroup);
				AverageAARAcrossSimulationsGroup3 = calculateMean(AARBeatGroup);
				
				AverageCAARAcrossSimulationsGroup1 = AverageCAARAcrossSimulationsGroup1/40;
				AverageCAARAcrossSimulationsGroup2 = AverageCAARAcrossSimulationsGroup2/40;
				AverageCAARAcrossSimulationsGroup3 = AverageCAARAcrossSimulationsGroup3/40;
				
				ResultofSimulation[0].push_back(AverageAARAcrossSimulationsGroup1);
				ResultofSimulation[0].push_back(AARSTDMissGroup);
				ResultofSimulation[0].push_back(AverageCAARAcrossSimulationsGroup1);
				ResultofSimulation[0].push_back(CAARSTDMissGroup);
				
				ResultofSimulation[1].push_back(AverageAARAcrossSimulationsGroup2);
				ResultofSimulation[1].push_back(AARSTDMeetGroup);
				ResultofSimulation[1].push_back(AverageCAARAcrossSimulationsGroup2);
				ResultofSimulation[1].push_back(CAARSTDMeetGroup);
				
				ResultofSimulation[2].push_back(AverageAARAcrossSimulationsGroup3);
				ResultofSimulation[2].push_back(AARSTDBeatGroup);
				ResultofSimulation[2].push_back(AverageCAARAcrossSimulationsGroup3);
				ResultofSimulation[2].push_back(CAARSTDBeatGroup);
                break;
                
            case 2:
                // string stock;
			    cout << "Enter the stock ticker: ";
			    cin >> stock;
				
				// converting into format that getData2 accepts
				ssymbols.push_back(stock);
				sstart_dates.push_back(addOrSubtractDays(earningsDateMap[stock], -1*daysAround));
				send_dates.push_back(addOrSubtractDays(earningsDateMap[stock], daysAround));
				getData2(stockDataMapVec, ssymbols, sstart_dates, send_dates);
				
				stockData = stockDataMapVec[0];
				if(stockData.size() == 0){
					cout << "Stock with bad prices." << endl;
				}
				for(auto it = stockData.begin(); it!=stockData.end(); it++){
					prices.push_back(it->second);
				}
				calculateLogReturns(prices, logReturn);
				
				cumLogReturns = calculateCAARt(logReturn);
				
				for(int i=0; i < 2349; i++){
					if(stock.compare((miss + i)->first) == 0){
						indexofstock = i;
						if(indexofstock >= 0 && indexofstock < 783){
							group = 0;
						}else if(indexofstock >= 783 && indexofstock < 1566){
							group = 1;
						}else{
							group = 2;
						}
						break;
					}
					
				}
				
				annDate = earningsDateMap[stock];
				periodEndDate = periodEndingDateMap[stock];
				estimated = nonComputationalData[stock][0];
				reportedEarnings = nonComputationalData[stock][1];
				surprise = nonComputationalData[stock][2];
				// surprisePercent;
				for (const auto& pair : earningsSurprisePairVect) {
			        if (pair.first == stock) {
			            surprisePercent = pair.second;
			            break;
			        }
			    }
				stockInformation.setDailyPrice(prices);
				stockInformation.setCumulativeDailyReturn(cumLogReturns);
				stockInformation.setGroupNum(group);
				stockInformation.setTicker(stock);
				stockInformation.setAnnouncementDate(annDate);
				stockInformation.setPeriodEnding(periodEndDate);
				stockInformation.setEstimatedEarning(estimated);
				stockInformation.setReportEarning(reportedEarnings);
				stockInformation.setSurprise(surprise);
				stockInformation.setSurprisePercentage(surprisePercent);
				
				stockInformation.printInfo();
                break;
            case 3:
                int groupNum;
                cout << "Enter group number (0: Miss, 1: Meet, 2: Beat) for detailed information: ";
                cin >> groupNum;
                
				if(groupNum > 2){
					cout << "Invalid group number! Please enter again! (0: Miss, 1: Meet, 2: Beat) ";
				}
				
				res = ResultofSimulation[groupNum];
				AvgAAR = res[0];
				AARSTD = res[1];
				AvgCAAR = res[2];
				CAARSTD = res[3];
				cout << "Average AAR for group selected: " << endl;
				displayVector(AvgAAR);
				cout << "STD AAR for group selected: "<< endl;
				displayVector(AARSTD);
				cout << "Average CAAR for group selected: "<< endl;
				displayVector(AvgCAAR);
				cout << "STD CAAR for group selected: "<< endl;
				displayVector(CAARSTD);
                break;
            case 4:
            {
				mat.push_back(ResultofSimulation[0][2]);
				mat.push_back(ResultofSimulation[1][2]);
				mat.push_back(ResultofSimulation[2][2]);
				
				Gnuplot plotter(mat);
			    plotter.PlotToFile("caar_chart.png");
			    // plotter.PlotToScreenAndFile("caar_chart.png");
			
			    std::cout << "CAAR chart generated as 'caar_chart.png'." << std::endl;
                break;
            }
            
            case 5:
                running = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
	return 0;
}


