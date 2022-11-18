#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <Windows.h>
#include <stdio.h>

void getCurrentTime(int*);
void timeSettings(int*);
void setTime(int*);
void showCurrentTime(int*);
void stopShowCurrentTime();
void getStopwatch(int* forStopwatch);
void stopStopwatch(int* forStopwatch);
void setTimer(int* forTimer);
void getTimer(int* forTimer);
void showTimer(int* forTimer);
void runTimer(int* forTimer);
void settingsTimer(int* forTimer);


HANDLE workWithTime[12]; // Для потоков
int option; // Для вариантов выбора при взаимодействии с пользователем
time_t rawtime;
struct tm* timeinfo;

int main(void)
{
	system("chcp 1251>n");

	int* timeArray = calloc(3, sizeof(int)); // Указатель, в котором хранится время (часы, минуты, секунды)

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	timeArray[0] = timeinfo->tm_hour;
	timeArray[1] = timeinfo->tm_min;
	timeArray[2] = timeinfo->tm_sec - 1;

	workWithTime[0] = CreateThread(NULL, 0, getCurrentTime, timeArray, 0, 0); // Поток с подсчетом времени
	workWithTime[1] = CreateThread(NULL, 0, showCurrentTime, timeArray, 0, 0); // Поток для показа времени
	workWithTime[2] = CreateThread(NULL, 0, stopShowCurrentTime, NULL, 0, 0); // Поток для остановки показа времени
	workWithTime[3] = CreateThread(NULL, 0, timeSettings, timeArray, CREATE_SUSPENDED, 0); // Поток для показа меню времени
	workWithTime[4] = CreateThread(NULL, 0, setTime, timeArray, CREATE_SUSPENDED, 0); // Поток для установки времени
	
	int* stopwatch = calloc(4, sizeof(int));

	for (int i = 0; i < 4; i++) stopwatch[i] = 0;

	workWithTime[5] = CreateThread(NULL, 0, getStopwatch, stopwatch, CREATE_SUSPENDED, 0); // Поток для запуска секундомера
	workWithTime[6] = CreateThread(NULL, 0, stopStopwatch, stopwatch, CREATE_SUSPENDED, 0); // Поток для остановки секундомера

	int* timer = calloc(3, sizeof(int));

	for (int i = 0; i < 3; i++) timer[i] = 0;

	workWithTime[7] = CreateThread(NULL, 0, setTimer, timer, CREATE_SUSPENDED, 0); // Поток для запуска таймера
	workWithTime[8] = CreateThread(NULL, 0, getTimer, timer, CREATE_SUSPENDED, 0); // Поток для запуска таймера
	workWithTime[9] = CreateThread(NULL, 0, showTimer, timer, CREATE_SUSPENDED, 0); // Поток для запуска таймера
	workWithTime[10] = CreateThread(NULL, 0, runTimer, timer, CREATE_SUSPENDED, 0); // Поток для запуска таймера
	workWithTime[11] = CreateThread(NULL, 0, settingsTimer, timer, CREATE_SUSPENDED, 0); // Поток для запуска таймера

	WaitForMultipleObjects(1, workWithTime, TRUE, INFINITE);

	return 0;
}

void timeSettings(int* forTimeArray) // Настройки часов
{
	while (1)
	{
		printf("Настройки часов\n1 - Показать время\n2 - Настроить время\n3 - Включить секундомер\n4 - Включить таймер\n");
		scanf_s("%d", &option);
		switch (option)
		{
		case 1:
			ResumeThread(workWithTime[1]); // Включаем поток с показом времени
			ResumeThread(workWithTime[2]); // Включаем поток для остановки показа времени
			SuspendThread(workWithTime[3]); // Останавливаем поток с меню
			break;
		case 2:
			ResumeThread(workWithTime[4]); // Включаем поток с установкой времени
			SuspendThread(workWithTime[3]); // Останавливаем поток с меню
			break;
		case 3:
			ResumeThread(workWithTime[5]); // Включаем поток с секундомером
			ResumeThread(workWithTime[6]); // Включаем поток с остановкой секундомера
			SuspendThread(workWithTime[3]); // Останавливаем поток с меню
			break;
		case 4:
			ResumeThread(workWithTime[10]);
			SuspendThread(workWithTime[3]);
		}
	}
}

void setTimer(int* forTimer)
{
	while (1)
	{
		printf("Часы: ");
		scanf_s("%d", &forTimer[0]);

		printf("Минуты: ");
		scanf_s("%d", &forTimer[1]);

		printf("Секунды: ");
		scanf_s("%d", &forTimer[2]);

		ResumeThread(workWithTime[8]);
		ResumeThread(workWithTime[10]);
		SuspendThread(workWithTime[7]);
	}
}

void getTimer(int* forTimer)
{
	while (1)
	{
		forTimer[2]--;
		if (forTimer[2] == 0 && forTimer[1] != 0)
		{
			forTimer[2] = 59;
			forTimer[1]--;
		}
		if (forTimer[1] == 0 && forTimer[0] != 0)
		{
			forTimer[1] = 59;
			forTimer[0]--;
		}
		if (forTimer[2] == 0 && forTimer[1] == 0 && forTimer[0] == 0)
		{
			scanf_s("%d", &option);
			printf("Таймер закончил работу!\n");
			SuspendThread(workWithTime[8]);
		}
		Sleep(1000);
	}
}

void showTimer(int* forTimer)
{
	while (1)
	{
		if (forTimer[2] == 0 && forTimer[1] == 0 && forTimer[0] == 0)
		{
			SuspendThread(workWithTime[11]);
			ResumeThread(workWithTime[10]);
			SuspendThread(workWithTime[9]);
		}
		printf("%d:%d:%d\n", forTimer[0], forTimer[1], forTimer[2]);
		Sleep(950);
	}
}

void settingsTimer(int* forTimer)
{
	while (1)
	{
		printf("0 - назад, 1 - сбросить таймер, 2 - пауза, 3 - возобновить таймер\n");

		scanf_s("%d", &option);

		switch (option)
		{
		case 0:
			ResumeThread(workWithTime[10]);
			SuspendThread(workWithTime[9]);
			SuspendThread(workWithTime[11]);
			break;
		case 1:
			printf("Таймер сброшен!\n");
			for (int i = 0; i < 3; i++) forTimer[i] = 0;
			SuspendThread(workWithTime[9]);
			SuspendThread(workWithTime[8]);
			ResumeThread(workWithTime[10]);
			SuspendThread(workWithTime[11]);
			break;
		case 2:
			SuspendThread(workWithTime[8]);
			SuspendThread(workWithTime[9]);
			break;
		case 3:
			ResumeThread(workWithTime[8]);
			ResumeThread(workWithTime[9]);
			break;
		}
	}
}

void runTimer(int* forTimer)
{
	while (1)
	{
		if (forTimer[2] == 0 && forTimer[1] == 0 && forTimer[0] == 0)
		{
			printf("0 - назад, 1 - установить новое время\n");
			scanf_s("%d", &option);

			switch (option)
			{
				case 0:
					ResumeThread(workWithTime[3]);
					SuspendThread(workWithTime[10]);
					break;
				case 1:
					ResumeThread(workWithTime[7]);
					SuspendThread(workWithTime[10]);
					break;
				default:
					printf("Нет такого варианта!\n");
					break;
			}
		}
		else
		{
			printf("0 - назад, 1 - показать таймер\n");
			scanf_s("%d", &option);

			switch (option)
			{
			case 0:
				ResumeThread(workWithTime[3]);
				SuspendThread(workWithTime[10]);
				break;
			case 1:
				ResumeThread(workWithTime[11]);
				ResumeThread(workWithTime[9]);
				SuspendThread(workWithTime[10]);
				break;
			}
		}
	}
}

void getStopwatch(int* forStopwatch) //Секундомер
{
	while (1)
	{
		forStopwatch[3]++;
		if (forStopwatch[3] == 100)
		{
			forStopwatch[3] = 0;
			forStopwatch[2]++;
		}
		
		if (forStopwatch[2] == 60)
		{
			forStopwatch[2] = 0;
			forStopwatch[1]++;
		}

		if (forStopwatch[1] == 60)
		{
			forStopwatch[1] = 0;
			forStopwatch[0]++;
		}
		printf("%d:%d:%d:%d\n", forStopwatch[0], forStopwatch[1], forStopwatch[2], forStopwatch[3]);
		Sleep(5);
	}
}

void stopStopwatch(int* forStopwatch) //Остановка секундомера
{
	while (1)
	{
		printf("Нажмите 0 для выхода\n");
		scanf_s("%d", &option);
		switch (option)
		{
		case 0:

			for (int i = 0; i < 4; i++) forStopwatch[i] = 0;

			SuspendThread(workWithTime[5]);
			ResumeThread(workWithTime[3]);
			SuspendThread(workWithTime[6]);
			break;
		default:
			printf("Нет такого варианта!");
			break;
		}
	}
}

void setTime(int* forTimeArray) // Установка времени
{
	while(1)
	{
		printf("1 - назначить системное время, 2 - назначить собственное время\n");
		scanf_s("%d", &option);
		switch (option)
		{
			case 1:
				time(&rawtime);
				timeinfo = localtime(&rawtime);

				forTimeArray[0] = timeinfo->tm_hour;
				forTimeArray[1] = timeinfo->tm_min;
				forTimeArray[2] = timeinfo->tm_sec - 1;
				break;
			case 2:
				while (1)
				{
					printf("Введите часы: ");
					scanf_s("%d", &forTimeArray[0]);
					if (forTimeArray[0] < 0 || forTimeArray[0] > 23) printf("Введите число в диапазоне от 0 до 23\n");
					else
					{
						printf("Введите минуты: ");
						scanf_s("%d", &forTimeArray[1]);
					}
					if (forTimeArray[1] < 0 || forTimeArray[1] > 59) printf("Введите число в диапазоне от 0 до 59\n");
					else
					{
						printf("Введите секунды: ");
						scanf_s("%d", &forTimeArray[2]);
					}
					if (forTimeArray[2] < 0 || forTimeArray[2] > 59) printf("Введите число в диапазоне от 0 до 59\n");
					else break;
				}
				break;
			default:
				printf("Нет такого варианта! Повторите ввод!");
				break;
		}
		if (option == 1 || option == 2)
		{
			ResumeThread(workWithTime[3]); // Включаем поток с меню
			SuspendThread(workWithTime[4]); // Останавливаем поток с настройкой времени
		}
	}
}


void getCurrentTime(int* forTimeArray) // Подсчет времени
{
	while (1)
	{
		forTimeArray[2]++;
		if (forTimeArray[2] == 60)
		{
			forTimeArray[2] = 0;
			forTimeArray[1]++;
		}
		
		if (forTimeArray[1] == 60)
		{
			forTimeArray[1] = 0;
			forTimeArray[0]++;
		}

		if (forTimeArray[0] == 24) forTimeArray[0] = 0;

		Sleep(1000);
	}
}

void showCurrentTime(int* forTimeArray) // Показ времени
{
	while (1)
	{
		printf("%d:%d:%d\n", forTimeArray[0], forTimeArray[1], forTimeArray[2]);
		Sleep(950);
	}
}

void stopShowCurrentTime() // Остановка показа времени
{
	while (1)
	{
		printf("Нажмите 0 для выхода!\n");
		scanf_s("%d", &option);
		switch (option)
		{
			case 0:
				SuspendThread(workWithTime[1]); // Останавливаем поток с показом времени
				ResumeThread(workWithTime[3]); // Включаем поток с меню
				SuspendThread(workWithTime[2]); // Выключаем поток с остановкой показа времени
				break;

			default:
				printf("Нет такого действия! Нажмите 0 для выхода!\n");
				break;
		}
	}
}
