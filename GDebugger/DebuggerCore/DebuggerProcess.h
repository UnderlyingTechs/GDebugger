#pragma once

extern class DebuggerService;

enum ProcessDebugState
{
	/*
	 * ��ǰ�����Ѿ����ж�
	 */
	Break,

	/*
	 * ��ǰ������������
	 */
	Running
};

/*
* ����һ����ǰ��ȡ�ĵ��Խ���
*/
class DebuggerProcess
{
public:

	DebuggerProcess(DebuggerService* pDebuggerService) { this->pDebugService = pDebuggerService; }

	/*
	 * ��ȡ��ǰ���Խ��̶���ĵ��Է���
	 */
	DebuggerService * GetService() const { return this->pDebugService; }



private:
	DebuggerService * pDebugService;
	
};