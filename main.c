#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// You can easily rename your subjects here
#define SUBJECTS 5
const char *SUBJECT_NAMES[SUBJECTS] = {"Maths", "Physics", "Chemistry", "Biology", "English"};
#define PASS_MARK 40 
#define MAX_MARK 100 

// ---------------- STRUCTURE ----------------
typedef struct {
    int roll_no;
    char name[50];
    int marks[SUBJECTS];
    int total;
    float average;
    float percentage;
    char grade;
 } Student;
 

 // =============== FUNCTION PROTOTYPES & UTILITIES ==================
void printSeparator(char c, int len);
int getIntInput();
int rollNumberExists(Student *s, int n, int roll); 
int compareStudents(const void *a, const void *b); 

void inputStudent(Student *s);
void displayStudentDetailed(Student s); // Renamed: Detailed output for single record display
void displayStudentCSV(Student s);      // NEW: Single-line CSV output

void calculateResults(Student *s);
char assignGrade(float percentage);
void classTopper(Student *s, int n);
void subjectWiseHighest(Student *s, int n);
void subjectAverages(Student *s, int n);
void failAnalysis(Student *s, int n); 
void gradeDistribution(Student *s, int n); 
void generateMeritList(Student *s, int n);

void saveToText(Student *s, int n);
void saveToCSV(Student *s, int n);
void loadFromCSV(Student **s, int *n);
void searchByRoll(Student *s, int n);
void searchByName(Student *s, int n);
void modifyStudent(Student *s, int n);
void deleteStudent(Student **s, int *n);