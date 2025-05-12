//JeremyArias
//02/10/2021
//Prof. Bassel
//Assignment 3

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

int main(int argc, char *argv []) {
	istream *in = &cin;
	ifstream file;
	string line;
	map<char, int> freq;
	int maxdigit = 0, maxl = 0, tiearg = 0, nonalpha = -1;
	string argletter, argdigit;

	if (argc >= 2) {
		file.open(argv[1]);

	} else {
		string name;
		cin>>name;
		file.open(name);
	}
	if(file.is_open()) {
				in = &file;
				while (!file.eof()) {
					getline(*in, line);
					for (int i = 0; i < line.length(); i++) {
						freq[toupper(line[i])]++;
					}
				}
				for(auto it : freq) {
					if (isdigit(it.first)) {
						if (it.second > maxdigit) {
							if (it.second == maxdigit) {
								tiearg = it.first;
							}
							argdigit = it.first;
							maxdigit = it.second;
						}
					}
					if (isalpha(it.first)) {
						if (it.second > maxl) {
							argletter = it.first;
							maxl = it.second;
						}
					}
					if (!isdigit(it.first) && !isalpha(it.first)) {
						nonalpha++;
					}
				}
            if(freq.empty()){
					cout<<"File is empty."<<endl;exit(1);
			}
				if (tiearg == maxdigit) {
					if (maxdigit == 0 ) {
						cout << "Letters of Largest number of occurrences: " << argletter << ": " << maxl << '\n';
						cout << "List of characters seen in the file and their number of occurrences:" << '\n';
						bool isFirst = true;
						for (auto i : freq) {
							if(isFirst){
								isFirst = false;
								continue;
							}	
						cout <<i.first << ": " << i.second << '\n';
						}
						cout << "Number of non-alphanumeric characters seen in the file: " << nonalpha << '\n';	
					} else {
						cout << "Digits of Largest number of occurrences: " << argdigit << ": " << maxdigit << '\n';
						cout << "Digits of Largest number of occurrences: " << tiearg << ": " << maxdigit << '\n';
						cout << "Letters of Largest number of occurrences: " << argletter << ": " << maxl << '\n';
						cout << "List of characters seen in the file and their number of occurrences:" << '\n';
						bool isFirst = true;
						for (auto i : freq) {
							if(isFirst){
								isFirst = false;
								continue;
							}
							cout <<i.first << ": " << i.second << '\n';
						}
						cout << "Number of non-alphanumeric characters seen in the file: " << nonalpha << '\n';
					}
				} else {
					if (maxdigit == 0) {
						cout << "Letters of Largest number of occurrences: " << argletter << ": " << maxl << '\n';
						cout << "List of characters seen in the file and their number of occurrences:" << '\n';
						bool isFirst = true;
						for (auto i : freq) {
							if(isFirst){
								isFirst = false;
								continue;
						}	
						cout <<i.first << ": " << i.second << '\n';
						}
						cout << "Number of non-alphanumeric characters seen in the file: " << nonalpha << '\n';	
					} else if (maxl == 0) {
						cout << "Digits of Largest number of occurrences: " << argdigit << ": " << maxdigit << '\n';
						cout << "List of characters seen in the file and their number of occurrences:" << '\n';
						bool isFirst = true;
						for (auto i : freq) {
							if(isFirst){
								isFirst = false;
								continue;
							}
							cout <<i.first << ": " << i.second << '\n';
						}
						cout << "Number of non-alphanumeric characters seen in the file: " << nonalpha << '\n';
					} else {
						cout << "Digits of Largest number of occurrences: " << argdigit << ": " << maxdigit << '\n';
						cout << "Letters of Largest number of occurrences: " << argletter << ": " << maxl << '\n';
						cout << "List of characters seen in the file and their number of occurrences:" << '\n';
						bool isFirst = true;
						for (auto i : freq) {
							if(isFirst){
								isFirst = false;
								continue;
							}
							cout <<i.first << ": " << i.second << '\n';
						}
						cout << "Number of non-alphanumeric characters seen in the file: " << nonalpha << '\n';
					}
				}

			} else {
				cerr << "File cannot be opened: " <<argv[1]<<endl; exit(1);
			}
}
