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


HANDLE workWithTime[12]; // ��� �������
int option; // ��� ��������� ������ ��� �������������� � �������������
time_t rawtime;
struct tm* timeinfo;

int main(void)
{
	system("chcp 1251>n");

	int* timeArray = calloc(3, sizeof(int)); // ���������, � ������� �������� ����� (����, ������, �������)

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	timeArray[0] = timeinfo->tm_hour;
	timeArray[1] = timeinfo->tm_min;
	timeArray[2] = timeinfo->tm_sec - 1;

	workWithTime[0] = CreateThread(NULL, 0, getCurrentTime, timeArray, 0, 0); // ����� � ��������� �������
	workWithTime[1] = CreateThread(NULL, 0, showCurrentTime, timeArray, 0, 0); // ����� ��� ������ �������
	workWithTime[2] = CreateThread(NULL, 0, stopShowCurrentTime, NULL, 0, 0); // ����� ��� ��������� ������ �������
	workWithTime[3] = CreateThread(NULL, 0, timeSettings, timeArray, CREATE_SUSPENDED, 0); // ����� ��� ������ ���� �������
	workWithTime[4] = CreateThread(NULL, 0, setTime, timeArray, CREATE_SUSPENDED, 0); // ����� ��� ��������� �������
	
	int* stopwatch = calloc(4, sizeof(int));

	for (int i = 0; i < 4; i++) stopwatch[i] = 0;

	workWithTime[5] = CreateThread(NULL, 0, getStopwatch, stopwatch, CREATE_SUSPENDED, 0); // ����� ��� ������� �����������
	workWithTime[6] = CreateThread(NULL, 0, stopStopwatch, stopwatch, CREATE_SUSPENDED, 0); // ����� ��� ��������� �����������

	int* timer = calloc(3, sizeof(int));

	for (int i = 0; i < 3; i++) timer[i] = 0;

	workWithTime[7] = CreateThread(NULL, 0, setTimer, timer, CREATE_SUSPENDED, 0); // ����� ��� ������� �������
	workWithTime[8] = CreateThread(NULL, 0, getTimer, timer, CREATE_SUSPENDED, 0); // ����� ��� ������� �������
	workWithTime[9] = CreateThread(NULL, 0, showTimer, timer, CREATE_SUSPENDED, 0); // ����� ��� ������� �������
	workWithTime[10] = CreateThread(NULL, 0, runTimer, timer, CREATE_SUSPENDED, 0); // ����� ��� ������� �������
	workWithTime[11] = CreateThread(NULL, 0, settingsTimer, timer, CREATE_SUSPENDED, 0); // ����� ��� ������� �������

	WaitForMultipleObjects(1, workWithTime, TRUE, INFINITE);

	return 0;
}

void timeSettings(int* forTimeArray) // ��������� �����
{
	while (1)
	{
		printf("��������� �����\n1 - �������� �����\n2 - ��������� �����\n3 - �������� ����������\n4 - �������� ������\n");
		scanf_s("%d", &option);
		switch (option)
		{
		case 1:
			ResumeThread(workWithTime[1]); // �������� ����� � ������� �������
			ResumeThread(workWithTime[2]); // �������� ����� ��� ��������� ������ �������
			SuspendThread(workWithTime[3]); // ������������� ����� � ����
			break;
		case 2:
			ResumeThread(workWithTime[4]); // �������� ����� � ���������� �������
			SuspendThread(workWithTime[3]); // ������������� ����� � ����
			break;
		case 3:
			ResumeThread(workWithTime[5]); // �������� ����� � ������������
			ResumeThread(workWithTime[6]); // �������� ����� � ���������� �����������
			SuspendThread(workWithTime[3]); // ������������� ����� � ����
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
		printf("����: ");
		scanf_s("%d", &forTimer[0]);

		printf("������: ");
		scanf_s("%d", &forTimer[1]);

		printf("�������: ");
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
			printf("������ �������� ������!\n");
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
		printf("0 - �����, 1 - �������� ������, 2 - �����, 3 - ����������� ������\n");

		scanf_s("%d", &option);

		switch (option)
		{
		case 0:
			ResumeThread(workWithTime[10]);
			SuspendThread(workWithTime[9]);
			SuspendThread(workWithTime[11]);
			break;
		case 1:
			printf("������ �������!\n");
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
			printf("0 - �����, 1 - ���������� ����� �����\n");
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
					printf("��� ������ ��������!\n");
					break;
			}
		}
		else
		{
			printf("0 - �����, 1 - �������� ������\n");
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

void getStopwatch(int* forStopwatch) //����������
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

void stopStopwatch(int* forStopwatch) //��������� �����������
{
	while (1)
	{
		printf("������� 0 ��� ������\n");
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
			printf("��� ������ ��������!");
			break;
		}
	}
}

void setTime(int* forTimeArray) // ��������� �������
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
			ResumeThread(workWithTime[3]); // �������� ����� � ����
			SuspendThread(workWithTime[4]); // ������������� ����� � ���������� �������
		}
	}
}


void getCurrentTime(int* forTimeArray) // ������� �������
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

void showCurrentTime(int* forTimeArray) // ����� �������
{
	while (1)
	{
		printf("%d:%d:%d\n", forTimeArray[0], forTimeArray[1], forTimeArray[2]);
		Sleep(950);
	}
}

void stopShowCurrentTime() // ��������� ������ �������
{
	while (1)
	{
		printf("������� 0 ��� ������!\n");
		scanf_s("%d", &option);
		switch (option)
		{
			case 0:
				SuspendThread(workWithTime[1]); // ������������� ����� � ������� �������
				ResumeThread(workWithTime[3]); // �������� ����� � ����
				SuspendThread(workWithTime[2]); // ��������� ����� � ���������� ������ �������
				break;

			default:
				printf("��� ������ ��������! ������� 0 ��� ������!\n");
				break;
		}
	}
}
