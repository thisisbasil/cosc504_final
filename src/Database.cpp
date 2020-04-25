#include "include/Database.h"
#include <algorithm>

using std::cout;
using std::endl;
using std::cin;

void Database::insertFromFile(const Student &s)
{
    try
    {
        students.ordered_insert(s,Student::lniCmp);
    }
    catch (const std::exception& e)
    {
        try
        {
            binsearch(s,0,students.size()-1,Student::lniCheckCmp).insertCourse(s);
        }
        catch (const std::exception& e)
        {
            std::cout << "insertFromFile(const Student&): " << e.what() << std::endl;
        }
    }
}

void Database::remove(const std::string &fn, const std::string &ln)
{
    remove(Student(fn,ln));
}

void Database::remove(int id)
{
    for (int i = 0; i < students.size(); ++i)
    {
        if (students.at(i).getID() == id)
        {
            students.remove(i);
            return;
        }
    }
    throw StudentPresent();
}

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
        if (!buffer.size() ||buffer.substr(0,5) == "First" || buffer.at(0) == '#') continue;

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
        else
        {
            s = prev;
            std::stringstream in1(buffer);
            in1 >> c;
        }
        s.insertCourse(c);
        insertFromFile(s);
        prev = s;
        prev.clearCourseList();
    }
}

void Database::writeToFile(const std::string &fname)
{
    std::ofstream file(fname.c_str());
    writeToFile(file);
}

void Database::writeToFile(std::ostream& file)
{
    file << *this << endl;
}

bool Database::insert(const std::string &fname, const std::string &lname, int ID)
{
    Student s (fname,lname,ID);
    return insert(s);
}

bool Database::insert(const Student& student)
{
    try
    {
        students.ordered_insert(student,Student::lniCmp);
        return true;
    }
    catch (const std::exception& e)
    {
        std::cout << "insert(const Student&): " <<  e.what() << std::endl;
        return false;
    }
}

void Database::sortByID()
{
    LL<Student> other;
    for (int i = 0; i < students.size(); ++i)
    {
        other.ordered_insert(students[i],Student::idCmp);
    }
    std::cout << std::setw(15) << std::left << "First Name"
              << std::setw(15) << std::left << "Last Name"
              << std::setw(10) << std::left << "ID"
              << std::setw(10) << std::left << "Course"
              << std::setw(10) << std::left << "Credits"
              << std::setw(5) << std::left << "Grade"
              << std::endl << other << std::endl;
}

void Database::sortByGPA()
{
    LL<Student> other;
    for (int i = 0; i < students.size(); ++i)
    {
        Student temp = students[i];
        other.ordered_insert(temp,Student::gpaCmp);
    }
    std::cout << std::setw(15) << std::left << "First Name"
              << std::setw(15) << std::left << "Last Name"
              << std::setw(10) << std::left << "ID"
              << std::setw(10) << std::left << "Course"
              << std::setw(10) << std::left << "Credits"
              << std::setw(5) << std::left << "Grade"
              << std::endl << other << std::endl;
}

void Database::sortByName()
{
    LL<Student> other;
    for (int i = 0; i < students.size(); ++i)
    {
        other.ordered_insert(students[i],Student::fnCmp);
    }
    std::cout << std::setw(15) << std::left << "First Name"
              << std::setw(15) << std::left << "Last Name"
              << std::setw(10) << std::left << "ID"
              << std::setw(10) << std::left << "Course"
              << std::setw(10) << std::left << "Credits"
              << std::setw(5) << std::left << "Grade"
              << std::endl << other << std::endl;
}

void Database::honorStudents()
{
    std::cout << "Honors students: " << std::endl;
    int count = 0;
    for (int i = 0; i < students.size(); ++i)
    {
        Student s = students[i];
        auto gpa = s.getGPA();
        if (students[i].getGPA() >= gpaHonor)
        {
            ++count;
            std::cout << std::setw(15) << students[i].getFirstName()
                      << std::setw(15) << students[i].getLastName() << std::endl;
        }
    }
    std::cout << "Total number of honors students: " << count << std::endl;
}

bool Database::areMultipleStudents(const Student &s)
{
    int l = students.find_last_instance(s,Student::lnCmp),
        r = students.find_first_instance(s,Student::lnCmp);
    if (students.find_last_instance(s,Student::lnCmp) ==
        students.find_first_instance(s,Student::lnCmp)) return false;
    return true;
}

void Database::warningStudents()
{
    std::cout << "Warning students: " << std::endl;
    int count = 0;
    for (int i = 0; i < students.size(); ++i)
    {
        if (students[i].getGPA() <= gpaWarn)
        {
            ++count;
            std::cout << std::setw(15) << students[i].getFirstName()
                      << std::setw(15) << students[i].getLastName() << std::endl;
        }
    }
    std::cout << "Total number of students on warning: " << count << std::endl;
}

void Database::failingStudents()
{
    std::cout << "Failing students: " << std::endl;
    int count = 0;
    for (int i = 0; i < students.size(); ++i)
    {
        if (students[i].getGPA() < gpaFail)
        {
            ++count;
            std::cout << std::setw(15) << students[i].getFirstName()
                      << std::setw(15) << students[i].getLastName() << std::endl;
        }
    }
    std::cout << "Total number of failing students: " << count << std::endl;
}

template <typename compare>
Student& Database::binsearch(const Student& s, int l, int r, compare cmp)
{
    if (l > r) throw NotPresent();
    int mid = (l+r)/2;
    Student& temp = students[mid];
    int res = cmp(s,temp);
    if (res == 0) return temp;
    if (res == 1) return binsearch(s,mid+1,r,cmp);
    else return binsearch(s,l,mid-1,cmp);
}

template <typename compare>
Student& Database::binsearch(const Student& s, LL<Student>& ll, int l, int r, compare cmp)
{
    if (l > r) throw NotPresent();
    int mid = (l+r)/2;
    Student& temp = ll[mid];
    int res = cmp(s,temp);
    if (res == 0) return temp;
    if (res == 1) return binsearch(s,ll,mid+1,r,cmp);
    else return binsearch(s,ll,l,mid-1,cmp);
}

template <typename compare>
int Database::binsearchpos(const Student &s, int l, int r, compare cmp)
{
    if (l > r) throw NotPresent();
    int mid = (l+r)/2;
    Student& temp = students[mid];
    if (cmp(s,temp) == 0) return mid;
    if (cmp(s,temp) == 1) return binsearchpos(s,mid+1,r,cmp);
    else return binsearchpos(s,l,mid-1,cmp);
}

Student& Database::findStudent(const Student& s)
{
    auto cmp = (s.getID() <= 0) ? Student::lnCmp : Student::lniCmp;
    return binsearch(s,0,students.size()-1,cmp);
}

Student& Database::findStudent(int _id)
{
   for (int i = 0; i < students.size(); ++i)
   {
       if (students[i].getID() == _id) return students[i];
   }
   throw StudentNotFound();
}

void Database::remove(const Student& s)
{
    auto cmp = (s.getID() <= 0) ? Student::lnCmp : Student::lniCmp;
    int pos = binsearchpos(s,0,students.size()-1,cmp);
    Student temp = students.at(pos);
    students.remove(pos);
}
