#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines the length of the file. Changeable to function
// correctly with whatever the length of the used file is.
#define FILELEN 50
// Same with majors, safe assumption is 10.
#define MAJNUM 10

/*
/ This program reads in a file (most likely students_dataset.csv) and answers five questions based on the data found in 
/ the file. This is done by storing students and their associated data as a structure/object for the sake of convienence
/ and consolidation. These structures are then stored in an array mimicing the file.
*/
// Stucture for the student object
struct student{
    char *firstName, *lastName, *major, *degree, *TA, *advisor;
    float gpa;
    int creditHrs;

};

// Structure for the department object
struct department {
    char *deptName;
    int numTAs;
    float totalGPA;
    int numStudents;
};

// Function to create a department onbject based on a student input.
// Returns a newly created department.
struct department makeDept(struct student chkStudent) {
    struct department newDept;

    memcpy(&newDept.deptName, &chkStudent.major, sizeof(chkStudent.major));
    memcpy(&newDept.totalGPA, &chkStudent.gpa, sizeof(chkStudent.gpa));
    newDept.numTAs = 1;
    newDept.numStudents = 1;

    return newDept;
}

// only serves to indicate another student and/or TA are part of the department
void updateDept(struct department *currDept, int check, float gpa) {
    
    (*currDept).numStudents++;
    
    if (check == 0) {
        (*currDept).numTAs += 1;
    }

    (*currDept).totalGPA += gpa;

}

// Creates a student object based on input from the file
// Returns a newly created student object.
struct student fillInfo(char* line) {
    struct student newStudent;

    newStudent.firstName = strdup(strtok(line, ","));
    newStudent.lastName = strdup(strtok(NULL, ","));
    newStudent.major = strdup(strtok(NULL, ","));
    newStudent.degree = strdup(strtok(NULL, ","));
    newStudent.gpa = (float) atof(strtok(NULL, ","));
    newStudent.creditHrs = atoi(strtok(NULL, ","));
    newStudent.TA = strdup(strtok(NULL, ","));
    newStudent.advisor = strdup(strtok(NULL, ","));

    return newStudent;

}

/* Was used in testing to make sure the data was properly read in.
/ Still included for the sake of testing if an input has any errors. 
/ Based off the code shown in class slides from the seventh lecture.
/ Cited as CS332_07.pdf
*/
/*
void showStud(struct student chkStud) {
    printf("Name: %s %s\n", chkStud.firstName, chkStud.lastName);
    printf("Major: %s\n", chkStud.major);
    printf("Degree: %s\n", chkStud.degree);
    printf("GPA: %f\n", chkStud.gpa);
    printf("Credit Hours: %d\n", chkStud.creditHrs);
    printf("TA: %s\n", chkStud.TA);
    printf("Advisor: %s\n", chkStud.advisor);
}
*/

// method/function to compare strings
int comparer(char *first, char *second) {
    return strcmp(first, second);
}


int main(int argc, char** argv) {
    // initialize variables for storing and looping through data
    int counter;
    int lineSize = 1024;
    char line[lineSize];

    // arrays to store data structures
    struct student stdntList[FILELEN];
    struct department deptList[MAJNUM];

    // Open the file, catch thrown error if file is not found
    FILE *filePtr = fopen("students_dataset.csv", "r");
    if (filePtr == NULL) {
        printf("Error reading input file students_dataset.csv\n");
        exit (-1);
    }

    /*
    / Code snippet based on a question from stack overflow.
    / Uses fgets to skip a line by reading it, but will most
    / likely malfunction when used on a line over 100 characters long.
    / Source: https://stackoverflow.com/questions/2799612/how-to-skip-a-line-when-fscanning-a-text-file
    */
    char buffer[100];
    fgets(buffer, 100, filePtr);
    // End of snippet

    // Read through the entire file and store data in structures in the array
    counter = 0;
    while (fgets(line, lineSize, filePtr) != NULL) {
        stdntList[counter] = fillInfo(line);
        counter++;
    }

    fclose(filePtr);


    // Function to store all majors/departments as structures
    // Is very useful later on.
    // Loops through all stored student values

    // Initialize needed variables
    int i, j;
    int check, majAmt;
    check = majAmt = 1;
    int crntDep = 0;
    // Store first value in the array
    deptList[crntDep] = makeDept(stdntList[0]);

    // loop through all student values
    for (i = 1; i < FILELEN; i++) {
        // Loop through all majors
        for (j = 0; j < majAmt; j++) {
            // Make a comparison. If the department/major appears in the array, flag it. 
            // Also flag if a TA is present
            int val = comparer(deptList[j].deptName, stdntList[i].major);
            int isTA = comparer(stdntList[i].TA, "Yes");
            // If flagged, add it to the flagged (existing) department as an information update
            if (val == 0) {
                updateDept(&deptList[j], isTA, stdntList[i].gpa);
                check = 0;
                break;
            }
        }

        // If department/major not flagged as present, add it.
        if (check == 1) {
            crntDep++;
            deptList[crntDep] = makeDept(stdntList[i]);
            majAmt++;
        }
        // reset flag for next check
        check = 1;

    }
    // Answer for Question 1: Different degrees
    int degAmt, degCheck;
    degAmt = degCheck = 1;
    // Initialize with 5 slots for degrees as a safe number, and a length of 10 characters
    char degrees[5][10];
    int crntDeg = 0;
    // Add first degree into array
    strcpy(degrees[crntDeg], stdntList[0].degree);
    // Loop through all students
    for (i = 1; i < FILELEN; i++) {
        // Loop through all degrees
        for (j = 0; j < degAmt; j++) {
            // If degree is found in degrees array, flag it
            int val = comparer(degrees[j], stdntList[i].degree);
            if (val == 0) {
                degCheck = 0;
                break;
            }
        }

        // If degree is not found in the area, add it to the area
        if (degCheck == 1) {
            crntDeg++;
            strcpy(degrees[crntDeg], stdntList[i].degree);
            degAmt++;
        }
        // reset flag for next check
        degCheck = 1;
    }

    // Print the answer to the first question using the array
    printf("Question 1: There are %d degrees. They are: \n\n", degAmt);
    for (i = 0; i < degAmt; i++) {
        printf("%s\n", degrees[i]);
    }
    printf("\n---------------------------------\n\n");

    // Answer for Question 2: Full name of 3 students with highest gpa
    // Based off of a "top three values" alogorithm
    // Source: https://algorithms.tutorialhorizon.com/find-first-three-largest-elements-in-a-given-array/
    // Initialize the "top three" values
    int first, second, third;
    first = second = third = 0;

    for (i = 0; i < FILELEN; i++) {
        float curr = stdntList[i].gpa;
        // If the current gpa is larger than the one labeled as "first", make curr the first one
        if (curr > stdntList[first].gpa) {
            // swapping values to put the current value as the largest
            third = second;
            second = first;
            first = i;
        }
        // if current is larger than second value, set it as second
        else if (curr > stdntList[second].gpa){
            // swaps
            third = second;
            second = curr;
        }
        // if current is larger than third, set it as third
        else if (curr > stdntList[third].gpa) {
            // swap
            third = curr;
        }
    }
    // Print answer to question 1
    printf("Question 2: The students with the highest GPA are: \n\n");
    printf("1st: %s %s : %.2f GPA\n\n", stdntList[first].firstName, stdntList[first].lastName, stdntList[first].gpa);
    printf("2nd: %s %s : %.2f GPA\n\n", stdntList[second].firstName, stdntList[second].lastName, stdntList[second].gpa);
    printf("3rd: %s %s : %.2f GPA\n\n", stdntList[third].firstName, stdntList[third].lastName, stdntList[third].gpa);
    printf("\n---------------------------------\n\n");
    
    // Answer for Question 3: What are the average Credit Hours of the
    // college?
    double credTotal = 0;\
    // add up credits of all students
    for (i = 0; i < FILELEN; i++) {
        credTotal += stdntList[i].creditHrs;
    }
    // average based on number of students
    double avgCred = credTotal / FILELEN;
    // Print average credit hours to 2 digits of precision.
    printf("Question 3: The average credit hours are %.2f hours.\n", avgCred);
    printf("\n---------------------------------\n\n");

    // Answer for Question 4: What is the Average GPA of the CS Department
    int CSpos;
    // finds the computer science department in the department array
    for (i = 0; i < majAmt; i++) {
        if (comparer(deptList[i].deptName, "Computer Science") == 0) {
            CSpos = i;
        }
    }
    // Takes the computed total gpa value that was stored in the department object, and divides it by its number
    // of students value. Then, prints the answer to the question at 2 digits of precision.
    float totGPA = deptList[CSpos].totalGPA;
    int numStdnt = deptList[CSpos].numStudents;
    double avgGPA = totGPA / numStdnt;
    printf("Question 4: The average GPA of the Computer Science Department is %.2f.\n", avgGPA);
    printf("\n---------------------------------\n\n");

    // Answer for Question 5: Displaying the List of Departments and advisors
    char advisors[10][20];
    int crntAdvsr, advMatch, advAmt;
    advAmt = advMatch = 1;
    crntAdvsr = 0;
    // add first advisor into array
    strcpy(advisors[crntAdvsr], stdntList[0].advisor);
    // Loop through all students
    for (i = 0; i < FILELEN; i++) {
        // Loop through all advisors
        for (j = 0; j < advAmt; j++) {
            // if advisor is in array flag it
            int val = comparer(advisors[j], stdntList[i].advisor);
            if (val == 0) {
                advMatch = 0;
                break;
            }
        }
        // if no advisor was flagged (not founf in array), add the current one to the array of advisors
        if (advMatch == 1) {
            crntAdvsr++;
            strcpy(advisors[crntAdvsr], stdntList[i].advisor);
            advAmt++;
        }
        // reset flag
        advMatch = 1;
    }

    // Print answer to the question by looping and comparing
    // (Not my most clever solution.)
    printf("Question 5: The Departments and each's respective advisor numbers are: \n\n");
    // Loop through all the majors/departments
    for (i = 0; i < majAmt; i++) {
        // Make a value to hold the number of advisors, an array to flag advisors already listed, and a 
        // variable for incrementing.
        int deptAdvisor = 0;
        char foundAdv[advAmt + 1][20];
        int y;
        // Loop through all students
        for (j = 0; j < FILELEN; j++) {
            // If the student is in the current department...
            if (!comparer(stdntList[j].major, deptList[i].deptName)) {
                int z;
                // Loop though the list of advisors...
                for (z = 0; z < advAmt; z++) {
                    // And check which advisor they have, and therefore belongs in the department
                    // Also make sure the advisor isn't flagged as already being found.
                    if (!comparer(stdntList[j].advisor, advisors[z]) && comparer(foundAdv[z], advisors[z])) {
                        // increment the amount of advisors in the current department, and
                        // flag the found advisor by copying it into the flagging array
                        deptAdvisor++;
                        strcpy(foundAdv[z], advisors[z]);
                        break;
                    }
                }
            }
        }

        // Clear array
        for (y = 0; y < advAmt + 1; y++) {
            strcpy(foundAdv[y], "");
        }
        // TPrint the department name, and the number of advisors.
        printf("%s : %d\n", deptList[i].deptName, deptAdvisor);
    }
    
    
    return 0;
}
