#ifndef TEST_H
#define TEST_H

#include <string>

class Test{
    private:
        std::string s;
    public:
        Test(std::string s){
            this->s = s;
        }

        std::string getString(){
            return s;
        }
};

#endif