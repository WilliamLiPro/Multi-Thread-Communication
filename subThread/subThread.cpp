// subThread.cpp : 定义控制台应用程序的入口点。
// 进程通信子程序
// 被主进程调用，通过共享内存的方式实现进程间通信

#include "stdafx.h"
#include <windows.h>
#include <string>

using namespace std;

void cleanUp(HANDLE hMapFile,PVOID pView);

int _tmain(int argc, _TCHAR* argv[])
{
	printf("大家好，我是子进程 t(^v^)t ,负责发送信息 \n");

	HANDLE hMapFile;		//共享内存块句柄
	PVOID pView;			//共享内存块在当前内存的映射
	STARTUPINFOA si;		//父进程给子进程传递的参数
	PROCESS_INFORMATION pi;	//新进程的信息保存于此

	string map_name_="map_test";		//共享内存名称

	//连接并打开共享内存
	hMapFile = OpenFileMappingA(    
        FILE_MAP_ALL_ACCESS,        // Read and write access    
        FALSE,						// Do not inherit the name    
        map_name_.c_str()           // File mapping name     
        );    
    if (hMapFile == NULL)     
    {    
        printf("子进程: OpenFileMapping failed w/err 0x%08lx\n", GetLastError());    
        cleanUp( hMapFile, NULL);
		return -1;
    }    
    printf("子进程: The file mapping (%s) is opened\n", map_name_.c_str());    


	//创建共享内存块在当前内存的映射
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
        printf("子进程: MapViewOfFile failed w/err 0x%08lx\n", GetLastError());     
        cleanUp(hMapFile, pView);
		return -1;
    }    
    printf("子进程: The file view is mapped\n");


	unsigned char image[120000];

	for (int i = 0; i < 120000; i++)
	{
		image[i]=1;
	}

	//保存并显示结果
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

	printf("按下任意键退出\n");
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