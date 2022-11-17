#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <Windows.h>
#include <stdio.h>

void getCurrentTime(int*);
void timeSettings(int*);

HANDLE workWithTime[5];

int main(void)
{
	int* timeArray = calloc(3, sizeof(int));
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	timeArray[0] = timeinfo->tm_hour;
	timeArray[1] = timeinfo->tm_min;
	timeArray[2] = timeinfo->tm_sec - 1;

	workWithTime[0] = CreateThread(NULL, 0, getCurrentTime, timeArray, 0, 0);
	workWithTime[1] = CreateThread(NULL, 0, showCurrentTime, timeArray, 0, 0);
	workWithTime[2] = CreateThread(NULL, 0, stopShowCurrentTime, NULL, 0, 0);


	return 0;
}

void timeSettings(int* forTimeArray)
{
	printf("Настройки часов\n1 - Показать время\n2 - Настроить время\n3 - Включить секундомер\nВключить таймер");
	int option;
	scanf_s("%d", &option);
	switch (option)
	{
		
	}
}


void getCurrentTime(int* forTimeArray)
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

		//printf("%d %d %d\n", forHours, forMinutes, forSeconds);
		Sleep(1000);
	}
}

void showCurrentTime(int* forTimeArray)
{
	printf("Нажмите 0 для выхода!\n");
	while (1)
	{
		printf("%d:%d:%d\n", forTimeArray[0], forTimeArray[1], forTimeArray[2]);
		Sleep(1000);
	}
}

void stopShowCurrentTime()
{
	int forStopThread;
	while (1)
	{
		scanf_s("%d", &forStopThread);
		switch (forStopThread)
		{
			case 0:
				SuspendThread(workWithTime[1]);
				ResumeThread();
				SuspendThread(workWithTime[2]);
				break;

			default:
				printf("Нет такого действия! Нажмите 0 для выхода!\n");
				break;
		}
	}
}
