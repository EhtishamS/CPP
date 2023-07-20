#ifndef QRCODEMATERIAL_H
#define QRCODEMATERIAL_H

#include <map>
#include <string>
#include <iostream>

// versioni della tabella di dimensioni 
#define VERSIONI_RIGHE 160 // 40 versioni moltiplicato per 4 livelli di sicurezza
#define VERSIONI_COLONNE 3 // 3 perché ho solo scelto 3 modalità {numeric, alphanumeric, byte}

// tabella della lunghezza del character_count in base alle versioni
#define CHARACTER_COUNT_LINES 3 
#define CHARACTER_COUNT_COLONS 3 

//Modes
#define NUMERIC_MODE 0
#define ALPHANUMERIC_MODE 1
#define BYTE_MODE 2

// Mode indicator binary value
#define NUMERIC_INDICATOR "0001"
#define ALPHANUMERIC_INDICATOR "0010"
#define BYTE_INDICATOR "0100"

// params of error correction table
#define TOTAL_CODEWORDS 1
#define EC_CODEWORDS_PER_BLOCK 2
#define FIRST_GROUP_BLOCK_SIZE 3
#define FIRST_BLOCK_SIZE 4
#define SECOND_GROUP_BLOCK_SIZE 5
#define SECOND_BLOCK_SIZE 6

// dimensione tabella log antilog table
#define LOGANTILOG_SIZE 256
#define BITWISE_MODULO 285

// dimensione tabella aligment pattern
#define ALIGNMENT_PATTER_RIGHE 40
#define ALIGNMENT_PATTER_COLS 7

// Livelli correzione errori
enum livelli{L = 0,M = 1,Q = 2,H = 3};

std::string DecimalTobinary(int n, int size){
    std::string r; //Number in binary
    std::string add; //0's to add in order to made the size fit

    while(n!=0){r = (n%2 == 0 ? "0":"1")+r; n/=2;} //Binary conversion
    while(r.size()<size){add+='0';size--;} //Add 0.
    
    return add+r;
}

long long int binaryToDecimal(std::string s){
	return std::stoi(s, 0, 2);
}

std::string *split(std::string *dst, std::string src, char separator, int &lunghezza){
	std::string src_temp = src;
	std::string::size_type found;
	int begin = 0;
	int currentIndex = 0;
	int counter = 0;
	
	if(src_temp.at(0) == separator)
		src_temp.replace(0, 1, "");

	if(src_temp.at(src_temp.size()-1) == separator){
		src_temp.replace(src_temp.size()-1, 1, "");
	}

	for(int i=0;i<src_temp.size();i++){
		if(src_temp.at(i) == separator){
			counter++;
		}
	}
	
	dst = new std::string[counter+1];

	while((found = src_temp.find(";")) != std::string::npos){
		src_temp.replace(found, 1, " ");
		dst[currentIndex++] = src_temp.substr(begin, found-begin);
		begin = found+1;
	}

	dst[currentIndex++] = src_temp.substr(begin, (src_temp.size())-begin);

	lunghezza = currentIndex;

	return dst;
} 

void trims(std::string &str){
	if(str.at(0) == ' '){
		str.replace(0, 1, "");
		trims(str);
	}

	if(str.at(str.size()-1) == ' '){
		str.replace(str.size()-1, 1, "");
		trims(str);
	}
}

struct ErrorCorrectionParam{
	int total_codewords;
	int ec_codewords_per_block; // ec = error correction
	int first_group_block_size;
	int first_block_size;
	int second_group_block_size;
	int second_block_size;
};

struct coordinate{
	int x;
	int y;	
};

struct coordinates{
	coordinate *c;
	int size;
};

class QrCodeMaterial{
	private:
		int tabella_versioni[VERSIONI_RIGHE][VERSIONI_COLONNE];
		int tabella_size_character_count[CHARACTER_COUNT_LINES][CHARACTER_COUNT_COLONS];
		std::map<char, int> numeric;
		std::map<char, int> alphaNumeric;
		std::map<char, int> byte;
		std::map<int, std::string> mode_indicator;
		std::map<std::string, ErrorCorrectionParam> errorCorrectionTable;
		std::map<int, int> logAntiLogTable;
		std::map<int, int> remainderBits;
		int alignmentPatternLocation[ALIGNMENT_PATTER_RIGHE][ALIGNMENT_PATTER_COLS];

		// Riempimento dati
		void fillTabellaVersioni(){
			tabella_versioni [0][0] = 41; tabella_versioni [0][1] = 25; tabella_versioni [0][2] = 17;
			tabella_versioni [1][0] = 34; tabella_versioni [1][1] = 20; tabella_versioni [1][2] = 14;
			tabella_versioni [2][0] = 27; tabella_versioni [2][1] = 16; tabella_versioni [2][2] = 11;
			tabella_versioni [3][0] = 17; tabella_versioni [3][1] = 10; tabella_versioni [3][2] = 7;
			tabella_versioni [4][0] = 77; tabella_versioni [4][1] = 47; tabella_versioni [4][2] = 32;
			tabella_versioni [5][0] = 63; tabella_versioni [5][1] = 38; tabella_versioni [5][2] = 26;
			tabella_versioni [6][0] = 48; tabella_versioni [6][1] = 29; tabella_versioni [6][2] = 20;
			tabella_versioni [7][0] = 34; tabella_versioni [7][1] = 20; tabella_versioni [7][2] = 14;
			tabella_versioni [8][0] = 127; tabella_versioni [8][1] = 77; tabella_versioni [8][2] = 53;
			tabella_versioni [9][0] = 101; tabella_versioni [9][1] = 61; tabella_versioni [9][2] = 42;
			tabella_versioni [10][0] = 77; tabella_versioni [10][1] = 47; tabella_versioni [10][2] = 32;
			tabella_versioni [11][0] = 58; tabella_versioni [11][1] = 35; tabella_versioni [11][2] = 24;
			tabella_versioni [12][0] = 187; tabella_versioni [12][1] = 114; tabella_versioni [12][2] = 78;
			tabella_versioni [13][0] = 149; tabella_versioni [13][1] = 90; tabella_versioni [13][2] = 62;
			tabella_versioni [14][0] = 111; tabella_versioni [14][1] = 67; tabella_versioni [14][2] = 46;
			tabella_versioni [15][0] = 82; tabella_versioni [15][1] = 50; tabella_versioni [15][2] = 34;
			tabella_versioni [16][0] = 255; tabella_versioni [16][1] = 154; tabella_versioni [16][2] = 106;
			tabella_versioni [17][0] = 202; tabella_versioni [17][1] = 122; tabella_versioni [17][2] = 84;
			tabella_versioni [18][0] = 144; tabella_versioni [18][1] = 87; tabella_versioni [18][2] = 60;
			tabella_versioni [19][0] = 106; tabella_versioni [19][1] = 64; tabella_versioni [19][2] = 44;
			tabella_versioni [20][0] = 322; tabella_versioni [20][1] = 195; tabella_versioni [20][2] = 134;
			tabella_versioni [21][0] = 255; tabella_versioni [21][1] = 154; tabella_versioni [21][2] = 106;
			tabella_versioni [22][0] = 178; tabella_versioni [22][1] = 108; tabella_versioni [22][2] = 74;
			tabella_versioni [23][0] = 139; tabella_versioni [23][1] = 84; tabella_versioni [23][2] = 58;
			tabella_versioni [24][0] = 370; tabella_versioni [24][1] = 224; tabella_versioni [24][2] = 154;
			tabella_versioni [25][0] = 293; tabella_versioni [25][1] = 178; tabella_versioni [25][2] = 122;
			tabella_versioni [26][0] = 207; tabella_versioni [26][1] = 125; tabella_versioni [26][2] = 86;
			tabella_versioni [27][0] = 154; tabella_versioni [27][1] = 93; tabella_versioni [27][2] = 64;
			tabella_versioni [28][0] = 461; tabella_versioni [28][1] = 279; tabella_versioni [28][2] = 192;
			tabella_versioni [29][0] = 365; tabella_versioni [29][1] = 221; tabella_versioni [29][2] = 152;
			tabella_versioni [30][0] = 259; tabella_versioni [30][1] = 157; tabella_versioni [30][2] = 108;
			tabella_versioni [31][0] = 202; tabella_versioni [31][1] = 122; tabella_versioni [31][2] = 84;
			tabella_versioni [32][0] = 552; tabella_versioni [32][1] = 335; tabella_versioni [32][2] = 230;
			tabella_versioni [33][0] = 432; tabella_versioni [33][1] = 262; tabella_versioni [33][2] = 180;
			tabella_versioni [34][0] = 312; tabella_versioni [34][1] = 189; tabella_versioni [34][2] = 130;
			tabella_versioni [35][0] = 235; tabella_versioni [35][1] = 143; tabella_versioni [35][2] = 98;
			tabella_versioni [36][0] = 652; tabella_versioni [36][1] = 395; tabella_versioni [36][2] = 271;
			tabella_versioni [37][0] = 513; tabella_versioni [37][1] = 311; tabella_versioni [37][2] = 213;
			tabella_versioni [38][0] = 364; tabella_versioni [38][1] = 221; tabella_versioni [38][2] = 151;
			tabella_versioni [39][0] = 288; tabella_versioni [39][1] = 174; tabella_versioni [39][2] = 119;
			tabella_versioni [40][0] = 772; tabella_versioni [40][1] = 468; tabella_versioni [40][2] = 321;
			tabella_versioni [41][0] = 604; tabella_versioni [41][1] = 366; tabella_versioni [41][2] = 251;
			tabella_versioni [42][0] = 427; tabella_versioni [42][1] = 259; tabella_versioni [42][2] = 177;
			tabella_versioni [43][0] = 331; tabella_versioni [43][1] = 200; tabella_versioni [43][2] = 137;
			tabella_versioni [44][0] = 883; tabella_versioni [44][1] = 535; tabella_versioni [44][2] = 367;
			tabella_versioni [45][0] = 691; tabella_versioni [45][1] = 419; tabella_versioni [45][2] = 287;
			tabella_versioni [46][0] = 489; tabella_versioni [46][1] = 296; tabella_versioni [46][2] = 203;
			tabella_versioni [47][0] = 374; tabella_versioni [47][1] = 227; tabella_versioni [47][2] = 155;
			tabella_versioni [48][0] = 1022; tabella_versioni [48][1] = 619; tabella_versioni [48][2] = 425;
			tabella_versioni [49][0] = 796; tabella_versioni [49][1] = 483; tabella_versioni [49][2] = 331;
			tabella_versioni [50][0] = 580; tabella_versioni [50][1] = 352; tabella_versioni [50][2] = 241;
			tabella_versioni [51][0] = 427; tabella_versioni [51][1] = 259; tabella_versioni [51][2] = 177;
			tabella_versioni [52][0] = 1101; tabella_versioni [52][1] = 667; tabella_versioni [52][2] = 458;
			tabella_versioni [53][0] = 871; tabella_versioni [53][1] = 528; tabella_versioni [53][2] = 362;
			tabella_versioni [54][0] = 621; tabella_versioni [54][1] = 376; tabella_versioni [54][2] = 258;
			tabella_versioni [55][0] = 468; tabella_versioni [55][1] = 283; tabella_versioni [55][2] = 194;
			tabella_versioni [56][0] = 1250; tabella_versioni [56][1] = 758; tabella_versioni [56][2] = 520;
			tabella_versioni [57][0] = 991; tabella_versioni [57][1] = 600; tabella_versioni [57][2] = 412;
			tabella_versioni [58][0] = 703; tabella_versioni [58][1] = 426; tabella_versioni [58][2] = 292;
			tabella_versioni [59][0] = 530; tabella_versioni [59][1] = 321; tabella_versioni [59][2] = 220;
			tabella_versioni [60][0] = 1408; tabella_versioni [60][1] = 854; tabella_versioni [60][2] = 586;
			tabella_versioni [61][0] = 1082; tabella_versioni [61][1] = 656; tabella_versioni [61][2] = 450;
			tabella_versioni [62][0] = 775; tabella_versioni [62][1] = 470; tabella_versioni [62][2] = 322;
			tabella_versioni [63][0] = 602; tabella_versioni [63][1] = 365; tabella_versioni [63][2] = 250;
			tabella_versioni [64][0] = 1548; tabella_versioni [64][1] = 938; tabella_versioni [64][2] = 644;
			tabella_versioni [65][0] = 1212; tabella_versioni [65][1] = 734; tabella_versioni [65][2] = 504;
			tabella_versioni [66][0] = 876; tabella_versioni [66][1] = 531; tabella_versioni [66][2] = 364;
			tabella_versioni [67][0] = 674; tabella_versioni [67][1] = 408; tabella_versioni [67][2] = 280;
			tabella_versioni [68][0] = 1725; tabella_versioni [68][1] = 1046; tabella_versioni [68][2] = 718;
			tabella_versioni [69][0] = 1346; tabella_versioni [69][1] = 816; tabella_versioni [69][2] = 560;
			tabella_versioni [70][0] = 948; tabella_versioni [70][1] = 574; tabella_versioni [70][2] = 394;
			tabella_versioni [71][0] = 746; tabella_versioni [71][1] = 452; tabella_versioni [71][2] = 310;
			tabella_versioni [72][0] = 1903; tabella_versioni [72][1] = 1153; tabella_versioni [72][2] = 792;
			tabella_versioni [73][0] = 1500; tabella_versioni [73][1] = 909; tabella_versioni [73][2] = 624;
			tabella_versioni [74][0] = 1063; tabella_versioni [74][1] = 644; tabella_versioni [74][2] = 442;
			tabella_versioni [75][0] = 813; tabella_versioni [75][1] = 493; tabella_versioni [75][2] = 338;
			tabella_versioni [76][0] = 2061; tabella_versioni [76][1] = 1249; tabella_versioni [76][2] = 858;
			tabella_versioni [77][0] = 1600; tabella_versioni [77][1] = 970; tabella_versioni [77][2] = 666;
			tabella_versioni [78][0] = 1159; tabella_versioni [78][1] = 702; tabella_versioni [78][2] = 482;
			tabella_versioni [79][0] = 919; tabella_versioni [79][1] = 557; tabella_versioni [79][2] = 382;
			tabella_versioni [80][0] = 2232; tabella_versioni [80][1] = 1352; tabella_versioni [80][2] = 929;
			tabella_versioni [81][0] = 1708; tabella_versioni [81][1] = 1035; tabella_versioni [81][2] = 711;
			tabella_versioni [82][0] = 1224; tabella_versioni [82][1] = 742; tabella_versioni [82][2] = 509;
			tabella_versioni [83][0] = 969; tabella_versioni [83][1] = 587; tabella_versioni [83][2] = 403;
			tabella_versioni [84][0] = 2409; tabella_versioni [84][1] = 1460; tabella_versioni [84][2] = 1003;
			tabella_versioni [85][0] = 1872; tabella_versioni [85][1] = 1134; tabella_versioni [85][2] = 779;
			tabella_versioni [86][0] = 1358; tabella_versioni [86][1] = 823; tabella_versioni [86][2] = 565;
			tabella_versioni [87][0] = 1056; tabella_versioni [87][1] = 640; tabella_versioni [87][2] = 439;
			tabella_versioni [88][0] = 2620; tabella_versioni [88][1] = 1588; tabella_versioni [88][2] = 1091;
			tabella_versioni [89][0] = 2059; tabella_versioni [89][1] = 1248; tabella_versioni [89][2] = 857;
			tabella_versioni [90][0] = 1468; tabella_versioni [90][1] = 890; tabella_versioni [90][2] = 611;
			tabella_versioni [91][0] = 1108; tabella_versioni [91][1] = 672; tabella_versioni [91][2] = 461;
			tabella_versioni [92][0] = 2812; tabella_versioni [92][1] = 1704; tabella_versioni [92][2] = 1171;
			tabella_versioni [93][0] = 2188; tabella_versioni [93][1] = 1326; tabella_versioni [93][2] = 911;
			tabella_versioni [94][0] = 1588; tabella_versioni [94][1] = 963; tabella_versioni [94][2] = 661;
			tabella_versioni [95][0] = 1228; tabella_versioni [95][1] = 744; tabella_versioni [95][2] = 511;
			tabella_versioni [96][0] = 3057; tabella_versioni [96][1] = 1853; tabella_versioni [96][2] = 1273;
			tabella_versioni [97][0] = 2395; tabella_versioni [97][1] = 1451; tabella_versioni [97][2] = 997;
			tabella_versioni [98][0] = 1718; tabella_versioni [98][1] = 1041; tabella_versioni [98][2] = 715;
			tabella_versioni [99][0] = 1286; tabella_versioni [99][1] = 779; tabella_versioni [99][2] = 535;
			tabella_versioni [100][0] = 3283; tabella_versioni [100][1] = 1990; tabella_versioni [100][2] = 1367;
			tabella_versioni [101][0] = 2544; tabella_versioni [101][1] = 1542; tabella_versioni [101][2] = 1059;
			tabella_versioni [102][0] = 1804; tabella_versioni [102][1] = 1094; tabella_versioni [102][2] = 751;
			tabella_versioni [103][0] = 1425; tabella_versioni [103][1] = 864; tabella_versioni [103][2] = 593;
			tabella_versioni [104][0] = 3517; tabella_versioni [104][1] = 2132; tabella_versioni [104][2] = 1465;
			tabella_versioni [105][0] = 2701; tabella_versioni [105][1] = 1637; tabella_versioni [105][2] = 1125;
			tabella_versioni [106][0] = 1933; tabella_versioni [106][1] = 1172; tabella_versioni [106][2] = 805;
			tabella_versioni [107][0] = 1501; tabella_versioni [107][1] = 910; tabella_versioni [107][2] = 625;
			tabella_versioni [108][0] = 3669; tabella_versioni [108][1] = 2223; tabella_versioni [108][2] = 1528;
			tabella_versioni [109][0] = 2857; tabella_versioni [109][1] = 1732; tabella_versioni [109][2] = 1190;
			tabella_versioni [110][0] = 2085; tabella_versioni [110][1] = 1263; tabella_versioni [110][2] = 868;
			tabella_versioni [111][0] = 1581; tabella_versioni [111][1] = 958; tabella_versioni [111][2] = 658;
			tabella_versioni [112][0] = 3909; tabella_versioni [112][1] = 2369; tabella_versioni [112][2] = 1628;
			tabella_versioni [113][0] = 3035; tabella_versioni [113][1] = 1839; tabella_versioni [113][2] = 1264;
			tabella_versioni [114][0] = 2181; tabella_versioni [114][1] = 1322; tabella_versioni [114][2] = 908;
			tabella_versioni [115][0] = 1677; tabella_versioni [115][1] = 1016; tabella_versioni [115][2] = 698;
			tabella_versioni [116][0] = 4158; tabella_versioni [116][1] = 2520; tabella_versioni [116][2] = 1732;
			tabella_versioni [117][0] = 3289; tabella_versioni [117][1] = 1994; tabella_versioni [117][2] = 1370;
			tabella_versioni [118][0] = 2358; tabella_versioni [118][1] = 1429; tabella_versioni [118][2] = 982;
			tabella_versioni [119][0] = 1782; tabella_versioni [119][1] = 1080; tabella_versioni [119][2] = 742;
			tabella_versioni [120][0] = 4417; tabella_versioni [120][1] = 2677; tabella_versioni [120][2] = 1840;
			tabella_versioni [121][0] = 3486; tabella_versioni [121][1] = 2113; tabella_versioni [121][2] = 1452;
			tabella_versioni [122][0] = 2473; tabella_versioni [122][1] = 1499; tabella_versioni [122][2] = 1030;
			tabella_versioni [123][0] = 1897; tabella_versioni [123][1] = 1150; tabella_versioni [123][2] = 790;
			tabella_versioni [124][0] = 4686; tabella_versioni [124][1] = 2840; tabella_versioni [124][2] = 1952;
			tabella_versioni [125][0] = 3693; tabella_versioni [125][1] = 2238; tabella_versioni [125][2] = 1538;
			tabella_versioni [126][0] = 2670; tabella_versioni [126][1] = 1618; tabella_versioni [126][2] = 1112;
			tabella_versioni [127][0] = 2022; tabella_versioni [127][1] = 1226; tabella_versioni [127][2] = 842;
			tabella_versioni [128][0] = 4965; tabella_versioni [128][1] = 3009; tabella_versioni [128][2] = 2068;
			tabella_versioni [129][0] = 3909; tabella_versioni [129][1] = 2369; tabella_versioni [129][2] = 1628;
			tabella_versioni [130][0] = 2805; tabella_versioni [130][1] = 1700; tabella_versioni [130][2] = 1168;
			tabella_versioni [131][0] = 2157; tabella_versioni [131][1] = 1307; tabella_versioni [131][2] = 898;
			tabella_versioni [132][0] = 5253; tabella_versioni [132][1] = 3183; tabella_versioni [132][2] = 2188;
			tabella_versioni [133][0] = 4134; tabella_versioni [133][1] = 2506; tabella_versioni [133][2] = 1722;
			tabella_versioni [134][0] = 2949; tabella_versioni [134][1] = 1787; tabella_versioni [134][2] = 1228;
			tabella_versioni [135][0] = 2301; tabella_versioni [135][1] = 1394; tabella_versioni [135][2] = 958;
			tabella_versioni [136][0] = 5529; tabella_versioni [136][1] = 3351; tabella_versioni [136][2] = 2303;
			tabella_versioni [137][0] = 4343; tabella_versioni [137][1] = 2632; tabella_versioni [137][2] = 1809;
			tabella_versioni [138][0] = 3081; tabella_versioni [138][1] = 1867; tabella_versioni [138][2] = 1283;
			tabella_versioni [139][0] = 2361; tabella_versioni [139][1] = 1431; tabella_versioni [139][2] = 983;
			tabella_versioni [140][0] = 5836; tabella_versioni [140][1] = 3537; tabella_versioni [140][2] = 2431;
			tabella_versioni [141][0] = 4588; tabella_versioni [141][1] = 2780; tabella_versioni [141][2] = 1911;
			tabella_versioni [142][0] = 3244; tabella_versioni [142][1] = 1966; tabella_versioni [142][2] = 1351;
			tabella_versioni [143][0] = 2524; tabella_versioni [143][1] = 1530; tabella_versioni [143][2] = 1051;
			tabella_versioni [144][0] = 6153; tabella_versioni [144][1] = 3729; tabella_versioni [144][2] = 2563;
			tabella_versioni [145][0] = 4775; tabella_versioni [145][1] = 2894; tabella_versioni [145][2] = 1989;
			tabella_versioni [146][0] = 3417; tabella_versioni [146][1] = 2071; tabella_versioni [146][2] = 1423;
			tabella_versioni [147][0] = 2625; tabella_versioni [147][1] = 1591; tabella_versioni [147][2] = 1093;
			tabella_versioni [148][0] = 6479; tabella_versioni [148][1] = 3927; tabella_versioni [148][2] = 2699;
			tabella_versioni [149][0] = 5039; tabella_versioni [149][1] = 3054; tabella_versioni [149][2] = 2099;
			tabella_versioni [150][0] = 3599; tabella_versioni [150][1] = 2181; tabella_versioni [150][2] = 1499;
			tabella_versioni [151][0] = 2735; tabella_versioni [151][1] = 1658; tabella_versioni [151][2] = 1139;
			tabella_versioni [152][0] = 6743; tabella_versioni [152][1] = 4087; tabella_versioni [152][2] = 2809;
			tabella_versioni [153][0] = 5313; tabella_versioni [153][1] = 3220; tabella_versioni [153][2] = 2213;
			tabella_versioni [154][0] = 3791; tabella_versioni [154][1] = 2298; tabella_versioni [154][2] = 1579;
			tabella_versioni [155][0] = 2927; tabella_versioni [155][1] = 1774; tabella_versioni [155][2] = 1219;
			tabella_versioni [156][0] = 7089; tabella_versioni [156][1] = 4296; tabella_versioni [156][2] = 2953;
			tabella_versioni [157][0] = 5596; tabella_versioni [157][1] = 3391; tabella_versioni [157][2] = 2331;
			tabella_versioni [158][0] = 3993; tabella_versioni [158][1] = 2420; tabella_versioni [158][2] = 1663;
			tabella_versioni [159][0] = 3057; tabella_versioni [159][1] = 1852; tabella_versioni [159][2] = 1273;
		}

		void fillTabellaSizeCharacterCount(){
			tabella_size_character_count[0][0] = 10; tabella_size_character_count[0][1] = 9; tabella_size_character_count[0][2] = 8;   // versione 1  a versione 9
			tabella_size_character_count[1][0] = 12; tabella_size_character_count[1][1] = 11; tabella_size_character_count[0][2] = 16; // versione 10 a versione 26
			tabella_size_character_count[2][0] = 14; tabella_size_character_count[2][1] = 13; tabella_size_character_count[2][2] = 16; // versione 27 a versione 40
		}

		void fillNumeric(){
			numeric.insert(std::pair<char, int>('0', 0));
			numeric.insert(std::pair<char, int>('1', 1));
			numeric.insert(std::pair<char, int>('2', 2));
			numeric.insert(std::pair<char, int>('3', 3));
			numeric.insert(std::pair<char, int>('4', 4));
			numeric.insert(std::pair<char, int>('5', 5));
			numeric.insert(std::pair<char, int>('6', 6));
			numeric.insert(std::pair<char, int>('7', 7));
			numeric.insert(std::pair<char, int>('8', 8));
			numeric.insert(std::pair<char, int>('9', 9));
		}

		void fillAlphaNumeric(){
			alphaNumeric.insert(std::pair<char, int>('0', 0));
			alphaNumeric.insert(std::pair<char, int>('1', 1));
			alphaNumeric.insert(std::pair<char, int>('2', 2));
			alphaNumeric.insert(std::pair<char, int>('3', 3));
			alphaNumeric.insert(std::pair<char, int>('4', 4));
			alphaNumeric.insert(std::pair<char, int>('5', 5));
			alphaNumeric.insert(std::pair<char, int>('6', 6));
			alphaNumeric.insert(std::pair<char, int>('7', 7));
			alphaNumeric.insert(std::pair<char, int>('8', 8));
			alphaNumeric.insert(std::pair<char, int>('9', 9));	
			alphaNumeric.insert(std::pair<char, int>('A', 10));
			alphaNumeric.insert(std::pair<char, int>('B', 11));
			alphaNumeric.insert(std::pair<char, int>('C', 12));
			alphaNumeric.insert(std::pair<char, int>('D', 13));
			alphaNumeric.insert(std::pair<char, int>('E', 14));
			alphaNumeric.insert(std::pair<char, int>('F', 15));
			alphaNumeric.insert(std::pair<char, int>('G', 16));
			alphaNumeric.insert(std::pair<char, int>('H', 17));
			alphaNumeric.insert(std::pair<char, int>('I', 18));
			alphaNumeric.insert(std::pair<char, int>('J', 19));
			alphaNumeric.insert(std::pair<char, int>('K', 20));
			alphaNumeric.insert(std::pair<char, int>('L', 21));
			alphaNumeric.insert(std::pair<char, int>('M', 22));
			alphaNumeric.insert(std::pair<char, int>('N', 23));
			alphaNumeric.insert(std::pair<char, int>('O', 24));
			alphaNumeric.insert(std::pair<char, int>('P', 25));
			alphaNumeric.insert(std::pair<char, int>('Q', 26));
			alphaNumeric.insert(std::pair<char, int>('R', 27));
			alphaNumeric.insert(std::pair<char, int>('S', 28));
			alphaNumeric.insert(std::pair<char, int>('T', 29));
			alphaNumeric.insert(std::pair<char, int>('U', 30));
			alphaNumeric.insert(std::pair<char, int>('V', 31));
			alphaNumeric.insert(std::pair<char, int>('W', 32));
			alphaNumeric.insert(std::pair<char, int>('X', 33));
			alphaNumeric.insert(std::pair<char, int>('Y', 34));
			alphaNumeric.insert(std::pair<char, int>('Z', 35));
			alphaNumeric.insert(std::pair<char, int>(' ', 36));
			alphaNumeric.insert(std::pair<char, int>('$', 37));
			alphaNumeric.insert(std::pair<char, int>('%', 38));
			alphaNumeric.insert(std::pair<char, int>('*', 39));
			alphaNumeric.insert(std::pair<char, int>('+', 40));
			alphaNumeric.insert(std::pair<char, int>('-', 41));
			alphaNumeric.insert(std::pair<char, int>('.', 42));
			alphaNumeric.insert(std::pair<char, int>('/', 43));
			alphaNumeric.insert(std::pair<char, int>(':', 44));
		}

		void fillByte(){
			byte.insert(std::pair<char, int>(' ', 32));
			byte.insert(std::pair<char, int>('!', 33));
			byte.insert(std::pair<char, int>('"', 34));
			byte.insert(std::pair<char, int>('#', 35));
			byte.insert(std::pair<char, int>('$', 36));
			byte.insert(std::pair<char, int>('%', 37));
			byte.insert(std::pair<char, int>('&', 38));
			byte.insert(std::pair<char, int>('\'', 39));
			byte.insert(std::pair<char, int>('(', 40));
			byte.insert(std::pair<char, int>(')', 41));
			byte.insert(std::pair<char, int>('*', 42));
			byte.insert(std::pair<char, int>('+', 43));
			byte.insert(std::pair<char, int>(',', 44));
			byte.insert(std::pair<char, int>('-', 45));
			byte.insert(std::pair<char, int>('.', 46));
			byte.insert(std::pair<char, int>('/', 47));
			byte.insert(std::pair<char, int>('0', 48));
			byte.insert(std::pair<char, int>('1', 49));
			byte.insert(std::pair<char, int>('2', 50));
			byte.insert(std::pair<char, int>('3', 51));
			byte.insert(std::pair<char, int>('4', 52));
			byte.insert(std::pair<char, int>('5', 53));
			byte.insert(std::pair<char, int>('6', 54));
			byte.insert(std::pair<char, int>('7', 55));
			byte.insert(std::pair<char, int>('8', 56));
			byte.insert(std::pair<char, int>('9', 57));
			byte.insert(std::pair<char, int>(':', 58));
			byte.insert(std::pair<char, int>(';', 59));
			byte.insert(std::pair<char, int>('<', 60));
			byte.insert(std::pair<char, int>('=', 61));
			byte.insert(std::pair<char, int>('>', 62));
			byte.insert(std::pair<char, int>('?', 63));
			byte.insert(std::pair<char, int>('@', 64));
			byte.insert(std::pair<char, int>('A', 65));
			byte.insert(std::pair<char, int>('B', 66));
			byte.insert(std::pair<char, int>('C', 67));
			byte.insert(std::pair<char, int>('D', 68));
			byte.insert(std::pair<char, int>('E', 69));
			byte.insert(std::pair<char, int>('F', 70));
			byte.insert(std::pair<char, int>('G', 71));
			byte.insert(std::pair<char, int>('H', 72));
			byte.insert(std::pair<char, int>('I', 73));
			byte.insert(std::pair<char, int>('J', 74));
			byte.insert(std::pair<char, int>('K', 75));
			byte.insert(std::pair<char, int>('L', 76));
			byte.insert(std::pair<char, int>('M', 77));
			byte.insert(std::pair<char, int>('N', 78));
			byte.insert(std::pair<char, int>('O', 79));
			byte.insert(std::pair<char, int>('P', 80));
			byte.insert(std::pair<char, int>('Q', 81));
			byte.insert(std::pair<char, int>('R', 82));
			byte.insert(std::pair<char, int>('S', 83));
			byte.insert(std::pair<char, int>('T', 84));
			byte.insert(std::pair<char, int>('U', 85));
			byte.insert(std::pair<char, int>('V', 86));
			byte.insert(std::pair<char, int>('W', 87));
			byte.insert(std::pair<char, int>('X', 88));
			byte.insert(std::pair<char, int>('Y', 89));
			byte.insert(std::pair<char, int>('Z', 90));
			byte.insert(std::pair<char, int>('[', 91));
			byte.insert(std::pair<char, int>('\\', 92));
			byte.insert(std::pair<char, int>(']', 93));
			byte.insert(std::pair<char, int>('^', 94));
			byte.insert(std::pair<char, int>('_', 95));
			byte.insert(std::pair<char, int>('`', 96));
			byte.insert(std::pair<char, int>('a', 97));
			byte.insert(std::pair<char, int>('b', 98));
			byte.insert(std::pair<char, int>('c', 99));
			byte.insert(std::pair<char, int>('d', 100));
			byte.insert(std::pair<char, int>('e', 101));
			byte.insert(std::pair<char, int>('f', 102));
			byte.insert(std::pair<char, int>('g', 103));
			byte.insert(std::pair<char, int>('h', 104));
			byte.insert(std::pair<char, int>('i', 105));
			byte.insert(std::pair<char, int>('j', 106));
			byte.insert(std::pair<char, int>('k', 107));
			byte.insert(std::pair<char, int>('l', 108));
			byte.insert(std::pair<char, int>('m', 109));
			byte.insert(std::pair<char, int>('n', 110));
			byte.insert(std::pair<char, int>('o', 111));
			byte.insert(std::pair<char, int>('p', 112));
			byte.insert(std::pair<char, int>('q', 113));
			byte.insert(std::pair<char, int>('r', 114));
			byte.insert(std::pair<char, int>('s', 115));
			byte.insert(std::pair<char, int>('t', 116));
			byte.insert(std::pair<char, int>('u', 117));
			byte.insert(std::pair<char, int>('v', 118));
			byte.insert(std::pair<char, int>('w', 119));
			byte.insert(std::pair<char, int>('x', 120));
			byte.insert(std::pair<char, int>('y', 121));
			byte.insert(std::pair<char, int>('z', 122));
			byte.insert(std::pair<char, int>('{', 123));
			byte.insert(std::pair<char, int>('|', 124));
			byte.insert(std::pair<char, int>('}', 125));
			byte.insert(std::pair<char, int>('~', 126));
		}

		void fillModeIncator(){
			mode_indicator.insert(std::pair<int, std::string>(0,NUMERIC_INDICATOR)); // for numeric mode is 0001
			mode_indicator.insert(std::pair<int, std::string>(1,ALPHANUMERIC_INDICATOR)); // for ALphanumeric mode is 0010
			mode_indicator.insert(std::pair<int, std::string>(2,BYTE_INDICATOR)); // for byte mode is 0100
		}
	
		void fillErrorCorrectionTable(){
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("1-L",{19,7,1,19,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("1-M",{16,10,1,16,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("1-Q",{13,13,1,13,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("1-H",{9,17,1,9,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("2-L",{34,10,1,34,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("2-M",{28,16,1,28,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("2-Q",{22,22,1,22,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("2-H",{16,28,1,16,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("3-L",{55,15,1,55,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("3-M",{44,26,1,44,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("3-Q",{34,18,2,17,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("3-H",{26,22,2,13,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("4-L",{80,20,1,80,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("4-M",{64,18,2,32,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("4-Q",{48,26,2,24,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("4-H",{36,16,4,9,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("5-L",{108,26,1,108,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("5-M",{86,24,2,43,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("5-Q",{62,18,2,15,2,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("5-H",{46,22,2,11,2,12}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("6-L",{136,18,2,68,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("6-M",{108,16,4,27,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("6-Q",{76,24,4,19,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("6-H",{60,28,4,15,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("7-L",{156,20,2,78,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("7-M",{124,18,4,31,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("7-Q",{88,18,2,14,4,15}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("7-H",{66,26,4,13,1,14}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("8-L",{194,24,2,97,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("8-M",{154,22,2,38,2,39}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("8-Q",{110,22,4,18,2,19}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("8-H",{86,26,4,14,2,15}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("9-L",{232,30,2,116,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("9-M",{182,22,3,36,2,37}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("9-Q",{132,20,4,16,4,17}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("9-H",{100,24,4,12,4,13}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("10-L",{274,18,2,68,2,69}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("10-M",{216,26,4,43,1,44}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("10-Q",{154,24,6,19,2,20}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("10-H",{122,28,6,15,2,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("11-L",{324,20,4,81,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("11-M",{254,30,1,50,4,51}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("11-Q",{180,28,4,22,4,23}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("11-H",{140,24,3,12,8,13}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("12-L",{370,24,2,92,2,93}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("12-M",{290,22,6,36,2,37}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("12-Q",{206,26,4,20,6,21}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("12-H",{158,28,7,14,4,15}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("13-L",{428,26,4,107,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("13-M",{334,22,8,37,1,38}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("13-Q",{244,24,8,20,4,21}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("13-H",{180,22,12,11,4,12}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("14-L",{461,30,3,115,1,116}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("14-M",{365,24,4,40,5,41}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("14-Q",{261,20,11,16,5,17}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("14-H",{197,24,11,12,5,13}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("15-L",{523,22,5,87,1,88}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("15-M",{415,24,5,41,5,42}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("15-Q",{295,30,5,24,7,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("15-H",{223,24,11,12,7,13}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("16-L",{589,24,5,98,1,99}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("16-M",{453,28,7,45,3,46}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("16-Q",{325,24,15,19,2,20}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("16-H",{253,30,3,15,13,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("17-L",{647,28,1,107,5,108}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("17-M",{507,28,10,46,1,47}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("17-Q",{367,28,1,22,15,23}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("17-H",{283,28,2,14,17,15}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("18-L",{721,30,5,120,1,121}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("18-M",{563,26,9,43,4,44}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("18-Q",{397,28,17,22,1,23}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("18-H",{313,28,2,14,19,15}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("19-L",{795,28,3,113,4,114}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("19-M",{627,26,3,44,11,45}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("19-Q",{445,26,17,21,4,22}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("19-H",{341,26,9,13,16,14}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("20-L",{861,28,3,107,5,108}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("20-M",{669,26,3,41,13,42}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("20-Q",{485,30,15,24,5,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("20-H",{385,28,15,15,10,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("21-L",{932,28,4,116,4,117}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("21-M",{714,26,17,42,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("21-Q",{512,28,17,22,6,23}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("21-H",{406,30,19,16,6,17}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("22-L",{1006,28,2,111,7,112}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("22-M",{782,28,17,46,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("22-Q",{568,30,7,24,16,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("22-H",{442,24,34,13,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("23-L",{1094,30,4,121,5,122}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("23-M",{860,28,4,47,14,48}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("23-Q",{614,30,11,24,14,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("23-H",{464,30,16,15,14,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("24-L",{1174,30,6,117,4,118}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("24-M",{914,28,6,45,14,46}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("24-Q",{664,30,11,24,16,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("24-H",{514,30,30,16,2,17}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("25-L",{1276,26,8,106,4,107}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("25-M",{1000,28,8,47,13,48}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("25-Q",{718,30,7,24,22,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("25-H",{538,30,22,15,13,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("26-L",{1370,28,10,114,2,115}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("26-M",{1062,28,19,46,4,47}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("26-Q",{754,28,28,22,6,23}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("26-H",{596,30,33,16,4,17}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("27-L",{1468,30,8,122,4,123}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("27-M",{1128,28,22,45,3,46}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("27-Q",{808,30,8,23,26,24}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("27-H",{628,30,12,15,28,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("28-L",{1531,30,3,117,10,118}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("28-M",{1193,28,3,45,23,46}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("28-Q",{871,30,4,24,31,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("28-H",{661,30,11,15,31,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("29-L",{1631,30,7,116,7,117}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("29-M",{1267,28,21,45,7,46}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("29-Q",{911,30,1,23,37,24}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("29-H",{701,30,19,15,26,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("30-L",{1735,30,5,115,10,116}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("30-M",{1373,28,19,47,10,48}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("30-Q",{985,30,15,24,25,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("30-H",{745,30,23,15,25,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("31-L",{1843,30,13,115,3,116}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("31-M",{1455,28,2,46,29,47}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("31-Q",{1033,30,42,24,1,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("31-H",{793,30,23,15,28,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("32-L",{1955,30,17,115,0,0}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("32-M",{1541,28,10,46,23,47}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("32-Q",{1115,30,10,24,35,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("32-H",{845,30,19,15,35,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("33-L",{2071,30,17,115,1,116}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("33-M",{1631,28,14,46,21,47}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("33-Q",{1171,30,29,24,19,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("33-H",{901,30,11,15,46,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("34-L",{2191,30,13,115,6,116}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("34-M",{1725,28,14,46,23,47}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("34-Q",{1231,30,44,24,7,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("34-H",{961,30,59,16,1,17}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("35-L",{2306,30,12,121,7,122}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("35-M",{1812,28,12,47,26,48}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("35-Q",{1286,30,39,24,14,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("35-H",{986,30,22,15,41,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("36-L",{2434,30,6,121,14,122}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("36-M",{1914,28,6,47,34,48}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("36-Q",{1354,30,46,24,10,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("36-H",{1054,30,2,15,64,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("37-L",{2566,30,17,122,4,123}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("37-M",{1992,28,29,46,14,47}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("37-Q",{1426,30,49,24,10,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("37-H",{1096,30,24,15,46,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("38-L",{2702,30,4,122,18,123}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("38-M",{2102,28,13,46,32,47}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("38-Q",{1502,30,48,24,14,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("38-H",{1142,30,42,15,32,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("39-L",{2812,30,20,117,4,118}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("39-M",{2216,28,40,47,7,48}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("39-Q",{1582,30,43,24,22,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("39-H",{1222,30,10,15,67,16}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("40-L",{2956,30,19,118,6,119}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("40-M",{2334,28,18,47,31,48}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("40-Q",{1666,30,34,24,34,25}));
			errorCorrectionTable.insert(std::pair<std::string, ErrorCorrectionParam>("40-H",{1276,30,20,15,61,16}));
		}

		void fillLogAntiLogTable(){
			int i=1;
			int value = 2;

			logAntiLogTable.insert(std::pair<int, int>(0,1));

			while(i<LOGANTILOG_SIZE){
				logAntiLogTable.insert(std::pair<int, int>(i,value)); 
				value *= 2;
				if(value >= LOGANTILOG_SIZE){
					value = value ^ BITWISE_MODULO;
				}
				i++;
			}
		}
	
		void fillRemainderBits(){
			remainderBits.insert(std::pair<int, int>(0, 0));
			remainderBits.insert(std::pair<int, int>(1, 7));
			remainderBits.insert(std::pair<int, int>(2, 7));
			remainderBits.insert(std::pair<int, int>(3, 7));
			remainderBits.insert(std::pair<int, int>(4, 7));
			remainderBits.insert(std::pair<int, int>(5, 7));
			remainderBits.insert(std::pair<int, int>(6, 0));
			remainderBits.insert(std::pair<int, int>(7, 0));
			remainderBits.insert(std::pair<int, int>(8, 0));
			remainderBits.insert(std::pair<int, int>(9, 0));
			remainderBits.insert(std::pair<int, int>(10, 0));
			remainderBits.insert(std::pair<int, int>(11, 0));
			remainderBits.insert(std::pair<int, int>(12, 0));
			remainderBits.insert(std::pair<int, int>(13, 3));
			remainderBits.insert(std::pair<int, int>(14, 3));
			remainderBits.insert(std::pair<int, int>(15, 3));
			remainderBits.insert(std::pair<int, int>(16, 3));
			remainderBits.insert(std::pair<int, int>(17, 3));
			remainderBits.insert(std::pair<int, int>(18, 3));
			remainderBits.insert(std::pair<int, int>(19, 3));
			remainderBits.insert(std::pair<int, int>(20, 4));
			remainderBits.insert(std::pair<int, int>(21, 4));
			remainderBits.insert(std::pair<int, int>(22, 4));
			remainderBits.insert(std::pair<int, int>(23, 4));
			remainderBits.insert(std::pair<int, int>(24, 4));
			remainderBits.insert(std::pair<int, int>(25, 4));
			remainderBits.insert(std::pair<int, int>(26, 4));
			remainderBits.insert(std::pair<int, int>(27, 3));
			remainderBits.insert(std::pair<int, int>(28, 3));
			remainderBits.insert(std::pair<int, int>(29, 3));
			remainderBits.insert(std::pair<int, int>(30, 3));
			remainderBits.insert(std::pair<int, int>(31, 3));
			remainderBits.insert(std::pair<int, int>(32, 3));
			remainderBits.insert(std::pair<int, int>(33, 3));
			remainderBits.insert(std::pair<int, int>(34, 0));
			remainderBits.insert(std::pair<int, int>(35, 0));
			remainderBits.insert(std::pair<int, int>(36, 0));
			remainderBits.insert(std::pair<int, int>(37, 0));
			remainderBits.insert(std::pair<int, int>(38, 0));
			remainderBits.insert(std::pair<int, int>(39, 0));
		}
	
		void fillalignmentPatternLocation(){
			alignmentPatternLocation[0][0] = 0; alignmentPatternLocation[0][1] = 0; alignmentPatternLocation[0][2] = 0; alignmentPatternLocation[0][3] = 0; alignmentPatternLocation[0][4] = 0; alignmentPatternLocation[0][5] = 0; alignmentPatternLocation[0][6] = 0; 
			alignmentPatternLocation[1][0] = 6; alignmentPatternLocation[1][1] = 18; alignmentPatternLocation[1][2] = 0; alignmentPatternLocation[1][3] = 0; alignmentPatternLocation[1][4] = 0; alignmentPatternLocation[1][5] = 0; alignmentPatternLocation[1][6] = 0;
			alignmentPatternLocation[2][0] = 6; alignmentPatternLocation[2][1] = 22; alignmentPatternLocation[2][2] = 0; alignmentPatternLocation[2][3] = 0; alignmentPatternLocation[2][4] = 0; alignmentPatternLocation[2][5] = 0; alignmentPatternLocation[2][6] = 0;
			alignmentPatternLocation[3][0] = 6; alignmentPatternLocation[3][1] = 26; alignmentPatternLocation[3][2] = 0; alignmentPatternLocation[3][3] = 0; alignmentPatternLocation[3][4] = 0; alignmentPatternLocation[3][5] = 0; alignmentPatternLocation[3][6] = 0;
			alignmentPatternLocation[4][0] = 6; alignmentPatternLocation[4][1] = 30; alignmentPatternLocation[4][2] = 0; alignmentPatternLocation[4][3] = 0; alignmentPatternLocation[4][4] = 0; alignmentPatternLocation[4][5] = 0; alignmentPatternLocation[4][6] = 0;
			alignmentPatternLocation[5][0] = 6; alignmentPatternLocation[5][1] = 34; alignmentPatternLocation[5][2] = 0; alignmentPatternLocation[5][3] = 0; alignmentPatternLocation[5][4] = 0; alignmentPatternLocation[5][5] = 0; alignmentPatternLocation[5][6] = 0;
			alignmentPatternLocation[6][0] = 6; alignmentPatternLocation[6][1] = 22; alignmentPatternLocation[6][2] = 38; alignmentPatternLocation[6][3] = 0; alignmentPatternLocation[6][4] = 0; alignmentPatternLocation[6][5] = 0; alignmentPatternLocation[6][6] = 0;
			alignmentPatternLocation[7][0] = 6; alignmentPatternLocation[7][1] = 24; alignmentPatternLocation[7][2] = 42; alignmentPatternLocation[7][3] = 0; alignmentPatternLocation[7][4] = 0; alignmentPatternLocation[7][5] = 0; alignmentPatternLocation[7][6] = 0;
			alignmentPatternLocation[8][0] = 6; alignmentPatternLocation[8][1] = 26; alignmentPatternLocation[8][2] = 46; alignmentPatternLocation[8][3] = 0; alignmentPatternLocation[8][4] = 0; alignmentPatternLocation[8][5] = 0; alignmentPatternLocation[8][6] = 0;
			alignmentPatternLocation[9][0] = 6; alignmentPatternLocation[9][1] = 28; alignmentPatternLocation[9][2] = 50; alignmentPatternLocation[9][3] = 0; alignmentPatternLocation[9][4] = 0; alignmentPatternLocation[9][5] = 0; alignmentPatternLocation[9][6] = 0;
			alignmentPatternLocation[10][0] = 6; alignmentPatternLocation[10][1] = 30; alignmentPatternLocation[10][2] = 54; alignmentPatternLocation[10][3] = 0; alignmentPatternLocation[10][4] = 0; alignmentPatternLocation[10][5] = 0; alignmentPatternLocation[10][6] = 0;
			alignmentPatternLocation[11][0] = 6; alignmentPatternLocation[11][1] = 32; alignmentPatternLocation[11][2] = 58; alignmentPatternLocation[11][3] = 0; alignmentPatternLocation[11][4] = 0; alignmentPatternLocation[11][5] = 0; alignmentPatternLocation[11][6] = 0;
			alignmentPatternLocation[12][0] = 6; alignmentPatternLocation[12][1] = 34; alignmentPatternLocation[12][2] = 62; alignmentPatternLocation[12][3] = 0; alignmentPatternLocation[12][4] = 0; alignmentPatternLocation[12][5] = 0; alignmentPatternLocation[12][6] = 0;
			alignmentPatternLocation[13][0] = 6; alignmentPatternLocation[13][1] = 26; alignmentPatternLocation[13][2] = 46; alignmentPatternLocation[13][3] = 66; alignmentPatternLocation[13][4] = 0; alignmentPatternLocation[13][5] = 0; alignmentPatternLocation[13][6] = 0;
			alignmentPatternLocation[14][0] = 6; alignmentPatternLocation[14][1] = 26; alignmentPatternLocation[14][2] = 48; alignmentPatternLocation[14][3] = 70; alignmentPatternLocation[14][4] = 0; alignmentPatternLocation[14][5] = 0; alignmentPatternLocation[14][6] = 0;
			alignmentPatternLocation[15][0] = 6; alignmentPatternLocation[15][1] = 26; alignmentPatternLocation[15][2] = 50; alignmentPatternLocation[15][3] = 74; alignmentPatternLocation[15][4] = 0; alignmentPatternLocation[15][5] = 0; alignmentPatternLocation[15][6] = 0;
			alignmentPatternLocation[16][0] = 6; alignmentPatternLocation[16][1] = 30; alignmentPatternLocation[16][2] = 54; alignmentPatternLocation[16][3] = 78; alignmentPatternLocation[16][4] = 0; alignmentPatternLocation[16][5] = 0; alignmentPatternLocation[16][6] = 0;
			alignmentPatternLocation[17][0] = 6; alignmentPatternLocation[17][1] = 30; alignmentPatternLocation[17][2] = 56; alignmentPatternLocation[17][3] = 82; alignmentPatternLocation[17][4] = 0; alignmentPatternLocation[17][5] = 0; alignmentPatternLocation[17][6] = 0;
			alignmentPatternLocation[18][0] = 6; alignmentPatternLocation[18][1] = 30; alignmentPatternLocation[18][2] = 58; alignmentPatternLocation[18][3] = 86; alignmentPatternLocation[18][4] = 0; alignmentPatternLocation[18][5] = 0; alignmentPatternLocation[18][6] = 0;
			alignmentPatternLocation[19][0] = 6; alignmentPatternLocation[19][1] = 34; alignmentPatternLocation[19][2] = 62; alignmentPatternLocation[19][3] = 90; alignmentPatternLocation[19][4] = 0; alignmentPatternLocation[19][5] = 0; alignmentPatternLocation[19][6] = 0;
			alignmentPatternLocation[20][0] = 6; alignmentPatternLocation[20][1] = 28; alignmentPatternLocation[20][2] = 50; alignmentPatternLocation[20][3] = 72; alignmentPatternLocation[20][4] = 94; alignmentPatternLocation[20][5] = 0; alignmentPatternLocation[20][6] = 0;
			alignmentPatternLocation[21][0] = 6; alignmentPatternLocation[21][1] = 26; alignmentPatternLocation[21][2] = 50; alignmentPatternLocation[21][3] = 74; alignmentPatternLocation[21][4] = 98; alignmentPatternLocation[21][5] = 0; alignmentPatternLocation[21][6] = 0;
			alignmentPatternLocation[22][0] = 6; alignmentPatternLocation[22][1] = 30; alignmentPatternLocation[22][2] = 54; alignmentPatternLocation[22][3] = 78; alignmentPatternLocation[22][4] = 102; alignmentPatternLocation[22][5] = 0; alignmentPatternLocation[22][6] = 0;
			alignmentPatternLocation[23][0] = 6; alignmentPatternLocation[23][1] = 28; alignmentPatternLocation[23][2] = 54; alignmentPatternLocation[23][3] = 80; alignmentPatternLocation[23][4] = 106; alignmentPatternLocation[23][5] = 0; alignmentPatternLocation[23][6] = 0;
			alignmentPatternLocation[24][0] = 6; alignmentPatternLocation[24][1] = 32; alignmentPatternLocation[24][2] = 58; alignmentPatternLocation[24][3] = 84; alignmentPatternLocation[24][4] = 110; alignmentPatternLocation[24][5] = 0; alignmentPatternLocation[24][6] = 0;
			alignmentPatternLocation[25][0] = 6; alignmentPatternLocation[25][1] = 30; alignmentPatternLocation[25][2] = 58; alignmentPatternLocation[25][3] = 86; alignmentPatternLocation[25][4] = 114; alignmentPatternLocation[25][5] = 0; alignmentPatternLocation[25][6] = 0;
			alignmentPatternLocation[26][0] = 6; alignmentPatternLocation[26][1] = 34; alignmentPatternLocation[26][2] = 62; alignmentPatternLocation[26][3] = 90; alignmentPatternLocation[26][4] = 118; alignmentPatternLocation[26][5] = 0; alignmentPatternLocation[26][6] = 0;
			alignmentPatternLocation[27][0] = 6; alignmentPatternLocation[27][1] = 26; alignmentPatternLocation[27][2] = 50; alignmentPatternLocation[27][3] = 74; alignmentPatternLocation[27][4] = 98; alignmentPatternLocation[27][5] = 122; alignmentPatternLocation[27][6] = 0;
			alignmentPatternLocation[28][0] = 6; alignmentPatternLocation[28][1] = 30; alignmentPatternLocation[28][2] = 54; alignmentPatternLocation[28][3] = 78; alignmentPatternLocation[28][4] = 102; alignmentPatternLocation[28][5] = 126; alignmentPatternLocation[28][6] = 0;
			alignmentPatternLocation[29][0] = 6; alignmentPatternLocation[29][1] = 26; alignmentPatternLocation[29][2] = 52; alignmentPatternLocation[29][3] = 78; alignmentPatternLocation[29][4] = 104; alignmentPatternLocation[29][5] = 130; alignmentPatternLocation[29][6] = 0;
			alignmentPatternLocation[30][0] = 6; alignmentPatternLocation[30][1] = 30; alignmentPatternLocation[30][2] = 56; alignmentPatternLocation[30][3] = 82; alignmentPatternLocation[30][4] = 108; alignmentPatternLocation[30][5] = 134; alignmentPatternLocation[30][6] = 0;
			alignmentPatternLocation[31][0] = 6; alignmentPatternLocation[31][1] = 34; alignmentPatternLocation[31][2] = 60; alignmentPatternLocation[31][3] = 86; alignmentPatternLocation[31][4] = 112; alignmentPatternLocation[31][5] = 138; alignmentPatternLocation[31][6] = 0;
			alignmentPatternLocation[32][0] = 6; alignmentPatternLocation[32][1] = 30; alignmentPatternLocation[32][2] = 58; alignmentPatternLocation[32][3] = 86; alignmentPatternLocation[32][4] = 114; alignmentPatternLocation[32][5] = 142; alignmentPatternLocation[32][6] = 0;
			alignmentPatternLocation[33][0] = 6; alignmentPatternLocation[33][1] = 34; alignmentPatternLocation[33][2] = 62; alignmentPatternLocation[33][3] = 90; alignmentPatternLocation[33][4] = 118; alignmentPatternLocation[33][5] = 146; alignmentPatternLocation[33][6] = 0;
			alignmentPatternLocation[34][0] = 6; alignmentPatternLocation[34][1] = 30; alignmentPatternLocation[34][2] = 54; alignmentPatternLocation[34][3] = 78; alignmentPatternLocation[34][4] = 102; alignmentPatternLocation[34][5] = 126; alignmentPatternLocation[34][6] = 150;
			alignmentPatternLocation[35][0] = 6; alignmentPatternLocation[35][1] = 24; alignmentPatternLocation[35][2] = 50; alignmentPatternLocation[35][3] = 76; alignmentPatternLocation[35][4] = 102; alignmentPatternLocation[35][5] = 128; alignmentPatternLocation[35][6] = 154;
			alignmentPatternLocation[36][0] = 6; alignmentPatternLocation[36][1] = 28; alignmentPatternLocation[36][2] = 54; alignmentPatternLocation[36][3] = 80; alignmentPatternLocation[36][4] = 106; alignmentPatternLocation[36][5] = 132; alignmentPatternLocation[36][6] = 158;
			alignmentPatternLocation[37][0] = 6; alignmentPatternLocation[37][1] = 32; alignmentPatternLocation[37][2] = 58; alignmentPatternLocation[37][3] = 84; alignmentPatternLocation[37][4] = 110; alignmentPatternLocation[37][5] = 136; alignmentPatternLocation[37][6] = 162;
			alignmentPatternLocation[38][0] = 6; alignmentPatternLocation[38][1] = 26; alignmentPatternLocation[38][2] = 54; alignmentPatternLocation[38][3] = 82; alignmentPatternLocation[38][4] = 110; alignmentPatternLocation[38][5] = 138; alignmentPatternLocation[38][6] = 166;
			alignmentPatternLocation[39][0] = 6; alignmentPatternLocation[39][1] = 30; alignmentPatternLocation[39][2] = 58; alignmentPatternLocation[39][3] = 86; alignmentPatternLocation[39][4] = 114; alignmentPatternLocation[39][5] = 142; alignmentPatternLocation[39][6] = 170;
		}
	public:
		QrCodeMaterial(){
			fillTabellaVersioni();
			fillTabellaSizeCharacterCount();
			fillNumeric();
			fillAlphaNumeric();
			fillByte();
			fillModeIncator();
			fillErrorCorrectionTable();
			fillLogAntiLogTable();
			fillRemainderBits();
			fillalignmentPatternLocation();
		}

		int getNumericCar(char car){
			if(searchNumericMap(car)){
				return numeric.find(car)->second;
			}
			return -1;
		}

		int getAlphaNumericCar(char car){
			if(searchAlphaNumericMap(car)){
				return alphaNumeric.find(car)->second; 
			}
			return -1;
		}

		int getByteCar(char car){
			if(searchByteMap(car)){
				return byte.find(car)->second;
			}
			return -1;
		}

		int getCodeWordInfo(int version, livelli lvl, int param){
			std::string s;
			switch(lvl){
				case 0:
					s = std::to_string(version+1) + "-L";
					break;
				case 1:
					s = std::to_string(version+1) + "-M";
					break;
				case 2:
					s = std::to_string(version+1) + "-Q";
					break;
				case 3:
					s = std::to_string(version+1) + "-H";
					break;
			}


			if(errorCorrectionTable.count(s)>0){
				switch(param){
					case TOTAL_CODEWORDS:
						return errorCorrectionTable.find(s)->second.total_codewords;
						break;
					case EC_CODEWORDS_PER_BLOCK:
						return errorCorrectionTable.find(s)->second.ec_codewords_per_block;
						break;
					case FIRST_GROUP_BLOCK_SIZE:
						return errorCorrectionTable.find(s)->second.first_group_block_size;
						break;
					case FIRST_BLOCK_SIZE:
						return errorCorrectionTable.find(s)->second.first_block_size;
						break;
					case SECOND_GROUP_BLOCK_SIZE:
						return errorCorrectionTable.find(s)->second.second_group_block_size;
						break;
					case SECOND_BLOCK_SIZE:
						return errorCorrectionTable.find(s)->second.second_block_size;
						break;
				} 
			}
			return -1;
		}

		bool searchNumericMap(char car){
			if(numeric.count(car)>0)
				return true;	
			return false;
		}
		
		bool searchAlphaNumericMap(char car){
			if(alphaNumeric.count(car)>0)
				return true;	
			return false;
		}

		bool searchByteMap(char car){
			if(byte.count(car)>0)
				return true;	
			return false;
		}

		int getVersionCapacity(int line_index, int colon_index){
			if(line_index < VERSIONI_RIGHE && colon_index < VERSIONI_COLONNE)
				return tabella_versioni[line_index][colon_index];
			return -1;
		}

		std::string getModeIndicator(int mode){ // returns the mode indicator in binary
			if(mode_indicator.count(mode)>0){
				return mode_indicator.find(mode)->second;
			}
			return NULL;
		}

		int getCharacterCountSize(int version, int mode){ // return mode the size of the character cout based on mode and version
			if((version>=0 && version <40) && mode>=0){
				if(version>=0 && version<=9)
					return tabella_size_character_count[0][mode];
				else if(version>=10 && version<=26)
					return tabella_size_character_count[1][mode];
				else 
					return tabella_size_character_count[2][mode];
			}
			return -1;
		}

		int getExponentToValue(int exponent){
			if(logAntiLogTable.count(exponent)>0){
				return logAntiLogTable.find(exponent)->second;
			}	
			return -1;
		}

		int geValueToExponent(int alpha){
			if(alpha>0 && alpha<LOGANTILOG_SIZE){
				int i=0;
				while(i<LOGANTILOG_SIZE){
					if(logAntiLogTable.find(i)->second == alpha){
						return logAntiLogTable.find(i)->first;
					}
					i++;
				}
			}

			return -1;
		}

		int getRemainderForVersion(int version){
			if(version >= 0 && version <= 39){
				return remainderBits.find(version)->second;
			}
			return -1;
		}

		coordinates getAlignmentPatternCoordinates(int version){
			coordinates cords;

			if(version >= 1 && version <= 5){
				cords.size = 2*2;
				cords.c = new coordinate[cords.size];

				for(int i=0;i<cords.size;i++){
					cords.c[i].x = alignmentPatternLocation[version][i/2];
					cords.c[i].y = alignmentPatternLocation[version][i%2];
				}

				return cords;
			}
			
			if(version >= 6 && version <= 12){
				cords.size = 3*3;
				cords.c = new coordinate[cords.size];

				for(int i=0;i<cords.size;i++){
					cords.c[i].x = alignmentPatternLocation[version][i/3];
					cords.c[i].y = alignmentPatternLocation[version][i%3];
				}

				return cords;
			}

			if(version >= 13 && version <= 19){
				cords.size = 4*4;
				cords.c = new coordinate[cords.size];

				for(int i=0;i<cords.size;i++){
					cords.c[i].x = alignmentPatternLocation[version][i/4];
					cords.c[i].y = alignmentPatternLocation[version][i%4];
				}

				return cords;
			}

			if(version >= 20 && version <= 26){
				cords.size = 5*5;
				cords.c = new coordinate[cords.size];

				for(int i=0;i<cords.size;i++){
					cords.c[i].x = alignmentPatternLocation[version][i/5];
					cords.c[i].y = alignmentPatternLocation[version][i%5];
				}

				return cords;
			}

			if(version >= 27 && version <= 33){
				cords.size = 6*6;
				cords.c = new coordinate[cords.size];

				for(int i=0;i<cords.size;i++){
					cords.c[i].x = alignmentPatternLocation[version][i/6];
					cords.c[i].y = alignmentPatternLocation[version][i%6];
				}

				return cords;
			}

			if(version >= 34 && version <= 39){
				cords.size = 7*7;
				cords.c = new coordinate[cords.size];

				for(int i=0;i<cords.size;i++){
					cords.c[i].x = alignmentPatternLocation[version][i/7];
					cords.c[i].y = alignmentPatternLocation[version][i%7];
				}

				return cords;
			}

			cords.size = 0;
			return cords; // version equals 0
		}
};

#endif