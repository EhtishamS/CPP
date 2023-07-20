#include <iostream>
#include <string>
#include "Test.h"

using namespace std;

int main(){
    string s="HELLO WORLD!";
    Test test(s);

    cout << test.getString() <<"\n";

    return 0;
}