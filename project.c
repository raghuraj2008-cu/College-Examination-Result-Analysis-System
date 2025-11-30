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


// ---------------- FUNCTION PROTOTYPES & UTILITIES ----------------
void printSeparator(char c, int len);
int getIntInput();
int rollNumberExists(Student *s, int n, int roll); 
int compareStudents(const void *a, const void *b); 

void inputStudent(Student *s);
void displayStudentDetailed(Student s); 
void displayStudentCSV(Student s);      

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
// getIntInput(): integer input safely lene ke liye. Agar scanf fail ho jaye
// toh input buffer clear karke -1 return karta hai, taaki validation ki ja sake.
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
    return s2->total - s1->total;
}
// compareStudents(): qsort() ke liye comparison function. Dono students ka total
// compare karke descending order (highest total first) return karta hai.
// ---------------- INPUT & DISPLAY ----------------
void inputStudent(Student *s) {
    int roll;
    
    printf("\n--- Adding New Student ---\n");
    
    do {
        printf("Enter Roll Number: ");
        roll = getIntInput();
        if (rollNumberExists(s - (s - &s[0]), (s - &s[0]), roll)) {
            printf("Error: Roll number %d already exists. Please enter a unique roll number.\n", roll);
        } else if (roll <= 0) {
             printf("Error: Roll number must be a positive integer.\n");
        }
    } while (rollNumberExists(s - (s - &s[0]), (s - &s[0]), roll) || roll <= 0);
    s->roll_no = roll;

    printf("Enter Name: ");
    scanf(" %[^\n]", s->name);

    printf("Enter %d Subject Marks (0-%d):\n", SUBJECTS, MAX_MARK);
    for (int i = 0; i < SUBJECTS; i++) {
        do {
            printf("%s (%d): ", SUBJECT_NAMES[i], i + 1);
            s->marks[i] = getIntInput();
            if (s->marks[i] < 0 || s->marks[i] > MAX_MARK) {
                printf("Error: Marks must be between 0 and %d.\n", MAX_MARK);
            }
        } while (s->marks[i] < 0 || s->marks[i] > MAX_MARK);
    }
}

// Detailed output (used by search/modify)
void displayStudentDetailed(Student s) {
    printSeparator('-', 44);
    printf("Roll No - %d\n", s.roll_no);
    printf("Name - %s\n", s.name);
    printSeparator('-', 44);
    
    printf("| MARKS SUMMARY:\n");
    for(int i = 0; i < SUBJECTS; i++) {
        printf("|  - %-10s: %3d\n", SUBJECT_NAMES[i], s.marks[i]);
    }
    
    printf("|------------------------------------\n");
    printf("| Total Score   : %d / %d\n", s.total, SUBJECTS * MAX_MARK);
    printf("| Average       : %.2f\n", s.average);
    printf("| Percentage : %.2f%%\n", s.percentage);
    printf("| Grade : %c\n", s.grade);
    printSeparator('-', 44);
}
// print karta hai. Ye option 2 (Display All Students - CSV) ke liye use hota hai.
// NEW FUNCTION: Single-line CSV output (used by case 2)
void displayStudentCSV(Student s) {
    // Print main data fields
    printf("%d,%s,%d,%.2f,%c",
           s.roll_no, s.name, s.total, s.percentage, s.grade);
    
    // Print subject marks
    for (int i = 0; i < SUBJECTS; i++) {
        printf(",%d", s.marks[i]);
    }
    printf("\n");
}


// ---------------- RESULT CALCULATION ----------------
void calculateResults(Student *s) {
    s->total = 0;
    for (int i = 0; i < SUBJECTS; i++)
        s->total += s->marks[i];

    s->average = s->total / (float) SUBJECTS;
    s->percentage = (s->total / (float)(SUBJECTS * MAX_MARK)) * 100.0;
    s->grade = assignGrade(s->percentage);
}

char assignGrade(float p) {
    if (p >= 90.0) return 'A';
    if (p >= 80.0) return 'B';
    if (p >= 70.0) return 'C';
    if (p >= 60.0) return 'D';
    if (p >= 50.0) return 'E';
    return 'F';
}

// ---------------- REPORTING & ANALYSIS ----------------

void classTopper(Student *s, int n) {
    if (n == 0) { printf("No records available to determine the topper.\n"); return; }

    int top_total = -1;
    
    for (int i = 0; i < n; i++) {
        if (s[i].total > top_total) {
            top_total = s[i].total;
        }
    }
    
    printf("\n*** CLASS TOPPER(S) (Highest Total: %d / %d) ***\n", top_total, SUBJECTS * MAX_MARK);
    int topper_count = 0;
    for (int i = 0; i < n; i++) {
        if (s[i].total == top_total) {
            printf(">> %s (Roll %d) - %.2f%%, Grade %c\n", 
                   s[i].name, s[i].roll_no, s[i].percentage, s[i].grade);
            topper_count++;
        }
    }
    printf("Total Toppers: %d\n", topper_count);
    printSeparator('-', 50);
}

void subjectWiseHighest(Student *s, int n) {
    if (n == 0) { printf("No records available for subject analysis.\n"); return; }
    
    printf("\n--- SUBJECT-WISE HIGHEST MARKS ---\n");

    for (int sub = 0; sub < SUBJECTS; sub++) {
        int max_mark = -1;
        
        for (int i = 0; i < n; i++) {
            if (s[i].marks[sub] > max_mark) {
                max_mark = s[i].marks[sub];
            }
        }
        
        printf("%-10s Highest (%d/%d): ", SUBJECT_NAMES[sub], max_mark, MAX_MARK);
        int topper_count = 0;
        for (int i = 0; i < n; i++) {
            if (s[i].marks[sub] == max_mark) {
                if (topper_count > 0) printf(", ");
                printf("%s (Roll %d)", s[i].name, s[i].roll_no);
                topper_count++;
            }
        }
        printf("\n");
    }
    printSeparator('-', 50);
}

void subjectAverages(Student *s, int n) {
    if (n == 0) { printf("No records available to calculate averages.\n"); return; }
    
    printf("\n--- Subject-wise Class Averages ---\n");
    printSeparator('-', 50);

    for (int sub = 0; sub < SUBJECTS; sub++) {
        float sum = 0;
        for (int i = 0; i < n; i++)
            sum += s[i].marks[sub];

        printf("%-10s Average: *%.2f*\n", SUBJECT_NAMES[sub], sum / n);
    }
    printSeparator('-', 50);
}

void failAnalysis(Student *s, int n) {
    if (n == 0) { printf("No records available for failure analysis.\n"); return; }
    
    printf("\n--- FAILURE ANALYSIS (Pass Mark: %d) ---\n", PASS_MARK);
    printSeparator('-', 50);
    
    int total_failures = 0;

    for (int i = 0; i < n; i++) {
        int failed_subjects = 0;
        printf("Roll %d, %s: ", s[i].roll_no, s[i].name);
        
        for (int sub = 0; sub < SUBJECTS; sub++) {
            if (s[i].marks[sub] < PASS_MARK) {
                if (failed_subjects > 0) printf(", ");
                printf("%s (%d)", SUBJECT_NAMES[sub], s[i].marks[sub]);
                failed_subjects++;
            }
        }
        
        if (failed_subjects > 0) {
            printf(" -> FAILED in %d subjects.\n", failed_subjects);
            total_failures++;
        } else {
            printf(" PASSED all subjects.\n");
        }
    }
    printSeparator('-', 50);
    printf("Total Students failed in at least one subject: %d / %d\n", total_failures, n);
}

void gradeDistribution(Student *s, int n) {
    if (n == 0) { printf("No records available for grade distribution.\n"); return; }
    
    int grade_counts[6] = {0}; // A, B, C, D, E, F

    for (int i = 0; i < n; i++) {
        switch (s[i].grade) {
            case 'A': grade_counts[0]++; break;
            case 'B': grade_counts[1]++; break;
            case 'C': grade_counts[2]++; break;
            case 'D': grade_counts[3]++; break;
            case 'E': grade_counts[4]++; break;
            case 'F': grade_counts[5]++; break;
        }
    }
    
    printf("\n--- CLASS GRADE DISTRIBUTION ---\n");
    printSeparator('*', 50);
    printf("Grade | Count | Percentage of Class\n");
    printSeparator('-', 50);

    char grades[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    for (int i = 0; i < 6; i++) {
        float percent = (n > 0) ? (grade_counts[i] * 100.0 / n) : 0.0;
        printf("  %c   | %5d | %9.2f%%\n", grades[i], grade_counts[i], percent);
    }
    printSeparator('*', 50);
    printf("Total Students: %d\n", n);
}

void generateMeritList(Student *s, int n) {
    if (n == 0) { printf("No records available to generate a merit list.\n"); return; }
    
    Student *merit_list = (Student *)malloc(n * sizeof(Student));
    if (!merit_list) {
        printf("Error: Memory allocation failed for merit list.\n");
        return;
    }
    memcpy(merit_list, s, n * sizeof(Student));
    
    qsort(merit_list, n, sizeof(Student), compareStudents);

    printf("\n============== CLASS MERIT LIST (Sorted by Total) ==============\n");
    printf("Rank\tRoll\t\tName\t\t\tTotal\tPercent\tGrade\n");
    printSeparator('-', 64);
    for (int i = 0; i < n; i++) {
        printf("%-7d %-15d %-20s %-7d %-7.2f %-5c\n",
               i + 1, merit_list[i].roll_no, merit_list[i].name, merit_list[i].total, merit_list[i].percentage, merit_list[i].grade);
    }
    printSeparator('-', 64);
               
    free(merit_list);
}


// ---------------- FILE I/O AND MODIFICATIONS ----------------

void saveToText(Student *s, int n) {
    if (n == 0) { printf("No data to save.\n"); return; }
    FILE *fp = fopen("result_report.txt", "w");
    if (!fp) { printf("Error: Could not open result_report.txt for writing.\n"); return; }

    fprintf(fp, "====================== STUDENT REPORT (%d RECORDS) ======================\n", n);
    fprintf(fp, "Roll\tName\t\tTotal\tPercent\tGrade\t");
    for(int i = 0; i < SUBJECTS; i++) {
        fprintf(fp, "%s\t", SUBJECT_NAMES[i]);
    }
    fprintf(fp, "\n");
    
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d\t%s\t\t%d\t%.2f\t%c\t",
                s[i].roll_no, s[i].name, s[i].total, s[i].percentage, s[i].grade);
        for(int j = 0; j < SUBJECTS; j++) {
            fprintf(fp, "%d\t", s[i].marks[j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("\nSuccessfully saved %d records to result_report.txt\n", n);
}

void saveToCSV(Student *s, int n) {
    if (n == 0) { printf("No data to save.\n"); return; }
    FILE *fp = fopen("students.csv", "w");
    if (!fp) { printf("Error: Could not open students.csv for writing.\n"); return; }

    // Header line
    fprintf(fp, "Roll,Name,Total,Percentage,Grade");
    for(int i = 0; i < SUBJECTS; i++) {
        fprintf(fp, ",%s", SUBJECT_NAMES[i]);
    }
    fprintf(fp, "\n");
    
    for (int i = 0; i < n; i++) {
        // Use the same single-line output format as displayStudentCSV
        fprintf(fp, "%d,%s,%d,%.2f,%c",
                s[i].roll_no, s[i].name, s[i].total, s[i].percentage, s[i].grade);
        for(int j = 0; j < SUBJECTS; j++) {
            fprintf(fp, ",%d", s[i].marks[j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("\nSuccessfully saved %d records to students.csv (Backup File)\n", n);
}

void loadFromCSV(Student **s, int *n) {
    FILE *fp = fopen("students.csv", "r");
    if (!fp) return;

    char line[200];
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        Student *new_s = realloc(*s, (*n + 1) * sizeof(Student));
        if (!new_s) {
            printf("Error: Memory reallocation failed during file load.\n");
            free(*s);
            *s = NULL;
            *n = 0;
            fclose(fp);
            return;
        }
        *s = new_s;

        Student *st = &(*s)[*n];
        
        int result = sscanf(line, "%d,%49[^,],%d,%f,%c,%d,%d,%d,%d,%d",
            &st->roll_no, st->name, &st->total, &st->percentage, &st->grade,
            &st->marks[0], &st->marks[1], &st->marks[2], &st->marks[3], &st->marks[4]);

        if (result == 5 + SUBJECTS) { 
            calculateResults(st); 
            (*n)++;
        } else {
             printf("Warning: Skipped malformed line in CSV.\n");
        }
    }

    fclose(fp);
}

void searchByRoll(Student *s, int n) {
    if (n == 0) { printf("No records to search.\n"); return; }
    int roll;
    printf("\nEnter roll number to search: ");
    roll = getIntInput();

    for (int i = 0; i < n; i++)
        if (s[i].roll_no == roll) {
            printf("\n--- Found Record ---\n");
            displayStudentDetailed(s[i]);
            return;
        }

    printf("No record found for Roll Number %d.\n", roll);
}

void searchByName(Student *s, int n) {
    if (n == 0) { printf("No records to search.\n"); return; }
    char name[50];
    printf("\nEnter name to search: ");
    scanf(" %[^\n]", name);
    
    printf("\n--- Matching Records ---\n");
    int found = 0;
    for (int i = 0; i < n; i++)
        if (strcasecmp(s[i].name, name) == 0) { 
            displayStudentDetailed(s[i]);
            found = 1;
        }

    if (!found) {
        printf("No record found for Name: %s.\n", name);
    }
}

void modifyStudent(Student *s, int n) {
    if (n == 0) { printf("No records to modify.\n"); return; }
    int roll;
    printf("\nEnter roll number to modify: ");
    roll = getIntInput();

    for (int i = 0; i < n; i++) {
        if (s[i].roll_no == roll) {
            printf("\nEditing record for Roll %d (%s):\n", s[i].roll_no, s[i].name);
            
            int old_roll = s[i].roll_no; 
            
            printf("Keeping Roll Number: %d\n", old_roll);
            s[i].roll_no = old_roll; 

            printf("Enter NEW Name: ");
            scanf(" %[^\n]", s[i].name);

            printf("Enter NEW %d Subject Marks (0-%d):\n", SUBJECTS, MAX_MARK);
            for (int k = 0; k < SUBJECTS; k++) {
                do {
                    printf("%s (%d): ", SUBJECT_NAMES[k], k + 1);
                    s[i].marks[k] = getIntInput();
                    if (s[i].marks[k] < 0 || s[i].marks[k] > MAX_MARK) {
                        printf("Error: Marks must be between 0 and %d.\n", MAX_MARK);
                    }
                } while (s[i].marks[k] < 0 || s[i].marks[k] > MAX_MARK);
            }

            calculateResults(&s[i]);
            printf("\nRecord updated successfully!\n");
            displayStudentDetailed(s[i]);
            return;
        }
    }

    printf("Record not found.\n");
}

void deleteStudent(Student **s, int *n) {
    if (*n == 0) { printf("No records to delete.\n"); return; }
    int roll;
    printf("\nEnter roll number to delete: ");
    roll = getIntInput();

    for (int i = 0; i < *n; i++) {
        if ((*s)[i].roll_no == roll) {

            for (int j = i; j < *n - 1; j++)
                (*s)[j] = (*s)[j + 1];

            (*n)--;
            
            *s = realloc(*s, (*n) * sizeof(Student));

            printf("\nRecord for Roll %d deleted successfully!\n", roll);
            return;
        }
    }

    printf("Record not found.\n");
}
