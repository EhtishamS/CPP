#include "Test.h"

Test::Test(std::string s){
    this->s = s;
}

std::string Test::getString(){
    return s;
}