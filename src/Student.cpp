#include "include/Student.h"
#include <exception>

Student& Student::operator=(const Student& other)
{
    name = other.name;
    ID = other.ID;
    courses = other.courses;
    return *this;
}

Student::Student(const Student& other)
{
    name = other.name;
    ID = other.ID;
    courses = other.courses;
}

Student::Student(Student&& other)
{
    name = std::move(other.name);
    ID = other.ID;
    other.ID = -1;
    courses = std::move(other.courses);
}

Student& Student::operator=(Student&& other)
{
    name = std::move(other.name);
    ID = other.ID;
    other.ID = -1;
    courses = std::move(other.courses);
    return *this;
}

void Student::insertCourse(const std::string& name, int credits, char grade)
{
    insertCourse(course(name, credits, std::toupper(grade)));
}

void Student::insertCourse(const course& _course)
{
    try
    {
        courses.ordered_insert(_course,course::courseCmpI);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Student::insertCourse(const Student &s)
{
    for (int i = 0; i < courses.size(); ++i)
    {
        try
        {
            courses.ordered_insert(s.courses.at(i),course::courseCmpI);
        }
        catch (const OutOfBounds& e) { /* stack unwinding can cause this to happen, treat as no-op */ }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
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
    if (name.second == other.name.second &&
        name.first == other.name.first &&
        ID == other.ID) return true;
    return false;
}

bool Student::operator!=(const Student &other) const
{
    if (name.second == other.name.second &&
        name.first == other.name.first &&
        ID == other.ID) return false;
    return true;
}

// static comparison function
int Student::gpaCmp(const Student &l, const Student &r)
{

    int gpaL = l.getGPA(), gpaR = r.getGPA();
    if (gpaL == gpaR) return 0;
    if (gpaL > gpaR) return 1;
    return -1;
}

int Student::fnCmp(const Student &l, const Student &r)
{
    std::string _l = l.name.first + l.name.second,
                _r = r.name.first + r.name.second;
    if (_l == _r) return 0;
    if (_l > _r) return 1;
    return -1;
}

int Student::idCmp(const Student &l, const Student &r)
{
    if (l.ID == r.ID) return 0;
    if (l.ID > r.ID) return 1;
    return -1;
}

int Student::lniCmp(const Student &l, const Student &r)
{
    std::string _l = l.name.second + l.name.first + std::to_string(l.ID),
                _r = r.name.second + r.name.first + std::to_string(l.ID);
    if (_l == _r) throw StudentPresent();
    if (_l > _r) return 1;
    return -1;
}

int Student::lniCheckCmp(const Student &l, const Student &r)
{
    std::string _l = l.name.second + l.name.first + std::to_string(l.ID),
                _r = r.name.second + r.name.first + std::to_string(l.ID);
    if (_l == _r) return 0;
    if (_l > _r) return 1;
    return -1;
}

int Student::lnCmp(const Student &l, const Student &r)
{
    std::string _l = l.name.second + l.name.first,
                _r = r.name.second + r.name.first ;
    if (_l == _r) return 0;
    if (_l > _r) return 1;
    return -1;
}

double Student::getGPA() const
{
    double retval = 0.0;
    int hours = 0;
    //for (const auto& i : courses)
    for (int i = 0; i < courses.size(); ++i)
    {
        double curr = 0;
        hours += courses.at(i).credits;
        switch (courses.at(i).grade)
        {
        case 'A': curr = 4.0 * courses.at(i).credits; break;
        case 'B': curr = 3.0 * courses.at(i).credits; break;
        case 'C': curr = 2.0 * courses.at(i).credits; break;
        case 'D': curr = 1.0 * courses.at(i).credits; break;
        default: break;
        };
        retval += curr;
    }
    return retval/hours;
}

void Student::removeCourse()
{
    for (int i = 0; i < courses.size(); ++i)
    {
        std::cout << i+1 << ". " << courses.at(i) << std::endl;
    }
    int posn;
    while (true)
    {
        std::cout << "Selections: ";
        std::cin >> posn;
        if (posn < 1 || posn > courses.size())
        {
            std::cout << "Invalid selection! ";
        }
        else
        {
            posn--;
            break;
        }
    }
    courses.remove(posn);
}

void Student::modifyCourse()
{
    for (int i = 0; i < courses.size(); ++i)
    {
        std::cout << i+1 << ". " << courses.at(i) << std::endl;
    }
    int posn;
    while (true)
    {
        std::cout << "Selections: ";
        std::cin >> posn;
        if (posn < 1 || posn > courses.size())
        {
            std::cout << "Invalid selection! ";
        }
        else
        {
            posn--;
            break;
        }
    }
    char grade = '\0';
    while (true)
    {
        std::cout << "Enter new grade: ";
        std::cin >> grade;
        grade = toupper(grade);
        if (grade == 'A' || grade == 'B' || grade == 'C' || grade == 'D' || grade == 'F') break;
        std::cout << "Invalid grade! ";
    }
    courses[posn].grade = grade;
}
