#ifndef QRINFO_H
#define QRINFO_H

#include"QrCodeMaterial.hpp"

class QrInfo{
	private:
		QrCodeMaterial *material;

		std::string s;
		livelli lvl;
		int mode;
		int version;
        
        //aggiungere gestione delle errori dopo
		
		//Optimal mode calculator
		bool isNumericalMode(){        
			for(int i=0;i<s.size();i++){ // conferma che ogni lettera sia presente nella tabella
				if(material->searchNumericMap(s[i]) == false){
					// std::cout << s[i] << "\n";
					return false;
				}
			}
			return true;
		}
		
		bool isAlphaNumericalMode(){
			for(int i=0;i<s.size();i++){ // conferma che ogni lettera sia presente nella tabella
				if(material->searchAlphaNumericMap(s[i]) == false){
					return false;
				}
			}
			return true;
		}
		
		bool isByteMode(){
			for(int i=0;i<s.size();i++){ // conferma che ogni lettera appartenga alla tabella byte
				if(material->searchByteMap(s[i]) == false){
					return false;
				}
			}
			return true;
		}

		int optimalMode(){
			if(isNumericalMode())
				return NUMERIC_MODE;
			else if(isAlphaNumericalMode())
				return ALPHANUMERIC_MODE;
			return BYTE_MODE;
		}
	
		//Finding the smallest version for data
		void smallestDataVersion(){
			int length = s.size();
			int min = INT_MAX;
			int index = 0;
			int temp;
			int livello;


            // Cerca quale versione è la migliore basandosi sulla 
            // sottrazione della lunghezza di una versione meno la lunghezza reale 
            // quindi la differenza più piccola sarà la versione ottimale
			for(int i=0;i<VERSIONI_RIGHE;i++){ 
				temp = material->getVersionCapacity(i, mode)-length;

				if(temp>0 && min > temp){
					min = temp;
					index = i;
				}
			}

			livello = index%4; // vedo quale livello di sicurezza usare perché ogni versione ha 4 livelli

			switch(livello){
				case 0:
					lvl = L;
					break;
				case 1:
					lvl = M;
					break;
				case 2:
					lvl = Q;
					break;
				case 3:
					lvl = H;
					break;
			}

			version = index/4; // trovo la versione dividendo per 4 la l'indice trovato, perché ci sono 4 livelli
		}

	public:
		// Constructor		
		QrInfo(QrCodeMaterial* material, std::string s){
			this->material = material;
			this->s = s;
			mode = optimalMode();	
			smallestDataVersion(); 
		}
		
		// Getter method
		std::string getString(){
			return s;
		}

		livelli getLevel(){
			return lvl;
		}

		int getMode(){ // mode is already selected through the program
			return mode;
		}
		
		int getVersion(){
			return version;
		}

		// All information is given through a string
		std::string getInfo(){
			std::string str = "";

			str += "############## INFO ###############\n\n";
			str += "String: " + s + "\n";
			str += "Size: " + std::to_string(s.size()) + "\n";
			str += "Mode: ";

			switch(mode){
				case 0:
					str += "Numeric\n";
					break;
				case 1:
					str += "AlphaNumeric\n";
					break;
				case 2:
					str += "Byte\n";
					break;
			} 

			str += "Version: " + std::to_string(version+1) + "\n";
			str += "Level: ";

			switch(lvl){
				case L:
					str += "L\n";
					break;
				case M:
					str += "M\n";
					break;
				case Q:
					str += "Q\n";
					break;
				case H:
					str += "H\n";
					break;
			}

			return str;
		}
};

#endif
