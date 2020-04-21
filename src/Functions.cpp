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
    std::cout << "Number of credit hours: ";
    std::cin >> credits;
    std::cout << "Letter grade: ";
    std::cin >> grade;
    return course(std::move(trim(name)), credits, std::toupper(grade));
}

void AddStudent(Database& d, bool fromFile)
{
    Student tempS = inputStudent();
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

    bool res = d.insert(tempS);
    if (res) std::cout << d << std::endl;
}

void AddCourse(Database& d)
{
    Student temp = inputStudent(NAME);
    if (d.areMultipleStudents(temp))
    {
        int id;
        std::cout << "Multiple students with that name! Please enter ID also: ";
        std::cin >> id;
        temp = std::move(Student(temp.getFirstName(),temp.getLastName(),id));
    }
    AddCourse(d,temp);
}

void AddCourse(Database& d, const Student& s)
{
    course c = inputCourse();

}

void ModifyGrade(Database& d)
{
    try 
    {
        auto s = d.findStudent(inputStudent());
        
        for (const auto& i : s)
        {
            
        }
    } 
    catch (const std::exception& e) 
    {
        std::cout << e.what() << std::endl;
    }
}

void RemoveStudent(Database& d)
{
    bool loop = true;
    int type;
    while (loop)
    {
        std::cout << "Delete by (1) name or by (2) id? ";
        std::cin >> type;
        if (type != NAME || type != ID)
            std::cout << "Invalid input!\n";
        else
            loop = false;
    }
    Student s = inputStudent(StudentInputType(type));
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
        std::cout << e.what() << std::endl;
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
        std::cout << temp << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
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
{}

void CreateList(Database& d)
{
    std::string fname;
    std::cout << "Enter name of output file: ";
    std::cin >> fname;
    d.writeToFile(fname);
}
