#include <iostream>
#include "QrCodeMaterial.hpp"
#include "QrInfo.hpp"
#include "QrString.hpp"
#include "QrErrorCorrection.hpp"
#include "QrmatrixPlacement.hpp"
#include "Grafic.hpp"

using namespace std;

int main(){
	string s = "HELLO";
	trims(s);

	QrCodeMaterial *material = new QrCodeMaterial();
	QrInfo *info = new QrInfo(material,s);
	QrString *str = new QrString(material, info);
	QrErrorCorrection *error = new QrErrorCorrection(material, info, str);
	QrMatrixPlacement *matrix = new QrMatrixPlacement(material, info, str, error);
	Grafic *grf = new Grafic(matrix->getMatrix(), matrix->getSize());
	

	cout << info->getInfo() << "\n";
	cout << str->getInfo() << "\n";
	cout << error->getInfo() << "\n";
	cout << "############### MATRIX PLACEMENT ###############" << "\n";
	cout << "Matrix size: " << matrix->getSize() << "\n\n";
	matrix->stampa();
    return 0;
}

