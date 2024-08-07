#include "indicators.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>  // for std::replace
#include <numeric>    // for std::accumulate

namespace indicator
{
    // Parses CSV data  
    std::vector<Data> importData(const std::string& fileName)
    {
        std::vector<Data> dataset;
        std::ifstream file(fileName);

        if (!file.is_open())
        {
            std::cerr << "Could not open file: " << fileName << "\n";
            return dataset;
        }

        std::string line;
        bool header = true;

        while (std::getline(file, line))
        {
            if (header)
            {
                header = false;
                continue;
            }

            std::replace(line.begin(), line.end(), ',', ' ');
            std::stringstream ss(line);
            Data point;
            ss >> point.Ticker >> point.date >> point.open >> point.high >> point.low >> point.Close >> point.volume >> point.EPS >> point.P_BV  >> point.Yield ;  
            dataset.push_back(point);
        }

        return dataset;
    }

    void MA::simpleMA(std::vector<Data>& dataset, const unsigned int period)
    {
        std::cout << "Calculating SMA\n";

        for (size_t i = period - 1; i < dataset.size(); ++i)
        {
            double sum = 0;
            for (size_t j = i - (period - 1); j <= i; ++j)
            {
                sum += dataset[j].Close;
            }

            dataset[i].SMA = sum / period;
        }
    }

    void MA::exponentialMA(std::vector<Data>& dataset, const unsigned int period)
    {
        std::cout << "Calculating EMA\n";

        double multiplier = 2.0 / (period + 1);

        dataset[period - 1].EMA = dataset[period - 1].Close;

        for (size_t i = period; i < dataset.size(); ++i)
        {
            dataset[i].EMA = (dataset[i].Close - dataset[i - 1].EMA) * multiplier + dataset[i - 1].EMA;
        }
    }

    void Oscillator::rsi(std::vector<Data>& dataset, const unsigned int period)
    {
        std::cout << "Calculating RSI\n";

        if (dataset.size() < period)
            return;

        std::vector<double> gains, losses;
        gains.reserve(dataset.size());
        losses.reserve(dataset.size());

        // Calculate the initial gains and losses
        for (size_t i = 1; i < dataset.size(); ++i)
        {
            double change = dataset[i].Close - dataset[i - 1].Close;
            if (change > 0)
            {
                gains.push_back(change);
                losses.push_back(0);
            }
            else
            {
                gains.push_back(0);
                losses.push_back(-change);
            }
        }

        // Calculate average gain and loss
        double avgGain = std::accumulate(gains.begin(), gains.begin() + period, 0.0) / period;
        double avgLoss = std::accumulate(losses.begin(), losses.begin() + period, 0.0) / period;

        // Calculate RSI
        for (size_t i = period; i < dataset.size(); ++i)
        {
            double gain = gains[i - 1];
            double loss = losses[i - 1];

            avgGain = ((avgGain * (period - 1)) + gain) / period;
            avgLoss = ((avgLoss * (period - 1)) + loss) / period;

            double rs = avgLoss == 0 ? 100 : avgGain / avgLoss;
            dataset[i].RSI = 100 - (100 / (1 + rs));
        }
    }

    void MA::calculatePERatio(std::vector<Data>& dataset, const std::vector<double>& epsData)
    {
        std::cout << "Calculating P/E Ratio\n";

        for (size_t i = 0; i < dataset.size(); ++i)
        {
            if (epsData[i] != 0 && dataset[i].Close != 0)
            {
                dataset[i].PE = dataset[i].Close / epsData[i];
            }
            else
            {
                dataset[i].PE = 0;
            }
        }
    }
    void defineBuyingConditions(std::vector<Data>& dataset)
    {
        std::cout << "Calculating Buying Conditions\n";

        if (dataset.size() < 52) // Ensure we have enough data
            return;

        // Calculate rolling max and min for 15 days
        std::vector<double> rollingMax(dataset.size(), 0.0);
        std::vector<double> rollingMin(dataset.size(), 0.0);

        for (size_t i = 14; i < dataset.size(); ++i)
        {
            double max = dataset[i].Close;
            double min = dataset[i].Close;
            for (size_t j = i - 14; j <= i; ++j)
            {
                max = std::max(max, dataset[j].high);
                min = std::min(min, dataset[j].low);
            }
            rollingMax[i] = max;
            rollingMin[i] = min;
        }

        // Set new high and new low flags
        for (size_t i = 1; i < dataset.size(); ++i)
        {
            bool newHigh = dataset[i].high > rollingMax[i - 1];
            bool newLow = dataset[i].low < rollingMin[i - 1];
            dataset[i].SETNewHigh = newHigh;
            dataset[i].SETNewLow = newLow;
            dataset[i].SETGood = (newHigh != dataset[i - 1].SETNewLow);
        }

        // Calculate 52 weeks high
        std::vector<double> z52WeeksHigh(dataset.size(), 0.0);
        for (size_t i = 51; i < dataset.size(); ++i)
        {
            double max = dataset[i].Close;
            for (size_t j = i - 51; j <= i; ++j)
            {
                max = std::max(max, dataset[j].Close);
            }
            z52WeeksHigh[i] = max;
        }

        // Calculate Buy conditions
        for (size_t i = 0; i < dataset.size(); ++i)
        {
             dataset[i].Buy1 = dataset[i].SETGood;
             dataset[i].Buy2 = dataset[i].high > z52WeeksHigh[i];
             dataset[i].Buy3 = dataset[i].Close * dataset[i].volume > 1500000;
             dataset[i].Buy4 = (dataset[i].PE > 0) && (dataset[i].PE < 10);
             dataset[i].Buy5 = (dataset[i].P_BV > 0) && (dataset[i].P_BV < 2);
             dataset[i].Buy6 = dataset[i].Yield > 3;
            // dataset[i].BuySignal = dataset[i].Buy6;
            // dataset[i].BuySignal = dataset[i].Buy2 && dataset[i].Buy4 && dataset[i].Buy5 && dataset[i].Buy6;
             //dataset[i].BuySignal = dataset[i].Buy1 && dataset[i].Buy2 && dataset[i].Buy3 && dataset[i].Buy4 && dataset[i].Buy5 && dataset[i].Buy6;
             dataset[i].BuySignal = dataset[i].Buy1 && dataset[i].Buy2 && dataset[i].Buy3 && dataset[i].Buy4 && dataset[i].Buy5 && dataset[i].Buy6;
              
        
        }
    }
    void MA::saveBuySignalToFile(const std::vector<Data>& dataset, const std::string& fileName)
    {
        std::ofstream file(fileName);

        if (!file.is_open())
        {
            std::cerr << "Could not open output file\n";
            return;
        }

        file << "Ticker,Date,Open,Close,SMA,EMA,RSI,EPS,P/E,P/BV,Yield,BuySignal\n";

        std::cout << "Ticker\tDate\tOpen\tClose\tSMA\tEMA\tRSI\tEPS\tP/E\tP/BV\tYield\tBuySignal\n";

        for (const auto& data : dataset)
        {
            if (data.BuySignal == 1)
            //if (!file.is_open())
            //if (data.BuySignal)
            { 
                file << data.Ticker << "," << data.date << "," << data.open << "," << data.Close << "," << data.SMA << "," << data.EMA << "," << data.RSI << "," << data.EPS << "," << data.PE << "," << data.P_BV << "," << data.Yield << "," << data.BuySignal << "\n";
                
                // Optional: print to console for verification
                 std::cout << data.Ticker << "\t" << data.date << "\t" << data.open << "\t" << data.Close << "\t" << data.SMA << "\t" << data.EMA << "\t" << data.RSI << "\t" << data.PE << "\t" << data.P_BV << "\t" << data.Yield << "\t" << data.BuySignal << "\n";
            }
        }

        file.close();
    }


    void MA::saveToFile(const std::vector<Data>& dataset, const std::string& fileName)
    {
        std::ofstream file(fileName);

        if (!file.is_open())
        {
            std::cerr << "Could not open output file\n";
            return;
        }

        file << "Ticker,Date,Open,Close,SMA,EMA,RSI,EPS,P/E,P/BV,Yield,BuySignal\n";
    
        std::cout << "Ticker\tDate\tOpen\tClose\tSMA\tEMA\tRSI\tEPS\tP/E\tP/BV\tYield\tBuySignal\n";

        for (const auto& data : dataset)
        {
            file << data.Ticker << "," << data.date << "," << data.open << "," << data.Close << "," << data.SMA << "," << data.EMA << "," << data.RSI <<","<< data.EPS << "," << data.PE << "," << data.P_BV << "," << data.Yield << "," << data.BuySignal << "\n";
           // std::cout << data.Ticker << "\t" << data.date << "\t" << data.open << "\t" << data.Close << "\t" << data.SMA << "\t" << data.EMA << "\t" << data.RSI << "\t" << data.PE << "\t" << data.BuySignal << "\n";
        }

        file.close();
    }
}
