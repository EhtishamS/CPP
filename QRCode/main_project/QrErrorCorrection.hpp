#ifndef QRERRORCORRECTION_H
#define QRERRORCORRECTION_H

#include "Polynomial.hpp"

class QrErrorCorrection{
    private:
        QrCodeMaterial *material;
        QrInfo *info;
        QrString *str; 
        Polynomial* p;
        
        int total_codewords;
        int ec_codewords_per_block;
        int first_group_blocks_number;
        int first_block_size;
        int second_group_blocks_number;
	    int second_block_size;
        
        std::string polynomial_generator;
        std::string* first_group_message_polinomial;
        std::string* second_group_message_polinomial;
        std::string* first_group_error_correction_codewords;
        std::string* second_group_error_correction_codewords;
        std::string interleave_message_blocks;
        std::string interleave_error_code_blocks;
        std::string final_string;

        // setting parameters variables
        void set_total_codewords_required(){
            total_codewords = material->getCodeWordInfo(info->getVersion(), info->getLevel(), TOTAL_CODEWORDS);
        }

        void set_ec_codewords_per_block(){
            ec_codewords_per_block = material->getCodeWordInfo(info->getVersion(), info->getLevel(), EC_CODEWORDS_PER_BLOCK);
        }

        void set_first_group_blocks_number(){
            first_group_blocks_number = material->getCodeWordInfo(info->getVersion(), info->getLevel(), FIRST_GROUP_BLOCK_SIZE);
            if(first_group_blocks_number > 0){
                first_group_message_polinomial = new std::string[first_group_blocks_number];
                first_group_error_correction_codewords = new std::string[first_group_blocks_number];
            } else {
                first_group_message_polinomial = NULL;
                first_group_error_correction_codewords = NULL;
            }
        }

        void set_first_block_size(){
            first_block_size = material->getCodeWordInfo(info->getVersion(), info->getLevel(), FIRST_BLOCK_SIZE);
        }

        void set_second_group_blocks_number(){
            second_group_blocks_number = material->getCodeWordInfo(info->getVersion(), info->getLevel(), SECOND_GROUP_BLOCK_SIZE);
            if(second_group_blocks_number>0){
                second_group_message_polinomial = new std::string[second_group_blocks_number];
                second_group_error_correction_codewords = new std::string[second_group_blocks_number];
            } else {
                second_group_message_polinomial = NULL;
                second_group_error_correction_codewords = NULL;
            }
        }

        void set_second_block_size(){
            second_block_size = material->getCodeWordInfo(info->getVersion(), info->getLevel(), SECOND_BLOCK_SIZE);
        }
        
        // setting codewords strings
        void set_polynomial_generator(){
            polynomial_generator = p->getPolynomialGenerator(ec_codewords_per_block);
        }

        void set_first_group_message_polinomial(){
            if(first_group_blocks_number > 0){
                int begin = 0;
                std::string binary_string = str->getStr();
                for(int i=0;i<first_group_blocks_number;i++){
                    first_group_message_polinomial[i] = str->getStr().substr(begin, first_block_size*8);
                    begin += first_block_size;
                }
            } else {
                first_group_message_polinomial = NULL;
            }
        }

        void set_second_group_message_polinomial(){
            if(second_group_blocks_number>0){
                int begin = 0;
                for(int i=0;i<second_group_blocks_number;i++){
                    second_group_message_polinomial[i] = str->getStr().substr(begin, second_block_size*8);
                    begin += second_block_size; 
                }
            } else {
                second_group_message_polinomial = NULL;
            }
        }
    
        void set_first_group_error_correction_codewords(){
            if(first_group_blocks_number > 0){
                for(int i=0;i<first_group_blocks_number;i++){
                    first_group_error_correction_codewords[i] = p->divisioneLunga(first_group_message_polinomial[i], polynomial_generator, first_block_size, ec_codewords_per_block);
                }
            } else {
                first_group_error_correction_codewords = NULL;
            }
        }
    
        void set_second_group_error_correction_codewords(){
            if(second_group_blocks_number > 0){
                for(int i=0;i<second_group_blocks_number;i++){
                    second_group_error_correction_codewords[i] = p->divisioneLunga(second_group_message_polinomial[i], polynomial_generator, second_block_size, ec_codewords_per_block);
                }
            } else {
                second_group_error_correction_codewords = NULL;
            }
        }
    
        void set_interleave_message_blocks(){
            // condizioni:
            // 1. se ho solo un blocco di dati nel primo gruppo e secondo è vuoto allora non devo fare nulla che assengare alla variabile
            // 2. se 
            std::string** matrix;
            int begin;
            int i, j;

            if(first_group_blocks_number == 1 && second_group_blocks_number == 0){
                interleave_message_blocks = *first_group_message_polinomial;
            } else if(first_group_blocks_number > 1 && second_group_blocks_number == 0) {
                begin = 0;
                for(i=0;begin<(first_block_size*8);i++){
                    j=0;
                    while(j<first_group_blocks_number){
                        interleave_message_blocks += first_group_message_polinomial[j].substr(begin, 8);    
                        j++;
                    }

                    begin+=8;
                }
            } else {
                begin = 0;
                for(i=0;begin<(first_block_size*8) && begin<(second_block_size*8);i++){
                    j=0;
                    while(j<first_group_blocks_number){
                        interleave_message_blocks += first_group_message_polinomial[j].substr(begin, 8);    
                        j++;
                    }
                    j=0;
                    while(j<second_group_blocks_number){
                        interleave_message_blocks += second_group_message_polinomial[j].substr(begin, 8);    
                        j++;
                    }
                    begin+=8;
                }

                while(begin<(first_block_size*8)){
                    j = 0;
                    while(j<first_group_blocks_number){
                        interleave_message_blocks += first_group_message_polinomial[j].substr(begin, 8);    
                        j++;
                    }
                    begin+=8;
                }

                while(begin<(second_block_size*8)){
                    j = 0;
                    while(j<second_group_blocks_number){
                        interleave_message_blocks += second_group_message_polinomial[j].substr(begin, 8);    
                        j++;
                    }
                    begin+=8;
                }
                
            }
        }
    
        void set_interleave_error_code_blocks(){
            int begin = 0;
            interleave_error_code_blocks = "";

            if(first_group_error_correction_codewords != NULL && second_group_error_correction_codewords == NULL){           
                for(int i=0;begin<(ec_codewords_per_block*8);i++){
                    int j = 0;
                    while(j<first_group_blocks_number){
                        interleave_error_code_blocks += first_group_error_correction_codewords[j].substr(begin, 8);
                        j++;
                    }
                    begin += 8;
                }
            } else if(first_group_error_correction_codewords != NULL && second_group_error_correction_codewords != NULL){
                for(int i=0;begin<(ec_codewords_per_block*8);i++){
                    int j = 0;
                    while(j<first_group_blocks_number){
                        interleave_error_code_blocks += first_group_error_correction_codewords[j].substr(begin, 8);
                        j++;
                    }
                    j=0;
                    while(j<second_group_blocks_number){
                        interleave_error_code_blocks += second_group_error_correction_codewords[j].substr(begin, 8);
                        j++;
                    }
                    begin += 8;
                }
            }   
        }
    
        void set_final_string(){
            final_string = interleave_message_blocks + interleave_error_code_blocks + DecimalTobinary(0, material->getRemainderForVersion(info->getVersion()));
        }
    public:
        QrErrorCorrection(QrCodeMaterial *material,QrInfo *info, QrString *str){
            this->material = material;
            this->info = info;
            this->str = str;
            p = new Polynomial(material, info, str);

            set_total_codewords_required();
            set_ec_codewords_per_block();
            set_first_group_blocks_number();
            set_first_block_size();
            set_second_group_blocks_number();
            set_second_block_size();
            set_polynomial_generator();
            set_first_group_message_polinomial();
            set_second_group_message_polinomial();
            set_first_group_error_correction_codewords();
            set_second_group_error_correction_codewords();
            set_interleave_message_blocks();
            set_interleave_error_code_blocks();
            set_final_string();
        }

        int getTotalCodewordsRequired(){
            return total_codewords;
        }

        int getEcCodewordsRequired(){
            return ec_codewords_per_block;
        }
        
        int getFirstGroupBlocksNumber(){
            return first_group_blocks_number;
        }

        int getFirstBlockSize(){
            return first_block_size;
        }

        int getSecondGroupBlocksNumber(){
            return second_group_blocks_number;
        }

        int getSecondBlockSize(){
            return second_block_size;
        }

        std::string getPolinomialGenerator(){
            return polynomial_generator;
        }

        std::string *getFirstMessagePolinomial(){
            return first_group_message_polinomial;
        }

        std::string *getSecondMessagePoliomial(){
            return second_group_message_polinomial;
        }

        std::string *getFirstGroupErrorCorrectionCodewords(){
            return first_group_error_correction_codewords;
        }

        std::string *getSecondGroupErrorCorrectionCodewords(){
            return second_group_error_correction_codewords;
        }

        std::string getInterleaveMessageBlocks(){
            return interleave_message_blocks;
        }

        std::string getInterleaveCodeCorrectionBlocks(){
            return interleave_error_code_blocks;
        }

        std::string getFinalString(){
            return final_string;
        }

        std::string getInfo(){
            std::string s = "";

            s += "###### ERROR CORRECTION INFO ######\n\n";
            s += "Total codewords required: " + std::to_string(total_codewords) + "\n";
            s += "Ec codewords required: " + std::to_string(ec_codewords_per_block) + "\n";
            s += "Polinomial Generator: " + polynomial_generator + "\n";
            s += "Number of blocks in group 1: " + std::to_string(first_group_blocks_number) + "\n";
            s += "Number of data codewords in each of group 1's blocks: " + std::to_string(first_block_size) + "\n";
            if(first_group_message_polinomial != NULL){
                for(int i=0;i<first_group_blocks_number;i++){
                    s += "Block " + std::to_string(i+1) + ": " + first_group_message_polinomial[i] + "\n";
                }
            }
            s += "Number of blocks in group 2: " + std::to_string(second_group_blocks_number) + "\n";
            s += "Number of data codewords in each of group 2's blocks: " + std::to_string(second_block_size) + "\n";
            if(second_group_message_polinomial != NULL){
                for(int i=0;i<second_group_blocks_number;i++){
                    s += "Block " + std::to_string(i+1) + ": " + second_group_message_polinomial[i] + "\n";
                }
            }
            s += "Ec codewords for the first group:\n";
            if(first_group_error_correction_codewords!=NULL){
                for(int i=0;i<first_group_blocks_number;i++){
                    s += "Block " + std::to_string(i+1) + ": " + first_group_error_correction_codewords[i] + "\n";
                }
            }
            s += "Ec codewords for the second group:\n";
            if(second_group_error_correction_codewords!=NULL){
                for(int i=0;i<second_group_blocks_number;i++){
                    s += "Block " + std::to_string(i+1) + ": " + second_group_error_correction_codewords[i] + "\n";
                }
            }
            s += "Interleave data: " + interleave_message_blocks + "\n";
            s += "Interleave codewords: " + interleave_error_code_blocks + "\n";
            s += "Final string: " + final_string + "\n";

            return s;
        }
};

#endif