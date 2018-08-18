#pragma once
#include "../DebuggerCore/DebuggerService.h"
#include <list>

/**
 * ���������������
 */
class CommandParameterDescription
{
public:
	/**
	 * ��ȡ��������
	 */
	std::string GetName() const { return this->parameterName; }

	/**
	 * ��ȡ����������֧�� command /?����ṩ����������������Ϣ
	 */
	std::string GetDescription() const { return this->parameterDescription; }

	/**
	 * ����ָ���Ĳ������ƺͲ�����������һ��������������
	 */
	static CommandParameterDescription* Create(std::string name, std::string description)
	{
		const auto parameter = new CommandParameterDescription(name, description);
		return parameter;
	}

private:
	std::string parameterName;
	std::string parameterDescription;

	CommandParameterDescription(std::string, std::string);
};

/**
 * ����Ԫ���ݣ��ṩ����������ʽ��˵�����Լ�����˵������Ϣ
 */
class CommandMetadata
{
public:

    /**
     * ��ȡ��ǰ����֧�ֵĲ�������
     */
	std::list<CommandParameterDescription*> GetParameterDescriptions() const
	{
		return this->parameters;
	};

	/**
	 * ��ȡ��ǰ����֧�ֵ�������ʽ������
	 */
	std::list<std::string*> GetCommandNames() const
	{
		return this->names;
	}

	///**
	// * ��������Ƿ�ƥ�䵱ǰ����
	// */
	//BOOL IsCommandNameMatch(TCHAR*) const;

	/**
	 * ��������Ƿ�ƥ�䵱ǰ����
	 */
	BOOL IsCommandNameMatch(WCHAR*) const;

	/**
	 * ��������Ƿ�ƥ�䵱ǰ����
	 */
	BOOL IsCommandNameMatch(const std::string&) const;

	/**
	 * ����µĲ���������
	 */
	void AddParameterDescription(CommandParameterDescription* parameter)
	{
		this->parameters.push_back(parameter);
	};

	/**
	 * ���һ����������
	 */
	void AddCommandName(std::string* name)
	{
		// TODO: ����������Ʋ�������ͬ
		this->names.push_back(name);
	}

private:
	std::list<CommandParameterDescription*> parameters;
	std::list<std::string*> names;
};

/**
 * \brief ��ͷ�ļ��������̨����ṹ֧��
 */
class DebugCommand
{
public:
	virtual ~DebugCommand() = default;

	/**
	 * ִ������ķ������ض�������ʵ�ָ÷�����ɵ��������
	 */
	virtual void RunCommand() = 0;

	/**
	 * ��ȡ�ض��������������������������ƣ���ǰ�Ĳ����Լ�ע�͵ȵȡ�
	 */
	CommandMetadata* GetCommandMetadata() const
	{
		return this->pMetadata;
	}

protected:

	/**
	 * ���õ�ǰ�����Ԫ���ݶ���
	 */
	void SetCommandMetadata(CommandMetadata* pMetadata)
	{
		this->pMetadata = pMetadata;
	}

private:
	CommandMetadata * pMetadata = nullptr;
};

/**
 * \brief ����������񣬲������ʹ�ø÷���ע���µ�֧������
 */
class CommandService
{
public :
	/**
	 * ��ȡ�������ʵ��
	 */
	static CommandService* GetInstance();

	/**
	 * ��ȡ���Է���ʵ��
	 */
	DebuggerService* GetDebuggerService() const { return this->pDebuggerService; }

	/**
	 * ��ȡ��ǰ�ĵ��Խ���
	 */
	DebuggerProcess* GetDebuggerProcess() const { return this->pDebuggerProcess; }

	/**
	 * ���õ�ǰ�ĵ��Խ���
	 */
	void SetDebuggerProcess(DebuggerProcess* pDebuggerProcess)
	{
		this->pDebuggerProcess = pDebuggerProcess;
	}

	/**
	 * �����������Ƽ�����������
	 */
	DebugCommand* FindCommand(WCHAR*);

	/**
	 * �����������Ƽ�����������
	 */
	DebugCommand* FindCommand(const std::string&);

	/**
	 * ����һ���ַ������ϲ������ֻҪ���������κ�һ�����ƣ������ص�ǰ�ҵ����������ָ��
	 */
	DebugCommand* FindCommand(std::list<std::string*> names);

	/**
	 * ע���������ⲿ������������dllMain�л�ȡ���Է��񣬲�ע��֧�ֵĲ��
	 */
	void RegisterDebugCommand(DebugCommand*);

	/**
	 * ���ص�ǰ������������֧��������е�������������޸�
	 */
	std::list<DebugCommand*> GetCommands() const
	{
		return this->commands;
	}

private: 
	CommandService();

	CommandService(const CommandService&);

	static CommandService*   instance;
	DebuggerService*         pDebuggerService;
	DebuggerProcess*         pDebuggerProcess;
	std::list<DebugCommand*> commands;
};
