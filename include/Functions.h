#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Database.h"
#include "Student.h"

bool isValid(const std::string&);
void AddStudent(Database& d);
void AddCourse(Database& d);
void AddCourse(Database& d, const Student& s);
void RemoveStudent(Database& d);
void RemoveStudent(Database& d, const Student& s);
void RemoveCourse(Database& d);
void FindGPA(Database& d);
void Sort(Database& d);
void FindStudent(Database& d);
void PrintHonors(Database& d);
void PrintWarn(Database& d);
void PrintFail(Database& d);
void ModifyGrade(Database& d);

#endif // FUNCTIONS_H
