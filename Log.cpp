//
// Created by Шахова Анастасия on 19.04.2021.
//

#include "Log.h"
#include "SqlParser.h"

Log * Log::logger_instance = nullptr;

Log::Log():
        run(true)
{
    out_.open("log.txt", std::ios::app);
}

Log * Log::getInstance()
{
    if (logger_instance == nullptr)
    {
        return new Log();
    }
    return logger_instance;
}

void Log::log(std::vector<unsigned char> data_for_logging)
{
    std::unique_lock<std::mutex> lock(the_mutex);

    if (!packets.empty() && !packets.back().is_ready_) {
        sql_parser.parse(packets.back(), data_for_logging);
    }
    else {
        Packet new_packet;
        sql_parser.parse(new_packet, data_for_logging);
        packets.push_back(new_packet);
    }
    if (packets.back().is_ready_) {
        buffer_is_not_empty_.notify_one();
    }
}

void Log::writeData()
{
    while (run)
    {
        std::unique_lock<std::mutex> lock(the_mutex);
        buffer_is_not_empty_.wait(lock, [this]() { return !packets.empty(); });

        size_t size_of_commands_buffer = 0;
        for (Packet &packet : packets)
        {
            size_of_commands_buffer += packet.command_string_.size() + 4;
        }
        std::string commands_buffer;
        commands_buffer.reserve(size_of_commands_buffer);

        while (!packets.empty())
        {
            commands_buffer += packets.front().command_string_ + " | " + packets.front().query_ + "\n";
            packets.erase(packets.begin());
        }

        lock.unlock();

        out_ << commands_buffer;
    }
}

Log::~Log()
{
    out_.close();
    run = false;
}