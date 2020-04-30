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

// constants related to gpa
const double gpaHonor = 3.6;
const double gpaWarn = 2.5;
const double gpaFail = 2.0;

// exceptions for when students not present
class NotPresent : public std::exception
{
private:
    std::string msg;
public:
    NotPresent() : msg("Student not present!") {}
    const char * what() const noexcept { return msg.c_str(); }
};

class StudentNotFound : public std::exception
{
private:
    std::string msg;
public:
    StudentNotFound() : msg("Student not found!") {}
    const char * what() const noexcept { return msg.c_str(); }
};

// database class
class Database
{
private:
    // linked list containing the students
    LL<Student> students;

    // various binary searches which return a ref to a student, or it's position
    template <typename compare>
    Student& binsearch(const Student&, int, int, compare);

    template <typename compare>
    int binsearchpos(const Student&, int, int, compare);

    template <typename compare>
    Student& binsearch(const Student&, LL<Student>&, int, int, compare);

    // allows for insertion from file as opposed to manual
    void insertFromFile(const Student& s);

public:
    // destroys the list of students
    void clearList() { students.clear(); }

    // read from and write to file
    void readFromFile(const std::string&);
    void readFromFile(std::istream&);
    void writeToFile(const std::string& fname = "SeniorClass");
    void writeToFile(std::ostream&);
    
    // insert and remove students to the list in various methods
    bool insert(const std::string&,const std::string&,int);
    bool insert(const Student&);
    void remove(const std::string&,const std::string&);
    void remove(int);
    void remove(const Student&);

    // insert or remove a course for a student in various differ methods
    void insertCourse(const std::string&,const std::string&,const course&);
    void insertCourse(const Student&, const course&);
    void insertCourse(int,const course&);
    void removeCourse(const std::string&,const std::string&,const course&);
    void removeCourse(const Student&, const course&);
    void removeCourse(int,const course&);

    // find a student, return a reference to it
    Student& findStudent(const Student&);
    Student& findStudent(int);
    inline int numStudents() { return students.size(); }

    // sort and print the linked list in various differt methods
    void sortByName();
    void sortByGPA();
    void sortByID();

    // determine if there are multiple instances of a student's name
    bool areMultipleStudents(const Student& s);

    // print the student status
    void warningStudents();
    void failingStudents();
    void honorStudents();

    // overloaded stream operator, allow the class to output itself
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
