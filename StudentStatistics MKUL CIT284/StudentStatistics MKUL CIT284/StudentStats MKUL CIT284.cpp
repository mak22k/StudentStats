/*  Student Test Statistics - Project 2
 *  CIT-284 Fall 2019
 *
 *  Author: Marisha Kulseng
 *  Date last modified: 10/27/2019
 *
 *  This project seeks to randomly generate test scores  
 *  for students and calculate statistics about these scores.
 *  The scores will be written to a file and the console.
 */

#include <iostream>
#include <cstdlib>
#include <string> 
#include <fstream>
#include <iomanip>
#include <cmath>
#include <vector>
using namespace std;

////////////////////////////////////////////
class ArrayIns
{
private:
	vector<double>theVect;
	int nElems;
public:
	ArrayIns() {
		nElems = 0;
	};
	ArrayIns(int max)  {
		nElems = 0;
		theVect.resize(max);
	}
	~ArrayIns() {};

	void insert(double value) {
		//theVect[nElems] = value;
		//nElems++;
		theVect.push_back(value);
		nElems++;
	}

	double at(int index) {
		return theVect.at(index);
	}

	int size() {
		return theVect.size();
	}

/*	void display() {
		cout << "A=";
		for (int j = 0; j < nElems; j++)
			cout << theVect[j] << " ";
		cout << endl;
	}*/

	void quickSort() {
		recQuickSort(0, nElems - 1);
	}

	void recQuickSort(int left, int right) {
		if (right - left <= 0)
			return;
		else {
			double pivot = theVect[right];

			int partition = partitionIt(left, right, pivot);
			recQuickSort(left, partition - 1);
			recQuickSort(partition + 1, right);
		}
	}

	int partitionIt(int left, int right, double pivot) {
		int leftMark = left - 1;
		int rightMark = right;
		while (true) {
			while (theVect[++leftMark] < pivot);
			while (rightMark > 0 && theVect[--rightMark] > pivot);
			if (leftMark >= rightMark)
				break;
			else
				swap(leftMark, rightMark);
		} // end while
		swap(leftMark, right);
		return leftMark;
	}

	void swap(int dex1, int dex2) {
		int temp = theVect[dex1];
		theVect[dex1] = theVect[dex2];
		theVect[dex2] = temp;
	}
};
////////////////////////////////////////////

struct StudentRecord {
	string name;
	string matYr; // matriculation year: Freshman, Sophomore, Junior, Senior
	string major; // Liberal arts or general science
	//int testScores[10];
	//vector<int> testScores;// (10);
	ArrayIns testScores;// = testScores(10);
	double mean;
	int median;
	//vector<double> mode; // vector to accomodate mulitple values if needed
	ArrayIns mode;
	double stDev; // standard deviation 
};

////////////////////////////////////////////
class StudentRecordArr
{
private:
	vector<StudentRecord>theVect; // student records vector
	int nElems;
public:
	StudentRecordArr() {
		nElems = 0;
	};
	StudentRecordArr(int max) {
		nElems = 0;
		theVect.resize(max);
	}
	~StudentRecordArr() {};

	void insert(StudentRecord value) {
		//theVect[nElems] = value;
		//nElems++;
		theVect.push_back(value);
		nElems++;
	}

	StudentRecord at(int index) {
		return theVect.at(index);
	}

	int size() {
		return theVect.size();
	}

	void quickSort() {
		recQuickSort(0, nElems - 1);
	}

	void recQuickSort(int left, int right) {
		if (right - left <= 0)
			return;
		else {
			double pivot = (theVect[right]).stDev;

			int partition = partitionIt(left, right, pivot);
			recQuickSort(left, partition - 1);
			recQuickSort(partition + 1, right);
		}
	}

	int partitionIt(int left, int right, double pivot) {
		int leftMark = left - 1;
		int rightMark = right;
		while (true) {
			while ((theVect[++leftMark]).stDev < pivot);
			while (rightMark > 0 && (theVect[--rightMark]).stDev > pivot);
			if (leftMark >= rightMark)
				break;
			else
				swap(leftMark, rightMark);
		} // end while
		swap(leftMark, right);
		return leftMark;
	}

	void swap(int dex1, int dex2) {
		StudentRecord temp = theVect[dex1];
		theVect[dex1] = theVect[dex2];
		theVect[dex2] = temp;
	}
};
////////////////////////////////////////////


double getMean(ArrayIns testScores, int numScores = 10);
int getMedian(ArrayIns testScores);
double getStdDev(ArrayIns testScores);
ArrayIns getMode(ArrayIns testScores, int numScores = 10); // ONLY use with sorted list of scores!
bool isSequentialInArray(ArrayIns originalArray, ArrayIns occursArray, int logicalSize); // used with getMode
ArrayIns loadArray(ArrayIns a, int size);
void autoImportStruct(StudentRecord * record);
void printStuRecords(StudentRecordArr record, int index, ostream &os = cout);


int main() {
	system("title Student Scores - M. Kulseng - CIT-284");
	unsigned seed = time(0); // seed random number generator
	srand(seed);

	ofstream outFile;
	const int NUM_STU_RECORDS = 20;
	StudentRecord stuRecords[NUM_STU_RECORDS];
	StudentRecordArr stuRecArr;


	autoImportStruct(stuRecords); // import records

	for (int i = 0; i < NUM_STU_RECORDS; i++) { //insert records into StudentRecordArr
		stuRecArr.insert(stuRecords[i]);
	}
	stuRecArr.quickSort(); // sort by Standard Deviation

	// print to console
	for (int j = 0; j < 20; j++) {
		printStuRecords(stuRecArr, j);
	}

	// print to file
	outFile.open("student_statistics.txt");
	for (int j = 0; j < 20; j++) {
		printStuRecords(stuRecArr, j, outFile);
	}
	outFile.close();
	
	cout << "\nDone\n";
	system("pause");
	return 0;
}


double getMean(ArrayIns testScores, int numScores) {
	double accumulator = 0.0; // store accumulator as double to cause implicit
	for (int i = 0; i < numScores; i++)          // typecast at return statement
		accumulator += testScores.at(i);
	return (accumulator / static_cast<double>(numScores));
}

int getMedian(ArrayIns testScores) { // ONLY use with sorted list of scores!!
	//int numScores = 10; // even number, so median is at i = 4 and i = 5 // Per prof Richmond, use i = 4 ONLY
	//ArrayIns middleScores;
	//middleScores.insert(testScores.at(4));
	//middleScores.insert(testScores.at(5)); //[1] = testScores[5];
	//return static_cast<int>(getMean(middleScores, 2));
	return static_cast<int>(testScores.at(4));
}

double getStdDev(ArrayIns testScores) {
	double mean = getMean(testScores),
		accumulator = 0.0;

	for (int i = 0; i < 10; i++) {
		accumulator += pow((testScores.at(i) - mean), 2); // find the difference and square them, then add up
	}
	return sqrt(accumulator / 10.0);
}

ArrayIns getMode(ArrayIns testScores, int numScores) { // assigns end value to passed vector
	int frequencyArray[101] = { 0 };
	ArrayIns occursTwice, // int occursTwice[5];
		occursThreeTimes, // int occursThreeTimes[3];
		occursFourTimes, // int occursFourTimes[2];
	    occursFiveTimes;
	int logicalSize2 = 0, logicalSize3 = 0, logicalSize4 = 0, logicalSize5 = 0; // logical size for each "occurs" array
	ArrayIns mode;

	for (int i = 0; i < 10; i++) {   // see the number of times a score is represented
		frequencyArray[static_cast<int>(testScores.at(i))] += 1;
	}

	for (int j = 0; j < 101; j++) {
		if (frequencyArray[j] > 5) { // if an element occurs more than 5 times, no other mode can exist
			mode.insert(j);
			return mode;
		}
		if (frequencyArray[j] == 5) {
			occursFiveTimes.insert(j);
			logicalSize5++;
		}
		if (frequencyArray[j] == 4) {
			occursFourTimes.insert(j); 
			logicalSize4++;
		}
		else if (frequencyArray[j] == 3) {
			occursThreeTimes.insert(j);
			logicalSize3++;
		}
		else if (frequencyArray[j] == 2) {
			occursTwice.insert(j);
			logicalSize2++;
		}
	}
	
	if (logicalSize5 != 0) {
		if (logicalSize5 == 1) {
			mode.insert(occursFiveTimes.at(0));
			return mode;
		}
		else if (isSequentialInArray(testScores, occursFiveTimes, logicalSize5)) {			
			mode.insert(getMean(occursFiveTimes, logicalSize5));
			return mode;
		}
		else {
			for (int i = 0; i < logicalSize5; i++) {
				mode.insert(occursFiveTimes.at(i));
			}
			return mode;
		}		// return more than one mode 
	}

	else if (logicalSize4 != 0) {
		if (logicalSize4 == 1) {
			mode.insert(occursFourTimes.at(0));
			return mode;
		}
		else if (isSequentialInArray(testScores, occursFourTimes, logicalSize4)) {
			mode.insert(getMean(occursFourTimes, logicalSize4));
			return mode;
		}
		else {
			for (int i = 0; i < logicalSize4; i++) {
				mode.insert(occursFourTimes.at(i));
			}
			return mode;
		} 		// return more than one mode 
	}
	else if (logicalSize3 != 0) {
		if (logicalSize3 == 1) {
			mode.insert(occursThreeTimes.at(0));
			return mode;
			//return occursThreeTimes[0];
		}
		else if (isSequentialInArray(testScores, occursThreeTimes, logicalSize3)) {
			mode.insert(getMean(occursThreeTimes, logicalSize3));
			return mode;
		}
		else {
			for (int i = 0; i < logicalSize3; i++) {
				mode.insert(occursThreeTimes.at(i));
			}
			return mode;
		}		// return more than one mode 
	}
	else if (logicalSize2 != 0) {
		if (logicalSize2 == 1) {
		    mode.insert(occursTwice.at(0));
			return mode;
		}
		else if (isSequentialInArray(testScores, occursTwice, logicalSize2)) {
			mode.insert(getMean(occursTwice, logicalSize2));
			return mode;
		}
		else {			
			for (int i = 0; i < logicalSize2; i++) {
				mode.insert(occursTwice.at(i));
			}
			return mode;
		} // return more than one mode 
	}
	else {
		mode.insert(getMean(testScores));
		return mode;
	}
}

bool isSequentialInArray(ArrayIns originalArray, ArrayIns occursArray, int logicalSize) {

	for (int i = 9; i >= 0; i--) {
		if (logicalSize == 5 && 
			originalArray.at(i) == occursArray.at(logicalSize-1)
			&& originalArray.at(i - 2) == occursArray.at(logicalSize - 2)
			&& originalArray.at(i - 4) == occursArray.at(logicalSize - 3)
			&& originalArray.at(i - 6) == occursArray.at(logicalSize - 4)
			&& originalArray.at(i - 8) == occursArray.at(logicalSize - 5)) {
			return true;
		}
		if (logicalSize == 4 && originalArray.at(i) == occursArray.at(logicalSize - 1)
			&& originalArray.at(i - 1) == occursArray.at(logicalSize - 2)
			&& originalArray.at(i - 2) == occursArray.at(logicalSize - 3)
			&& originalArray.at(i - 3) == occursArray.at(logicalSize - 4)) {
			return true;
		}
		if (logicalSize == 3 && originalArray.at(i) == occursArray.at(logicalSize - 1)
			&& originalArray.at(i - 1) == occursArray.at(logicalSize - 2)
			&& originalArray.at(i - 2) == occursArray.at(logicalSize - 3)) {
			return true;
		}
		if (logicalSize == 2 && originalArray.at(i) == occursArray.at(logicalSize - 1)
			&& originalArray.at(i - 1) == occursArray.at(logicalSize - 2)) {
			return true;
		}

	}
	return false; // only happens if the others do not return true first
}

ArrayIns loadArray(ArrayIns a, int size) {  // loads random numbers into array
	for (int i = 0; i < size; i++)
		a.insert(rand() % 101);
	a.quickSort();
	return a;
}

void autoImportStruct(StudentRecord * record) { //automatically create 20 records

	(record + 0)->name = "Chidi Anagonye";
	(record + 0)->matYr = "Senior";
	(record + 0)->major = "Liberal Arts";
	(record + 0)->testScores = loadArray((record + 0)->testScores, 10);
	(record + 0)->mean = getMean((record + 0)->testScores);
	(record + 0)->median = getMedian((record + 0)->testScores);
	(record + 0)->mode = getMode((record + 0)->testScores);
	(record + 0)->stDev = getStdDev((record + 0)->testScores);


	(record + 1)->name = "Eleanor Shellstrop";
	(record + 1)->matYr = "Junior";
	(record + 1)->major = "Liberal Arts";
	(record + 1)->testScores = loadArray((record + 1)->testScores, 10);
	(record + 1)->mean = getMean((record + 1)->testScores);
	(record + 1)->median = getMedian((record + 1)->testScores);
	(record + 1)->mode = getMode((record + 1)->testScores);
	(record + 1)->stDev = getStdDev((record + 1)->testScores);


	(record + 2)->name = "Tahani Al-Jamil";
	(record + 2)->matYr = "Sophomore";
	(record + 2)->major = "Liberal Arts";
	(record + 2)->testScores = loadArray((record + 2)->testScores, 10);
	(record + 2)->mean = getMean((record + 2)->testScores);
	(record + 2)->median = getMedian((record + 2)->testScores);
	(record + 2)->mode = getMode((record + 2)->testScores);
	(record + 2)->stDev = getStdDev((record + 2)->testScores);


	(record + 3)->name = "Jason Mendoza";
	(record + 3)->matYr = "Freshman";
	(record + 3)->major = "General Science";
	(record + 3)->testScores = loadArray((record + 3)->testScores, 10);
	(record + 3)->mean = getMean((record + 3)->testScores);
	(record + 3)->median = getMedian((record + 3)->testScores);
	(record + 3)->mode = getMode((record + 3)->testScores);
	(record + 3)->stDev = getStdDev((record + 3)->testScores);


	(record + 4)->name = "Janet Notagirl";
	(record + 4)->matYr = "Senior";
	(record + 4)->major = "General Science";
	(record + 4)->testScores = loadArray((record + 4)->testScores, 10);
	(record + 4)->mean = getMean((record + 4)->testScores);
	(record + 4)->median = getMedian((record + 4)->testScores);
	(record + 4)->mode = getMode((record + 4)->testScores);
	(record + 4)->stDev = getStdDev((record + 4)->testScores);


	(record + 5)->name = "Michael Danson";
	(record + 5)->matYr = "Sophomore";
	(record + 5)->major = "Liberal Arts";
	(record + 5)->testScores = loadArray((record + 5)->testScores, 10);
	(record + 5)->mean = getMean((record + 5)->testScores);
	(record + 5)->median = getMedian((record + 5)->testScores);
	(record + 5)->mode = getMode((record + 5)->testScores);
	(record + 5)->stDev = getStdDev((record + 5)->testScores);


	(record + 6)->name = "\"Donkey\" Doug Mendoza";
	(record + 6)->matYr = "Freshman";
	(record + 6)->major = "Liberal Arts";
	(record + 6)->testScores = loadArray((record + 6)->testScores, 10);
	(record + 6)->mean = getMean((record + 6)->testScores);
	(record + 6)->median = getMedian((record + 6)->testScores);
	(record + 6)->mode = getMode((record + 6)->testScores);
	(record + 6)->stDev = getStdDev((record + 6)->testScores);


	(record + 7)->name = "Derek Hofstetler";
	(record + 7)->matYr = "Freshman";
	(record + 7)->major = "General Science";
	(record + 7)->testScores = loadArray((record + 7)->testScores, 10);
	(record + 7)->mean = getMean((record + 7)->testScores);
	(record + 7)->median = getMedian((record + 7)->testScores);
	(record + 7)->mode = getMode((record + 7)->testScores);
	(record + 7)->stDev = getStdDev((record + 7)->testScores);


	(record + 8)->name = "Doug Forcett";
	(record + 8)->matYr = "Senior";
	(record + 8)->major = "Liberal Arts";
	(record + 8)->testScores = loadArray((record + 8)->testScores, 10);
	(record + 8)->mean = getMean((record + 8)->testScores);
	(record + 8)->median = getMedian((record + 8)->testScores);
	(record + 8)->mode = getMode((record + 8)->testScores);
	(record + 8)->stDev = getStdDev((record + 8)->testScores);


	(record + 9)->name = "Donna Shellstrop";
	(record + 9)->matYr = "Freshman";
	(record + 9)->major = "General Science";
	(record + 9)->testScores = loadArray((record + 9)->testScores, 10);
	(record + 9)->mean = getMean((record + 9)->testScores);
	(record + 9)->median = getMedian((record + 9)->testScores);
	(record + 9)->mode = getMode((record + 9)->testScores);
	(record + 9)->stDev = getStdDev((record + 9)->testScores);


	(record + 10)->name = "Harry Potter";
	(record + 10)->matYr = "Junior";
	(record + 10)->major = "Liberal Arts";
	(record + 10)->testScores = loadArray((record + 10)->testScores, 10);
	(record + 10)->mean = getMean((record + 10)->testScores);
	(record + 10)->median = getMedian((record + 10)->testScores);
	(record + 10)->mode = getMode((record + 10)->testScores);
	(record + 10)->stDev = getStdDev((record + 10)->testScores);


	(record + 11)->name = "Hermione Granger";
	(record + 11)->matYr = "Junior";
	(record + 11)->major = "Liberal Arts";
	(record + 11)->testScores = loadArray((record + 11)->testScores, 10);
	(record + 11)->mean = getMean((record + 11)->testScores);
	(record + 11)->median = getMedian((record + 11)->testScores);
	(record + 11)->mode = getMode((record + 11)->testScores);
	(record + 11)->stDev = getStdDev((record + 11)->testScores);


	(record + 12)->name = "Ronald Weasley";
	(record + 12)->matYr = "Junior";
	(record + 12)->major = "Liberal Arts";
	(record + 12)->testScores = loadArray((record + 12)->testScores, 10);
	(record + 12)->mean = getMean((record + 12)->testScores);
	(record + 12)->median = getMedian((record + 12)->testScores);
	(record + 12)->mode = getMode((record + 12)->testScores);
	(record + 12)->stDev = getStdDev((record + 12)->testScores);


	(record + 13)->name = "Ginny Weasley";
	(record + 13)->matYr = "Sophomore";
	(record + 13)->major = "General Science";
	(record + 13)->testScores = loadArray((record + 13)->testScores, 10);
	(record + 13)->mean = getMean((record + 13)->testScores);
	(record + 13)->median = getMedian((record + 13)->testScores);
	(record + 13)->mode = getMode((record + 13)->testScores);
	(record + 13)->stDev = getStdDev((record + 13)->testScores);


	(record + 14)->name = "Luna Lovegood";
	(record + 14)->matYr = "Sophomore";
	(record + 14)->major = "Liberal Arts";
	(record + 14)->testScores = loadArray((record + 14)->testScores, 10);
	(record + 14)->mean = getMean((record + 14)->testScores);
	(record + 14)->median = getMedian((record + 14)->testScores);
	(record + 14)->mode = getMode((record + 14)->testScores);
	(record + 14)->stDev = getStdDev((record + 14)->testScores);


	(record + 15)->name = "Bellatrix Lestrange";
	(record + 15)->matYr = "Senior";
	(record + 15)->major = "General Science";
	(record + 15)->testScores = loadArray((record + 15)->testScores, 10);
	(record + 15)->mean = getMean((record + 15)->testScores);
	(record + 15)->median = getMedian((record + 15)->testScores);
	(record + 15)->mode = getMode((record + 15)->testScores);
	(record + 15)->stDev = getStdDev((record + 15)->testScores);


	(record + 16)->name = "Neville Longbottom";
	(record + 16)->matYr = "Junior";
	(record + 16)->major = "Liberal Arts";
	(record + 16)->testScores = loadArray((record + 16)->testScores, 10);
	(record + 16)->mean = getMean((record + 16)->testScores);
	(record + 16)->median = getMedian((record + 16)->testScores);
	(record + 16)->mode = getMode((record + 16)->testScores);
	(record + 16)->stDev = getStdDev((record + 16)->testScores);


	(record + 17)->name = "Hannah Abbott";
	(record + 17)->matYr = "Junior";
	(record + 17)->major = "General Science";
	(record + 17)->testScores = loadArray((record + 17)->testScores, 10);
	(record + 17)->mean = getMean((record + 17)->testScores);
	(record + 17)->median = getMedian((record + 17)->testScores);
	(record + 17)->mode = getMode((record + 17)->testScores);
	(record + 17)->stDev = getStdDev((record + 17)->testScores);


	(record + 18)->name = "Padma Patil";
	(record + 18)->matYr = "Junior";
	(record + 18)->major = "Liberal Arts";
	(record + 18)->testScores = loadArray((record + 18)->testScores, 10);
	(record + 18)->mean = getMean((record + 18)->testScores);
	(record + 18)->median = getMedian((record + 18)->testScores);
	(record + 18)->mode = getMode((record + 18)->testScores);
	(record + 18)->stDev = getStdDev((record + 18)->testScores);


	(record + 19)->name = "Lee Jordan";
	(record + 19)->matYr = "Senior";
	(record + 19)->major = "General Science";
	(record + 19)->testScores = loadArray((record + 19)->testScores, 10);
	(record + 19)->mean = getMean((record + 19)->testScores);
	(record + 19)->median = getMedian((record + 19)->testScores);
	(record + 19)->mode = getMode((record + 19)->testScores);
	(record + 19)->stDev = getStdDev((record + 19)->testScores);
	
}


void printStuRecords(StudentRecordArr record, int index, ostream &os) {
	os << record.at(index).name << "\n\t" << record.at(index).matYr << "\n\t"
		<< record.at(index).major << "\n\t";

	for (int i = 0; i < 10; i++)
		os << record.at(index).testScores.at(i) << " ";

	os << "\n\tMean: " << record.at(index).mean << "\n\tMedian: " << record.at(index).median << "\n\tMode: ";

	for (int j = 0; j < record.at(index).mode.size(); j++)
		os << record.at(index).mode.at(j) << " ";

	os << "\n\tStandard Deviation: " << record.at(index).stDev << "\n\n";
}

