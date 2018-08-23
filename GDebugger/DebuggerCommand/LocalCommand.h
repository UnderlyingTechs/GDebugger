#pragma once
#include "../DebuggerCore/DebuggerService.h"
#include <string>
#include <vector>

using String = std::string;
using Size = size_t;
using StringList = std::vector<String*>*;
using ConstStringList = std::vector<const String*>;

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

using ErrorInfoList = std::vector<ErrorInfo*>;

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
	StringList GetValues() const
	{
		return this->values;
	}

	/**
	 * ���һ�������ɹ��Ĳ���ֵ��ע��ýӿ�ֻ�ڹ���������󣬽�������ʱ����
	 */
	void AddValue(String* const value) const
	{
		this->values->push_back(value);
	}

private:
	String*    name = nullptr;
	StringList values{};
};

using ParsedParameterList = std::vector<ParsedParameter*>;

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
	ParsedParameterList GetParameters() const
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

	void AddError(Size row, Size col, String* pError)
	{
		const auto pErrorInfo = new ErrorInfo(row, col, pError);
		this->AddError(pErrorInfo);
	}

	/**
	 * ��ȡ��ǰ��������д�����Ϣ�б�
	 */
	ErrorInfoList GetErrors() const
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

	/**
	 * ����������ָ��λ�ü�������ֵ
	 */
	String* FindParameterValue(const String& name, Size index = 0) const;

	/**
	 * ����ָ�����Ƽ�������ֵ����
	 */
	StringList FindParameterValues(const String& name) const;

private:
	String*             pName = nullptr;
	ParsedParameterList parameters;
	ErrorInfoList       errors;
};

/**
 * ����ʱ����������
 */
struct RunningContext
{
	ParsedCommand*		pCommandInfo;

};

struct ParseContext
{
	String*        pText;
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
	ConstStringList GetNames() const
	{
		return this->names;
	}

	/**
	* ���һ����������
	*/
	void AddName(const String* name)
	{
		// TODO: ����������Ʋ�������ͬ
		this->names.push_back(name);
	}
private:
	ConstStringList names;
};

/**
 * ���������������
 */
class __declspec(dllexport) ParameterDescription : public DifferenceNamesOwner
{
public:
	ParameterDescription(String name, String description, BOOL isOptional)
	{
		this->AddName(new String(name));
		this->parameterDescription = description;
		this->isOptional = isOptional;
	}

	/**
	 * ��ȡ����������֧�� command /?����ṩ����������������Ϣ
	 */
	String GetDescription() const { return this->parameterDescription; }

	/**
	 * ��ȡһ����־˵����ǰ�����Ƿ��ѡ
	 */
	BOOL IsOptional() const
	{
		return this->isOptional;
	}
private:
	String parameterDescription;
	BOOL   isOptional;
};

using ParameterDescriptionList = std::vector<ParameterDescription*>;

/**
 * ����Ԫ���ݣ��ṩ����������ʽ��˵�����Լ�����˵������Ϣ
 */
class __declspec(dllexport) CommandMetadata : public DifferenceNamesOwner
{
public:

    /**
     * ��ȡ��ǰ����֧�ֵĲ�������
     */
	ParameterDescriptionList GetParameterDescriptions() const
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
	ParameterDescriptionList parameters;

};

/**
 * \brief ��ͷ�ļ��������̨����ṹ֧��
 */
class __declspec(dllexport) DebugCommand
{
public:
	virtual ~DebugCommand() = default;

	/**
	 * ��ȡ�ض��������������������������ƣ���ǰ�Ĳ����Լ�ע�͵ȵȡ�
	 */
	CommandMetadata* GetCommandMetadata() const
	{
		return this->pMetadata;
	}


	/**
	* ִ������ķ������ض�������ʵ�ָ÷�����ɵ��������
	*/
	void RunCommand(RunningContext*);

protected:

	/**
	 * ���õ�ǰ�����Ԫ���ݶ���
	 */
	void SetCommandMetadata(CommandMetadata* pMetadata)
	{
		this->pMetadata = pMetadata;
	}

	virtual void RunCommandCore(RunningContext*) = 0;
private:
	CommandMetadata * pMetadata = nullptr;
};

using DebugCommandList = std::vector<DebugCommand*>;

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
	DebugCommand* FindCommand(std::vector<std::string const*> names);

	

	/**
	 * ע���������ⲿ������������dllMain�л�ȡ���Է��񣬲�ע��֧�ֵĲ��
	 */
	void RegisterDebugCommand(DebugCommand*);

	/**
	 * ���ص�ǰ������������֧��������е�������������޸�
	 */
	DebugCommandList GetCommands() const
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
	DebugCommandList         commands;
};
