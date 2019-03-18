// subThread.cpp : �������̨Ӧ�ó������ڵ㡣
// ����ͨ���ӳ���
// �������̵��ã�ͨ�������ڴ�ķ�ʽʵ�ֽ��̼�ͨ��

#include "stdafx.h"
#include <windows.h>
#include <string>

using namespace std;

void cleanUp(HANDLE hMapFile,PVOID pView);

int _tmain(int argc, _TCHAR* argv[])
{
	printf("��Һã������ӽ��� t(^v^)t ,��������Ϣ \n");

	HANDLE hMapFile;		//�����ڴ����
	PVOID pView;			//�����ڴ���ڵ�ǰ�ڴ��ӳ��
	STARTUPINFOA si;		//�����̸��ӽ��̴��ݵĲ���
	PROCESS_INFORMATION pi;	//�½��̵���Ϣ�����ڴ�

	string map_name_="map_test";		//�����ڴ�����

	//���Ӳ��򿪹����ڴ�
	hMapFile = OpenFileMappingA(    
        FILE_MAP_ALL_ACCESS,        // Read and write access    
        FALSE,						// Do not inherit the name    
        map_name_.c_str()           // File mapping name     
        );    
    if (hMapFile == NULL)     
    {    
        printf("�ӽ���: OpenFileMapping failed w/err 0x%08lx\n", GetLastError());    
        cleanUp( hMapFile, NULL);
		return -1;
    }    
    printf("�ӽ���: The file mapping (%s) is opened\n", map_name_.c_str());    


	//���������ڴ���ڵ�ǰ�ڴ��ӳ��
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	int view_size=240000;

	pView = MapViewOfFile(    
        hMapFile,               // Handle of the map object    
        FILE_MAP_ALL_ACCESS,    // All access    
        0,                      // High-order DWORD of the file offset     
        0,						// Low-order DWORD of the file offset    
        view_size				// The number of bytes to map to view    
        );

	if (pView == NULL)    
    {     
        printf("�ӽ���: MapViewOfFile failed w/err 0x%08lx\n", GetLastError());     
        cleanUp(hMapFile, pView);
		return -1;
    }    
    printf("�ӽ���: The file view is mapped\n");


	unsigned char image[120000];

	for (int i = 0; i < 120000; i++)
	{
		image[i]=1;
	}

	//���沢��ʾ���
	char number[1024];
	for(int i=0;i<10000;i++)
	{
		sprintf(number, "%d", i);
		string message="the number is ";
		int str_length=message.size();

		char* msg_send=(char*)malloc(122400);
		char* msg_p=msg_send;
		memcpy(msg_p, &str_length, sizeof(int));
		msg_p+=sizeof(int);
		memcpy(msg_p, message.c_str(), str_length);
		msg_p+=str_length;
		memcpy(msg_p, &i, sizeof(int));
		msg_p+=sizeof(int);
		memcpy(msg_p, image, 120000*sizeof(unsigned char));


		memcpy_s(pView, view_size, msg_send, 122400);

		printf("This message is written to the view:\n\"%s%d\"\n", message.c_str(),i);
		Sleep(1);

		free(msg_send);
	}

	cleanUp(hMapFile, pView);

	printf("����������˳�\n");
	getchar();
	return 0;
}

void cleanUp(HANDLE hMapFile,PVOID pView)
{
	if (hMapFile)    
    {    
        if (pView)    
        {    
            // Unmap the file view.    
            UnmapViewOfFile(pView);    
            pView = NULL;    
        }    
        // Close the file mapping object.    
        CloseHandle(hMapFile);    
        hMapFile = NULL;    
    }    
}