/**
 * Alp Eren gençoðlu
 * 150170019
 * 29.11.2019
 * 
 * Assignment 2 - Event Scheduler Using Min Heap.
 */


#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Event {
  public:
	string name;
	int time;
	bool start;	// true if start, false if end
	
	Event(string& n, int t, bool s) : name(n), time(t), start(s) {}
	Event() : name("e"), time(0), start(1) {}
};

void minHeapify(vector<Event*> A, int size, int index){
	int left = 2*index + 1;
	int right = 2*index + 2;
	int min_index = index;
	
	if(left < size && A[left]->time < A[min_index]->time)
		min_index = left;
	if(right < size && A[right]->time < A[min_index]->time)
		min_index = right;
	
	if(min_index != index){
		//swap(A[index], A[min_index]);
		Event temp = *A[index];
		*A[index] = *A[min_index];
		*A[min_index] = temp;
		minHeapify(A, size, min_index);
	}
}

void heapSortSchedule(vector<Event*> A, int size){
	// Build min heap
	for(int i=(size)/2-1; i>=0; i--){
		minHeapify(A, size, i);
	}
	
	
	// Print Schedule by popping from heap (first element has smallest value)
	int t = 1;	// virtual clock tick
	while(true){	// loop until printing the last element of schedule
		int min = A[0]->time;
		bool no_event = true;
		while(min == t){
			cout << "TIME " << t << ": " << A[0]->name << " ";
			if(A[0]->start) cout << "STARTED" << endl;
			else cout << "ENDED" << endl;
			
			A[0] = A[size-1];
			size--;
			if(size <= 0) {	// printed the last element in schedule, exiting...
				cout << "TIME " << t << ": NO MORE EVENTS, SCHEDULER EXITS" << endl; 
				return;
			}
			minHeapify(A, size, 0);
			
			min = A[0]->time;
			
			no_event = false;
		}
		if(no_event) cout << "TIME " << t << ": " << "NO EVENT" << endl;
		
		t++;	// increment clock tick in each loop
	}

}


int main(int argc, char* argv[]){
	// check for the input file existence
	if(argc != 2){
		cerr << "Please provide an events input file!" << endl;
		return -1;
	}
	
	
	// declare neccessary variables
	vector<Event*> schedule;
	
	ifstream event_file;
	try {
		event_file.open(argv[1]);
		if(!event_file.is_open()) throw "couldnotopen";
	} catch(...) {
		cerr << "File could not be opened. exiting..." << endl;
		return -2;
	}
	
	string event_name;
	int starttime, endtime;
	
	
	// Read events file and fill schedule, the array of elements.
	while(event_file >> event_name >> starttime >> endtime){
		Event* e1 = new Event(event_name, starttime, true);
		Event* e2 = new Event(event_name, endtime, false);
		schedule.push_back(e1);
		schedule.push_back(e2);
	}
	
	
	// Sort the events in the array with respect to time,
	// and print events in schedule in ascending order of time.
	heapSortSchedule(schedule, schedule.size());
	
	return 0;
}










