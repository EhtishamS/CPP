#include<bits/stdc++.h>
using namespace std;


class Solution
{
    public:
    void sort012(int a[], int n)
    {
        int countZ = 0, countO = 0, countT = 0;
        
        for(int i=0;i<n;i++){
            if(a[i] == 0){
                countZ++;
            } else if(a[i] == 1){
                countO++;
            } else {
                countT++;
            }
        }
        
        int i = 0;
        
        while(countZ>0){
            a[i] = 0;
            i++;
            countZ--;
        }
        
        while(countO>0){
            a[i] = 1;
            i++;
            countO--;
        }
        
        while(countT>0){
            a[i] = 2;
            i++;
            countT--;
        }
    }
    
};


int main() {

    int t;
    cin >> t;

    while(t--){
        int n;
        cin >>n;
        int a[n];
        for(int i=0;i<n;i++){
            cin >> a[i];
        }

        Solution ob;
        ob.sort012(a, n);

        for(int i=0;i<n;i++){
            cout << a[i]  << " ";
        }

        cout << endl;
        
        
    }
    return 0;
}
