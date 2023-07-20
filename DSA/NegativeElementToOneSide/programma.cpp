/*
Move all negative numbers to beginning and positive to end with constant extra space
*/

#include<bits/stdc++.h>

using namespace std;

class Solution{
	public:
		void sortNegative(vector<int>& nums){
			int first = 0;
			int last = nums.size()-1;
			
			while(first<last){
				if(nums[first] < 0 || nums[last] > 0){
					if(nums[first] < 0){
						first++;
					}
					
					if(nums[last] > 0){
						last--;
					}
				} else{
					int temp = nums[first];
					nums[first] = nums[last];
					nums[last] = temp;
				}
				
			}
			
		}
};

int main(){
	Solution ob;
	vector<int> nums;
	
	nums.push_back(-12);
	nums.push_back(11);
	nums.push_back(-13);
	nums.push_back(-5);
	nums.push_back(6);
	nums.push_back(-7);
	nums.push_back(5);
	nums.push_back(-3);
	nums.push_back(-6);
	
	ob.sortNegative(nums);
	
	for(int i=0;i<nums.size();i++){
		cout<<nums[i]<<" ";
	}
	
	return 0;
}
