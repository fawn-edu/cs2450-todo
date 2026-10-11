#include <stdexcept>
#include <sstream>
#include <string>
#include <iomanip>
#include "task.hpp"

static std::chrono::sys_seconds parse_date(const std:: string& s) {
    std::istringstream iss(s);
    std::chrono::sys_time<std::chrono::seconds> tp;
    iss >> std::chrono::parse("%F", tp); // %F in this case is YYYY-MM-DD if we want a different date format, let me know

    if (iss.fail()) {
        throw std::invalid_argument("Invalid date" + s);
    }
    return tp;
}

task task::parse(std::string_view serialized) {
    std::string s(serialized);
    std::istringstream iss(s);

    task t;
    std::string token;

    iss >> token;
    if (token == "x") {
        // If the task is completed
        std::string completed_date_str;
        std::string created_date_str;

        if (!(iss >> completed_date_str)) {
            throw std::invalid_argument("Missing completed date");
        }
        if (!(iss >> created_date_str)) {
            throw std::invalid_argument("Missing created date");
        }
        t.completed = parse_date(completed_date_str);
        t.created = parse_date(created_date_str);
    }
    else {
        // If it's not completed, makes the first token the created date
        t.completed = std::nullopt;
        t.created = parse_date(token);
    }

    // This will make everything beyond the date the task name
    std::string name;
    std::getline(iss, name);

    if (name.empty()) {
        throw std::invalid_argument("Missing a task name");
    }

    // Getting rid of the space that comes before the date
    if (!name.empty() && name.front() == ' ') {
        name.erase(0,1);
    }
    t.name = name;
    return t;
}
