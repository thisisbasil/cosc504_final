#include "include/Student.h"
#include <exception>

class CourseNotFound : public std::exception
{
private:
    std::string msg;
public:
    CourseNotFound() : msg("Course not found!") {};
    const char * what() const noexcept { return msg.c_str(); }
};

void Student::insertCourse(const std::string& name, int credits, char grade)
{
    insertCourse(course(name, credits, std::toupper(grade)));
}

void Student::insertCourse(const course& _course)
{
    courses.push_back(_course);
}

std::vector<course>::iterator Student::begin()
{
    return courses.begin();
}

std::vector<course>::iterator Student::end()
{
    return courses.end();
}

std::vector<course>::reverse_iterator Student::rbegin()
{
    return courses.rbegin();
}

std::vector<course>::reverse_iterator Student::rend()
{
    return courses.rend();
}

course& Student::operator[](int idx)
{
    return courses[idx];
}

const course& Student::at(int idx)
{
    return courses.at(idx);
}

std::string Student::getFirstName()
{
    return name.first;
}

std::string Student::getLastName()
{
    return name.second;
}

std::pair<std::string,std::string> Student::getName()
{
    return name;
}

bool Student::operator==(const Student &other) const
{
    if (name.second == other.name.second && name.first == other.name.first) return true;
    return false;
}

bool Student::operator!=(const Student &other) const
{
    if (name.second == other.name.second && name.first == other.name.first) return false;
    return true;
}

bool Student::operator<(const Student &other) const
{
    std::string l = name.second + name.first,
                r = other.name.second + other.name.first;
    if (l < r) return true;
    return false;
}

bool Student::operator>(const Student &other) const
{
    std::string l = name.second + name.first,
                r = other.name.second + other.name.first;
    if (l > r) return true;
    return false;
}

bool Student::operator<=(const Student &other) const
{
    std::string l = name.second + name.first,
                r = other.name.second + other.name.first;
    if (l <= r) return true;
    return false;
}

bool Student::operator>=(const Student &other) const
{
    std::string l = name.second + name.first,
                r = other.name.second + other.name.first;
    if (l >= r) return true;
    return false;
}


double Student::getGPA() const
{
    double retval = 0.0;
    int hours = 0;
    for (const auto& i : courses)
    {
        double curr = 0;
        hours += i.credits;
        switch (i.grade)
        {
        case 'A': curr = 4.0 * i.credits; break;
        case 'B': curr = 3.0 * i.credits; break;
        case 'C': curr = 2.0 * i.credits; break;
        case 'D': curr = 1.0 * i.credits; break;
        default: break;
        };
        retval += curr;
    }
    return retval/hours;
}