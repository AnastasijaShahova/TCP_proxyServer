//
// Created by Шахова Анастасия on 19.04.2021.
//

#ifndef SEEERVER_LOG_H
#define SEEERVER_LOG_H


#include <mutex>
#include <thread>
#include <fstream>
#include <vector>
#include "SqlParser.h"

class Log {
public:

    static Log * getInstance();
    void log(std::vector<unsigned char> data_for_logging);


private:
    Log();

    std::mutex the_mutex;
    static Log * logger_instance;
    std::condition_variable buffer_is_not_empty_;
    bool run;
    std::vector<class Packet> packets;
    std::ofstream out_;
    SqlParser sql_parser;

    void writeData();

    ~Log();
};


#endif //SEEERVER_LOG_H
