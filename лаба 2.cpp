#include <fstream>
#include <iostream>
#include <conio.h>
#include <omp.h>
#include <locale.h>
#include <math.h>
#include "stdafx.h"

using namespace std;

void SwapRows(double **arr,int firstRow, int secondRow, int size)
{
	for (int i = 0; i < size; ++i)
	{
		double tmp = arr[firstRow][i];
		arr[firstRow][i] = arr[secondRow][i];
		arr[secondRow][i] = tmp;
	}
}

void main()
{
	setlocale (LC_ALL,"Rus");
	fstream file;
	file.open("matrix4.txt");
	int size;
	file>>size;
	
	double** matrix = new double *[size];
	for (int i = 0; i < size; ++i)
		matrix[i] = new double[size];
	
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			matrix[i][j]=0;
	
	for (int i = 0; i < size; ++i)
	{
		int countNotNull;
		file>>countNotNull;
		for (int j = 0; j < countNotNull; ++j)
		{
			int columnNumber;
			file>>columnNumber;
			file>>matrix[i][columnNumber];//для 1000х1000
		}
	}

	double startTime = omp_get_wtime();
	double determinant = 1;
	for(int i = 0;i < size - 1;++i)
	{
		int k =  i + 1; 
		while (matrix[i][i]==0 && k < size) 
		{
			SwapRows(matrix,i,k,size);
			++k;
			determinant*=-1;
		}
		#pragma omp parallel for num_threads(2)
		for (int j = i+1; j < size; ++j)
		{
			double multiplier = matrix[j][i]/matrix[i][i];
			for (int k = 0; k < size; ++k)
				matrix[j][k]-=matrix[i][k]*multiplier;
		}
	}
	
	
	for (int i = 0; i < size; ++i)
		determinant*=matrix[i][i];

	double finishTime = omp_get_wtime();

	cout<<"Время работы вычислительного алгоритма: "<<finishTime-startTime<<"\n";
	cout<<"Определитель: "<<determinant<<"\n";
	file.close();
	getch();
}
