//Milton Flores
//02/10/2021
//Prof. Bassel
//Assignment 3

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char *argv []) {
	istream *in = &cin;
	ifstream file;
//	ofstream outClientFile("cients.dat", ios::out);
	int blank_lines = 0, non_blank_lines = 0, characters = 0, words = 0, alchar = 0, digits = 0;
	if (argc >= 2) {
		file.open(argv[1]);
		if(file.is_open()) {
			in = &file;
			
			while(!file.eof()){
				getline(file,line);
				istringstream mystring(line);
				string w1;
					while (mystring >> w1){
						for (int i=0; i < w1.length(); i++){
							if (isdigit(w1[i])) {
								digits++;
								characters++;
							}
						} else if (isalpha(w1[i]) {
							characters++;
							alchar++;
						} else {
							characters++;
						}
						words++;
					}
				if(line.length() == 0) {
					blank_lines++;
				} else {
					non_blank_lines++;
				}
				total_lines = blank_lines + non_blank_lines;
		}
		} else {
			cerr << "File could not be opened" << endl; exit(1);
		}
	} else {
		cout << "Enter the account, name, and balance." << endl;
	}
}

