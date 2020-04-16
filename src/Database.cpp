#include "include/Database.h"
#include <algorithm>

using std::cout;
using std::endl;
using std::cin;

class StudentNotFound : public std::exception
{
private:
    std::string msg;
public:
    StudentNotFound() : msg("Student not found!") {}
    const char * what() const noexcept { return msg.c_str(); }
};

void Database::readFromFile(const std::string& fname)
{
    std::ifstream file(fname.c_str());
    readFromFile(file);
}

void Database::readFromFile(std::istream& file)
{
    std::string buffer;
    Student prev;
    while (getline(file,buffer))
    {
        Student s;
        course c;
        std::stringstream in;
        in << buffer;
        if (buffer.substr(0,5) == "First" || buffer.at(0) == '#') continue;

        int numfields=0;
        while (!in.eof()) {
            std::string temp;
            in >> temp;
            numfields++;
        }
        if (numfields == 6) 
        {
            std::stringstream in1(buffer);
            in1 >> s >> c;
            prev = s;
        }
        else in >> c;
        s.insertCourse(c);
        insert(s);
    }
}

void Database::insert(const std::string &fname, const std::string &lname, int ID)
{
    Student s (fname,lname,ID);
    insert(s);
}

void Database::insert(const Student& student)
{
    try
    {
        students.ordered_insert(student);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Database::sortByID()
{
}

void Database::sortByGPA()
{

}

void Database::sortByName()
{

}

/*LL<Student>::iterator Database::findStudent(int ID)
{
    Student temp("","",ID);
    LL<Student>::iterator i = students.begin();
    while (i != students.end())
    {
        if (Student::idEQ(temp,*i))
            return i;
        i++;
    }
    throw StudentNotFound();
}

LL<Student>::iterator Database::findStudent(const Student& s)
{
    LL<Student>::iterator i = students.begin();
    while(i != students.end())
    {
        if (Student::lastNameEQ(s,*i) && Student::firstNameEQ(s,*i))
            return i;
        i++;
    }
    throw StudentNotFound();
}*/

void Database::honorStudents()
{
    std::cout << "Honors students: " << std::endl;
    for (const auto& a : students)
    {
        if (a.getGPA() >= gpaHonor)
            std::cout << a << std::endl;
    }
}

void Database::warningStudents()
{
    std::cout << "Warning students: " << std::endl;
    for (const auto& a : students)
    {
       if (a.getGPA() <= gpaWarn)
            std::cout << a << std::endl;
    }
}

void Database::failingStudents()
{
    std::cout << "Failing students: " << std::endl;
    for (const auto& a : students)
    {
        if (a.getGPA() < gpaFail)
            std::cout << a << std::endl;
    }
}

Student& Database::findStudent(const Student& s)
{
    return binsearch(s,0,students.size()-1);
}
