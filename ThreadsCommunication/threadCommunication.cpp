//
/*********************************************

�����ͨ��ͷ�ļ�

ʹ�ö��߳�ͨ�Ż��ƴ�����Ϣ
�����߳�ͨ�ŵĽ�����Ӧ���Ƽ�״̬��

**********************************************/

#include "threadCommunication.h"

/********** public���� ***********/

//ͨ�����ʼ������
threadCommunication::threadCommunication()
{
	//������ʼ��
	map_name_="map_test";			//�����ڴ�����

	sub_thread_path_="E:/��������ʶ��/�Ա����/���߳�ͨ��/ThreadsCommunication/Debug/";	//�ӽ���·��
	sub_thread_name_="subThread.exe";	//�ӽ�������
	commandline_="";		//�ӽ��̵������в���

}

threadCommunication::~threadCommunication()
{
	cleanUp();
}


//ͨ��������
void threadCommunication::run()
{
	//�����ڴ��ʼ��
	printf("���������� \\(^v^)/ ,��������ӽ��̲�������Ϣ\n");
	communicationInitialize();

	//���ý��̴���
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	memset(&si, 0, sizeof(si));  
    si.cb = sizeof(STARTUPINFO);  
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;//SW_HIDE���ش���
	si.lpTitle="sub thread";

	//ִ���ӽ���
	bool success=CreateProcessA((sub_thread_path_+sub_thread_name_).c_str(),(LPSTR)commandline_.c_str(), 0, 0, TRUE, CREATE_NEW_CONSOLE, 0, 0, &si, &pi);
	if(!success)
	{
		printf("���̴���ʧ��\n");
		return;
	}

	//��ʾ�����ڴ�����
	int finish=-1;
	string showmsg;showmsg.reserve(100);
	string lastmsg="";
	while(finish!=0)
	{
		//��ȡ��������
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

		//��ʾ���ݵ�����
		if(showmsg!=lastmsg)
		{
			printf("Read from the file mapping:\n\"%s\"\n", showmsg.c_str());
			lastmsg=showmsg;
		}
		Sleep(1);

		//�ȴ����̽���
		finish=WaitForSingleObject(pi.hProcess, 2);
	}  

	//�������ڴ�
	cleanUp();

	//��ȡ�ӽ��̵��˳���  
	DWORD  dwExitCode; 
	GetExitCodeProcess(pi.hProcess, &dwExitCode);  

	//�ر��ӽ��̾��  
	CloseHandle(pi.hProcess);  

	printf("����������˳�\n");
	getchar();
}


/********** private���� ***********/
//ͨ�ų�ʼ��
int threadCommunication::communicationInitialize()
{
	//���������ڴ�鼰���
	hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, 4200224, map_name_.c_str());	//���������ڴ����(�ɶ�д)
	if (hMapFile == NULL)     
    {    
        printf("������: CreateFileMapping failed w/err 0x%08lx\n", GetLastError());    
        cleanUp();
		return -1;
    }    
	printf("������: The file mapping (%s) is created\n", map_name_.c_str());

	SetHandleInformation(hMapFile, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);			//ʹ�þ�����Լ̳е��ӽ���

	//���������ڴ���ڵ�ǰ�ڴ��ӳ��
	pView = MapViewOfFile(    
        hMapFile,               // Handle of the map object    
        FILE_MAP_READ,          // Read access    
        0,                      // High-order DWORD of the file offset     
        0,						// Low-order DWORD of the file offset    
        4200124					// The number of bytes to map to view    
        );

	if (pView == NULL)    
    {     
        printf("������: MapViewOfFile failed w/err 0x%08lx\n", GetLastError());     
        cleanUp();
		return -1;
    }    
    printf("������: The file view is mapped\n");    

	return 0;
}

//����ռ�
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