#include "stdafx.h"
#include <omp.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <Windows.h>
#include <time.h>
#include <vector>
#include <stdlib.h>


using namespace std;


void main()
{
	setlocale (LC_ALL,"RUS");
	/* 1 */
    double startTime = omp_get_wtime(); 
	#pragma omp parallel num_threads(10) 
	{	printf("Hello, world!%i\n",omp_get_thread_num());
	}
	double finishTime = omp_get_wtime();
	cout<<"Время: "<<finishTime-startTime<<"\n";
	/* _1_ */

	/* 3 */

	int size = 1000;
	
	int** matrix = new int *[size];
	for (int i = 0; i < size; ++i)
		matrix[i] = new int[size];
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			matrix[i][j] = rand()%100 + 1;

	long int sum = 0;

	startTime = omp_get_wtime();
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			sum += matrix[i][j];
	finishTime = omp_get_wtime();
	cout<<"Сумма: "<<sum<<"\n"<<"Время суммирования без распараллеливания: "<<finishTime - startTime<<"\n";

	sum = 0;
	startTime = omp_get_wtime(); 
	#pragma omp parallel for num_threads(2) reduction(+:sum)
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			sum += matrix[i][j];
	finishTime = omp_get_wtime();
	cout<<"Сумма: "<<sum<<"\n"<<"Время суммирования с параметром reduction: "<<finishTime - startTime<<"\n";

	sum = 0;
	startTime = omp_get_wtime(); 
	#pragma omp parallel for num_threads(2)
	for (int i = 0; i < size; ++i)
	{
		long int currSum = 0;
		for (int j = 0; j < size; ++j)
			currSum += matrix[i][j];
		#pragma omp critical
		sum += currSum;
	}
	finishTime = omp_get_wtime();
	cout<<"Сумма: "<<sum<<"\n"<<"Время суммирования с критической секцией: "<<finishTime - startTime<<"\n";
	/* _3_ */

	/* 4 */
	#pragma omp parallel num_threads(5)
	{
		Sleep(omp_get_thread_num() * 1000);
		cout<<"Нить "<<omp_get_thread_num()<<" выполнила действие до barrier\n";
		#pragma omp barrier
		#pragma omp critical
		cout<<"Нить "<<omp_get_thread_num()<<" продолжила работу\n";
	}
	/* _4_ */

	/* 2 */
	int calcCount = 20;
	double results[4][3];

	// с равномерным распределением
	startTime = omp_get_wtime();
	#pragma omp for schedule (static, 2)
	for(int i = 0; i < calcCount; i++) Sleep(5);	
	finishTime = omp_get_wtime();
	results[0][0] = finishTime - startTime;

	startTime = omp_get_wtime();
	#pragma omp for schedule (dynamic, 2)
	for(int i = 0; i < calcCount; i++) Sleep(5);	
	finishTime = omp_get_wtime();
	results[0][1] = finishTime - startTime;

	startTime = omp_get_wtime();
	#pragma omp for schedule (guided, 2)
	for(int i = 0; i < calcCount; i++) Sleep(5);	
	finishTime = omp_get_wtime();
	results[0][2] = finishTime - startTime;

	startTime = omp_get_wtime();
	#pragma omp for schedule (runtime)
	for(int i = 0; i < calcCount; i++) Sleep(5);	
	finishTime = omp_get_wtime();
	results[0][3] = finishTime - startTime;

	//нечетные дольше
	startTime = omp_get_wtime();
	#pragma omp for schedule (static, 2)
	for(int i = 0; i < calcCount; i++)
		if (i%2==0) Sleep(5);
		else Sleep(10);
	finishTime = omp_get_wtime();
	results[1][0] = finishTime - startTime;

	startTime = omp_get_wtime();
	#pragma omp for schedule (dynamic, 2)
	for(int i = 0; i < calcCount; i++)
		if (i%2==0) Sleep(5);
		else Sleep(10);	
	finishTime = omp_get_wtime();
	results[1][1] = finishTime - startTime;

	startTime = omp_get_wtime();
	#pragma omp for schedule (guided, 2)
	for(int i = 0; i < calcCount; i++)
		if (i%2==0) Sleep(5);
		else Sleep(10);	
	finishTime = omp_get_wtime();
	results[1][2] = finishTime - startTime;

	startTime = omp_get_wtime();
	#pragma omp for schedule (runtime)
	for(int i = 0; i < calcCount; i++)
		if (i%2==0) Sleep(5);
		else Sleep(10);	
	finishTime = omp_get_wtime();
	results[1][3] = finishTime - startTime;

	//в первой половине выполняются дольше
	startTime = omp_get_wtime();
	#pragma omp for schedule (static, 2)
	for(int i = 0; i < calcCount; i++)
		if (i>calcCount/2) Sleep(5);
		else Sleep(10);
	finishTime = omp_get_wtime();
	results[2][0] = finishTime - startTime;

	startTime = omp_get_wtime();
	#pragma omp for schedule (dynamic, 2)
	for(int i = 0; i < calcCount; i++)
		if (i>calcCount/2) Sleep(5);
		else Sleep(10);	
	finishTime = omp_get_wtime();
	results[2][1] = finishTime - startTime;

	startTime = omp_get_wtime();
	#pragma omp for schedule (guided, 2)
	for(int i = 0; i < calcCount; i++)
		if (i>calcCount/2) Sleep(5);
		else Sleep(10);	
	finishTime = omp_get_wtime();
	results[2][2] = finishTime - startTime;

	startTime = omp_get_wtime();
	#pragma omp for schedule (runtime)
	for(int i = 0; i < calcCount; i++)
		if (i>calcCount/2) Sleep(5);
		else Sleep(10);	
	finishTime = omp_get_wtime();
	results[2][3] = finishTime - startTime;


	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 4; j++){
			printf("  %4.4f", results[i][j]);
		}
		printf("\n");
	}
	/* _2_ */


	/* 5 */
	int N = 1000;
    int** arr = new int *[N];
	for (int i = 0; i < N; ++i)
		arr[i] = new int[N];
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			arr[i][j] = 1;

	startTime = omp_get_wtime();
	for (int i = 2; i < N; ++i)
		for (int j = 2; j < N; ++j)
			arr[i][j] = arr[i - 2][j] + arr[i][j - 2];
	finishTime = omp_get_wtime();
	cout<<"\nВремя выполнения без распараллеливания: "<<finishTime-startTime<<"\narr[N-1][N-1]="<<arr[N-1][N-1];

	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			arr[i][j] = 1;

	startTime = omp_get_wtime();
	for (int k = 4; k < 2*N - 1;k++){
		#pragma omp parallel for num_threads(2)
			for (int i = 2; i < N;i++)
			{
				int j = k - i;
				if ((j > 1)&&(j < N))
					arr[i][j] = arr[i - 2][j] + arr[i][j - 2];
			}
	}
	finishTime = omp_get_wtime();
	cout<<"\nВремя выполнения с распараллеливанием: "<<finishTime-startTime<<"\narr[N-1][N-1]="<<arr[N-1][N-1];
	/* _5_ */
	getch();
}

