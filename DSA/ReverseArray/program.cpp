/*You are given a string s. You need to reverse the string.*/

//{ Driver Code Starts
//Initial Template for C++


#include<bits/stdc++.h>
using namespace std;


string reverseWord(string str);


int main() {
	
	int t;
	cin>>t;
	while(t--)
	{
	string s;
	cin >> s;
	
	cout << reverseWord(s) << endl;
	}
	return 0;
	
}


// } Driver Code Ends


//User function Template for C++

string reverseWord(string str){
    int first = 0;
    int last = str.size()-1;
    
    
    while(first<last){
        int temp = str[first];
        str[first] = str[last];
        str[last] = temp;
        --last;
        ++first;
    }
    
    return str;
}

