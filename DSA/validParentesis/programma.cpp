/*
Given a string s containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input 
string is valid.

An input string is valid if:

Open brackets must be closed by the same type of brackets.
Open brackets must be closed in the correct order.
Every close bracket has a corresponding open bracket of the same type.
 

Example 1:
Input: s = "()"
Output: true

Example 2:
Input: s = "()[]{}"
Output: true

Example 3:
Input: s = "(]"
Output: false
 

Constraints:
1 <= s.length <= 104
s consists of parentheses only '()[]{}'.
*/

#include<bits/stdc++.h>

using namespace std;

class Solution {
public:
    bool isValid(string s) {
        if(s.size()%2!=0)
            return false;
            
        stack<char> stk;
        
        for(int i=0;i<s.size();i++){
            if(s[i] == '(')
                stk.push(')');
            else if(s[i] == '[')
                stk.push(']');
            else if(s[i] == '{')
                stk.push('}');
            else if(stk.empty() || stk.top() != s[i])
                return false;
            else 
                stk.pop();
        }
        
        return stk.empty();
    }
};

int main(){
	Solution ob;
	string s = "()";
	
	bool ris = ob.isValid(s);
	
	cout<<ris;
	
	return 0;	
}

