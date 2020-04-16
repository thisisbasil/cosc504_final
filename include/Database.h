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
    const char * what() const noexcept { throw msg.c_str(); }
};

class Database
{
private:
    LL<Student> students;

    Student& binsearch(const Student& s, int l, int r)
    {
        if (l > r) throw NotPresent();
        int mid = (l+r)/2;
        Student& temp = students[mid];
        if (s == temp) return temp;
        if (s > temp) return binsearch(s,mid+1,r);
        else return binsearch(s,l,mid-1);
    }

public:

    void readFromFile(const std::string&);
    void readFromFile(std::istream&);
    
    void insert(const std::string&,const std::string&,int);
    void insert(const Student&);
    void remove(const std::string&,const std::string&);
    void remove(int);

    void insertCourse(const std::string&,const std::string&,const course&);
    void insertCourse(const Student&, const course&);
    void insertCourse(int,const course&);
    void removeCourse(const std::string&,const std::string&,const course&);
    void removeCourse(const Student&, const course&);
    void removeCourse(int,const course&);
    Student& findStudent(const Student&);

    void sortByName();
    void sortByGPA();
    void sortByID();

    void warningStudents();
    void failingStudents();
    void honorStudents();
    //LL<Student>::iterator findStudent(int);
    //LL<Student>::iterator findStudent(const Student&);

    //inline int numStudents() { return students.size(); }

    friend std::ostream& operator<<(std::ostream& out, const Database& other)
    {
        std::cout << std::setw(18) << std::left << "   First Name"
                  << std::setw(15) << std::left << "Last Name"
                  << std::setw(10) << std::left << "ID"
                  << std::setw(10) << std::left << "Course"
                  << std::setw(10) << std::left << "Credits"
                  << std::setw(5) << std::left << "Grade" << std::endl << other.students;
        return out;
    }
};

#endif // DATABASE_H
