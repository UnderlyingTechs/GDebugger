#pragma once
#include "../DebuggerCore/DebuggerService.h"
#include <list>

using String = std::string;
using Int32 = long;

enum CommandTokenType
{
	Identifier,
	ParameterName,
	StringValue
};

struct CommandToken
{
	Int32			 Row;
	Int32			 Col;

	CommandTokenType TokenType;
	String*			 TokenValue;
};

/**
 * ������Ϣ����
 */
class __declspec(dllexport) ErrorInfo
{
public:

	/**
	 * ��ȡ�����ڵ��ı���
	 */
	Int32 GetColumn() const { return this->column; }

	/**
	 * ��ȡ�������ڵ��ı���
	 */
	Int32 GetRow() const { return this->row; }

	/**
	 * ��ȡ������Ϣ
	 */
	String* GetError() const { return this->error; }

	ErrorInfo(Int32 row, Int32 col, String* error)
	{
		this->row = row;
		this->column = col;
		this->error = error;
	}

	~ErrorInfo();
private:
	Int32 column = 0;
	Int32 row = 0;
	String* error = nullptr;
};

/**
 * ����һ���ַ��������Ĳ�������
 */
class __declspec(dllexport) ParsedParameter
{
public:

	ParsedParameter();
	~ParsedParameter();

	/**
	 * ���һ�������ɹ�����������
	 */
	void SetName(String* name)
	{
		this->name = name;
	}

	/**
	 * ��ȡ��ǰ����������ɵ�����
	 */
	String* GetName() const
	{
		return this->name;
	}

	/**
	 * ��ȡ��ǰ����������ɵ�����ֵ
	 */
	std::list<String*> GetValues() const
	{
		return this->values;
	}

	/**
	 * ���һ�������ɹ��Ĳ���ֵ��ע��ýӿ�ֻ�ڹ���������󣬽�������ʱ����
	 */
	void AddValue(String* const value)
	{
		this->values.push_back(value);
	}

private:
	String* name = nullptr;
	std::list<String*> values;
};

/**
 * ͨ���ַ�����������һ������ʵ��
 */
class __declspec(dllexport) ParsedCommand
{
public:

	/**
	* ���һ�������ɹ�����������
	*/
	void SetName(String* pName)
	{
		this->pName = pName;
	}

	/**
	* ��ȡ��ǰ����������ɵ�����
	*/
	String* GetName() const
	{
		return this->pName;
	}

	/**
	 * ��ȡ�����Ӧ�Ĳ����б�
	 */
	std::list<ParsedParameter*> GetParameters() const
	{
		return this->parameters;
	}

	/**
	 * ���һ����������ɹ��Ĳ������÷���ֻ�ڽ�������ʱ����
	 */
	void AddParameter(ParsedParameter* pParameter)
	{
		this->parameters.push_back(pParameter);
	}

	/**
	 * ���һ��������Ϣ����ǰ�����������У��÷���ֻ�ڽ�������ʱ����
	 */
	void AddError(ErrorInfo* pErrorInfo)
	{
		this->errors.push_back(pErrorInfo);
	}

	/**
	 * ��ȡ��ǰ��������д�����Ϣ�б�
	 */
	std::list<ErrorInfo*> GetErrors() const
	{
		return this->errors;
	}

	/**
	 * �жϵ�ǰ�����б����Ƿ���ڴ�����Ϣ
	 */
	BOOL HasError() const
	{
		return !this->errors.empty();
	}
private:
	String* pName = nullptr;
	std::list<ParsedParameter*> parameters;
	std::list<ErrorInfo*> errors;
};

/**
 * ����һ�����������࣬�ó��������Ծ��ж����ͬ���Ƶ����֣����ҿ��������Ƿ�֧�ִ�Сд
 * ���У�������������ƺͲ������ƾ�����������
 */
class DifferenceNamesOwner
{
public:

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
	BOOL IsCommandNameMatch(const String&) const;

	/**
	* ��ȡ��ǰ����֧�ֵ�������ʽ������
	*/
	std::list<String> GetNames() const
	{
		return this->names;
	}

	/**
	* ���һ����������
	*/
	void AddName(const String name)
	{
		// TODO: ����������Ʋ�������ͬ
		this->names.push_back(name);
	}
private:
	std::list<String> names;
};

/**
 * ���������������
 */
class __declspec(dllexport) ParameterDescription : public DifferenceNamesOwner
{
public:
	ParameterDescription(String description)
	{
		this->parameterDescription = description;
	}

	/**
	 * ��ȡ����������֧�� command /?����ṩ����������������Ϣ
	 */
	String GetDescription() const { return this->parameterDescription; }
private:
	String parameterDescription;
};

/**
 * ����Ԫ���ݣ��ṩ����������ʽ��˵�����Լ�����˵������Ϣ
 */
class __declspec(dllexport) CommandMetadata : public DifferenceNamesOwner
{
public:

    /**
     * ��ȡ��ǰ����֧�ֵĲ�������
     */
	std::list<ParameterDescription*> GetParameterDescriptions() const
	{
		return this->parameters;
	};

	/**
	 * ����µĲ���������
	 */
	void AddParameterDescription(ParameterDescription* parameter)
	{
		this->parameters.push_back(parameter);
	};

private:
	std::list<ParameterDescription*> parameters;

};

/**
 * \brief ��ͷ�ļ��������̨����ṹ֧��
 */
class __declspec(dllexport) DebugCommand
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
class __declspec(dllexport) CommandService
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
	DebugCommand* FindCommand(const String&);

	/**
	 * ����һ���ַ������ϲ������ֻҪ���������κ�һ�����ƣ������ص�ǰ�ҵ����������ָ��
	 */
	DebugCommand* FindCommand(std::list<String> names);

	

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

	/**
	 * �����ַ���������������
	 */
	static ParsedCommand* ParseCommand(String&);

	
private: 
	CommandService();

	CommandService(const CommandService&);

	static CommandToken* ReadNextToken(const String& content, Int32* pNextReadPosition);

	static ParsedParameter* ParseCommandParameter(const String& content, Int32* pNextReadPosition);

	static CommandService*   instance;
	DebuggerService*         pDebuggerService;
	DebuggerProcess*         pDebuggerProcess;
	std::list<DebugCommand*> commands;
};
