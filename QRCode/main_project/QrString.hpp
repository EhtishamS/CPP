#ifndef QRSTRING_H
#define QRSTRING_H

#include"QrInfo.hpp" 

#define NUMERIC_GROUP_SIZE 3 // gruppi da fare nella codifica numeric
#define ALPHANUMERIC_GROUP_SIZE 2 // gruppi da fare nella codifica alphanumeric
#define BYTE_GROUP_SIZE 1 // questo non lo utilizzo ma lo metto solo perché ho messo le altre nelle altre modalita

// dimensioni in binario rispetto a quante cifre trovo	
#define NUMERIC_SIZE_3 10 // se ho 3 cifre allora 10 bit
#define NUMERIC_SIZE_2 7  // se ho 2 cifre allora 7 bit
#define NUMERIC_SIZE_1 4  // se ho 1 cifra allora 4 bit

//dimensioni in binario rispetto al gruppo
#define ALPHANUMERIC_SIZE_2 11
#define ALPHANUMERIC_SIZE_1 6

//dimenisioni in binario di ogni lettera
#define BYTE_SIZE 8

// Pad bytes
#define PAD_BYTE_1 "11101100" // 236
#define PAD_BYTE_2 "00010001" // 17

class QrString{
	private:
		QrCodeMaterial *material;
		QrInfo *info;

		std::string mode_indicator;             
		std::string character_count_indicator;
		std::string encoded_data;
		std::string terminator;
		std::string add_to_mutiple_eight;
		std::string pad_byte;
		int required_bits;
		int total_length_in_bits;

		std::string result_string;

		void compile_required_bits(){
			required_bits = 8*material->getCodeWordInfo(info->getVersion(), info->getLevel(), TOTAL_CODEWORDS);
		}

		void compile_mode_indicator(){
			mode_indicator = material->getModeIndicator(info->getMode());
			total_length_in_bits += mode_indicator.size();
		}

		void compile_character_count_indicator(){
			character_count_indicator = DecimalTobinary(info->getString().size(), material->getCharacterCountSize(info->getVersion(), info->getMode()));
			total_length_in_bits += character_count_indicator.size();
		}
        
        void compile_encoded_data(){
			switch(info->getMode()){
				case NUMERIC_MODE:
					numericEncoding();
					break;
				case ALPHANUMERIC_MODE:
					alphaNumericEncoding();
					break;
				case BYTE_MODE: 
					byteEncoding();
					break;
			}
        }

        void numericEncoding(){
            std::string s = info->getString();
			int lunghezza = s.size();
			int begin = 0;
			int n = 0;
			size_t pos;

			// descrizione algritmo:
			// divido il numero in gruppo da 3 e ciascun gruppo verra trasformato in 10 bit binary. 
			// condizioni:
			// Se il gruppo parte con uno zero (ad es: 011) questo verra trattato come due cifre e verra convertita in 7 bit binary
			// Se il gruppo parte con due zeri (ad es: 001) questo verra trattato come una cifra e verra convertita in 4 bit binary
			// Al gruppo finale si applicano le stesse regole.
			while(begin<lunghezza){
				if(begin+3<=lunghezza){ // '<=' perché parto da zero è sommo 3 e non 2
					n = std::stoi(s.substr(begin, NUMERIC_GROUP_SIZE), &pos);
					if(s.at(begin) == '0'){ 
						if(s.at(begin+1) == '0')
							encoded_data += DecimalTobinary(n, NUMERIC_SIZE_1);
						else 
							encoded_data += DecimalTobinary(n, NUMERIC_SIZE_2);
					} else  
						encoded_data += DecimalTobinary(n, NUMERIC_SIZE_3);
					begin+=3;
				} else {
					int n = std::stoi(s.substr(begin, lunghezza-begin), &pos);
					if(pos == 2){
						if(s.at(begin) == '0')
							encoded_data += DecimalTobinary(n, NUMERIC_SIZE_1);
						else
							encoded_data += DecimalTobinary(n, NUMERIC_SIZE_2);
					} else 
						encoded_data += DecimalTobinary(n, NUMERIC_SIZE_1);
					begin = lunghezza;
				}
			}	

			total_length_in_bits += encoded_data.size();										
        }
	
		void alphaNumericEncoding(){
			std::string s = info->getString();
			int lunghezza = s.size();
			int begin = 0;
			int n = 0;
			size_t pos;
			
			while(begin<lunghezza){
				if(begin+2<=lunghezza){
					n = (material->getAlphaNumericCar(s.at(begin))) * 45;
					n += material->getAlphaNumericCar(s.at(begin+1));
					encoded_data += DecimalTobinary(n, ALPHANUMERIC_SIZE_2);
					begin+=2;
				} else {
					encoded_data += DecimalTobinary(material->getAlphaNumericCar(s.at(lunghezza-1)), ALPHANUMERIC_SIZE_1);
					begin = lunghezza;
				}
			}
			total_length_in_bits += encoded_data.size();
		}
	
		void byteEncoding(){
			std::string s = info->getString();
			int i=0;

			while(i<s.size()){
				encoded_data += DecimalTobinary(material->getByteCar(s.at(i)),BYTE_SIZE);
				i++;
			}
			total_length_in_bits += encoded_data.size();
		}

		void compile_terminator(){
			int multiple = 8;
			while(multiple<total_length_in_bits) {multiple+=8;}; // per trovare un multiplo di 8
			
			if(multiple-total_length_in_bits < 4){ // 0's to add up to the make the length divideable for 8
				terminator = DecimalTobinary(0, multiple-total_length_in_bits);
			} else if(required_bits-total_length_in_bits >= 4){
				terminator = DecimalTobinary(0, 4); // 0000
			} else if(required_bits-total_length_in_bits > 0 && required_bits-total_length_in_bits < 4){
				terminator = DecimalTobinary(0, required_bits-total_length_in_bits);
			} else {
				terminator = "";
			}

			total_length_in_bits += terminator.size();
		}

		void compile_add_to_multiple_eight(){
			if(total_length_in_bits%8!=0){
				int multiple = 8;
				while(multiple<total_length_in_bits) {multiple+=8;}; // per trovare un multiplo di 8
				add_to_mutiple_eight = DecimalTobinary(0, multiple-total_length_in_bits);
			} else {
				add_to_mutiple_eight = "";
			}
			total_length_in_bits += add_to_mutiple_eight.size();
		}
		
		void compile_pad_byte(){
			pad_byte = "";
			if(total_length_in_bits<required_bits){
				pad_byte = "";
				int n = (required_bits-total_length_in_bits)/8;

				while(n>0){
					if(n-2>=0){
						pad_byte += PAD_BYTE_1;
						pad_byte += PAD_BYTE_2;
						n-=2;
					} else {
						pad_byte += PAD_BYTE_1;
						n = 0; 
					}
				}
			}

			total_length_in_bits += pad_byte.size();
		}	

		void compile_result_string(){
			result_string = "";
			result_string += mode_indicator;
			result_string += character_count_indicator;
			result_string += encoded_data;
			result_string += terminator;
			result_string += add_to_mutiple_eight;
			result_string += pad_byte;
		}
	public:
		QrString(QrCodeMaterial *material, QrInfo *info){
			this->material = material;
			this->info = info;
			
			compile_required_bits();
			compile_mode_indicator();
			compile_character_count_indicator();
			compile_encoded_data();
			compile_terminator();
			compile_add_to_multiple_eight();
			compile_pad_byte();
			compile_result_string();
		}

		std::string getModeIndicator(){
			return mode_indicator;
		}

		std::string getcharacterCountIndicator(){
			return character_count_indicator;
		}

		std::string getEncodedData(){
			return encoded_data;
		}
		
		std::string getTerminator(){
			return terminator;
		}

		std::string getExtraBitsToEight(){
			return add_to_mutiple_eight;
		}

		std::string getPadByte(){
			return pad_byte;
		}

		int getTotalBitsRequired(){
			return required_bits;
		}

		int getLength(){
			return total_length_in_bits;
		}

		std::string getStr(){
			return result_string;
		}
		
		std::string getInfo(){
			std::string str = "########### BINARY INFO ###########\n\n";

			str += "Mode Indicator: " + getModeIndicator() +  "\n";
			str += "Character Count Indicator: " + getcharacterCountIndicator() + "\n";
			str += "Encoded Data: " + getEncodedData() + "\n";
			
			if(terminator.size() > 0)
				str += "Terminator: " + getTerminator() + "\n";
			else 
				str += "Terminator: None\n";

			if(add_to_mutiple_eight.size() > 0)
				str += "Extra bits: " + getExtraBitsToEight() + "\n";
			else 
				str += "Extra bits: None\n";

			if(pad_byte.size() > 0) 
				str += "Pad Bytes: " + getPadByte() + "\n";
			else 
				str += "Pad Bytes: None\n";
			return str;
		}
};

#endif