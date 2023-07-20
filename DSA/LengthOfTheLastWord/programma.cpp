/*
Given a string s consisting of words and spaces, return the length of the last word in the string.

A word is a maximal substring consisting of non-space characters only.

Example 1:
Input: s = "Hello World"
Output: 5
Explanation: The last word is "World" with length 5.

Example 2:
Input: s = "   fly me   to   the moon  "
Output: 4
Explanation: The last word is "moon" with length 4.

Example 3:
Input: s = "luffy is still joyboy"
Output: 6
Explanation: The last word is "joyboy" with length 6.
 
Constraints:
1 <= s.length <= 104
s consists of only English letters and spaces ' '.
There will be at least one word in s.
*/
#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int lengthOfLastWord(string s) {
        int start = -1;
        int end = start;
        
        for(int i=s.size()-1;i>=0;i--){
            if(start != -1 && s[i] == ' '){
                end = i;
                break;
            }
            
            if(s[i] != ' ' && start == end){
                start = i;
            }
        }
        
        
        return start-end;
    }
};

int main(){
	Solution ob;
	string s = "luffy is still joyboy";
	
	
	int length = ob.lengthOfLastWord(s);
	
	cout<<length;
	
	return 0;
}
