/**
 *	Alp Eren Gençoğlu
 *	150170019
 *	Analysis Of Algorithms
 *	15/10/2019
 */

#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cstdlib>
using namespace std;

/**
 * This function creates and returns a random integer between
 * zero and specified maximum number.
 */
int randomNumber(int num){
	return rand()%num + 1;
}

void printArray(int* Arr, int size){
	for(int i=0; i<size; i++){
		if (i%10 == 0 && i!=0) cout << endl;
		cout << setw(4) << Arr[i] << " ";
	}
	cout << endl << endl;
}

int findDuplicates(int* Arr, int size){
	int counter = 0;

	for(int i=0; i<size-1; i++){
		for(int j=i+1; j<size; j++){
			if(Arr[i] == Arr[j]){
				counter++;
				if(counter%10 == 0 && counter != 0) cout << endl;
				cout << "(" << i+1 << "," << j+1 << ") ";
			}
		}
	}
	cout << endl << endl;

	return counter;
}

int main(int argc, char const *argv[]) {
	
	// initial operations: command line argument and declaring variables
	if (argc != 2){
		cout << "Please specify array size!" << endl;
		return -1;
	}

	int size;
	int* A;
	int encountered_duplications;
	istringstream iss(argv[1]);

	try{
		iss >> size;
	}
	catch(...){
		cout << "Invalid Array size argument!" << endl;
		return -1;
	}


	// Set and print the Array
	A = new int[size];
	srand(time(0));
	
	for(int i=0; i<size; i++)
		A[i] = randomNumber(size);
	printArray(A, size);

	// Expected and Encountered duplicates
	cout << "EXPECTED NUMBER OF DUPLICATIONS = " << (size-1)/2.0 << endl << endl;
	encountered_duplications = findDuplicates(A, size);
	cout << "ENCOUNTERED NUMBER OF DUPLICATIONS = " << encountered_duplications << endl; 

	return 0;
}
