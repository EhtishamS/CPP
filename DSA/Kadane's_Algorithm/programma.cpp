#include<bits/stdc++.h>
using namespace std;

class Solution{
    public:
    long long maxSubarraySum(int arr[], int n){
        long long maxi = 0;
        long long max_final = 0;
        
        for(int i=0;i<n;i++){
        	maxi = max(arr[i]+maxi, max_final);
        	max_final = max(maxi, max_final);
		}
		
		return max_final;
    }
};
int main()
{
    int t,n;
    
    cin>>t;
    while(t--)
    {
        
        cin>>n;
        
        int a[n];
        
        for(int i=0;i<n;i++)
            cin>>a[i];
            
        Solution ob;
        
        cout << ob.maxSubarraySum(a, n) << endl;
    }
}

// } Driver Code Ends
