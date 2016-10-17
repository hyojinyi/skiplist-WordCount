#include <iostream>
#include <fstream>
#include <sstream>
#include <string>       // provides string class
#include <iterator>     // able to go through the vector
#include <cctype>       // provides isalpha() and tolower()
#include <ctime>        // for time()
#include <cstdlib>      // for rand(), srand(), and RAND_MAX
#include <vector>       // provides vector structure
#include <math.h>		// for ceil function
#include <algorithm>	// for sort(), unique(), distance()
#include <stdio.h>
#include <string.h>

using namespace std;

// milestone #1
struct indexWord {
	string word;
	int occurrence;
	vector<int> listOfPage;
	
	// indexWord constructor
	indexWord(string w) {
		word = w;
		occurrence = 0;
	}
};

const int MAX_HEIGHT = 16; // maximum height of SkipList

// milestone #2
struct SkipList {

	struct Node {
		indexWord* index;
		int height;
		Node *next[MAX_HEIGHT];
	};
	
	Node* head; // beginning of the SkipList
	
	// SkipList constructor
	SkipList() {	
		head = new Node;
		head->index = NULL;
		head->height = MAX_HEIGHT;
	}
	
	// function declarations
	indexWord* find(string word);
	indexWord* insert(string word);
	void printInOrder(ostream &output);
};

indexWord* SkipList::find(string findWord) {
	Node* node = head;
	int height = MAX_HEIGHT - 1;
	while (height >= 0) {
		Node* findIndex = node->next[height];
		while (findIndex != NULL) {
			if (findIndex->index->word < findWord) {
				node = findIndex;
				findIndex = node->next[height];
			}
			
			// if Node with word is found, return that index
			else if (findIndex->index->word == findWord)
				return findIndex->index;
				
			else 
				break;
		}
		
		// go down a level	
		height--;
	}
	
	return NULL;
}

int randBit(void) { // return a "random" bit
	static int bitsUpperBd=0;
	static int bits; // store bits returned by rand()
	if (bitsUpperBd == 0) { // refresh store when empty
		bitsUpperBd = RAND_MAX;
		bits = rand();
	}
	
	int b = bits & 1;
	bits >>= 1;
	bitsUpperBd >>= 1;
	return b;
}

int random_Height() {
	int ht = 1;
	for (int i = 1; i < MAX_HEIGHT; i++) {
		if (randBit() == 1) {
			ht = i;
			break;
		}
	}
	
	return ht;
}

indexWord* SkipList::insert(string insertWord) {
	Node* node = head;
	Node* update[MAX_HEIGHT];
	int height = MAX_HEIGHT - 1;
	while (height >= 0) {
		Node* insertIndex = node->next[height];		
		while (insertIndex != NULL) {
			if (insertIndex->index->word < insertWord) {
				node = insertIndex;
				insertIndex = node->next[height];
			}
			
			// if Node with word is found, return that index
			else if (insertIndex->index->word == insertWord)
				return insertIndex->index;
				
			else
				break;
		}
		
		update[height] = node;
		
		// go down a level
		height--;
	}
	
	// create new word if it was not found
	Node* newNode = new Node;
	newNode->index = new indexWord(insertWord);
	newNode->height = random_Height();

	// adding pointers to newNode from update
	for (int i = 0; i < newNode->height; i++) {
		newNode->next[i] = update[i]->next[i];
		update[i]->next[i] = newNode;
	}
	
	return newNode->index;	
}

// print method was given
void print(ostream &output, string &word) {
	output << word << endl;
}

// milestone #1 printVector helper function
/*
string printVector(vector<int> lop){
	stringstream ss;
	copy(lop.begin(), lop.end(), ostream_iterator<int>(ss, ", "));
	
	string s = ss.str();
	s = s.substr(0, s.length()-2); // gets rid of the last comma
	return s;
}
*/

// milestone #2 printVector helper function
// takes listOfPage and produces a string condensing consecutive numbers
string printVector(vector<int> lop) {
	stringstream ss;
	int a = *lop.begin(); // first page word is on
	int b = *lop.begin(); // last page word is on
	vector<int>::iterator it;
	for (it = lop.begin(); it != lop.end(); it++) {
		if (*it != a) {
			if (*it != b + 1) {
				if (a == b)
					ss << a << ", ";
					
				else {
					ss << a << "-" << b << ", ";
                }
				
				b = a = *it;
            }
			
			else
				b++;
        }
    }
	
    if (a == b)
		ss << a;
	
    else {
        ss << a << "-" << b;
	}
	
    return ss.str();
}

// remove duplicate it numbers
vector<int> removeDuplicates(vector<int> lop) {
	vector<int>::iterator it;
	it = unique(lop.begin(), lop.end());
	
	lop.resize(distance(lop.begin(), it));
	return lop;
}

// milestone #1 printInOrder function
/*
void printInOrder(ostream &output) {
	stringstream ss;
	vector<indexWord>::iterator it;
	for (it = input.begin(); it != input.end(); it++) {
		vector<int> listToPrint = removeDuplicates(it->listOfPage);
		ss << it->word << " (" << it->occurrence << ") " << printVector(listToPrint) << endl; 
    }
	
	string s = ss.str();
	print(output, s);
}
*/

// milestone #2 printInOrder function
void SkipList::printInOrder(ostream &output) {
	stringstream ss;
	Node* node = head->next[0];
	while (node) {
		vector<int> lop = node->index->listOfPage;
		vector<int> listToPrint = removeDuplicates(lop);
		ss << node->index->word << " (" << node->index->occurrence << ") " << printVector(listToPrint) << endl;
		node = node->next[0];
	}
	
	string s = ss.str();
	print(output, s);
}

// method to determine the it of a line
// assuming every it is 40 lines starting from line 1
int findPage (int line) {
	return (line / 41) + 1;
}

// Remove all characters except letters (A-Z,a-z) from line,
// except keep '-' or '\'' if they are between letters.
void lowercaseWords(string & line) {
	for( string::iterator it = line.begin(); it != line.end(); ++it ) {
		if( !isalpha(*it) ) {
			if( (*it != '-' && *it != '\'') || it == line.begin() || it+1 == line.end() || !isalpha(*(it-1)) || !isalpha(*(it+1)) )
					*it = ' ';
		}
		
		else
			*it = tolower(*it);
	}
}

int main (int argc, char *argv[]) {
	// incorrect number of inputs -> EXIT
	if (argc != 3) {
		cerr << "Usage: " << argv[0] << " book.txt index.txt" << endl;
		exit(1);
	}
	
	// input text unable to open -> EXIT
	ifstream fin;
	fin.open(argv[1]);
	if (!fin.is_open()) {
		cerr << "Unable to open " << argv[1] << endl;
		exit(2);
	}
	
	// output text unable to open -> EXIT
	ofstream fout;
	fout.open(argv[2]);
	if (!fout.is_open()) {
		cerr << "Unable to open " << argv[2] << endl;
		exit(3);
	}
	
	// all inputs pass
	string line, word;
	int count = 1; // line counter
	SkipList sl;
	while (!fin.eof()) {
		getline(fin,line);
		lowercaseWords(line);
		
		istringstream iss(line, istringstream::in);
		count++;
    	int currentPage = findPage(count);
		while (iss >> word) {
			if (sl.find(word) == NULL) {
				sl.insert(word); // if word doesn't exist, insert it
			}
		
			// adds it number to listOfPage
			sl.find(word)->listOfPage.push_back(currentPage);
		
			// if word exists, occurrence + 1
			sl.find(word)->occurrence++;
        }
    }
	
	sl.printInOrder(fout);
}

