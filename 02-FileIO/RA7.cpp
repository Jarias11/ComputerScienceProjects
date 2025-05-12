#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <queue>
#include <stack>

using namespace std;

bool inLanguage( const string &str )
{
        stack<char> st ;
        queue<char> qu ;
        
        int len1 = 0 , len2 = 0 ;
        
        int state = 0 ;
        for( int i=0 ; i < str.size(); i++ )
        {
                if  ( str[i] == '$' )
                {
                        if ( ( state == 0 )  )
                        state = 1 ;
                        else 
                        {
                                cout << "Multiple $ found " << str[i] << " state " << state << " \n";
                                return false ;
                        }
                } else {
                        if ( state == 0 )
                        {
                                st.push(str[i]);
                                len1 += 1;
                        }
                        else 
                        {
                                qu.push(str[i]);
                                len2 +=1 ;
                        }
                }
        }
        if ( state == 0 )
        {
                return 0 ;
        }
        
        if ( st.size() == qu.size() )
        {
                while ( !st.empty() )
                {
                        if ( st.top() != qu.front() )
                        {
                                cout << " s != s' ,  " << st.top() << " != " << qu.front() << "\n" ;
                                return false ;
                        } else 
                        {
                                st.pop();
                                qu.pop();
                        }
                }
                return true ;
        }
        else {
               // cout << " --- length(s) != length(s') \n";
                return false ;
        }
        
}

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
        if (sentence.empty()){
            cout << "Empty string is not in the Language."<< endl;
        }
        else {
            if (inLanguage(sentence) == 0 && !sentence.empty())
            {
                cout << "string is missing $." << endl;
            }
       	    if (isInLanguage)
       	    {
       		    cout << sentence << " is a sentence in the Language." << endl;
		    }
		    else
		    {
			    cout << sentence << " is not a sentence in the Language." << endl;
		    }
        }
		getline(file, sentence); 
	}
}
