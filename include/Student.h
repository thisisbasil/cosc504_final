#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "LL.hpp"


// various exceptions
class CourseNotFound : public std::exception
{
private:
    std::string msg;
public:
    CourseNotFound() : msg("Course not found!") {};
    const char * what() const noexcept { return msg.c_str(); }
};

class StudentPresent : public std::exception
{
private:
    std::string msg;
public:
    StudentPresent() : msg("Student already present!") {}
    const char * what() const noexcept { return msg.c_str(); }
};

class ClassPresent : public std::exception
{
private:
    std::string msg;
public:
    ClassPresent() : msg("Student already registered in this clas!") {}
    const char * what() const noexcept { return msg.c_str(); }
};

// struct representing individual courses
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
    bool operator==(const course& other)
    {
        if (name == other.name) return true;
        return false;
    }
    bool operator!=(const course& other)
    {
        if (name == other.name) return false;
        return true;
    }

    // allow for the course to print itself
    friend std::istream& operator>>(std::istream& in, course& other)
    {
        in >> other.name >> other.credits >> other.grade;
        std::for_each(other.name.begin(),other.name.end(),[](char& c){ c = std::toupper(c); });
        return in;
    }

    // static comparison methods, for ordered insertio
    static int courseCmp(const course& l, const course& r)
    {
        if (l.name < r.name) return -1;
        if (l.name == r.name) return 0;
        return 1;
    }

    static int courseCmpI(const course& l, const course& r)
    {
        if (l.name < r.name) return -1;
        if (l.name == r.name) throw ClassPresent();
        return 1;
    }
};

// class representing students
class Student
{
private:
    std::pair<std::string, std::string> name;
    int ID;

    // linked list containing the courses
    LL<course> courses;

public:
    // can rely on default since i am implementing no dynamic memory
    Student()                          = default;
    ~Student()                         = default;
    Student(const Student&);
    Student(Student&&);
    Student& operator=(const Student&);
    Student& operator=(Student&&);

    Student(std::string fname, std::string lname, int _ID = 0)
        : name(fname,lname), ID(_ID) {}
    Student(std::pair<std::string, std::string> _name, int _ID = 0)
        : name(_name), ID(_ID) {}

    // various methods to insert a course
    void insertCourse(const std::string&, int, char);
    void insertCourse(const Student& s);
    void insertCourse(const course&);

    // remove and modify course grade
    void removeCourse();
    void modifyCourse();

    // calculate and return gpa
    double getGPA() const;

    // return id
    inline int getID() const { return ID; }
    inline int numCourses() { return courses.size(); }
    std::string getFirstName();
    std::string getLastName();
    std::pair<std::string,std::string> getName();

    // allows to iterate though the list
    course& operator[](int);
    const course& at(int);

    bool operator==(const Student&) const;
    bool operator!=(const Student&) const;
    void clearCourseList() { courses.clear(); }

    // comparison functions:
    // these allow for insertion into the linked list
    // in various different manners. this makes it
    // it easier to just create one linked list class
    // and order it by whichever way the function allows
    static int gpaCmp(const Student&, const Student&);
    static int fnCmp(const Student&, const Student&);
    static int lniCmp(const Student&, const Student&);
    static int lniCheckCmp(const Student&, const Student&);
    static int idCmp(const Student&, const Student&);
    static int lnCmp(const Student&, const Student&);


    // allow a student to print itself out
    friend std::ostream& operator<<(std::ostream& out, const Student& other)
    {
        out << std::setw(15) << std::left << other.name.first
            << std::setw(15) << std::left << other.name.second
            << std::setw(10) << std::left << other.ID;
        for (auto i = 0; i < other.courses.size(); ++i)
        {
            if (i) out << std::setw(40) << std::right << ' ';
            out << other.courses.at(i);
            if (i < other.courses.size()-1) out << std::endl;
        }
        return out;
    }

    // allow a student to input
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
