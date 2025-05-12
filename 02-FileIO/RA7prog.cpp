#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <queue>
#include <stack>
using namespace std;
bool inLanguage(string &inputString);

int main(int argc, char *argv [])
{
	string sentence;
	bool isInLanguage;
	int Lcount=0;
	
	ifstream file;
	if( argc >= 2 ) {
		file.open(argv[1]);
		if( !file) 
		{
			/* error */ 
			cerr << "File cannot be opened: " << argv[1] << endl;
			exit (1);
        }
				
	}
	else
	{
		cerr << "No file is found" << endl;
		exit (1);
	}
	
        
	getline(file, sentence);
       
    while ( !file.eof()) {
       	Lcount++;
       	isInLanguage = inLanguage(sentence);
       	if (isInLanguage)
       	{
       		cout << sentence << " is a sentence in the Language." << endl;
		}
		else
		{
            if(sentence.length() != 0)
			    cout << sentence << " is not a sentence in the Language." << endl;
		}
		 	
		getline(file, sentence); 
	}
    return 0;
}

bool inLanguage( string &str )
{
          if(str.empty()) 
   {
       cout<<"Empty string is not in the Language."<<endl;
       return false;
       }
queue <char> S;//queue S using STL
stack <char> s;//Stack s using STL to Store the S' part of the strng
int n = str.size();
int i=0,j=n-1;//markers to traverse the string
if(!(str.find("$") != string::npos)){
cout<<"string is missing $."<<endl;
return false;}


while(i!=j) 
{
if(str[i]!=str[j]) return false;
S.push(str[i]);//storing S part in queue
s.push(str[j]);//stroring the S' part in stack
i++;
j--;
}
return true;
}