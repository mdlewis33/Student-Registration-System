// Program to implement a class registration system

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int STUIDSIZE = 5;		// Maximum size for a student ID
const int COURSEIDSIZE = 9;		// Maximum size for a course ID

const char STUFILE[] = "students.txt";
const char COURSEFILE[] = "courses.txt";

// Structure used to define a course registration
struct Course
{
	char id[COURSEIDSIZE];
	int section;
	int credit;
};

// Type definition to define the data type for the list data
typedef Course ListItemType;

// Node structure
struct Node
{
	ListItemType item;
	Node* next;
};

// Structure used to associate a student with the classes they are taking
struct Student
{
	char id[STUIDSIZE];
	Node* courses;
};
// Searches parameter array for parameter key using a recursive binary search algorithm with parameter start and end positions.
int binarySearch(const Student array[], const int startPos, const int endPos, const char key[]);

// Reads student file and inputs all student IDs in an array and returns a pointer to the array in dynamic memory.
Student* getStudents();

// Reads courses file and inputs all courses into linked lists in the parameter student array.
void getCourses(Student* stuPtr);

// Adds the parameter courseID, section, and credit values to a new node at the start of the linked list at head pointer location.
Node* addToList(Node*& head, char courseID[], int section, int credit);

// Counts amount of students in the student file.
int studentCount();

// Rewrites the course file to include all courses held in linked lists in the parameter array.
void rewriteFile(Student* stuPtr);

// Displays all of the courses held in the linked list of the parameter student.
void displayCourses(Student student);

int main()
{
	int choice;	// Holds the inputed menu choice.
	Student* stuPtr = NULL;	// Holds pointer to array of student structures in dynamic memory.
	char selected[STUIDSIZE];	// Holds the ID of the selected student.
	int stuPos = NULL;	// Holds the position of the selected student in the stuPtr array.
	bool studentSelected = false; // Holds whether or not a student has been selected.
	char courseID[COURSEIDSIZE];	// Array that holds a student's course ID.
	int section = NULL;	// Holds the section number of a course.
	int credit = NULL;	// Holds the amount of credits a course is worth.
	int remove = NULL;	// Holds which course the user wants to remove from a student's schedule.

	// Puts all students into an array in dynamic memory and stores location in stuPtr.
	stuPtr = getStudents();

	// Stores courses into a linked list for every student.
	getCourses(stuPtr);

	do
	{
		cout << "1. Select Student by ID\n";
		cout << "2. Add a Course to Student's Schedule\n";
		cout << "3. Remove a Course from Student's Schedule\n";
		cout << "4. Display Selected Student's Schedule\n";
		cout << "5. Exit\n";
		cout << endl;
		cout << "Select: ";

		// Gets the numeric entry from the menu
		cin >> choice;

		// Makes the 'enter' key that was pressed after the numeric entry be ignored
		cin.ignore();

		switch (choice)
		{
		case 1:
			// Asks for student ID to be inputted and searches for student in stuPtr array using binary search.
			cout << endl << "Enter student ID: ";
			cin.getline(selected, STUIDSIZE);
			stuPos = binarySearch(stuPtr, 0, studentCount(), selected);
			if (stuPos == -1)
			{
				cout << endl << "Student ID not found " << endl;
				studentSelected = false;
			}
			else
			{
				cout << endl << "Student selected " << endl << endl;
				studentSelected = true;
			}
			break;
		case 2:
			// Asks user to input courseID, section number, and credit hours and stores them in a new node in selected student's
			// linked list.
			if (!studentSelected)
			{
				cout << endl << "No student selected " << endl;
			}
			else
			{
				cout << endl << "Enter course ID: ";
				cin.getline(courseID, COURSEIDSIZE);
				cout << endl << "Enter section number: ";
				cin >> section;
				cout << endl << "Enter credit hours: " ;
				cin >> credit;
				cout << endl;
				stuPtr[stuPos].courses = addToList(stuPtr[stuPos].courses, courseID, section, credit);
			}
			break;
		case 3:
			// Display's selected student's courses, asks which course to remove, and removes that course from selected
			// student's linked list.
			if (!studentSelected)
			{
				cout << endl << "No student selected" << endl;
			}
			else
			{
				displayCourses(stuPtr[stuPos]);
				cout << "Choose course to remove: ";
				cin >> remove;
				Node* temp;
				Node* current;
				Node* previous;
				int count = 1;
				if (remove == 1)
				{
					temp = stuPtr[stuPos].courses->next;
					delete stuPtr[stuPos].courses;
					stuPtr[stuPos].courses = temp;
				}
				else
				{
					previous = stuPtr[stuPos].courses;
					current = previous->next;
					while (count != remove)
					{
						previous = current;
						current = previous->next;
						count++;
					}
					previous->next = current->next;
					delete current;
				}
				cout << endl << "Course removed." << endl << endl;
			}
			break;
		case 4:
			// Display's selected student's courses.
			if (!studentSelected)
			{
				cout << endl << "No student selected";
			}
			else
			{
				displayCourses(stuPtr[stuPos]);
			}
			break;
		}

	} while (choice != 5);
	// Deletes everything in courses file and rewrites it using courses stored in all of the linked lists in stuPtr array.
	rewriteFile(stuPtr);

	// Deletes everything stored in dynamic memory.
	for (int i = 0; i < studentCount(); i++)
	{
		while (stuPtr[i].courses->next != NULL)
		{
			Node* temp = stuPtr[i].courses->next;
			delete stuPtr[i].courses;
			stuPtr[i].courses = temp;
		}
	}
	delete stuPtr;

	return 0;
}

int binarySearch(const Student array[], const int startPos, const int endPos, const char key[])
{	// Base case
	if (startPos > endPos)
	{
		return -1;
	}
	int mid = (startPos + endPos) / 2;
	bool equal = true;
	// Finds if the key is equal to the student ID stored in the array at mid position.
	for (int i = 0; i < STUIDSIZE; i++)
	{
		if (key[i] != array[mid].id[i])
		{
			equal = false;
		}
	}
	// Base case
	if (equal)
	{
		return mid;
	}

	// Recursive function calls if key is less than or greater than student ID stored in array at mid position.
	for (int x = 0; x < STUIDSIZE; x++)
	{
		if (key[x] < array[mid].id[x])
		{
			return binarySearch(array, startPos, mid - 1, key);
		}
		if (key[x] > array[mid].id[x])
		{
			return binarySearch(array, mid + 1, endPos, key);
		}
	}
	return 0;
}

Student* getStudents()
{
	int stuCount = studentCount();
	char stuID[STUIDSIZE]; // array of all students
	ifstream inFile;
	inFile.open(STUFILE);

	//check to see if the student file can be read
	if (!inFile)
	{
		cout << "The student file cannot be read" << endl;
		exit(1);
	}

	Student* stuPtr = new Student[stuCount]; // makes a spot in dynamic memory to store an array of student structures
	inFile.getline(stuID, STUIDSIZE);

	// Puts each student into one array in dynamic memory
	for (int i = 0; i < stuCount && !inFile.eof(); i++)
	{
		for (int x = 0; x < STUIDSIZE; x++)
		{
			stuPtr[i].id[x] = stuID[x];
		}
		inFile.getline(stuID, STUIDSIZE);
	}
	inFile.close();

	return stuPtr;
}

int studentCount()
{
	int stuCount = 0;
	char stuID[STUIDSIZE]; // gets the students ID from the file
	ifstream inFile;
	inFile.open(STUFILE);

	//check to see if the student file can be read
	if (!inFile)
	{
		cout << "The student file cannot be read" << endl;
		exit(1);
	}

	inFile.getline(stuID, STUIDSIZE);

	// Counts number of students
	while (!inFile.eof())
	{
		stuCount++;
		inFile.getline(stuID, STUIDSIZE);
	}
	inFile.close();

	return stuCount;
}

void getCourses(Student* stuPtr)
{
	ifstream inFile;
	char courseID[COURSEIDSIZE]; // array to hold the name of the course
	int section; // section number
	int credit; // credit number
	int counter = 0;
	inFile.open(COURSEFILE);

	//check to see if the student file can be read
	if (!inFile)
	{
		cout << "The student file cannot be read" << endl;
		exit(1);
	}

	int amount = studentCount();

	inFile.getline(courseID, COURSEIDSIZE);
	
	// Makes a new node at each students course and sets the next to NULL
	for (counter = 0; counter < amount; counter++)
	{
		stuPtr[counter].courses = new Node;
		stuPtr[counter].courses->next = NULL;
	}

	// Gets the courseID, section number, and credit hours from the file and puts it 
	// in the students courses linked list.
	for (counter = 0; counter < amount; counter++)
	{
		while (!inFile.eof() && !(courseID[0] == 'E' && courseID[1] == 'N' && courseID[2] == 'D'))
		{
			section = inFile.get() - 48;
			inFile.get();
			credit = inFile.get() - 48;
			inFile.get();
			stuPtr[counter].courses = addToList(stuPtr[counter].courses, courseID, section, credit);
			inFile.getline(courseID, COURSEIDSIZE);
		}
		inFile.getline(courseID, COURSEIDSIZE);
	}
	inFile.close();
}

Node* addToList(Node*& head, char courseID[], int section, int credit)
{
	Node* temp = new Node;

	// Adds the courseID, section number, and credit hours in the front of the linked list
	for (int x = 0; x < COURSEIDSIZE; x++)
	{
		temp->item.id[x] = courseID[x];
	}
	temp->item.section = section;
	temp->item.credit = credit;
	temp->next = head;
	head = temp;

	return head;
}

void rewriteFile(Student* stuPtr)
{
	ofstream outFile;
	outFile.open(COURSEFILE); // clears and opens file

	//check to see if the student file can be read
	if (!outFile)
	{
		cout << "The student file cannot be read" << endl;
		exit(1);
	}

	Node* temp = stuPtr[0].courses;
	int amount = studentCount(); 

	// Changes a line of code starting with the courseID, then the Section number, then the credit number.
	// If a student has another class then this repeats, if not then the next line is END and the whole loop repeats
	for (int i = 0; i < amount; i++)
	{
		temp = stuPtr[i].courses;
		while (temp->next != NULL)
		{
			// Writes each letter of the courseID one at a time with an array of characters
			for (int x = 0; x < COURSEIDSIZE - 1; x++)
			{
				outFile << temp->item.id[x];
			}
			outFile << endl;
			outFile << temp->item.section << endl;
			outFile << temp->item.credit << endl;

			temp = temp->next;
		}
		outFile << "END" << endl;
	}

	outFile.close();
}

void displayCourses(Student student)
{
	cout << endl;
	int counter = 0;
	Node* temp = student.courses;

	// Prints a one for the first class, then prints out the courseID, Section number, and Credit hours
	while (temp->next != NULL)
	{
		counter++;
		cout << counter << ". ";
		for (int i = 0; i < COURSEIDSIZE; i++)
		{
			cout << temp->item.id[i];
		}
		cout << " Sect: " << temp->item.section << " Credit: " << temp->item.credit << endl;
		temp = temp->next;
	}

	// If the program above does not run then that means they have no courses, therefore this one runs
	if (counter == 0)
	{
		cout << "Student is not registered for any courses." << endl;
	}
	cout << endl;
}