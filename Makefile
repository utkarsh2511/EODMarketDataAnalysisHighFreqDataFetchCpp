

# # Define dependencies between your main program and the individual components
# EODMarketData: EODMarketData.o EarningsAnnouncementProcessing.o gnuplot.o stockInfo.o Overload.o Calculation.o
# 	$(CC) $(CFLAGS) -o EODMarketData EODMarketData.o EarningsAnnouncementProcessing.o gnuplot.o stockInfo.o Overload.o Calculation.o

# # Main program dependency
# EODMarketData.o: EODMarketData.cpp EarningsAnnouncementProcessing.h gnuplot.h Overload.h stockInfo.h Calculation.h
# 	$(CC) $(CFLAGS) -c EODMarketData.cpp

# # Component A dependency
# EarningsAnnouncementProcessing.o: EarningsAnnouncementProcessing.cpp EarningsAnnouncementProcessing.h
# 	$(CC) $(CFLAGS) -c EarningsAnnouncementProcessing.cpp

# # Component B dependency
# gnuplot.o: gnuplot.cpp gnuplot.h
# 	$(CC) $(CFLAGS) -c gnuplot.cpp

# # Component C dependency
# stockInfo.o: stockInfo.cpp stockInfo.h Calculation.h
# 	$(CC) $(CFLAGS) -c stockInfo.cpp
	
# Calculation.o: Calculation.cpp Calculation.h
# 	$(CC) $(CFLAGS) -c Calculation.cpp

# # Component D dependency
# Overload.o: Overload.cpp Overload.h
# 	$(CC) $(CFLAGS) -c Overload.cpp

# clean:
# 	rm -rf EODMarketData *.o

CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11 -lcurl

EODMarketData: EODMarketData.o EarningsAnnouncementProcessing.o gnuplot.o stockInfo.o Overload.o Calculation.o
	$(CC) $(CFLAGS) -o EODMarketData EODMarketData.o EarningsAnnouncementProcessing.o gnuplot.o stockInfo.o Overload.o Calculation.o

EODMarketData.o: EODMarketData.cpp
	$(CC) $(CFLAGS) -c EODMarketData.cpp

EarningsAnnouncementProcessing.o: EarningsAnnouncementProcessing.cpp EarningsAnnouncementProcessing.h
	$(CC) $(CFLAGS) -c EarningsAnnouncementProcessing.cpp

gnuplot.o: gnuplot.cpp gnuplot.h
	$(CC) $(CFLAGS) -c gnuplot.cpp

stockInfo.o: stockInfo.cpp stockInfo.h Calculation.h
	$(CC) $(CFLAGS) -c stockInfo.cpp

Overload.o: Overload.cpp Overload.h
	$(CC) $(CFLAGS) -c Overload.cpp

Calculation.o: Calculation.cpp Calculation.h Overload.h
	$(CC) $(CFLAGS) -c Calculation.cpp
    
clean:
	rm -rf EODMarketData *.o
