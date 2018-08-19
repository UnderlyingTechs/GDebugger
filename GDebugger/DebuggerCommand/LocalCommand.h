#pragma once
#include "../DebuggerCore/DebuggerService.h"
#include <string>
#include <vector>

using String = std::string;
using Size = size_t;

enum CommandTokenType
{
	Unknow,
	Identifier,
	ParameterName,
	StringValue,
	End
};

struct CommandToken
{
	Size			 Row;
	Size			 Col;
	BOOL			 IsTokenEated;
	CommandTokenType TokenType;
	String*			 pTokenValue;
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
	Size GetColumn() const { return this->column; }

	/**
	 * ��ȡ�������ڵ��ı���
	 */
	Size GetRow() const { return this->row; }

	/**
	 * ��ȡ������Ϣ
	 */
	String* GetError() const { return this->pError; }

	ErrorInfo(Size row, Size col, String* error)
	{
		this->row = row;
		this->column = col;
		this->pError = error;
	}

	~ErrorInfo();
private:
	Size column = 0;
	Size row = 0;
	String* pError = nullptr;
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
	std::vector<String*> GetValues() const
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
	std::vector<String*> values;
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
	std::vector<ParsedParameter*> GetParameters() const
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

	void AddError(Size row, Size col, std::string* pError)
	{
		const auto pErrorInfo = new ErrorInfo(row, col, pError);
		this->AddError(pErrorInfo);
	}

	/**
	 * ��ȡ��ǰ��������д�����Ϣ�б�
	 */
	std::vector<ErrorInfo*> GetErrors() const
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
	std::vector<ParsedParameter*> parameters;
	std::vector<ErrorInfo*> errors;
};


struct ParseContext
{
	std::string*   pText;
	Size           Row;
	Size		   Col;
	Size		   Pos;
	ParsedCommand* pCommand;
	CommandToken*  pToken;
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
	std::vector<String> GetNames() const
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
	std::vector<String> names;
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
	std::vector<ParameterDescription*> GetParameterDescriptions() const
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

	~CommandMetadata();
private:
	std::vector<ParameterDescription*> parameters;

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
	DebugCommand* FindCommand(std::vector<String> names);

	

	/**
	 * ע���������ⲿ������������dllMain�л�ȡ���Է��񣬲�ע��֧�ֵĲ��
	 */
	void RegisterDebugCommand(DebugCommand*);

	/**
	 * ���ص�ǰ������������֧��������е�������������޸�
	 */
	std::vector<DebugCommand*> GetCommands() const
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

	static BOOL ReadNextToken(ParseContext* pContext);

	static ParsedParameter* ParseCommandParameter(ParseContext* pContext);

	static BOOL ReadIdentifierToken(ParseContext* pContext);

	static BOOL ReadParameterNameToken(ParseContext* pContext);

	static BOOL ReadStringToken(ParseContext* pContext);

	static BOOL IsWhitespaceChar(char ch);

	static CommandService*   instance;
	DebuggerService*         pDebuggerService;
	DebuggerProcess*         pDebuggerProcess;
	std::vector<DebugCommand*> commands;
};
