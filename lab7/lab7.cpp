

#include "stdafx.h"
#include "cpuschedule.h"

void main()
{
	/*
	6 Для однієї й тієї ж послідовності процесів визначте всі критерії.
	Порівняйте 3 розглянуті алгоритми й зробіть висновки по 
	ефективності їх використання.
	*/
	int ch, cho;
	cpuschedule c;
	do
	{
		std::cout << "\tMENU\n";
		std::cout << "1.Getting BurstTime\n";
		std::cout << "2.FirstComeFirstServed\n";
		std::cout << "3.RoundRobin\n";
		std::cout << "4.ShortestJobFirst\n";
		std::cout << "5.EXIT\n";
		std::cout << "Enter your choice: ";
		std::cin >> ch;
		switch (ch)
		{
			case 1:
				c.Getdata();
				break;
			case 2:
				std::cout << "FIRST COME FIRST SERVED SCHEDULING\n";
					c.Fcfs();
				break;
			case 3:
				std::cout << "ROUND ROBIN SCHEDULING\n";
				c.RoundRobin();
				break;
			case 4:
				std::cout << "JOB FIRST SCHEDULING\n";
				c.Sjf();
				break;
			case 5:
				break;
		}
	} while (ch <= 4);
}

