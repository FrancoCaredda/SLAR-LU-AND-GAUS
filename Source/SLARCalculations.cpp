#include "../Headers/SLARCalculations.h"
#include "../Headers/Memory.h"
#include "../Headers/Arrays.h"

#include <math.h>
#include <stdio.h>

//--------------------------------------------------------------------------------
static double FindMainElementInMatrix(double **pMatrix,
                                      int nSize,
                                      int &nMainColumn,
                                      int &nMainRow) {
    double rMainElement = 0;
    
    for (int i = 0; i < nSize; i++) {
            for (int j = 0; j < nSize; j++) {
                if (fabs(rMainElement) < fabs(pMatrix[i][j])) {
                    rMainElement = pMatrix[i][j]; 
                    nMainRow = i;
                    nMainColumn = j;
                }
            }
    }

    return rMainElement;
}
//--------------------------------------------------------------------------------
double* GauseCalculations(double **pMultiplierMatrix, 
                          double *pMemberArray, 
                          int nSize) {
    double **pTempMultiplierMatrix = AllocateDoubleMatrix(nSize, nSize);
    double  *pTempMembersVector    = AllocateDoubleArray(nSize);

    CopyMatrix(pTempMultiplierMatrix, pMultiplierMatrix, nSize, nSize);
    CopyArray(pTempMembersVector, pMemberArray, nSize);

    double **pTriangleMatrix = AllocateDoubleMatrix(nSize, nSize);
    double *pNewVector = AllocateDoubleArray(nSize);

    double rMainElement = 0, rMultiplier = 0;

    int nMainRow = -1, nMainColumn;

    for (int k = 0; k < nSize; k++) {
        rMainElement = FindMainElementInMatrix(pTempMultiplierMatrix, nSize, 
                                               nMainColumn, 
                                               nMainRow);

        for (int i = 0; i < nSize; i++) {
            if (i != nMainRow) {    
                rMultiplier = (-1) * pTempMultiplierMatrix[i][nMainColumn] 
                                        / rMainElement;
                    
                for (int j = 0; j < nSize; j++) {
                    pTempMultiplierMatrix[i][j] += 
                        pTempMultiplierMatrix[nMainRow][j] * rMultiplier;
                }

                pTempMembersVector[i] += pTempMembersVector[nMainRow] * rMultiplier;
            }
        }

        CopyArray(pTriangleMatrix[k], pTempMultiplierMatrix[nMainRow], nSize);
        pNewVector[k] = pTempMembersVector[nMainRow];

        for (int i = 0; i < nSize; i++)
            pTempMultiplierMatrix[nMainRow][i] = 0;
    }

    FreeDoubleMatrix(pTempMultiplierMatrix, nSize);
    FreeDoubleArray(&pTempMembersVector);

    PrintDoubleMatrix(pTriangleMatrix, nSize, nSize);
    printf("\n");
    PrintDoubleArray(pNewVector, nSize);
    printf("\n");

    double *pResult = AllocateDoubleArray(nSize);

    double sum = 0;

    for (int i = (nSize - 1); i >= 0; i--) {
        for (int j = (nSize - 1); j > i; j--) {
            sum += pTriangleMatrix[i][j] * pResult[j]; 
        }
        pResult[i] = (pNewVector[i] - sum) / pTriangleMatrix[i][i];

        sum = 0;
    }

    FreeDoubleMatrix(pTriangleMatrix, nSize);
    FreeDoubleArray(&pNewVector);

    return pResult;
}
//--------------------------------------------------------------------------------
double* LUMethod(double **pMultiplierMatrix, double *pMembersArray, int nSize) {
    double **pTempMultiplierMatrix = AllocateDoubleMatrix(nSize, nSize);
    double *pTempMembersArray = AllocateDoubleArray(nSize);

    CopyMatrix(pTempMultiplierMatrix, pMultiplierMatrix, nSize, nSize);
    CopyArray(pTempMembersArray, pMembersArray, nSize);

    double **pLMatrix = AllocateDoubleMatrix(nSize, nSize);
    double **pUMatrix = AllocateDoubleMatrix(nSize, nSize);

    double sum = 0;
   

    for (int i = 0; i < nSize; i++) {
        for (int j = 0; j < nSize; j++) {
            pUMatrix[i][j] = 0;
            pLMatrix[i][j] = 0;
        }

        pLMatrix[i][i] = 1;
    }

    for (int i = 0; i < nSize; i++) {
        for (int j = 0; j < nSize; j++) {
            if (i <= j) {
                for (int k = 0; k < i; k++) {
                    sum += pLMatrix[i][k] * pUMatrix[k][j];
                }
                pUMatrix[i][j] = pTempMultiplierMatrix[i][j] - sum;
                sum = 0;
            } else if (i > j) {
                for (int k = 0; k < j; k++) {
                    sum += pLMatrix[i][k] * pUMatrix[k][j];
                }
                pLMatrix[i][j] = (pTempMultiplierMatrix[i][j] - sum) / pUMatrix[j][j];
                sum = 0;
            }
        }
    }

    FreeDoubleMatrix(pTempMultiplierMatrix, nSize);
    pTempMultiplierMatrix = nullptr;

    printf("LMatrix: \n");
    PrintDoubleMatrix(pLMatrix, nSize, nSize);
    printf("\nUMatrix: \n");
    PrintDoubleMatrix(pUMatrix, nSize, nSize);


    double *pYResult = AllocateDoubleArray(nSize);
    sum = 0;

    for (int i = 0; i < nSize; i++) {
        for (int j = 0; j < i; j++) {
            sum += pLMatrix[i][j] * pYResult[j];
        }
        pYResult[i] = pMembersArray[i] - sum;
        sum = 0;
    }

    printf("\nY vector: \n");
    PrintDoubleArray(pYResult, nSize);
    printf("\n");

    double *pResult = AllocateDoubleArray(nSize);
    sum = 0;
    for (int i = (nSize - 1); i >= 0; i--) {
        for (int j = (nSize - 1); j > i; j--) {
            sum += pUMatrix[i][j] * pResult[j]; 
        }
        pResult[i] = (pYResult[i] - sum) / pUMatrix[i][i];
        sum = 0;
    }

    FreeDoubleArray(&pYResult);
    FreeDoubleArray(&pTempMembersArray);
    FreeDoubleMatrix(pLMatrix, nSize);
    FreeDoubleMatrix(pUMatrix, nSize);

    pLMatrix = nullptr;
    pUMatrix = nullptr;

    return pResult;
}
//--------------------------------------------------------------------------------