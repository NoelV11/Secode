Challenge - Error Message Exposure

//
//  messages_model.cpp
//  Biology_Laboratory
//

#include "messages-model.hpp"

#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

MessagesModel::MessagesModel()
{
    dbMessages = "/home/Biology_Laboratory/data/messages.log";
}

vector<vector<string>> MessagesModel::AllData()
{
    vector<vector<string>> loggingData;
    vector<string> eachData;
    string line, element;
    int i = 0;
    std::ifstream ifs(dbMessages, std::ifstream::in);
    if (ifs.fail())
      std::cout << "Unable to open file." << std::endl;
    else
    {
      while (getline(ifs, line))
      {
          loggingData.push_back(eachData);
          std::stringstream ss(line);
          while(getline(ss, element, '|'))
          {
              loggingData.at(i).push_back(element);
          }
          i++;
      }
    }
    if (ifs.bad())
      std::cout << "Error while reading file." << dbMessages <<std::endl;
    ifs.close();
    line.clear();
    return loggingData;
}

vector<vector<string>> MessagesModel::GetAllMessagesLog()
{
    vector<vector<string>> messagesData;
    messagesData = AllData();
    return messagesData;
}

vector<vector<string>> MessagesModel::GetMessagesBy(string dataRec, int opc)
{
    vector<vector<string>> data = AllData();
    for(int i = 0; i < data.size(); i++)
    {
      if(!data.at(i).empty())
      {
        if(data.at(i).at(opc).compare(dataRec) != 0)
        {
            data.at(i).erase(data.at(i).begin(), data.at(i).begin()+3);
            data.erase(data.begin()+i);
        }
        else
            continue;
      }
    }
    return data;
}

void MessagesModel::SetMessageData(string message, string name, string date, string type)
{
    std::ofstream ofs(dbMessages, std::ofstream::app);
    ofs << type << " | " << name << " | " << date << " | " << message << endl;
    ofs.close();
}

Solution

//
//  messages_model.cpp
//  Biology_Laboratory
//

#include "messages-model.hpp"

#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

MessagesModel::MessagesModel()
{
    dbMessages = "../data/messages.log";
}

vector<vector<string>> MessagesModel::AllData()
{
    vector<vector<string>> loggingData;
    vector<string> eachData;
    string line, element;
    int i = 0;
    std::ifstream ifs(dbMessages, std::ifstream::in);
    if (ifs.fail())
      std::cout << "Unable to open file." << std::endl;
    else
    {
      while (getline(ifs, line))
      {
          loggingData.push_back(eachData);
          std::stringstream ss(line);
          while(getline(ss, element, '|'))
          {
              loggingData.at(i).push_back(element);
          }
          i++;
      }
    }
    if (ifs.bad())
      std::cout << "Error while reading system journal." << std::endl;
    ifs.close();
    line.clear();
    return loggingData;
}

vector<vector<string>> MessagesModel::GetAllMessagesLog()
{
    vector<vector<string>> messagesData;
    messagesData = AllData();
    return messagesData;
}

vector<vector<string>> MessagesModel::GetMessagesBy(string dataRec, int opc)
{
    vector<vector<string>> data = AllData();
    for(int i = 0; i < data.size(); i++)
    {
      if(!data.at(i).empty())
      {
        if(data.at(i).at(opc).compare(dataRec) != 0)
        {
            data.at(i).erase(data.at(i).begin(), data.at(i).begin()+3);
            data.erase(data.begin()+i);
        }
        else
            continue;
      }
    }
    return data;
}

void MessagesModel::SetMessageData(string message, string name, string date, string type)
{
    std::ofstream ofs(dbMessages, std::ofstream::app);
    ofs << type << " | " << name << " | " << date << " | " << message << endl;
    ofs.close();
}
