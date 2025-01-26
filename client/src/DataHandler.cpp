#include "../include/DataHandler.h"
#include "../include/OutputHandler.h"
#include "../include/event.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>


DataHandler::DataHandler(std::unordered_map<std::string, std::list<Frame>> &serverMessages) : serverMessages(serverMessages)
{
}
std::string DataHandler::getSummary(const std::string &user, const std::string &channel_name) const
{
    std::ostringstream summary;
    int reportCount = 0;
    int activeCount = 0;
    int forceCount = 0;
    std::list<Frame> reports;
    for (const auto &frame : serverMessages[channel_name])
    {
    for (const auto &frame : serverMessages[channel_name])
    {
        std::string user1 =frame.getValueFromBody("user");
        if (user1 == user)
        {
            reports.push_back(frame);
            if (frame.getValueFromBody("active") == "true")
                activeCount++;
            if (frame.getValueFromBody("forces_arrival_at_scene") == "true")
                forceCount++;
        }
    }
    reports.sort([](const Frame& a, const Frame& b) {
        return std::stoi(a.getValueFromBody("date time")) < std::stoi(b.getValueFromBody("date time")); // Sort in ascending order
    });
    summary << "Channel " << channel_name << "\n"
            << "Stats: \n"
            << "Total: " << reports.size() << "\n"
            << "Active: " << activeCount << "\n"
            << "Forces arrival at scene: " << forceCount << "\n\n"
            << "Event Reports: \n\n";
    for (const auto &frame : reports)
    {
        reportCount++;
        summary << "Report_" << reportCount << ":\n"
                << "\t" << "city: " << frame.getValueFromBody("city") << "\n"
                << "\t" << "date time: " << formatDateTime(frame.getValueFromBody("date time")) << "\n"
                << "\t" << "event name: " << frame.getValueFromBody("event name") << "\n"
                << "\t" << "description: " << formatSummary(frame.getValueFromBody("Description")) << "\n";
    }
    return summary.str();
}
}

std::string DataHandler::formatDateTime(std::string input) const
{
    // Try to parse the input as a Unix timestamp (integer)
    try {
        std::time_t timestamp = std::stoll(input); // Convert string to integer (std::time_t)

        // Convert the timestamp to a human-readable date and time
        std::ostringstream result;
        result << std::put_time(std::localtime(&timestamp), "%Y-%m-%d %H:%M:%S");

        return result.str();
    } catch (const std::exception& e) {
        return "Invalid timestamp input: " + input;
    }
}
std::string DataHandler::formatSummary(std::string input) const
{
    if (input.length() > 30)
        input = input.substr(0, 27) + "...";
    return input;
}