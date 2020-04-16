#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <iomanip>
//#include "Functions.h"

struct course
{
    std::string name;
    int credits;
    char grade;
    course(std::string _name = "", int _credits = 0, char _grade = '\0')
        :   name(_name), credits(_credits), grade(std::toupper(_grade)) {}
    friend std::ostream& operator<<(std::ostream& out, const course& other)
    {
        out << std::setw(10) << std::left << other.name
            << std::setw(10) << std::left << other.credits
            << std::setw(5) << std::left << other.grade;
        return out;
    }
    friend std::istream& operator>>(std::istream& in, course& other)
    {
        in >> other.name >> other.credits >> other.grade;
        return in;
    }
};

class Student
{
private:
    std::pair<std::string, std::string> name;
    int ID;
    std::vector<course> courses;

public:
    // can rely on default since i am implementing no dynamic memory
    Student()                          = default;
    ~Student()                         = default;
    Student(const Student&)            = default;
    Student(Student&&)                 = default;
    Student& operator=(const Student&) = default;
    Student& operator=(Student&&)      = default;

    Student(std::string fname, std::string lname, int _ID)
        : name(fname,lname), ID(_ID) {}
    Student(std::pair<std::string, std::string> _name, int _ID)
        : name(_name), ID(_ID) {}

    void insertCourse(const std::string&, int, char);
    void insertCourse(const course&);
    double getGPA() const;
    std::string getFirstName();
    std::string getLastName();
    std::pair<std::string,std::string> getName();

    std::vector<course>::iterator begin();
    std::vector<course>::iterator end();
    std::vector<course>::reverse_iterator rbegin();
    std::vector<course>::reverse_iterator rend();
    course& operator[](int);
    const course& at(int);


    // name sort is default
    bool operator>(const Student&) const;
    bool operator<(const Student&) const;
    bool operator>=(const Student&) const;
    bool operator<=(const Student&) const;
    bool operator==(const Student&) const;
    bool operator!=(const Student&) const;

    friend std::ostream& operator<<(std::ostream& out, const Student& other)
    {
        out << std::setw(15) << std::left << other.name.first
            << std::setw(15) << std::left << other.name.second
            << std::setw(10) << std::left << other.ID;
        if (other.courses.size())
        {
            bool first = true;
            for (const auto& i : other.courses)
            {
                if (!first)
                    out << std::setw(40);
                else first = false;
                out << i;
            }
        }
        return out;
    }

    friend std::istream& operator>>(std::iostream& in, Student& other)
    {
        std::string fname, lname;
        other.courses.clear();
        in >> fname >> lname >> other.ID;
        other.name = std::make_pair(fname,lname);
        return in;
    }
};

#endif // STUDENT_H
