#include "include/Functions.h"
#include <string>

// enumerated type to determine the typf of input the user
// desires
enum StudentInputType { ALL, NAME, ID };

bool file_exists(std::string fname)
{
    std::ifstream file;
    bool retval;
    file.open(fname.c_str());
    if (file)
    {
        retval = true;
    }
    else
    {
        retval = false;
    }
    file.close();
    return retval;
}

// determines if user input is valid
bool isValid(const std::string& s)
{
    for(const auto& c : s)
    {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

// trims a string
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

// allows a user to input a student and returns it
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
        std::string temp;
        while (true)
        {
            std::cout << "Student id: ";
            std::cin >> temp;//id;
            if (isValid(temp))
            {
                id = std::stoi(temp);
                break;
            }
            std::cout << "Invalid id! Must be numeric! ";
        }
    }
    return Student(std::move(trim(fname)),std::move(trim(lname)),id);
}

// allows user to input a course and returns it
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
        while (true)
        {
            std::string temp;
            std::cout << "Number of credit hours: ";
            std::cin >> temp;
            if (isValid(temp))
            {
                credits = std::stoi(temp);
                break;
            }
            std::cout << "Invalid entry, must be numeric! ";
        }
        if (credits > 0) break;
        std::cout << "Hours must be >0! ";
    }
    while (true)
    {
        while (true)
        {
            std::string temp;
            std::cout << "Letter grade: ";
            std::cin >> temp;
            if (temp.size() == 1)
            {
                grade = temp[0];
                break;
            }
            std::cout << "Invalid input! Must be A,B,C,D,F. ";
        }
        grade = std::toupper(grade);
        if (grade == 'A' || grade == 'B' || grade == 'C' || grade == 'D' || grade == 'F') break;
        std::cout << "Invalid grade, must be A,B,C,D,F. ";
    }
    return course(std::move(trim(name)), credits, std::toupper(grade));
}

// seeks to add a student to the database
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
        while (true)
        {
            std::string temp;
            std::cout << "number of courses enrolled in: ";
            std::cin >> temp;
            if (isValid(temp))
            {
                num = std::stoi(temp);
                break;
            }
            std::cout << "Invalid entry! Must be numeric and > 0! ";
        }
        if (num > 0) break;
        std::cout << "Invalid entry! Must be > 0! ";
    }
    while (num > 0)
    {
        try
        {
            tempS.insertCourse(inputCourse());
            --num;
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    try
    {
        d.insert(tempS);
     std::cout << d << std::endl
               << "Total number of students: " << d.numStudents() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

// adds a course to a student
void AddCourse(Database& d)
{
    if (d.numStudents() == 0)
    {
        std::cout << "No students currently enrolled!" << std::endl;
        return;
    }
    int type;
    Student s;
    while(true)
    {
        while (true)
        {
            std::string temp;
            std::cout << "Find student by (1) name or by (2) id? ";
            std::cin >> temp;
            if (isValid(temp))
            {
                type = std::stoi(temp);
                break;
            }
            std::cout << "Invalid input, must be numeric! ";
        }
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
                course c = inputCourse();
                d.findStudent(s).insertCourse(c);
            }
            catch(const std::exception& e)
            {
                std::cout << e.what() << std::endl;
                return;
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
            d.findStudent(id);
            course c = inputCourse();
            d.findStudent(id).insertCourse(c);
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return;
        }
    }
    std::cout << d << std::endl;
}

// removes a student from the database
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
        while (true)
        {
            std::string temp;
            std::cout << "Delete by (1) name or by (2) id? ";
            std::cin >> temp;
            if (isValid(temp))
            {
                type = std::stoi(temp);
                break;
            }
            std::cout << "Invalid entry! ";
        }
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
               std::cout << d << std::endl << "Number of students "
                         << d.numStudents() << std::endl;
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
        while (true)
        {
            std::string temp;
            std::cout << "Enter ID: ";
            std::cin >> temp;
            if (isValid(temp))
            {
                id = std::stoi(temp);
                break;
            }
            std::cout << "Invalid entry, must be numeric! ";
        }
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

// returns the gpa for a student
void FindGPA(Database& d)
{
    if (d.numStudents() == 0)
    {
        std::cout << "No students currently enrolled!" << std::endl;
        return;
    }
    int type;
    while (true)
    {
        while (true)
        {
            std::string temp;
            std::cout << "Find student by (1) name or by (2) id? ";
            std::cin >> temp;
            if (isValid(temp))
            {
                type = std::stoi(temp);
                break;
            }
        }
        if (type != NAME && type != ID)
            std::cout << "Invalid option!" << std::endl;
        else break;
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
                double gpa = d.findStudent(s).getGPA();
                std::cout << "GPA: " << gpa << std::endl;
            }
            catch(const std::exception& e)
            {
                std::cout << e.what() << std::endl;
            }
            return;
        }
    }
    if (type == ID)
    {
        int id;
        std::cout << "Enter ID: ";
        std::cin >> id;
        try
        {
            double gpa = d.findStudent(id).getGPA();
            std::cout << "GPA: " << gpa << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}

// allows the user to sort the list by various
// methods and then prints it
void Sort(Database& d)
{
    if (d.numStudents() == 0)
    {
        std::cout << "No students currently enrolled!" << std::endl;
        return;
    }
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

// allows the user to find a student and then
// print out info
void FindStudent(Database& d)
{
    if (d.numStudents() == 0)
    {
        std::cout << "No students currently enrolled!" << std::endl;
        return;
    }
    int id;
    while (true)
    {
        std::string temp;
        std::cout << "Enter student id: ";
        std::cin >> temp;
        if (isValid(temp))
        {
            id = std::stoi(temp);
            break;
        }
        std::cout << "Invalid input! ";
    }

    try
    {
        Student temp = d.findStudent(id);
        std::cout << std::setw(15) << std::left << "First Name"
                  << std::setw(15) << std::left << "Last Name"
                  << std::setw(10) << std::left << "ID"
                  << std::setw(10) << std::left << "Course"
                  << std::setw(10) << std::left << "Credits"
                  << std::setw(5) << std::left << "Grade"
                  << std::endl << temp << std::endl << "GPA: " << temp.getGPA() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

// print out the types of students by grade
void PrintHonors(Database& d)
{
    if (d.numStudents() == 0)
    {
        std::cout << "No students currently enrolled!" << std::endl;
        return;
    }
    d.honorStudents();
}

void PrintWarn(Database& d)
{
    if (d.numStudents() == 0)
    {
        std::cout << "No students currently enrolled!" << std::endl;
        return;
    }
    d.warningStudents();
}

void PrintFail(Database& d)
{
    if (d.numStudents() == 0)
    {
        std::cout << "No students currently enrolled!" << std::endl;
        return;
    }
    d.failingStudents();
}

// removes a course for a student in the
// database
void RemoveCourse(Database& d)
{
    bool removed = false;
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
        while (true)
        {
            std::string temp;
            std::cout << "Find student by (1) name or by (2) id? ";
            std::cin >> temp;
            if (isValid(temp))
            {
                type = std::stoi(temp);
                break;
            }
        }
        if (type != NAME && type != ID)
            std::cout << "Invalid input!\n";
        else
            loop = false;
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
                d.findStudent(s).removeCourse();
                if (d.findStudent(s).numCourses() == 0)
                {
                    removed = true;
                    d.remove(s);
                }
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << std::endl;
                return;
            }
        }
    }
    if (type == ID)
    {
        try
        {
            int id;
            while (true)
            {
                std::string temp;
                std::cout << "Enter ID: ";
                std::cin >> temp;
                if (isValid(temp))
                {
                    id = std::stoi(temp);
                    break;
                }
                std::cout << "Invalid entry, must be numeric! ";
            }
            d.findStudent(id).removeCourse();
            if (d.findStudent(id).numCourses() == 0)
            {
                removed = true;
                d.remove(id);
                std::cout << d << std::endl;
            }
       }
       catch (const std::exception& e)
       {
            std::cout << e.what() << std::endl;
            return;
        }
    }
    std::cout << d << std::endl << "Total number of students: " << d.numStudents() << std::endl;
    if (removed)
    {
        std::cout << "Student removed since no longer enrolled" << std::endl;
    }
}

// modify a student's grade in a course
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
        while (true)
        {
            std::string temp;
            std::cout << "Find student by (1) name or by (2) id? ";
            std::cin >> temp;
            if (isValid(temp))
            {
                type = std::stoi(temp);
                break;
            }
            std::cout << "Invalid input! ";
        }
        if (type != NAME && type != ID)
            std::cout << "Invalid input!\n";
        else
            loop = false;
    }

    if (type == NAME)
    {
        try
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
            std::cout << d.findStudent(s) << std::endl
                      << "GPA: " << d.findStudent(s).getGPA() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    if (type == ID)
    {
        try
        {
            int id;
            while (true)
            {
                std::string temp;
                std::cout << "Enter ID: ";
                std::cin >> temp;
                if (isValid(temp))
                {
                    id = std::stoi(temp);
                    break;
                }
                std::cout << "Invalid input, must be numeric! ";
            }
            d.findStudent(id).modifyCourse();
            std::cout << d.findStudent(id) << std::endl
                      << "GPA: " << d.findStudent(id).getGPA() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}

void Save(Database& d)
{
    std::string fname;
    while(true)
    {
        std::cout << "Enter file name: ";
        std::cin >> fname;
        bool exists = file_exists(fname);
        if (!exists) break;
        std::cout << "File exists! Overwrite (y/n)? ";
        char c;
        std::cin >> c;
        c = std::tolower(c);
        if (c == 'y') break;
    }
    d.writeToFile(fname);
}

// creates database from file
void CreateList(Database& d)
{
    if (d.numStudents() > 0)
    {
        char c;
        while (true)
        {
            std::cout << "Save current database before loading new (y/n)? ";
            std::cin >> c;
            c = std::tolower(c);
            if (c == 'y' || c == 'n') break;
            std::cout << "Invalid selection! ";
        }
        if (c == 'y')
        {
            Save(d);
        }
    }
    d.clearList();
    std::string fname;
    std::cout << "Enter name of input file: ";
    std::cin >> fname;
    while (!file_exists(fname))
    {
        std::cout << "File not present! Enter name of input file: ";
        std::cin >> fname;
    }
    std::cout << "Creating database..." << std::endl;
    d.readFromFile(fname);
    std::cout << d << std::endl
              << "Total number of students: "
              << d.numStudents() << std::endl;
}
