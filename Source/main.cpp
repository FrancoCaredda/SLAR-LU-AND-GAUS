#include "../Headers/Arrays.h"
#include "../Headers/Memory.h"
#include "../Headers/SLARCalculations.h"

#include <iostream>

using namespace std;

int main(void) {

    int nDimention = 0;
    bool bReadingFromFile = 0;

    cout << "Reading from data.txt file(1)\nReading from keyboard(0):\n";
    cin >> bReadingFromFile;

    cout << "Enter system dimention: ";
    cin >> nDimention;

    double **pArray = AllocateDoubleMatrix(nDimention, nDimention);
    double *pVector = AllocateDoubleArray(nDimention);

    if (bReadingFromFile) {
        ReadDataFromFileToMatrix(pArray, "data.txt", nDimention, nDimention);
        ReadDataFromFileToArray(pVector, "dataVector.txt", nDimention);

        PrintDoubleMatrix(pArray, nDimention, nDimention);
        PrintDoubleArray(pVector, nDimention);
    } else {
        cout << "Enter matrix: " << endl;
        EnterDoubleMatrix(pArray, nDimention, nDimention);

        cout << "\nEnter vector: ";
        EnterDoubleArray(pVector, nDimention);
    }

    cout << "Gause Method: " << endl;
    double *pResult = GauseCalculations(pArray, pVector, nDimention);
    
    cout << "\nResult: " << endl;
    PrintDoubleArray(pResult, nDimention);

    FreeDoubleArray(&pResult);

    cout << "\nLU Method: " << endl;
    pResult = LUMethod(pArray, pVector, nDimention);

    cout << "Result: ";
    PrintDoubleArray(pResult, nDimention);

    FreeDoubleMatrix(pArray, nDimention);
    FreeDoubleArray(&pResult);
    return 0;
}