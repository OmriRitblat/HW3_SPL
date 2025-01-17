#include "../include/DataHandler.h"
#include "../include/OutputHandler.h"
#include "../include/event.h"

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
    for (const auto &frame : serverMessages[user])
    {
        if (frame.getValue("channel_name") == channel_name)
        {
            reports.push_back(frame);
            if (frame.getValue("active") == "true")
                activeCount++;
            if (frame.getValue("forces_arrival_at_scene") == "true")
                forceCount++;
        }
    }
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
                << "\t" << "city: " << frame.getValue("city") << "\n"
                << "\t" << "date time: " << formatDateTime(frame.getValue("date_time")) << "\n"
                << "\t" << "event name: " << frame.getValue("event_name") << "\n"
                << "\t" << "description: " << formatSummary(frame.getValue("description")) << "\n";
    }
    return summary.str();
}

std::string DataHandler::formatDateTime(std::string input) const
{
    std::string day = input.substr(0, 2);
    std::string month = input.substr(2, 2);
    std::string year = input.substr(4, 2);
    std::string hour = input.substr(6, 2);
    std::string minute = input.substr(8, 2);

    return day + "/" + month + "/" + year + " " + hour + ":" + minute;
}
std::string DataHandler::formatSummary(std::string input) const
{
    if (input.length() > 30)
        input = input.substr(0, 27) + "...";
    return input;
}