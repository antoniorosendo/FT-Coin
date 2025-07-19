#include <iomanip>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include "Date.hpp"

using namespace std;

Date::Date() {
    time_t now = time(NULL);
    tm *currentTime = localtime(&now);
    day = currentTime->tm_mday;
    month = currentTime->tm_mon + 1;
    year = currentTime->tm_year + 1900;
}

Date::Date(string isoFormat) {
    try {
        if (isoFormat.size() < 10)
            throw std::invalid_argument("Formato de data incompleto");

        year = std::stoi(isoFormat.substr(0, 4));
        month = std::stoi(isoFormat.substr(5, 2));
        day = std::stoi(isoFormat.substr(8, 2));
    } catch (...) {
        throw std::invalid_argument("Formato invalido de data ISO: " + isoFormat);
    }
}

Date::Date(int day, int month, int year) :
    day(day), month(month), year(year)
{
}

Date::~Date() {}

int Date::getYear() { return year; }
int Date::getMonth() { return month; }
int Date::getDay() { return day; }

std::string Date::getIsoFormat() const {
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << year << "-"
        << std::setw(2) << std::setfill('0') << month << "-"
        << std::setw(2) << std::setfill('0') << day;
    return oss.str();
}

bool Date::operator==(const Date &other) const {
    return year == other.year && month == other.month && day == other.day;
}

bool Date::operator!=(const Date &other) const {
    return !(*this == other);
}

bool Date::operator>(const Date &other) const {
    if (year != other.year)
        return year > other.year;
    if (month != other.month)
        return month > other.month;
    return day > other.day;
}

bool Date::operator>=(const Date &other) const {
    return (*this == other) || (*this > other);
}

bool Date::operator<(const Date &other) const {
    return !(*this >= other);
}

bool Date::operator<=(const Date &other) const {
    return !(*this > other);
}

ostream& operator<<(ostream &os, const Date &date) {
    os << setfill('0') << setw(4) << date.year << "."
       << setfill('0') << setw(2) << date.month << "."
       << setfill('0') << setw(2) << date.day;
    return os;
}

istream& operator>>(std::istream &is, Date &date) {
    char separator;
    is >> date.day >> separator >> date.month >> separator >> date.year;
    return is;
}