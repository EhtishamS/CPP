#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include"QrString.hpp"

struct mononomio{
    int a;
    int x;
};

class Polynomial{
    private:
        QrCodeMaterial *material;
        QrInfo *info;
        QrString *str;

/* non mi serve ma può essere scommentato nel momento in cui può servire
        void swap(mononomio* a, mononomio* b){
            mononomio t = *a;
            *a = *b;
            *b = t;
        }

        int partition(mononomio arr[], int low, int high){
            int pivot = arr[high].x; // pivot
            int i = (low- 1);
        
            for (int j = low; j <= high - 1; j++) {
                if (arr[j].x < pivot) {
                    i++;
                    swap(&arr[i], &arr[j]);
                }
            }
            swap(&arr[i + 1], &arr[high]);
            return (i + 1);
        }

        void quickSort(mononomio arr[], int low, int high){
            if (low < high) {
                int pi = partition(arr, low, high);
        
                // Separately sort elements before
                // partition and after partition
                quickSort(arr, low, pi - 1);
                quickSort(arr, pi + 1, high);
            }
        }

        void sortPolinomio(std::string &espressione){
            const int elements = contatoreMonomio(result); 
            std::string temp = "";
            mononomio* arr = new mononomio[elements];
            int i=0;

            stringToArray(result, arr, elements);
            quickSort(arr, 0, elements-1);
            i = elements-1;

            temp += "(";
            while(i>=0){
                temp += "a" + std::to_string(arr[i].a) + "x" + std::to_string(arr[i].x) + "+";
                i--;
            }
            temp.replace(temp.size()-1, 1, "");
            temp += ")";

            delete[] arr;
            result = temp;

        }
*/
        
        void stringToArray(std::string expression, mononomio arr[], int elements){
            mononomio p;
            int i=0;

            while(i<elements){
                monomioToInt(expression, i+1, p);
                arr[i] = p;
                i++;
            }
        }

        int contatoreMonomio(std::string s){ // Conta quanti monomi sono presenti in un polinomio
            int i = 0;
            int contatore = 1;
            while(i<s.size()){
                if(s.at(i) == '+')
                    contatore++;
                i++;
            }

            return contatore;
        }

        void monomioToInt(std::string expression, int termNumber, mononomio &p){
            int a = 0;
            int x = 0;
            int i=0;
            int begin;
            int end;
            size_t pos; // variabili non usata ma server solo come paramentro nella stoi

            // skippo al monomio da trasformare il intero
            while(termNumber>1){
                if(expression.at(i) == '+'){
                    termNumber--;
                }
                i++;
            }

            //annoto le inizio e fine il grado della a e della x, e li trasformo in interi.
            while(expression.at(i) != 'a'){i++;};
            begin = i+1;
            while(expression.at(i) != 'x'){i++;};
            end = i;
            a = std::stoi(expression.substr(begin, end-begin), &pos);

            begin = end+1;
            while(expression.at(i) != '+' && expression.at(i) != ')'){i++;};
            end = i;
            x = std::stoi(expression.substr(begin, end-begin), &pos);

            p.a = a;
            p.x = x;
        }

        void raccoglimentoTerminiSimili(std::string &espressione){
            std::string risultato = "";
            int lunghezza = contatoreMonomio(espressione);
            mononomio *arr = new mononomio[lunghezza];
            mononomio *arr_finale = new mononomio[lunghezza];
            int indice_arr_finale = 0;
            int i = 1;
            int a = 0;
            bool sommato = false;

            stringToArray(espressione, arr, lunghezza); // trasformo tutto il polinomio in intero
            
            //XOR tra gli elementi con la X uguale 
            while(i<lunghezza){
                if(arr[i].x == arr[i-1].x){
                    a += material->getExponentToValue(arr[i].a) ^ material->getExponentToValue(arr[i-1].a); // devo far XOR tra valori perciò trasformo gli esponenti
                    sommato = true;
                } else {
                    if(a == 0 && sommato == true)
                        arr_finale[indice_arr_finale].a = a;
                    else if(sommato == true)
                        arr_finale[indice_arr_finale].a = material->geValueToExponent(a); // devo trasformare il risultato finale in forma esponenziale
                    else
                        arr_finale[indice_arr_finale].a = arr[i-1].a;
                    sommato = false;
                    arr_finale[indice_arr_finale].x = arr[i-1].x;
                    indice_arr_finale++;
                    a = 0;
                }
                i++;
            }
            // l'elemento finale sarà sempre con X^0 ed unico.
            arr_finale[indice_arr_finale].a = arr[i-1].a;
            arr_finale[indice_arr_finale].x = arr[i-1].x;
            indice_arr_finale++;
            

            // metto nella string il polinomio compatto
            i=0;
            risultato = "(";
            while(i<indice_arr_finale){
                risultato += "a" + std::to_string(arr_finale[i].a) + "x" + std::to_string(arr_finale[i].x) + "+";
                i++;
            }
            risultato.replace(risultato.size()-1, 1, "");
            risultato += ")";

            // elimino la memoria allocata
            delete[] arr;
            delete[] arr_finale;

            espressione = risultato;
        }

        std::string moltiplicatoreMononio(std::string str1, std::string str2){
            int i = 0;
            int j = 0;
            int elementi1 = 1;
            int elementi2 = 1;
            std::string prodotto = "";
            mononomio m1;
            mononomio m2;

            elementi1 = contatoreMonomio(str1);
            elementi2 = contatoreMonomio(str2);

            //faccio il prodotto tra due monomi.
            prodotto += "(";
            for(i=0;i<elementi1;i++){
                monomioToInt(str1, i+1, m1);
                for(j=0;j<elementi2;j++){
                    monomioToInt(str2, j+1, m2);
                    prodotto += "a" + std::to_string((m1.a + m2.a)%255) + "x" + std::to_string(m1.x + m2.x) + "+"; 
                }
            }
            
            prodotto.replace(prodotto.size()-1, 1, "");
            prodotto += ")";

            raccoglimentoTerminiSimili(prodotto);

            return prodotto;
        }

        std::string xorPolinomio(std::string str1, std::string str2){
            int i = 0;
            int elementi1 = 1;
            int elementi2 = 1;
            std::string risultato = "";
            mononomio m1;
            mononomio m2;

            elementi1 = contatoreMonomio(str1);
            elementi2 = contatoreMonomio(str2);

            //faccio il xor tra due monomi.
            risultato += "(";
            for(i=0;i<elementi1 && i<elementi2;i++){
                monomioToInt(str1, i+1, m1);
                monomioToInt(str2, i+1, m2);
                risultato += "a" + std::to_string(material->geValueToExponent(material->getExponentToValue(m1.a) ^ material->getExponentToValue(m2.a))) + "x" + std::to_string(m1.x) + "+";
            }

            while(i<elementi1){
                monomioToInt(str1, i+1, m1);
                risultato += "a" + std::to_string(m1.a) + "x" + std::to_string(m1.x) + "+";
                i++;
            }

            while(i<elementi2){
                monomioToInt(str2, i+1, m2);
                risultato += "a" + std::to_string(m2.a) + "x" + std::to_string(m2.x) + "+";
                i++;
            }

            risultato.replace(risultato.size()-1, 1, "");
            risultato += ")";

            return risultato;
        }

        std::string splitPolinomialExpression(std::string expression){
            std::string str1;
            std::string str2;
            int begin = 0;
            int i = 0;

            // separo la prima parentesi dalle successive
            while(expression.at(i) != ')'){i++;};
            i++; // i deve essere sempre aumentato perché rimane sul carattere ')' 
            str1 = expression.substr(begin, i-begin); 

            
            while(i<expression.size()){
                begin = i+1; // begin deve sempre essere sul carattere '(', in questo caso era s '+' quindi assegno i+1;

                // separo la seconda parentesi dalle successive
                while(expression.at(i) != ')'){i++;};
                i++;
                str2 = expression.substr(begin, i-begin);

                // la prima parentesi sarà sempre la soluzione delle prime due.
                str1 = moltiplicatoreMononio(str1,str2);
            }

            str1 = str1.substr(1,str1.size()-2); // tolgo le parentesi
            
            return str1;
        }
    
        std::string polinomialGenerator(int codewords_number){
            int i=0;

            std::string expression = ""; 

            while(i<codewords_number){
                expression += "(a0x1+a" + std::to_string(i) + "x0)*";
                i++;
            }

            expression.replace(expression.size()-1, 1, "");

            return splitPolinomialExpression(expression);
        }

        std::string conversioneStringToPolynomial(std::string message){
            std::string s = message;
            std::string result = "";
            int size = s.size()/8;
            int begin = 0;
            
            while(size>0){
                result += "a" + std::to_string(material->geValueToExponent(binaryToDecimal(s.substr(begin, 8)))) + "x" + std::to_string(size-1) + "+";
                begin += 8;
                size--;
            }

            result.replace(result.size()-1, 1, "");

            return result;
        }
        
        std::string conversionePolynomialToString(std::string polynomial){
            std::string result = "";
            mononomio m;
            int elements = contatoreMonomio(polynomial);

            for(int i=0;i<elements;i++){
                monomioToInt(polynomial, i+1, m);
                result += DecimalTobinary(material->getExponentToValue(m.a), 8);
            }

            return result;
        }

        std::string longDivision(std::string messagePolinomial, std::string generatorPolynomial, int numberOfDivisions){
            std::string risult_generator_polynomial;
            int elementi = contatoreMonomio(messagePolinomial);
            int j;
            mononomio m;

            // ripetere l'azione per numberOfDivisions volte:
                // 1. Moltiplicare il polinomio per il primo termine
                // 2. Il risultato ottentenuto lo faccio xor per il messaggio polinomiale
                // 3. Tolgo il primo termine dal risultato della xor dato che è zero. 
                // 4. Devo mettere a posto gli esponenti del polinomio generatore perchè il primo termine deve avere lo stesso esponente x quello che ha il messaggio.

            for(int i=0;i<numberOfDivisions;i++){
                // 1 
                monomioToInt(messagePolinomial, 1, m);
                risult_generator_polynomial = moltiplicatoreMononio("(a"+std::to_string(m.a)+"x0)", generatorPolynomial);

                // 2
                messagePolinomial = xorPolinomio(risult_generator_polynomial, messagePolinomial);

                // 3
                for(j=0;messagePolinomial.at(j)!='+';j++);
                messagePolinomial = "("+ messagePolinomial.substr(j+1, messagePolinomial.size()-(j+1));
                generatorPolynomial.shrink_to_fit();
                messagePolinomial.shrink_to_fit();

                // 4
                generatorPolynomial = moltiplicatoreMononio("(a0x-1)", generatorPolynomial);
            }
            
            return messagePolinomial;
        }
    public:
        Polynomial(QrCodeMaterial *material, QrInfo* info, QrString* str){
            this->material = material;
            this->info = info;
            this->str = str;
        }

        std::string getPolynomialGenerator(int n){
            return polinomialGenerator(n);
        }

        std::string converisioneInPolinomio(std::string message){
            return conversioneStringToPolynomial(message);
        }

        std::string moltiplicaPerX(std::string polinomio, int x_value){
            polinomio = "(" + polinomio + ")";
            std::string monomio = "(a0x" + std::to_string(x_value) + ")";
            std::string res = moltiplicatoreMononio(polinomio, monomio);
            res = res.substr(1,res.size()-2); // tolgo le parentesi
            return res;
        }

        std::string divisioneLunga(std::string messagePolinomial, std::string generatorPolynomial, int numberOfDivisions, int x_value){
            // Ricevo il messaggio in stringa binaria allora lo trasformo in un polinomio e dopo di quello lo moltipico per x_value in modo da avere l'esponente dello
            // abbastanza grande. Aggiungo anche le partensi che sono importanti
            messagePolinomial = this->conversioneStringToPolynomial(messagePolinomial); 
            messagePolinomial = "(" + this->moltiplicaPerX(messagePolinomial, x_value) + ")"; 
            messagePolinomial.shrink_to_fit();  

            // Devo solo aggiungo le prentesi e moltipolico per il (l'esponente più grande del messaggio polinomiale - x_value).
            mononomio m; 
            monomioToInt(messagePolinomial, 1, m); 
            generatorPolynomial = "(" + this->moltiplicaPerX(generatorPolynomial, m.x-x_value) +")"; 
            
            std::string result = longDivision(messagePolinomial, generatorPolynomial, numberOfDivisions); 

            result = conversionePolynomialToString(result); // riconverto in formato binario

            return result;
        }
};


#endif