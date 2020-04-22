#ifndef DATABASE_H
#define DATABASE_H

#include "Student.h"
#include <list>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include "LL.hpp"
#include <exception>
const double gpaHonor = 3.6;
const double gpaWarn = 2.5;
const double gpaFail = 2.0;

class NotPresent : public std::exception
{
private:
    std::string msg;
public:
    NotPresent() : msg("Student not present!") {}
    const char * what() const noexcept { return msg.c_str(); }
};

class Database
{
private:
    LL<Student> students;

    template <typename compare>
    Student& binsearch(const Student&, int, int, compare);

    template <typename compare>
    int binsearchpos(const Student&, int, int, compare);

    template <typename compare>
    Student& binsearch(const Student&, LL<Student>&, int, int, compare);

    void insertFromFile(const Student& s);

public:
    void clearList() { students.clear(); }
    void readFromFile(const std::string&);
    void readFromFile(std::istream&);
    void writeToFile(const std::string& fname = "SeniorClass");
    void writeToFile(std::ostream&);
    
    bool insert(const std::string&,const std::string&,int);
    bool insert(const Student&);
    void remove(const std::string&,const std::string&);
    void remove(int);
    void remove(const Student&);

    void insertCourse(const std::string&,const std::string&,const course&);
    void insertCourse(const Student&, const course&);
    void insertCourse(int,const course&);
    void removeCourse(const std::string&,const std::string&,const course&);
    void removeCourse(const Student&, const course&);
    void removeCourse(int,const course&);
    Student& findStudent(const Student&);
    Student& findStudent(int);
    inline int numStudents() { return students.size(); }
    void sortByName();
    void sortByGPA();
    void sortByID();
    bool areMultipleStudents(const Student& s);
    void warningStudents();
    void failingStudents();
    void honorStudents();
    //LL<Student>::iterator findStudent(int);
    //LL<Student>::iterator findStudent(const Student&);

    //inline int numStudents() { return students.size(); }

    friend std::ostream& operator<<(std::ostream& out, const Database& other)
    {
        out << std::setw(15) << std::left << "First Name"
            << std::setw(15) << std::left << "Last Name"
            << std::setw(10) << std::left << "ID"
            << std::setw(10) << std::left << "Course"
            << std::setw(10) << std::left << "Credits"
            << std::setw(5) << std::left << "Grade"
            << std::endl << other.students;
        return out;
    }
};

#endif // DATABASE_H
