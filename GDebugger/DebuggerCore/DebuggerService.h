#pragma once

#include <windows.h>
#include "DebuggerProcess.h"


/*
 * ���ĵĵ��Է����࣬һ��DebuggerService ����һ�����Է����û����Ի��ڸ÷�������
 * ���Խ��̣����߸��ӵ�һ���Ѿ����ڵĽ��̡�
 */
class __declspec(dllexport) DebuggerService
{
public:

	/*
	 * ʹ��Ĭ�����ó�ʼ��
	 */
	DebuggerService();

	/*
	 * ָ����Ӧ�������ļ���ʼ��
	 */
	DebuggerService(TCHAR*);

	/*
	 * ���Է����ʼ��������������ʽ������װ�ظ��Ӳ���������Ϣ������û�
	 * ��Ҫ��������·�������ã����ڵ���Initialize֮ǰ���ȵ��� AddParameterFilePath
	 * ����AddServiceOptionsָ��������Ϣ�������ó�ʼ������ʼ�����֮�󲿷�����
	 * ��֧�ָ��¡�
	 */
	void Initialzie();

	/*
	 * ���ӵ�һ���Ѿ����ڵĽ��̣�����ɹ��򷵻� ��Ӧ�ĵ��Զ���
	 */
	DWORD AttachProcess(HANDLE, DebuggerProcess*&);

	/*
	 * �����ļ�������һ���½��̵��ԣ�����ɹ��򷵻� ��Ӧ���̵ĵ��Զ���
	 */
	DWORD DebugProcess(TCHAR*, DebuggerProcess*&);
};

