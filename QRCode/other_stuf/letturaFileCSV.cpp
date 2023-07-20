#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

string elaboraStringa(string s){
	vector<char> str;
	bool first = false;
	int count = 0; 
	
	for(int i=0;i<s.size() && count<3;i++){
		if(s[i] != 'L' && s[i] != 'M' && s[i] != 'Q' && s[i] != 'H'){
			if(s[i]!='\t'){
				str.push_back(s[i]);
			} else if(first){
				count++;
				if(count<3)
					str.push_back(';');
			} else {
				first = true;
			}
		}
	}
	
	string st(str.begin(), str.end());
	
	return st;
}

int main(){
	string s;
	string rs;
	ifstream tabella("tabella.txt");
	ofstream saveFile("tabella_finale.txt");
	
	getline(tabella, s);
	
	while(!tabella.eof()){
		cout<<s<<"\n";
		rs = elaboraStringa(s);
		saveFile<<rs<<"\n";
		getline(tabella, s);
	}
	
	
	
	tabella.close();
	return 0;
}
