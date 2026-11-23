#ifndef ACTIVITY_H
#define ACTIVITY_H

#include "./nlohmann/json.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

struct activity
{
    std::string name;
    time_t timestamp;
    int time;
    
    json to_json() const
    {
        return json
        {
            {"name", name},
            {"date", static_cast<long>(timestamp)},
            {"time", time}
        };
    }
};

std::string filePath = "./activities";

std::string& activityName(std::string& name)
{
    char c;
    while (std::cin.get(c) && c != '\n')
    {
        if (c == ' ')
            name += '_';
        else
            name += c;
    }
    return name;
}

void createActivity()
{
    if (!std::filesystem::exists(filePath))
    {
        std::filesystem::create_directory("activities");
    }
    
    std::string name;
    
    time_t timestamp;
    time(&timestamp);
    
    std::cin >> name;
    activityName(name);
    
    activity act = {name, timestamp, 0};
    for (const auto & entry : std::filesystem::directory_iterator(filePath))
    {
        //std::cout << entry.path() << std::endl;
        if (entry.path().filename() == name + ".json")
        {
            std::cout << "Error. Can't have more than 1 activity with the same name" << "\n";
            return;
        }
    }
    std::ofstream activityFile(filePath + '/' + name + ".json");
    activityFile << act.to_json().dump(4);
    activityFile.close();
    
    std::cout << "\nCreated a new activity called: " << name << "\n" << ctime(&act.timestamp) << "\n";
}

void viewActivity();

std::string tolowerStr(const std::string& str)
{
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

void timeOutput(int elapsedTime)
{
    int hours = elapsedTime / 3600;
    int minutes = (elapsedTime % 3600) / 60;
    int seconds = elapsedTime % 60;

    std::cout << std::setw(2) << std::setfill('0') << hours << ':';
    std::cout << std::setw(2) << std::setfill('0') << minutes << ':';
    std::cout << std::setw(2) << std::setfill('0') << seconds;
}

void startActivity()
{
    std::cout << "#\t Write activity's name to start\t\t#" << "\n";

    std::vector<std::string> activityName;
    
    for (const auto & entry : std::filesystem::directory_iterator(filePath))
    {
        std::cout << entry.path().filename().replace_extension() << std::endl;
        activityName.push_back(entry.path().filename().replace_extension().string());
    }
        
    std::string userInput;
    std::cin >> userInput;

    for (auto & elem : activityName)
    {
        if (tolowerStr(userInput) == tolowerStr(elem))
        {
            time_t timestampStart;
            time(&timestampStart);

            char exitSymbol;

            do
            {
                std::cout << "Activity started at: " << ctime(&timestampStart);
                std::cout << "Press Q to exit: ";

                std::cin >> exitSymbol;

            } while (tolower(exitSymbol) != 'q');

            time_t timestampEnd;
            time(&timestampEnd);

            int elapsedTime = static_cast<int>(difftime(timestampEnd, timestampStart));

            std::cout << "Activity ended at: " << ctime(&timestampEnd);
            
            std::cout << "Activity lasted for ";
            timeOutput(elapsedTime);
            std::cout << "\n" << std::endl;

            std::ifstream activityFile(filePath + '/' + elem + ".json");
            if (!activityFile.is_open())
            {
                std::cerr << "Failed to open the json file for reading" << std::endl;
                return;
            }

            json j;
            activityFile >> j;
            activityFile.close();
            
            j["time"] = j["time"].get<int>() + elapsedTime;
            
            std::ofstream activityFileOut(filePath + '/' + elem + ".json");
            if (!activityFileOut.is_open())
            {
                std::cerr << "Failed to open the json file for writing" << std::endl;
                return;
            }

            activityFileOut << j.dump(4);
            activityFileOut.close();

            return;
        }
    }
    std::cout << "Error. Wrong activity name" << std::endl;
}

void viewActivity()
{
    int index = 1;

    for (const auto & entry : std::filesystem::directory_iterator(filePath))
    {
        std::ifstream activityFile(filePath + '/' + entry.path().filename().string());
        if (!activityFile.is_open())
        {
            std::cerr << "Failed to open the json file for reading" << std::endl;
            return;
        }
        
        json j;
        activityFile >> j;
        activityFile.close();
        
        std::cout << std::setiosflags(std::ios::left);
        std::cout << index << ". " << std::setw(20) << std::setfill(' ') << entry.path().filename().replace_extension().string();
        std::cout << std::resetiosflags(std::ios::left);
        
        timeOutput(j["time"]);
        std::cout << std::endl;

        index++;
    }
    std::cout << std::endl;
}
#endif
