/*
You are given the heads of two sorted linked lists list1 and list2.

Merge the two lists in a one sorted list. The list should be made by splicing together 
the nodes of the first two lists.

Example 1:
Input: list1 = [1,2,4], list2 = [1,3,4]
Output: [1,1,2,3,4,4]

Example 2:
Input: list1 = [], list2 = []
Output: []

Example 3:
Input: list1 = [], list2 = [0]
Output: [0]
 
Constraints:
The number of nodes in both lists is in the range [0, 50].
-100 <= Node.val <= 100
Both list1 and list2 are sorted in non-decreasing order.

*/

#include<bits/stdc++.h>

using namespace std;

struct ListNode {
	int val;
	ListNode *next;
	ListNode() : val(0), next(nullptr) {}
	ListNode(int x) : val(x), next(nullptr) {}
	ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode *mergedList;
        ListNode *head;
        
        if(list1 == NULL){
            return list2;
        }
        
        if(list2 == NULL){
            return list1;
        }
        
        if(list1->val < list2->val){
            mergedList = list1;
            list1 = list1->next;
        } else {
            mergedList = list2;
            list2 = list2->next;
        }
        
        head = mergedList;
        
        
        while(list1 != NULL && list2 != NULL){
            if(list1->val < list2->val){
                mergedList->next = list1;        
                list1 = list1->next;
            } else {
                mergedList->next = list2;  
                list2 = list2->next;
            }
            mergedList = mergedList->next;
        }
        
        if(list1!=NULL)
            mergedList->next = list1;
        else 
            mergedList->next = list2;
        
        return head;
    }
};

int main(){
	
	return 0;
}

