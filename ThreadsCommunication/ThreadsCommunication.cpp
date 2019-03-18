// ThreadsCommunication.cpp : 定义控制台应用程序的入口点。
// 进程通信主程序
// 调用子进程，通过共享内存的方式实现进程间通信
//

#include "stdafx.h"
#include "threadCommunication.h"

int _tmain(int argc, _TCHAR* argv[])
{
	threadCommunication main_thre;
	main_thre.run();

	return 0;
}

