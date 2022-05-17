#define _CRT_SECURE_NO_WARNINGS
/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>*/ //uncomment this block to check for heap memory allocation leaks.
// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct StudentCourseGrade
{
	char courseName[35];
	int grade;
}StudentCourseGrade;

typedef struct Student
{
	char name[35];
	StudentCourseGrade* grades; //dynamic array of courses
	int numberOfCourses;
}Student;


//Part A
void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
int countPipes(const char* lineBuffer, int maxCount);
char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents);
void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor);
void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents);

//Part B
Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents);
void writeToBinFile(const char* fileName, Student* students, int numberOfStudents);
Student* readFromBinFile(const char* fileName);

int main()
{
	

	//Part A
	int* coursesPerStudent = NULL;
	int numberOfStudents = 0;
	char*** students = makeStudentArrayFromFile("studentList.txt", &coursesPerStudent, &numberOfStudents);
	factorGivenCourse(students, coursesPerStudent, numberOfStudents, "Advanced Topics in C", +5);
	printStudentArray(students, coursesPerStudent, numberOfStudents);
	//studentsToFile(students, coursesPerStudent, numberOfStudents); //this frees all memory. Part B fails if this line runs. uncomment for testing (and comment out Part B)
	
	//Part B
	Student* transformedStudents = transformStudentArray(students, coursesPerStudent, numberOfStudents);
	writeToBinFile("students.bin", transformedStudents, numberOfStudents);
	Student* testReadStudents = readFromBinFile("students.bin");

	//add code to free all arrays of struct Student


	/*_CrtDumpMemoryLeaks();*/ //uncomment this block to check for heap memory allocation leaks.
	// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019

	return 0;
}

void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	FILE* studentList;
	int counterOfStudents = 0, i = 1; //Creating intiger for counting
	*numberOfStudents = 0;
	studentList = fopen(fileName, "r"); //Set the file path
	if (!studentList) {
		printf("Error opening file\n");
		exit(1);
	}
	char* tempBuffer = NULL; //Creating a string for the function to count the pipes
	while (!feof(studentList)) {
		if (i == 1) {
			tempBuffer = (char*)malloc(i * sizeof(char*));
		}
		else {
			tempBuffer = realloc(tempBuffer, i * sizeof(char)); // Allocating memory for the string
		}
		if (!tempBuffer) {
			printf("Memory allocation faild\n");
			exit(1);
		}
		tempBuffer[i-1] = fgetc(studentList); 
		i++;
		if ((tempBuffer[i-2] == '\n') || (feof(studentList))) {
			if (!counterOfStudents) {
				coursesPerStudent = (int*)malloc(sizeof(int*));
			}
			else {
				coursesPerStudent = realloc(coursesPerStudent, (counterOfStudents + 1) * sizeof(*coursesPerStudent));
			}
			if (!coursesPerStudent) {
				printf("Memory allocation faild\n");
				exit(1);
			}
			const char* lineBuffer = tempBuffer; //Creating a const string for the function
			*(coursesPerStudent + counterOfStudents) = countPipes(lineBuffer, i); //Set the number of the pipes in the pointer
			counterOfStudents++;
			*numberOfStudents = counterOfStudents; 
			free(tempBuffer);
			i = 1;
		}
		
	}
	fclose(studentList);
}

int countPipes(const char* lineBuffer, int maxCount)
{
	int p_count = 0, counter = 0;; //Creating the intigers 
	if (!lineBuffer) return -1; //Checking if lineBuffer points to Null
	if (maxCount <= 0) return 0; //Checking if maxCount less than zero
	while (*(lineBuffer + counter) != '\0' || p_count == maxCount) //Counting the pipes in the string
	{
		if (*(lineBuffer + counter) == '|') p_count++;
		counter++;
	}
	return p_count; //Returning the pipes count
}

char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	//add code here
}

void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor)
{
	//add code here
}

void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents)
{
	for (int i = 0; i < numberOfStudents; i++)
	{
		printf("name: %s\n*********\n", students[i][0]); 
		for (int j = 1; j <= 2 * coursesPerStudent[i]; j += 2)
		{
			printf("course: %s\n", students[i][j]);
			printf("grade: %s\n", students[i][j + 1]);
		}
		printf("\n");
	}
}

void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents)
{
	//add code here
}

void writeToBinFile(const char* fileName, Student* students, int numberOfStudents)
{
	//add code here
}

Student* readFromBinFile(const char* fileName)
{
	//add code here
}

Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents)
{
	//add code here
}
