#pragma once
#include "../DebuggerCommand/LocalCommand.h"

/*
 * �����������������
 */

/**
 * �������Խ����������������� debug��db��d��������ļ�·����Ϊ��������
 * ���Խ���
 */
class DebugProcessCommand : public DebugCommand
{
public:
	DebugProcessCommand();
	~DebugProcessCommand();

	void RunCommand() override;
};