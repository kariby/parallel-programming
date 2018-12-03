#include "stdio.h"
#include <time.h>
#include <algorithm>
#include <Windows.h>
#include <math.h>
#include <iostream>



using namespace std;
int main(int argc, char* argv[])
{
	int rank = 0, size = 0, M, N;
	int* m = new int[1];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		N = 99;//размер массива
		M = 50;//магическое число
		int R = 20;//диапазон случайных чисел
		cout << "Magic=\n";
		cin >> M;
		cout << "R=\n";
		cin >> R;
		cout << "Nsize=\n";
		cin >> N;
		m = new int[N];
		printf("\n");

		for (int i = 0; i<N; i++)
		{
			m[i] = rand() % R;//генератор
			/*printf("%i", m[i]);
			printf("\n");*/
		}
		
	}
	MPI_Barrier(MPI_COMM_WORLD);//синхронизаци€
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);//всем передаем N
	MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);//всем передаем M

	int* m1 = new int[N / size];//кусочек массива у каждого процесса
	MPI_Scatter(m, N / size, MPI_INT, m1, N / size, MPI_INT, 0, MPI_COMM_WORLD);//разбиение массивов по процессам

	MPI_Request stop[100];//чтобы оповестить,что магическое число встретилось, значит остановка
	MPI_Request next[100];//чтобы оповестить,что магическое число Ќ≈ встретилось, значит продолжаем
	MPI_Request opovest;
	MPI_Status status;
	int stoped = 0, reciveFlag = 1;
	int pr = 1;
	for (int i = 0, j = 0; i<size; i++, j++)
	{
		MPI_Send_init(&stoped, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &stop[j]);//инициализаци€, но не отпраул€ет
	}

	MPI_Irecv(&reciveFlag, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &opovest);//функци€ приема сообщени€ без блокировки
	MPI_Barrier(MPI_COMM_WORLD);//синхронизаци€
	int i;
	for (i = 0; i<N / size; i++)
	{
		if (reciveFlag == 0)
		{
			pr = 0;
			break;
		}
		// аждый узел измен€ет значени€ элементов массива по правилу
		if (m1[i] != M)
			m1[i] = (int)(sinf(m1[i] - M)*expf(m1[i]) - log(abs(cosf(m1[i]))));
		//программа должна остановитс€ (т.е. каждый узел должен прекратить модифицировать массив ј) и массив A остаетс€ без изменений
		else
		{
			MPI_Startall(size, stop);//проверка на завершение
			MPI_Waitall(size, stop, MPI_STATUSES_IGNORE);//завершить операцию
			printf("stop process: %i", rank+1);
			break;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);//синхронизаци€

	int* sob_buf;//собирающий буфер
	if (reciveFlag == 1)
	{
		sob_buf = new int[N];
		MPI_Gather(m1, N / size, MPI_INT, sob_buf, N / size, MPI_INT, 0, MPI_COMM_WORLD);//сборка блоков в один массив
		if (rank == 0)
		{
			printf("magicheskoe chislo ne vstretilos:");
			for (int i = 0; i<N; i++)
				printf("%i ", sob_buf[i]);
		}
	}
	else
	{
		sob_buf = new int[size];
		MPI_Gather(&i, 1, MPI_INT, sob_buf, 1, MPI_INT, 0, MPI_COMM_WORLD);//сборка блоков в один массив
		if (rank == 0)
		{
			printf("kol-vo iteracii:");
			for (int i = 0; i<size; i++)
				printf("%i ", sob_buf[i]);
		}
	}


	MPI_Finalize();//завершить прогу
	//getch();
	return 0;
}