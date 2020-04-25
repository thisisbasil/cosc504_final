#include "include/Functions.h"
#include <string>

enum StudentInputType { ALL, NAME, ID };
bool isValid(const std::string& s)
{
    for(const auto& c : s)
    {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

Student inputStudent(StudentInputType type = ALL)
{
    std::string fname, lname;
    int id = -1;
    if (type == ALL || type == NAME)
    {
        std::cout << "Student First Name: ";
        std::cin >> fname;
        std::cout << "Student Last Name: ";
        std::cin >> lname;
    }
    if (type == ALL || type == ID)
    {
        std::cout << "Student id: ";
        std::cin >> id;
    }
    return Student(std::move(trim(fname)),std::move(trim(lname)),id);
}

course inputCourse()
{
    std::string name;
    int credits;
    char grade;
    std::cout << "Enter course name: ";
    std::cin >> name;
    std::for_each(name.begin(),name.end(),[](char& c){ c = std::toupper(c); });
    while (true)
    {
        std::cout << "Number of credit hours: ";
        std::cin >> credits;
        if (credits >= 0) break;
        std::cout << "Hours must be >0! ";
    }
    while (true)
    {
        std::cout << "Letter grade: ";
        std::cin >> grade;
        grade = std::toupper(grade);
        if (grade == 'A' || grade == 'B' || grade == 'C' || grade == 'D' || grade == 'F') break;
        std::cout << "Invalid grade, must be A,B,C,D,F. ";
    }
    return course(std::move(trim(name)), credits, std::toupper(grade));
}

void AddStudent(Database& d, bool fromFile)
{
    Student tempS = inputStudent();
    try
    {
        d.findStudent(tempS.getID());
        std::cout << "Student already present!" << std::endl;
        return;
    }
    catch (const std::exception& s) {}
    int num = 0;
    while (true)
    {
        std::cout << "number of courses enrolled in: ";
        std::cin >> num;
        if (num > 0) break;
        std::cout << "Invalid entry! ";
    }
    while (num > 0)
    {
        tempS.insertCourse(inputCourse());
        --num;
    }
    d.insert(tempS);
    std::cout << d << std::endl
              << "Total number of students: " << d.numStudents() << std::endl;
}

void AddCourse(Database& d)
{
    if (d.numStudents() == 0)
    {
        std::cout << "No students currently enrolled!" << std::endl;
        return;
    }
    int type, pos;
    Student s;
    while(true)
    {
        std::cout << "Find student by (1) name or by (2) id? ";
        std::cin >> type;
        if (type == NAME || type == ID) break;
        std::cout << "Invalid input!\n";
    }
    if (type == NAME)
    {
        s = inputStudent(NAME);
        if (d.areMultipleStudents(s))
        {
            std::cout << "Multiple students with that name! ";
            type = ID;
        }
        else
        {
            try
            {
                d.findStudent(s);
            }
            catch(const std::exception& e)
            {
                std::cout << e.what() << std::endl;
                return;
            }
            course c = inputCourse();
            d.findStudent(s).insertCourse(c);
        }
    }
    if (type == ID)
    {
        int id;
        std::cout << "Enter ID: ";
        std::cin >> id;
        try
        {
            d.findStudent(id);
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return;
        }
        course c = inputCourse();
        d.findStudent(id).insertCourse(c);
    }
}

void AddCourse(Database& d, const Student& s)
{

    course c = inputCourse();
}

void RemoveStudent(Database& d)
{
    if (d.numStudents() == 0)
    {
        std::cout << "No students currently enrolled!" << std::endl;
        return;
    }
    bool loop = true;
    int type;
    while (loop)
    {
        std::cout << "Delete by (1) name or by (2) id? ";
        std::cin >> type;
        if (type != NAME && type != ID)
            std::cout << "Invalid input!\n";
        else
            loop = false;
    }

    if (type == NAME)
    {
        Student s = inputStudent(NAME);
        if (d.areMultipleStudents(s))
        {
            std::cout << "Multiple students with that name! ";
            type = ID;
        }
        else
        {
            try
            {
               d.remove(s);
               std::cout << d << std::endl;
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << std::endl;
            }
        }
    }
    if (type == ID)
    {
        int id;
        std::cout << "Enter ID: ";
        std::cin >> id;
        try
        {
            d.remove(id);
            std::cout << d << std::endl << "Number of students "
                      << d.numStudents() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}

void FindGPA(Database& d)
{
    try
    {
        double gpa = d.findStudent(inputStudent()).getGPA();
        std::cout << "GPA is: " << gpa/1.0 << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "FindGPPA(Database&): " << e.what() << std::endl;
    }
}

void Sort(Database& d)
{
    int option;
    bool loop = true;
    while (loop)
    {
        std::cout << "Sort by (1) First Name, (2) ID, or (3) GPA? ";
        std::cin >> option;
        if (option < 1 || option > 3)
            std::cout << "Invalid option!" << std::endl;
        else
            loop = false;
    }
    switch (option)
    {
    case 1: d.sortByName(); break;
    case 2: d.sortByID(); break;
    case 3: d.sortByGPA(); break;
    };
}

void FindStudent(Database& d)
{
    std::cout << "Enter student id: ";
    int id;
    std::cin >> id;
    try
    {
        Student temp = d.findStudent(id);
        std::cout << temp << std::endl << "GPA: " << temp.getGPA() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "FindStudent(Database&): " << e.what() << std::endl;
    }
}

void PrintHonors(Database& d)
{
    d.honorStudents();
}

void PrintWarn(Database& d)
{
    d.warningStudents();
}

void PrintFail(Database& d)
{
    d.failingStudents();
}

void RemoveCourse(Database& d)
{
    if (d.numStudents() == 0)
    {
        std::cout << "No students currently enrolled!" << std::endl;
        return;
    }
    bool loop = true;
    int type;
    Student s;
    while (loop)
    {
        std::cout << "Find student by (1) name or by (2) id? ";
        std::cin >> type;
        if (type != NAME && type != ID)
            std::cout << "Invalid input!\n";
        else
            loop = false;
    }
    int pos;
    if (type == NAME)
    {
        s = inputStudent(NAME);
        if (d.areMultipleStudents(s))
        {
            std::cout << "Multiple students with that name! ";
            type = ID;
        }
        else
        {
            d.findStudent(s).removeCourse();
            if (d.findStudent(s).numCourses() == 0)
            {
                std::cout << "Student is no longer enrolled in courses," <<
                             " removing from the database" << std::endl;
                d.remove(s);
            }
        }
    }
    if (type == ID)
    {
        int id;
        std::cout << "Enter ID: ";
        std::cin >> id;
        d.findStudent(id).removeCourse();
        if (d.findStudent(id).numCourses() == 0)
        {
            std::cout << "Student is no longer enrolled in courses," <<
                         " removing from the database" << std::endl;
            d.remove(id);
        }
    }
}

void ModifyGrade(Database& d)
{
    if (d.numStudents() == 0)
    {
        std::cout << "No students currently enrolled!" << std::endl;
        return;
    }
    bool loop = true;
    int type;
    Student s;
    while (loop)
    {
        std::cout << "Find student by (1) name or by (2) id? ";
        std::cin >> type;
        if (type != NAME && type != ID)
            std::cout << "Invalid input!\n";
        else
            loop = false;
    }
    int pos;
    if (type == NAME)
    {
        s = inputStudent(NAME);
        if (d.areMultipleStudents(s))
        {
            std::cout << "Multiple students with that name! ";
            type = ID;
        }
        else
        {
            d.findStudent(s).modifyCourse();
        }
    }
    if (type == ID)
    {
        int id;
        std::cout << "Enter ID: ";
        std::cin >> id;
        d.findStudent(id).modifyCourse();
    }
}


void CreateList(Database& d)
{
    std::string fname;
    std::cout << "Enter name of output file: ";
    std::cin >> fname;
    d.writeToFile(fname);
}
