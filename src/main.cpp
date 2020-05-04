#include "include/Student.h"
#include "include/Database.h"
#include "include/Functions.h"
#include <algorithm>
#include "include/LL.hpp"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv)
{
    std::string fname;
    if (argc != 2)
    {
        cout << "No filename passed in! Creating empty list." << endl;
    }
    else fname = argv[1];

    Database d;
    if (fname.size() && file_exists(fname))
    {
        d.readFromFile(fname);
    }
    bool exit = false;
    while(!exit)
    {
        cout << string('=',30) << endl;
        std::string option;
        cout << "Options: " << endl
             << " 1. Add new student" << endl             // X
             << " 2. Add course for student" << endl      // X
             << " 3. Remove student" << endl              // X
             << " 4. Remove course for student" << endl   // X
             << " 5. Find GPA for student" << endl        // X
             << " 6. Sort" << endl                        // X
             << " 7. Search for student" << endl          // X
             << " 8. Modify grade for student" << endl    // X
             << " 9. Print honors students" << endl       // X
             << "10. Print warnings" << endl              // X
             << "11. Print failing students" << endl      // X
             << "12. Print database" << endl              // X
             << "13. Create class list" << endl           // X
             << "14. Erase entire list" << endl           // X
             << "15. Clear screen" << endl
             << "16. Exit" << endl << "? ";               // X
        cin >> option;

        int selection = 0;
        if (isValid(option)) selection = std::stoi(option);

        switch(selection)
        {
        case 1: AddStudent(d); break;
        case 2: AddCourse(d); break;
        case 3: RemoveStudent(d); break;
        case 4: RemoveCourse(d); break;
        case 5: FindGPA(d); break;
        case 6: Sort(d); break;
        case 7: FindStudent(d); break;
        case 8: ModifyGrade(d); break;
        case 9: PrintHonors(d); break;
        case 10: PrintWarn(d); break;
        case 11: PrintFail(d); break;
        case 12: cout << d << endl
                      << "Total number of students: "
                      << d.numStudents() << endl; break;
        case 13: CreateList(d); break;
        case 14: d.clearList(); break;
        case 15: cout << string(50,'\n') << endl; break;
        case 16: exit = true; break;
        default: cout << "Invalid selection!" << endl;
        };
    }

    return 0;
}
