#ifndef QRMATRIXPLACEMENT
#define QRMATRIXPLACEMENT

#include "QrErrorCorrection.hpp"
#include "QrDataMasking.hpp"

class QrMatrixPlacement{
    private:
        QrCodeMaterial *material;
        QrInfo *info;
        QrString *str;
        QrErrorCorrection *error;
        QrDataMasking *mask;

        int size;
        int **matrix;

        std::string data_string;
        int size_data;
        
        const int default_data_color_black = 3;
        const int default_data_color_white = 4; 
        const int default_format_color_blue = 5;
        const int data_color_black = 1;
        const int data_color_white = 0;

        void set_size(){
            size = ((((info->getVersion()+1)-1)*4)+21); // {[(verision - 1)*4]+21}
        }

        void set_matrix(){
            matrix = new int*[size];

            for(int i=0;i<size;i++){
                matrix[i] = new int[size];
                for(int j=0;j<size;j++){
                    matrix[i][j] = 0; // 0 rappresenta nero 
                }
            }
        }
        
        void draw_finder_patterns(){ 
            for(int i=0;i<7;i++){ // outer part
                matrix[0][i] = default_data_color_black; // 3 is a special bit and is interpreted as black
                matrix[6][i] = default_data_color_black;
                matrix[i][0] = default_data_color_black;
                matrix[i][6] = default_data_color_black;
                matrix[0][(((((info->getVersion()+1)-1)*4)+21)-7)+i] = default_data_color_black;
                matrix[6][(((((info->getVersion()+1)-1)*4)+21)-7)+i] = default_data_color_black;
                matrix[i][(((((info->getVersion()+1)-1)*4)+21)-7)] = default_data_color_black;
                matrix[i][(((((info->getVersion()+1)-1)*4)+21)-7)+6] = default_data_color_black;
                matrix[(((((info->getVersion()+1)-1)*4)+21)-7)][i] = default_data_color_black;
                matrix[(((((info->getVersion()+1)-1)*4)+21)-7)+6][i] = default_data_color_black;
                matrix[(((((info->getVersion()+1)-1)*4)+21)-7)+i][0] = default_data_color_black;
                matrix[(((((info->getVersion()+1)-1)*4)+21)-7)+i][6] = default_data_color_black;
            }

            for(int i=1;i<6;i++){ // middle part
                matrix[1][i] = default_data_color_white;
                matrix[5][i] = default_data_color_white;
                matrix[i][1] = default_data_color_white;
                matrix[i][5] = default_data_color_white;
                matrix[1][(((((info->getVersion()+1)-1)*4)+21)-7)+i] = default_data_color_white;
                matrix[5][(((((info->getVersion()+1)-1)*4)+21)-7)+i] = default_data_color_white;
                matrix[i][(((((info->getVersion()+1)-1)*4)+21)-7)+1] = default_data_color_white;
                matrix[i][(((((info->getVersion()+1)-1)*4)+21)-7)+5] = default_data_color_white;
                matrix[(((((info->getVersion()+1)-1)*4)+21)-7)+1][i] = default_data_color_white;
                matrix[(((((info->getVersion()+1)-1)*4)+21)-7)+5][i] = default_data_color_white;
                matrix[(((((info->getVersion()+1)-1)*4)+21)-7)+i][1] = default_data_color_white;
                matrix[(((((info->getVersion()+1)-1)*4)+21)-7)+i][5] = default_data_color_white;
            }

            for(int i=2;i<5;i++){ // inner part
                matrix[2][i] = default_data_color_black;
                matrix[3][i] = default_data_color_black;
                matrix[4][i] = default_data_color_black;
                matrix[2][(((((info->getVersion()+1)-1)*4)+21)-7)+i] = default_data_color_black;
                matrix[3][(((((info->getVersion()+1)-1)*4)+21)-7)+i] = default_data_color_black;
                matrix[4][(((((info->getVersion()+1)-1)*4)+21)-7)+i] = default_data_color_black;
                matrix[(((((info->getVersion()+1)-1)*4)+21)-7)+2][i] = default_data_color_black;
                matrix[(((((info->getVersion()+1)-1)*4)+21)-7)+3][i] = default_data_color_black;
                matrix[(((((info->getVersion()+1)-1)*4)+21)-7)+4][i] = default_data_color_black;
            }
        }
    
        void draw_separators(){
            for(int i=0;i<8;i++){
                matrix[i][7] = default_data_color_white;
                matrix[7][i] = default_data_color_white;
                matrix[i][(((((info->getVersion()+1)-1)*4)+21)-7)-1] = default_data_color_white;
                matrix[(((((info->getVersion()+1)-1)*4)+21)-7)-1][i] = default_data_color_white;
                matrix[((((((info->getVersion()+1)-1)*4)+21)-7)-1)+i][7] = default_data_color_white;
                matrix[7][(((((info->getVersion()+1)-1)*4)+21)-7)+i] = default_data_color_white;
            }
        }
        
        void draw_shape_aligment_patterns(int cmx, int cmy){
            matrix[cmx][cmy] = default_data_color_black;

            for(int i=0;i<5;i++){
                matrix[cmx-2][cmy-2+i] = default_data_color_black;
                matrix[cmx+2][cmy-2+i] = default_data_color_black;
                matrix[cmx-2+i][cmy-2] = default_data_color_black;
                matrix[cmx-2+i][cmy+2] = default_data_color_black;
            }

            for(int i=0;i<3;i++){
                matrix[cmx-1][cmy-1+i] = default_data_color_white;
                matrix[cmx+1][cmy-1+i] = default_data_color_white;
                matrix[cmx-1+i][cmy-1] = default_data_color_white;
                matrix[cmx-1+i][cmy+1] = default_data_color_white;
            }
        }

        void draw_aligment_patterns(){
            coordinates cord = material->getAlignmentPatternCoordinates(info->getVersion());
            for(int i=0;i<cord.size;i++){
                if(matrix[cord.c[i].x][cord.c[i].y] != default_data_color_black){
                    draw_shape_aligment_patterns(cord.c[i].x, cord.c[i].y);
                }
            }
        }

        void draw_timing_pattern(){
            bool white_black = false;

            for(int i=8;i < (((((info->getVersion()+1)-1)*4)+21)-7);i++){
                if(white_black){
                    matrix[6][i] = default_data_color_white;
                    white_black = false;
                } else {
                    matrix[6][i] = default_data_color_black;
                    white_black = true;
                }
            }

            for(int i=8;i < (((((info->getVersion()+1)-1)*4)+21)-7);i++){
                if(white_black){
                    matrix[i][6] = default_data_color_white;
                    white_black = false;
                } else {
                    matrix[i][6] = default_data_color_black;
                    white_black = true;
                }
            }
        }

        void draw_dark_module(){
            matrix[(4*(info->getVersion()+1))+9][8] = default_data_color_black;
        }    
    
        void reserve_format_information(){
            for(int i=0;i<9;i++){
                if(i != 6){
                    matrix[i][8] = default_format_color_blue;
                    matrix[8][i] = default_format_color_blue;
                }
                if(i != 0)
                    matrix[8][(((((info->getVersion()+1)-1)*4)+21)-7)-2+i] = default_format_color_blue;
                if(i != 0 && i != 1)
                    matrix[((((((info->getVersion()+1)-1)*4)+21)-7)-2)+i][8] = default_format_color_blue;
            }
        }
    
        void reserve_version_information(){
            if(info->getVersion()>=6){
                for(int i=0;i<6;i++){
                    matrix[(((((info->getVersion()+1)-1)*4)+21)-7)-2][i] = default_format_color_blue;
                    matrix[(((((info->getVersion()+1)-1)*4)+21)-7)-3][i] = default_format_color_blue;
                    matrix[(((((info->getVersion()+1)-1)*4)+21)-7)-4][i] = default_format_color_blue;
                    matrix[i][(((((info->getVersion()+1)-1)*4)+21)-7)-2] = default_format_color_blue;
                    matrix[i][(((((info->getVersion()+1)-1)*4)+21)-7)-3] = default_format_color_blue;
                    matrix[i][(((((info->getVersion()+1)-1)*4)+21)-7)-4] = default_format_color_blue;
                }
            }
        }

        void fill_data_bits(){
            int line = size-1;
            int col = size-1;
            int data_index = 0;
            int string_size = data_string.size();

            while(col>=0){
                line = size-1;
                while(line>=0){
                    if(matrix[line][col] == 0){
                        matrix[line][col] = data_string.at(data_index++)-'0';
                    }

                    if(matrix[line][col-1] == 0){
                        matrix[line][col-1] = data_string.at(data_index++)-'0';
                    }
                    line--;
                }

                if(col-1 == 7){
                    col-=3;
                } else 
                    col-=2;

                if(col>=0){
                    line = 0;
                    while(line<size){
                        if(matrix[line][col] == 0){
                            matrix[line][col] = data_string.at(data_index++)-'0';
                        }   

                        if(matrix[line][col-1] == 0){
                            matrix[line][col-1] = data_string.at(data_index++)-'0';
                        }

                        line++;
                    }
                    col-=2;
                }
            }
        }
    public:
        QrMatrixPlacement(QrCodeMaterial *material, QrInfo *info, QrString *str, QrErrorCorrection *error){
            this->material = material;
            this->info = info;
            this->str = str;
            this->error = error;
            this->mask = new QrDataMasking();
            
            this->data_string = error->getFinalString();

            set_size();
            set_matrix();
            draw_finder_patterns();
            draw_separators();
            draw_aligment_patterns();
            draw_timing_pattern();
            draw_dark_module();
            reserve_format_information();
            reserve_version_information();
            fill_data_bits();

            this->mask->set_matrix(this->getMatrix());
            this->mask->set_size(this->size);

        }

        int getSize(){
            return size;
        }

        int **getMatrix(){
            int **res = new int*[size];

            for(int i=0;i<size;i++){
                res[i] = new int[size];
                for(int j=0;j<size;j++){
                    res[i][j] = matrix[i][j]; // 0 rappresenta bianco 
                }
            }

            return res;
        }

        void stampa(){
            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                    std::cout << matrix[i][j] << " ";
                }
                std::cout << "\n";
            }
        }

        ~QrMatrixPlacement(){
            for (int i = 0; i < size; i++){
                delete[] matrix[i];
            }
            delete[] matrix;
        }
};

#endif
