#include "include/Student.h"
#include <exception>

// copy assignment and constructor
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

// move assignment and constructor
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

Student::Student(std::string fname, std::string lname, int _ID)
{
    std::for_each(fname.begin(), fname.end(),[](char& c) { c = std::toupper(c); });
    std::for_each(lname.begin(), lname.end(),[](char& c) { c = std::toupper(c); });
    name = std::make_pair(fname,lname);
    ID = _ID;
}

Student::Student(std::pair<std::string,std::string> _name, int _ID)
{
    std::string fname = _name.first, lname = _name.second;
    std::for_each(fname.begin(),fname.end(),[](char& c){ c = std::toupper(c); });
    std::for_each(lname.begin(),lname.end(),[](char& c){ c = std::toupper(c); });
    name = std::make_pair(fname,lname);
    ID = _ID;
}

// various ways to insert a course
void Student::insertCourse(const std::string& name, int credits, char grade)
{
    insertCourse(course(name, credits, std::toupper(grade)));
}

void Student::insertCourse(const course& _course)
{
    for (int i = 0; i < courses.size(); ++i)
    {
        if (_course.name == courses[i].name)
        {
            throw ClassPresent();
        }
    }
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

// allow access to a particular element
// does this via reference so that modification
// can be done
course& Student::operator[](int idx)
{
    return courses[idx];
}

// allow access to a particular element
// does this via constand refence so that
// no information can be modified
const course& Student::at(int idx)
{
    return courses.at(idx);
}

// returns the name in various different manners
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

// comparison operators to determine how 2 students
// compare in all aspects.
// equivalence and non-equivalence are the only 2
// operators that make sense
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

// sort by gpa
int Student::gpaCmp(const Student &l, const Student &r)
{

    int gpaL = l.getGPA(), gpaR = r.getGPA();
    if (gpaL == gpaR) return 0;
    if (gpaL > gpaR) return -1;//1;
    return 1;//-1;
}

// sort by first name, last name
int Student::fnCmp(const Student &l, const Student &r)
{
    std::string _l = l.name.first + l.name.second + std::to_string(l.ID),
                _r = r.name.first + r.name.second + std::to_string(r.ID);
    if (_l == _r) return 0;
    if (_l > _r) return 1;
    return -1;
}

// sort student by id
int Student::idCmp(const Student &l, const Student &r)
{
    if (l.ID == r.ID) return 0;
    if (l.ID > r.ID) return 1;
    return -1;
}

// sort student by last name, throw exception if student
// is already present
int Student::lniCmp(const Student &l, const Student &r)
{
    std::string _l = l.name.second + l.name.first + std::to_string(l.ID),
                _r = r.name.second + r.name.first + std::to_string(r.ID);
    if (_l == _r) throw StudentPresent();
    if (_l > _r) return 1;
    return -1;
}

// sort student by last name, don't throw exception if
// student is already present
int Student::lniCheckCmp(const Student &l, const Student &r)
{
    std::string _l = l.name.second + l.name.first + std::to_string(l.ID),
                _r = r.name.second + r.name.first + std::to_string(r.ID);
    if (_l == _r) return 0;
    if (_l > _r) return 1;
    return -1;
}

// sort by last name without ID
int Student::lnCmp(const Student &l, const Student &r)
{
    std::string _l = l.name.second + l.name.first,
                _r = r.name.second + r.name.first ;
    if (_l == _r) return 0;
    if (_l > _r) return 1;
    return -1;
}

// calculates and returns gpa
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

// removes a course, allows for user input
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

// modify a course, allows for user input
void Student::modifyCourse()
{
    std::cout << "Which course do you want to modify the grade for?" << std::endl;
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
