//
/*********************************************

多进程通信头文件

使用多线程通信机制传递信息
测试线程通信的交互响应机制及状态锁

**********************************************/

#include "threadCommunication.h"

/********** public函数 ***********/

//通信类初始化函数
threadCommunication::threadCommunication()
{
	//参数初始化
	map_name_="map_test";			//共享内存名称

	sub_thread_path_="E:/物体检测与识别/自编程序/多线程通信/ThreadsCommunication/Debug/";	//子进程路径
	sub_thread_name_="subThread.exe";	//子进程名称
	commandline_="";		//子进程的命令行参数

}

threadCommunication::~threadCommunication()
{
	cleanUp();
}


//通信主程序
void threadCommunication::run()
{
	//共享内存初始化
	printf("我是主进程 \\(^v^)/ ,负责调用子进程并接收信息\n");
	communicationInitialize();

	//设置进程窗口
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	memset(&si, 0, sizeof(si));  
    si.cb = sizeof(STARTUPINFO);  
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;//SW_HIDE隐藏窗口
	si.lpTitle="sub thread";

	//执行子进程
	bool success=CreateProcessA((sub_thread_path_+sub_thread_name_).c_str(),(LPSTR)commandline_.c_str(), 0, 0, TRUE, CREATE_NEW_CONSOLE, 0, 0, &si, &pi);
	if(!success)
	{
		printf("进程创建失败\n");
		return;
	}

	//显示共享内存内容
	int finish=-1;
	string showmsg;showmsg.reserve(100);
	string lastmsg="";
	while(finish!=0)
	{
		//读取传递内容
		showmsg.clear();
		char* recieve=(PSTR)pView;
		int string_size=*((int*)recieve);
		recieve+=sizeof(int);
		for (int i = 0; i < string_size; i++)
		{
			showmsg.push_back(*(char*)recieve);
			(char*)recieve++;
		}
		showmsg=showmsg+std::to_string(*((int*)recieve));

		//显示传递的内容
		if(showmsg!=lastmsg)
		{
			printf("Read from the file mapping:\n\"%s\"\n", showmsg.c_str());
			lastmsg=showmsg;
		}
		Sleep(1);

		//等待进程结束
		finish=WaitForSingleObject(pi.hProcess, 2);
	}  

	//清理共享内存
	cleanUp();

	//获取子进程的退出码  
	DWORD  dwExitCode; 
	GetExitCodeProcess(pi.hProcess, &dwExitCode);  

	//关闭子进程句柄  
	CloseHandle(pi.hProcess);  

	printf("按下任意键退出\n");
	getchar();
}


/********** private函数 ***********/
//通信初始化
int threadCommunication::communicationInitialize()
{
	//创建共享内存块及句柄
	hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, 4200224, map_name_.c_str());	//创建共享内存对象(可读写)
	if (hMapFile == NULL)     
    {    
        printf("主进程: CreateFileMapping failed w/err 0x%08lx\n", GetLastError());    
        cleanUp();
		return -1;
    }    
	printf("主进程: The file mapping (%s) is created\n", map_name_.c_str());

	SetHandleInformation(hMapFile, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);			//使得句柄可以继承到子进程

	//创建共享内存块在当前内存的映射
	pView = MapViewOfFile(    
        hMapFile,               // Handle of the map object    
        FILE_MAP_READ,          // Read access    
        0,                      // High-order DWORD of the file offset     
        0,						// Low-order DWORD of the file offset    
        4200124					// The number of bytes to map to view    
        );

	if (pView == NULL)    
    {     
        printf("主进程: MapViewOfFile failed w/err 0x%08lx\n", GetLastError());     
        cleanUp();
		return -1;
    }    
    printf("主进程: The file view is mapped\n");    

	return 0;
}

//清理空间
void threadCommunication::cleanUp()
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