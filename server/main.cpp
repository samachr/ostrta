//TODO: remove memory leak caused by returns from queries
//TODO: allow multiple queries as input from user (stringstream + getline?)

//TODO: MAKE IT AWESOME!
//nodejs integration
//awesome web design

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;

//a linked list of documents that a word occurs on
class Page
{
public:
	Page *next;
	int documentNum;
	int frequency;

	Page(int d)
	{
		next = NULL;
		frequency = 1;
		documentNum=d;
	}
};

//to store a word and its list of pages (next pointer for separate chaining)
class Record
{
public:
	string word;
	Page *pages;
	Record *next;

	Record()
	{
		next = NULL;
	}

	Record(string s, int docNum)
	{
		word = s;
		pages=new Page(docNum);

		next = NULL;
	}

	~Record(){
		Page*p, *temp;
		p=pages;
		while(p!=NULL)
		{
			temp=p;
			p=p->next;
			delete temp;
		}
	}

};

void deletePages(Page*p) {
	Page *temp;
	while(p!=NULL)
	{
		temp=p;
		p=p->next;
		delete temp;
	}
}


//a dynamically allocated hash table for storing records
class Table
{
private:
	Record **entries;
	int N;

public:
	Table(int n)
	{
		this->N = n;
		entries = new Record *[N];
		for (int i = 0; i < N; i++)
		{
			entries[i] = NULL;
		}
	}
	~Table()
	{
		for (int i = 0; i < N; i++)
		{
			if (entries[i] != NULL)
				delete entries[i];
		}
		delete[] entries;

	}

	void add(string word, int docNum)
	{
		int h = hashCode(word);
		Record * temp;
		bool found = false;

		//IF THE SLOT IS EMPTY MAKE A NEW POINTER AND ASSIGN IT
		if (entries[h] == NULL)
		{
			Record *c = new Record(word, docNum);
			entries[h] = c;
		}
		else
		{
			//LOOK FOR THE WORD AND INCREASE THE FREQUENCY
			for (temp = entries[h]; temp != NULL && found == false; temp = temp->next)
			{
				if(temp->word==word)//CHECK IF IT IS THE SAME WORD
				{
					found=true;
					if (temp->pages->documentNum == docNum)//CHECK IF IT IS IN THE SAME DOCUMENT
					{
						temp->pages->frequency ++;
					}
					else//IF IT DOESN'T EXIST IN THE DOCUMENT, GIVE IT A NEW PAGE
					{
						Page* temp2 = new Page(docNum);
						temp2->next = temp->pages;
						temp->pages = temp2;
					}

				}
			}
			if(!found)//IF THE WORD DOESN'T EXIST, ADD A NEW WORD
			{
				Record* temp2 = new Record(word,docNum );
				temp2->next = entries[h];
				entries[h] = temp2;
			}
		}
	}

	int print()
	{
		ofstream fout("output.txt");
		int longestList = 0;
		int unique = 0;
		int nonEmpty = 1;

		for (int n = 0; n < N; n++)//THIS WILL GO THROUGH EACH ARRAY POSITION
		{
			int length = 0;
			if (entries[n] != NULL)
			{
				nonEmpty++;

				//GO THROUGH THE LINKED LIST AND PRINT OUT THE WORDS
				for (Record* p = entries[n]; p != NULL; p = p->next)
				{
					fout << p->word << " ";
					length++;
				}
			}
			fout << endl;
			if (length > longestList) {
				longestList = length;
			}
			unique += length;
		}
		
		

		fout << unique << " unique words." << endl;
		fout << "average length("<<unique<<"/"<<nonEmpty<<"): " << (double)unique / nonEmpty  << endl;
		fout.close();
		return longestList;
	}


	int hashCode(string s){
		int hash=0;

		for (unsigned int i = 0; i < s.length(); i++)
		{
			hash += s[i];
		}

		hash += s.length()*3;
		hash += (hash / N)*3;
		hash = hash % N;

		return hash;
	}

	Page* query(string word) {
		Record* hashLocation = entries[hashCode(word)];
		for( ; hashLocation != NULL; hashLocation = hashLocation->next) {
			if (hashLocation->word == word) {
				return hashLocation->pages;
			}
		}
		return NULL;
	}

	Page* andQuery(Page* left, Page* right) {
		Page* result = NULL;
		while (left != NULL && right != NULL) {
			if (left->documentNum == right->documentNum) {
				Page* temp = new Page(left->documentNum);
				temp->frequency = left->frequency;
				temp->next = result;
				result = temp;
				left = left->next;
				right = right->next;
				//move both
			}
			else if (left->documentNum < right->documentNum) {
				//move right
				right = right->next;
			}
			else if (left->documentNum > right->documentNum) {
				//move left
				left = left->next;
			}
		}
		Page* reversedResult = NULL;
		Page* p = result;
		while (p != NULL) {
			//reversedResult = p;
			p = p->next;
			//put result into reverse result at front
			result->next = reversedResult;
			reversedResult = result;

			result = p;
		}
		return reversedResult;
	}

	Page* orQuery(Page* left, Page* right) {
		Page* result = NULL;
		while (left != NULL && right != NULL) {
			if (left->documentNum == right->documentNum) {
				//found a match, add to list
				//create new
				Page* temp = new Page(left->documentNum);
				temp->frequency = left->frequency;
				temp->next = result;
				//put it at front
				result = temp;
				left = left->next;
				right = right->next;
				//move both
			}
			else if (left->documentNum < right->documentNum) {
				Page* temp = new Page(right->documentNum);
				temp->frequency = right->frequency;
				temp->next = result;
				//put it at front
				result = temp;
				//move right
				right = right->next;
			}
			else if (left->documentNum > right->documentNum) {
				//move left
				Page* temp = new Page(left->documentNum);
				temp->frequency = left->frequency;
				temp->next = result;
				//put it at front
				result = temp;
				left = left->next;
			}
		}
		while (left != NULL) {
			//move left
			Page* temp = new Page(left->documentNum);
			temp->frequency = left->frequency;
			temp->next = result;
			//put it at front
			result = temp;
			left = left->next;
		}
		while (right != NULL) {
			Page* temp = new Page(right->documentNum);
			temp->frequency = right->frequency;
			temp->next = result;
			//put it at front
			result = temp;
			//move right
			right = right->next;
		}
		Page* reversedResult = NULL;
		Page* p = result;
		while (p != NULL) {
			//reversedResult = p;
			p = p->next;
			//put result into reverse result at front
			result->next = reversedResult;
			reversedResult = result;

			result = p;
		}
		return reversedResult;
	}
};

int main() {

	Table indexHashTable(499);

	string baseFileName = "./documents/cranfield0001";
	
	int wordCount = 0;

	//cout << "Reading documents";
	for (int i = 1; i <= 50; i++) {
		//cout << " " << i;
		if(i<10) {
			baseFileName[23] = '0';
			baseFileName[24] = (char)(i+48);

		} else {
			baseFileName[23] = (char)((i/10)+48);//10's place
			baseFileName[24] = (char)((i%10)+48);//one's place
		}
		//baseFileName[2] = '\0';
		//cout << baseFileName << endl;
		ifstream fin(baseFileName.c_str());
		string currentWord;
		while(fin >> currentWord) {
			if (currentWord[0] != '<' && currentWord[0] != '.') {
				if (currentWord[currentWord.length()-1] == ',') {
					indexHashTable.add(currentWord.substr(0, currentWord.length() - 2), i);
					wordCount++;
				}else {
					indexHashTable.add(currentWord, i);
					wordCount++;
				}
			}
		}
		fin.close();
	} cout << endl;
	cout << "OSTRTA document searcher. Enter EOF to quit" << endl;
	cout << "Boolean logic operators AND and OR allowed" << endl;
	cout << "Ready for queries" << endl << flush;

	string rawLine;
	stringstream rawInput;

	//cout << "> ";
	getline(cin, rawLine);

	while (rawLine != "EOF") {
		rawInput.clear();
		rawInput << rawLine;
		//cout << "searching for " << rawLine << endl;
			
		Page* searchResults = NULL;

		string query;
		while (rawInput >> query) {
			if (query == "AND") {
				rawInput >> query;
				searchResults = indexHashTable.andQuery(searchResults, indexHashTable.query(query));
			} else if (query == "OR") {
				rawInput >> query;
				searchResults = indexHashTable.orQuery(searchResults, indexHashTable.query(query));
			} else if (searchResults == NULL && query != "EOF") {
				searchResults = indexHashTable.query(query);
			}
		}

		if (searchResults == NULL) {
			cout << "not found" << endl;
		}
		else {
			//cout << rawLine << " was found in documents: " << endl;
			for (Page* p = searchResults; p != NULL; p = p->next) {
				cout << p->documentNum << " ";
				//cout << p->documentNum << " " << p->frequency << " times\n";			
			}
			cout << endl;
			//deletePages(searchResults);
		}
		//cout << "> ";
		getline(cin, rawLine);
	} 

	return 0;
}
