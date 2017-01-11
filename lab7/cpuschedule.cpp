#include "stdafx.h"
#include "cpuschedule.h"

/*
1 Вивчить основні алгоритми планування (матеріал відповідної лекції);
2 Реалізуйте основні функції для роботи із чергою процесів;
*/
//Getting no of processes and Burst time
void cpuschedule::Getdata()
{
	int i;
	std::cout << "Enter the no of processes : ";
		std::cin >> n;
	for (i = 1; i <= n; i++)
	{
		std::cout << "\nEnter The BurstTime for Process p"<<i<<" = ";
		std::cin >> Bu[i];
	}
}

/*
3 Реалізуйте алгоритм FCFS. Визначте основні критерії для 
цього алгоритму;
*/
void cpuschedule::Fcfs()
{
	int i, B[10];
	Twt = 0.0;
	for (i = 1; i <= n; i++)
	{
		B[i] = Bu[i];
		std::cout << "\nBurst time for process p"<<i<<" = ";
		std::cout << B[i] << "\n";
	}
	Wt[1] = 0;
	for (i = 2; i <= n; i++)
	{
		Wt[i] = B[i - 1] + Wt[i - 1];
	}

	//Calculating Average Weighting Time
	for (i = 1; i <= n; i++)
		Twt = Twt + Wt[i];
	Awt = Twt / n;
	std::cout << "\nTotal   Weighting Time = "<<Twt;
	std::cout << "\nAverage Weighting Time = "<<Awt<<"\n";
}

/*
4 Реалізуйте алгоритм RR. Визначте основні критерії 
для цього алгоритму;
*/
void cpuschedule::RoundRobin()
{

	int i, j, tq, k, B[10], Rrobin[10][10], count[10];
	int max = 0;
	int m;
	Twt = 0.0;
	for (i = 1; i <= n; i++)
	{
		B[i] = Bu[i];
		std::cout << "\nBurst time for process p" << i << " = ";
		std::cout << B[i] << "\n";
		if (max<B[i])
			max = B[i];
		Wt[i] = 0;
	}
	std::cout << "\nEnter the Time Quantum = ";
	std::cin >> tq;
	//TO find the dimension of the Rrobin array
	m = max / tq + 1;

	//initializing Rrobin array
	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= m; j++)
		{
			Rrobin[i][j] = 0;
		}
	}
	//placing value in the Rrobin array
	i = 1;
	while (i <= n)
	{
		j = 1;
		while (B[i]>0)
		{
			if (B[i] >= tq)
			{
				B[i] = B[i] - tq;
				Rrobin[i][j] = tq;
				j++;
			}
			else
			{
				Rrobin[i][j] = B[i];
				B[i] = 0;
				j++;
			}
		}
		count[i] = j - 1;
		i++;
	}

	std::cout << "\nDisplay\n";
	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= m; j++)
		{
			std::cout << "\nRr[" << i << "," << j << "]=" << Rrobin[i][j];
			std::cout << "\n";
		}
		std::cout << "\n";
	}
	//calculating weighting time
	int x = 1;
	i = 1;
	while (x <= n)
	{
		for (int a = 1; a<x; a++)
		{
			Wt[x] = Wt[x] + Rrobin[a][i];
		}
		i = 1;
		int z = x;
		j = count[z];
		k = 1;
		while (k <= j - 1)
		{
			if (i == n + 1)
			{
				i = 1;
				k++;
			}
			else
			{
				if (i != z)
				{
					Wt[z] = Wt[z] + Rrobin[i][k];
				}
				i++;
			}
		}
		x++;
	}
	for (i = 1; i <= n; i++)
		std::cout << "\nWeighting Time for process P" << i << " = " << Wt[i] << "\n";

	//calculating Average Weighting Time
	for (i = 1; i <= n; i++)
		Twt = Twt + Wt[i];
	Awt = Twt / n;
	std::cout << "\nTotal   Weighting Time = " << Twt;
	std::cout << "\nAverage Weighting Time = " << Awt << "\n";
}

/*
5 Реалізуйте алгоритм SJF. Визначте основні критерії 
для цього алгоритму;
*/
void cpuschedule::Sjf()
{
	int i, j, temp, B[10];
	Twt = 0.0;
	for (i = 1; i <= n; i++)
	{
		B[i] = Bu[i];
		std::cout << "\nBurst time for process p"<<i<<" = ";
		std::cout << B[i];
	}
	for (i = n; i >= 1; i--)
	{
		for (j = 1; j <= n; j++)
		{
			if (B[j - 1]>B[j])
			{
				temp = B[j - 1];
				B[j - 1] = B[j];
				B[j] = temp;
			}
		}
	}

	Wt[1] = 0;
	for (i = 2; i <= n; i++)
	{
		Wt[i] = B[i - 1] + Wt[i - 1];
	}
	//calculating Average Weighting Time
	for (i = 1; i <= n; i++)
		Twt = Twt + Wt[i];
	Awt = Twt / n;
	std::cout << "\nTotal   Weighting Time = "<<Twt;
	std::cout << "\nAverage Weighting Time = "<<Awt<<"\n";
}