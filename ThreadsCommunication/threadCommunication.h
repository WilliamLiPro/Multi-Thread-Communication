//
/*********************************************

�����ͨ��ͷ�ļ�

ʹ�ö��߳�ͨ�Ż��ƴ�����Ϣ
�����߳�ͨ�ŵĽ�����Ӧ���Ƽ�״̬��

**********************************************/

#include <stdio.h>
#include <Windows.h>
#include <string>

using namespace std;
//�����ͨ�Ų�����
//���ý��̲����в�������
class threadCommunication
{
public:
	threadCommunication();

	~threadCommunication();

	//ͨ��������
	void run();
private:
	//ͨ�ų�ʼ��
	int communicationInitialize();

	//����ռ�
	void cleanUp();

	//�̼߳�ͨ�Ų���
	HANDLE hMapFile;		//�����ڴ����
	PVOID pView;			//�����ڴ���ڵ�ǰ�ڴ��ӳ��
	STARTUPINFOA si;		//�����̸��ӽ��̴��ݵĲ���
	PROCESS_INFORMATION pi;	//�½��̵���Ϣ�����ڴ�

	string map_name_;		//�����ڴ�����


	//�����ý�����Ϣ
	string sub_thread_path_;//�ӽ���·��
	string sub_thread_name_;//�ӽ�������
	string commandline_;	//�ӽ��̵������в���
};