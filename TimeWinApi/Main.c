#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <Windows.h>
#include <stdio.h>

void getCurrentTime(int*);
void timeSettings(int*);
void setTime(int*);
void showCurrentTime(int*);
void stopShowCurrentTime();
void getStopwatch();


HANDLE workWithTime[5];
int option;
time_t rawtime;
struct tm* timeinfo;

int main(void)
{
	system("chcp 1251");

	int* timeArray = calloc(3, sizeof(int));

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	timeArray[0] = timeinfo->tm_hour;
	timeArray[1] = timeinfo->tm_min;
	timeArray[2] = timeinfo->tm_sec - 1;

	workWithTime[0] = CreateThread(NULL, 0, getCurrentTime, timeArray, 0, 0);
	workWithTime[1] = CreateThread(NULL, 0, showCurrentTime, timeArray, 0, 0);
	workWithTime[2] = CreateThread(NULL, 0, stopShowCurrentTime, NULL, 0, 0);
	workWithTime[3] = CreateThread(NULL, 0, timeSettings, timeArray, 0, 0);
	workWithTime[4] = CreateThread(NULL, 0, setTime, timeArray, 0, 0);

	SuspendThread(workWithTime[3]);
	SuspendThread(workWithTime[4]);

	WaitForMultipleObjects(5, workWithTime, TRUE, INFINITE);

	return 0;
}

void timeSettings(int* forTimeArray)
{
	while (1)
	{
		printf("��������� �����\n1 - �������� �����\n2 - ��������� �����\n3 - �������� ����������\n4 - �������� ������\n");
		scanf_s("%d", &option);
		switch (option)
		{
		case 1:
			ResumeThread(workWithTime[1]);
			ResumeThread(workWithTime[2]);
			SuspendThread(workWithTime[3]);
			break;
		case 2:
			ResumeThread(workWithTime[4]);
			SuspendThread(workWithTime[3]);
			break;
		}
	}
}

void getStopwatch()
{

}

void setTime(int* forTimeArray)
{
	while(1)
	{
		printf("1 - ��������� ��������� �����, 2 - ��������� ����������� �����\n");
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
					printf("������� ����: ");
					scanf_s("%d", &forTimeArray[0]);
					if (forTimeArray[0] < 0 || forTimeArray[0] > 23) printf("������� ����� � ��������� �� 0 �� 23\n");
					else
					{
						printf("������� ������: ");
						scanf_s("%d", &forTimeArray[1]);
					}
					if (forTimeArray[1] < 0 || forTimeArray[1] > 59) printf("������� ����� � ��������� �� 0 �� 59\n");
					else
					{
						printf("������� �������: ");
						scanf_s("%d", &forTimeArray[2]);
					}
					if (forTimeArray[2] < 0 || forTimeArray[2] > 59) printf("������� ����� � ��������� �� 0 �� 59\n");
					else break;
				}
				break;
			default:
				printf("��� ������ ��������! ��������� ����!");
				break;
		}
		if (option == 1 || option == 2)
		{
			ResumeThread(workWithTime[3]);
			SuspendThread(workWithTime[4]);
		}
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

		Sleep(1000);
	}
}

void showCurrentTime(int* forTimeArray)
{
	while (1)
	{
		printf("%d:%d:%d\n", forTimeArray[0], forTimeArray[1], forTimeArray[2]);
		Sleep(1000);
	}
}

void stopShowCurrentTime()
{
	while (1)
	{
		printf("������� 0 ��� ������!\n");
		scanf_s("%d", &option);
		switch (option)
		{
			case 0:
				SuspendThread(workWithTime[1]);
				ResumeThread(workWithTime[3]);
				SuspendThread(workWithTime[2]);
				break;

			default:
				printf("��� ������ ��������! ������� 0 ��� ������!\n");
				break;
		}
	}
}
