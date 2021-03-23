#ifndef SLAR_CALCULATIONS_H
#define SLAR_CALCULATIONS_H


//--------------------------------------------------------------------------------
double* GauseCalculations(double **pMultiplierMatrix, 
                         double *pMemberArray, 
                         int nSize);
//--------------------------------------------------------------------------------
double* LUMethod(double **pMultiplierMatrix, double *pMembersArray, int nSize);
//--------------------------------------------------------------------------------
#endif