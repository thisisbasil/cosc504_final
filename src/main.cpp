#include "include/Student.h"
#include "include/Database.h"
#include "include/Functions.h"
#include <algorithm>
#include "include/LL.hpp"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv)
{
    cout << "COSC504 Final Project: Basil Huffman" << endl;
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
             << " 1. Add new student" << endl             
             << " 2. Add course for student" << endl      
             << " 3. Remove student" << endl              
             << " 4. Remove course for student" << endl   
             << " 5. Find GPA for student" << endl        
             << " 6. Sort" << endl                        
             << " 7. Search for student" << endl          
             << " 8. Modify grade for student" << endl    
             << " 9. Print honors students" << endl       
             << "10. Print warnings" << endl              
             << "11. Print failing students" << endl      
             << "12. Print database" << endl              
             << "13. Create class list" << endl           
			 << "14. Save class list" << endl
             << "15. Erase entire list" << endl           
             << "16. Clear screen" << endl
             << "17. Exit" << endl << "? ";               
        cin >> option;

        int selection = 0;
        if (isValid(option))
        {
            selection = std::stoi(option);
        }

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
		case 14: Save(d); break;
        case 15: d.clearList(); cout << "Destroying database..." << endl; break;
        case 16: cout << string(50,'\n') << endl; break;
        case 17: exit = true; break;
        default: cout << "Invalid selection!" << endl;
        };
    }

    return 0;
}
