#define _CRT_SECURE_NO_WARNINGS
#define SIZE 1023
/*#define _CRTDBG_MAP_ALLOC
#incounterLettersude <stdlib.h>
#incounterLettersude <crtdbg.h>*/ //uncomment this block to check for heap memory allocation leaks.
// Read https://docounterStudent.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
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
char** split(char* str, const char* delimiter, int coursesPerStudent);
void freeStr(char*** str, int** coursesPerStudent, int* numberOfStudents);

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
	printStudentArray(students, coursesPerStudent, numberOfStudents);
	factorGivenCourse(students, coursesPerStudent, numberOfStudents, "Advanced Topics in C", +5);
	free(students, &coursesPerStudent, &numberOfStudents);
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
	char text[1023];
	int counterOfStudents = 0; //Creating intiger for counting
	int* pointer = 0;
	studentList = fopen(fileName, "r"); //Set the file path
	if (!studentList) {
		printf("Error opening file\n");
		exit(1);
	}
	while (!feof(studentList)) {
		fgets(text, 1023, studentList);
		if (!counterOfStudents) {
			pointer = (int*)malloc((counterOfStudents + 1) * sizeof(int*));
		}
		else {
			pointer = realloc(pointer, (counterOfStudents + 1) * sizeof(int));
		}
		if (!pointer) {
			printf("Memory allocation faild\n");
			exit(1);
		}
		pointer[counterOfStudents] = countPipes(text, 1023);
		*numberOfStudents = counterOfStudents + 1;
		counterOfStudents++;
	}
	*coursesPerStudent = pointer; //Set the number of the pipes in the pointer
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
	char*** studentList;
	FILE* studentFile;
	char temp[1023], * check;
	int counterStudents = 0;
	studentFile = fopen(fileName, "r");
	if (!studentFile) {
		printf("Error opening file\n");
		exit(1);
	}
	countStudentsAndCourses(fileName, coursesPerStudent, numberOfStudents);
	studentList = (char*)malloc(*numberOfStudents * sizeof(char*));
	if (!studentList) {
		printf("Memory allocation faild\n");
		exit(1);
	}
	for (int i = 0; i < *numberOfStudents; i++) {
		studentList[i] = (char*)malloc((((*(*coursesPerStudent + i)) * 2) + 1)* sizeof(char*));
		if (!studentList[i]) {
			printf("Memory allocation faild\n");
			exit(1);
		}
	}
	char line[1023];
	while (fgets(line, 1023, studentFile)) {
		char** arr = split(line, "|", *(*coursesPerStudent + counterStudents) + 1);
		if (!arr) exit(1);
		studentList[counterStudents][0] = arr[0];
		for (int i = 1; i <= *(*coursesPerStudent + counterStudents); i++) {
			char** courses = split(arr[i], ",", 2);
			int j = i * 2 - 1;
			studentList[counterStudents][j] = courses[0];
			studentList[counterStudents][j + 1] = courses[1];
		}
		counterStudents++;
	}
	fclose(studentFile);
	return studentList;
}



void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor)
{
	
	if (factor < -20 || factor>20)
	{
		printf("factor out of range!\n");
		exit(1);
	}

	for (int i = 0; i < numberOfStudents; i++)
	{
		for (int j = 1; j <= 2 * coursesPerStudent[i]; j += 2)
		{
			if ((strcmp(students[i][j], courseName)) == 0)
			{
				int x = atoi(students[i][j + 1]);
				if ((x + factor) > 0 && (x + factor) < 100)
				{
					x += factor;
					snprintf(students[i][j+1], 10, "%d", x);
				}
			}
		}
	}


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
	FILE* studentFile;
	studentFile = fopen("studentList.txt", "w");

	for (int i = 0; i < numberOfStudents; i++)
	{
		fprintf(studentFile, "%s %s", students[i][0], "|"); // writing the student name to the text file.
		for (int j = 1; j <= 2 * coursesPerStudent[i]; j += 2)
		{
			fprintf(studentFile, "%s %s", students[i][j], ","); //writing the course of the student to the text file .
			if (((j + 1) / 2) == (coursesPerStudent[i])) //check if the loop have reached to the last course of the current student.
				fprintf(studentFile, "%s %s", students[i][j + 1], "\n"); //writing the  last grade of the course to the text file .
			else fprintf(studentFile, "%s %s", students[i][j + 1], "|"); // writing the grade of the course to the text file.
		}
	}
	// ???? ????? ?? ??????? ?? ???? ?????????,???? ???????? ????? ?? ??????,
	//?? ?????? ????? ????????? ?? ?? ??????, ??? ???????? ??????? ???? ?? ?????? ????? ????? ?????? ??????? ???????? 3
	fclose(studentFile);


}

void writeToBinFile(const char* fileName, Student* students, int numberOfStudents)
{
	//add code here
}

Student* readFromBinFile(const char* fileName)
{
	FILE* studentFile;
	studentFile = fopen(fileName, studentFile);
	Student* studentList;

}

Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents)
{

	Student** student_array;
	student_array = (Student**)malloc(sizeof(Student) * numberOfStudents);
	if (!student_array)
	{
		printf("bad allocation!");
		exit(1);
	}
	
	for (int i = 0; i < numberOfStudents; i++)
	{
		strcpy(student_array[i]->name, students[i][0]);
		student_array[i]->grades = (StudentCourseGrade*)malloc(sizeof(StudentCourseGrade) *  coursesPerStudent[i]);
		if (!student_array[i]->grades) {
			printf("Memory allocation faild\n");
			exit(1);
		}
		for (int j = 1; j <= 2 * coursesPerStudent[i]; j += 2)
		{
			student_array[i]->grades->courseName[j] = students[i][j];
			int temp = atoi(students[i][j + 1]);
			student_array[i]->grades->grade[j] = temp;
			snprintf(students[i][j + 1], 10, "%d", temp);
		}


	}


}


char** split(char* str, const char* delimiter, int coursesPerStudent) {
	int i = 0;
	char** studentArray = (char**)malloc(coursesPerStudent * sizeof(char*));

	if (!studentArray) {
		printf("Memory allocation faild\n");
		exit(1);
	}
	char* token = strtok(str, delimiter);
	while (token) {
		studentArray[i] = (char*)malloc(strlen(token) + 1);
		if (!studentArray[i]) {
			printf("Memory allocation faild\n");
			exit(1);
		}
		strcpy(studentArray[i], token);
		token = strtok(NULL, delimiter);
		i++;
	}
	return studentArray;
}

void freeStr(char*** str, int** coursesPerStudent, int* numberOfStudents) {
	for (int i = 0; i < *numberOfStudents; i++) {
		for (int j = 0; j < *(*coursesPerStudent + j) * 2 + 1; j++) {
			free(str[i][j]);
		}
		free(str[i]);
	}
	free(*coursesPerStudent);
	free(str);
}




