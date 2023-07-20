/*
Write a function to find the longest common prefix string amongst an array of strings.

If there is no common prefix, return an empty string "".

Example 1:

Input: strs = ["flower","flow","flight"]
Output: "fl"

Example 2:

Input: strs = ["dog","racecar","car"]
Output: ""
Explanation: There is no common prefix among the input strings.
 
Constraints:

1 <= strs.length <= 200
0 <= strs[i].length <= 200
strs[i] consists of only lowercase English letters.
*/

#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        string prefix = strs[0];
        string newPre = "";
       
        if(strs.size()==1)
            return prefix;
            
        
        for(int i=1;i<strs.size();i++){
            for(int j=0;j<strs[i].size();j++){
                if(prefix[j] == strs[i][j]){
                    newPre += strs[i][j];
                } else {
                    break;
                }
            }
            
            prefix = newPre;
            newPre.clear();
        }
       
        return prefix;
    }
};


int main(){
    Solution ob;
    vector<string> strs;

    strs.push_back("flower");
    strs.push_back("flow");
    strs.push_back("flight");

    string prefix = ob.longestCommonPrefix(strs);

    cout<<prefix;

    return 0;
}
