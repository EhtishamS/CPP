#ifndef QRDATAMASKING
#define QRDATAMASKING

#include<iostream>

/*Masking rules:
Mask number         formula for switching the at certain coordinate
     0                      (row + column) mod 2 == 0
     1                          (row) mod 2 == 0
     2                        (column) mod 3 == 0
     3                      (row + column) mod 3 == 0
     4              ( floor(row / 2) + floor(column / 3) ) mod 2 == 0
     5              ((row * column) mod 2) + ((row * column) mod 3) == 0
     6              ( ((row * column) mod 2) + ((row * column) mod 3) ) mod 2 == 0
     7              	( ((row + column) mod 2) + ((row * column) mod 3) ) mod 2 == 0*/

#define MASK_COUNT 8

struct score{
    std::string mask_name;
    int penalty_1;
    int penalty_2;
    int penalty_3;
    int penalty_4;
    int total_score;
};

class QrDataMasking{
    private:
        score count[8];
        int index_score;
        int** data_matrix;
        int** memory_matrix;
        int size;
        
        void convert_to_black_and_white(){
            
        }

        void mask_zero(){
            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                    if((memory_matrix[i][j] == 0 || memory_matrix[i][j] == 1) && (i+j)%2==0){
                        memory_matrix[i][j] = !memory_matrix[i][j];
                    }
                }
            }
        }

        void mask_one(){
            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                    if((memory_matrix[i][j] == 0 || memory_matrix[i][j] == 1) && (i)%2==0){
                        memory_matrix[i][j] = !memory_matrix[i][j];
                    }
                }
            }
        }

        void evaluate(){
            this->memory_matrix = this->data_matrix;

            this->count[index_score].mask_name = "mask zero";
            this->mask_zero();


        }

        void evaluantion_condition_one(){
            count[index_score].penalty_1 = 0;
            int h_count = 0;
            int v_count = 0;

            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                    if(this->memory_matrix[i][j] == 0){

                    }
                }
            }
        }

    public:
        QrDataMasking(){
            this->data_matrix = NULL;
            this->size = 0;
            this->index_score = 0;
        }

        QrDataMasking(int **matrix, int size){
            this->data_matrix = matrix;
            this->size = size;
            this->index_score = 0;
        }

        void set_matrix(int **matrix){
            this->data_matrix = matrix;
        }

        void set_size(int size){
            if(size > 0){
                this->size = size;
            } else {
                this->size = 0;
            }
        }

        void stampa(int**matrix){
            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                    std::cout << matrix[i][j] << " ";
                }
                std::cout << "\n";
            }
        }
};

#endif