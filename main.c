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

// ---------------- MAIN MENU ----------------
int main() {
    int n = 0, choice;
    Student *students = NULL;

    loadFromCSV(&students, &n);
    if (n > 0) {
        printf("\nSuccessfully loaded %d previous records from students.csv.\n", n);
    }

    do {
        printSeparator('=', 64);
        printf("||          STUDENT RESULT MANAGEMENT SYSTEM            ||\n");
        printSeparator('=', 64);
        printf(" 1. Add New Student             | 7. Class Topper\n");
        printf(" 2. Display All Students (CSV)  | 8. Subject-Wise Highest\n");
        printf(" 3. Search by Roll Number       | 9. Subject-Wise Averages\n");
        printf(" 4. Search by Name              | 10. Generate Merit List\n");
        printf(" 5. Modify Student Record       | 13. Fail Analysis\n");
        printf(" 6. Delete Student Record       | 14. Grade Distribution\n");
        printSeparator('-', 64);
        printf(" 11. Save to Text File          | 0. Exit\n");
        printf(" 12. Save to CSV (Data Backup)\n");
        printSeparator('=', 64);
        printf("Enter your choice: ");
        choice = getIntInput(); 

        switch(choice) {

            case 1:
                students = realloc(students, (n + 1) * sizeof(Student));
                if (students == NULL) {
                    printf("Error: Memory allocation failed.\n");
                    return 1; 
                }
                inputStudent(&students[n]);
                calculateResults(&students[n]);
                n++;
                printf("\nStudent added successfully!\n");
                break;

            case 2:
                if (n == 0) { printf("No student records available.\n"); break; }
                printf("\n============ ALL STUDENT RECORDS (%d) - CSV FORMAT ============\n", n);
                
                // Print the header exactly as requested
                printf("Roll,Name,Total,Percentage,Grade,Maths,Physics,Chemistry,Biology,English\n");
                
                for (int i = 0; i < n; i++) {
                    displayStudentCSV(students[i]);
                }
                printSeparator('=', 64);
                break;

            case 3: searchByRoll(students, n); break;
            case 4: searchByName(students, n); break;
            case 5: modifyStudent(students, n); break;
            case 6: deleteStudent(&students, &n); break;
            case 7: classTopper(students, n); break;
            case 8: subjectWiseHighest(students, n); break;
            case 9: subjectAverages(students, n); break;
            case 10: generateMeritList(students, n); break;
            case 11: saveToText(students, n); break;
            case 12: saveToCSV(students, n); break;
            case 13: failAnalysis(students, n); break;
            case 14: gradeDistribution(students, n); break;

            case 0:
                printf("\nExiting system. Goodbye!\n");
                break;

            default:
                printf("\nInvalid choice. Try again.\n");
        }
    } while(choice != 0);

    free(students);
    return 0;
}


// ---------------- UTILITY FUNCTIONS ----------------
void printSeparator(char c, int len) {
    for (int i = 0; i < len; i++) {
        printf("%c", c);
    }
    printf("\n");
}

int getIntInput() {
    int input;
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        return -1; 
    }
    return input;
}

int rollNumberExists(Student *s, int n, int roll) {
    for (int i = 0; i < n; i++) {
        if (s[i].roll_no == roll) {
            return 1;
        }
    }
    return 0;
}

int compareStudents(const void *a, const void *b) {
    Student *s1 = (Student *)a;
    Student *s2 = (Student *)b;
    return s2->total - s1->total;
}