//
/*********************************************

多进程通信头文件

使用多线程通信机制传递信息
测试线程通信的交互响应机制及状态锁

**********************************************/

#include <stdio.h>
#include <Windows.h>
#include <string>

using namespace std;
//多进程通信测试类
//调用进程并进行参数传递
class threadCommunication
{
public:
	threadCommunication();

	~threadCommunication();

	//通信主程序
	void run();
private:
	//通信初始化
	int communicationInitialize();

	//清理空间
	void cleanUp();

	//线程及通信参数
	HANDLE hMapFile;		//共享内存块句柄
	PVOID pView;			//共享内存块在当前内存的映射
	STARTUPINFOA si;		//父进程给子进程传递的参数
	PROCESS_INFORMATION pi;	//新进程的信息保存于此

	string map_name_;		//共享内存名称


	//被调用进程信息
	string sub_thread_path_;//子进程路径
	string sub_thread_name_;//子进程名称
	string commandline_;	//子进程的命令行参数
};