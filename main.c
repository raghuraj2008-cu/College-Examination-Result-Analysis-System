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