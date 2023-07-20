/*
Given an array of integers nums and an integer target, return indices of the two numbers such 
that they add up to target.

You may assume that each input would have exactly one solution, and you may not use the same 
element twice.

You can return the answer in any order.

Example 1:

Input: nums = [2,7,11,15], target = 9
Output: [0,1]
Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].

Example 2:

Input: nums = [3,2,4], target = 6
Output: [1,2]

Example 3:

Input: nums = [3,3], target = 6
Output: [0,1]

*/

#include<bits/stdc++.h>

using namespace std;

// non funziona perché uso una versione di c++98, ma se uso la c++11/14/17/20, funziona;
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        map<int, int> store;
        
        for(int i=0;i<nums.size();i++){
            int difference = target - nums[i];
            if(store.count(difference)){
                return vector<int> {store[difference], i};
            }
            store[nums[i]] = i;
        }
        
        
        return vector<int> {};
    }       
};

int main(){
	// Dichiarazione variabili
	Solution ob;
	vector<int> nums;
	
	
	//Riempo il vettore per il caso test e assegno a target un numero
	nums.push_back(2);
	nums.push_back(7);
	nums.push_back(11);
	nums.push_back(15);
	
	int target = 9;
	
	// chiamo la funzione dell'oggetto Solution e assengno il risultato ad un altro vettore.
	vector<int> ris = ob.twoSum(nums, target);
	
	
	// stampo il risultato ricevuto
	cout<<ris[0] << " "<<ris[1];
	
	
	return 0;
}

