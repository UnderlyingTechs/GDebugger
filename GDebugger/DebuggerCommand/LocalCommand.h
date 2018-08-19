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
 * 错误信息对象
 */
class __declspec(dllexport) ErrorInfo
{
public:

	/**
	 * 获取错误在的文本列
	 */
	Size GetColumn() const { return this->column; }

	/**
	 * 获取错误所在的文本行
	 */
	Size GetRow() const { return this->row; }

	/**
	 * 获取错误信息
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
 * 代表一个字符串解析的参数对象
 */
class __declspec(dllexport) ParsedParameter
{
public:

	ParsedParameter();
	~ParsedParameter();

	/**
	 * 添加一个解析成功的命令名称
	 */
	void SetName(String* name)
	{
		this->name = name;
	}

	/**
	 * 获取当前参数解析完成的名称
	 */
	String* GetName() const
	{
		return this->name;
	}

	/**
	 * 获取当前参数解析完成的所有值
	 */
	std::vector<String*> GetValues() const
	{
		return this->values;
	}

	/**
	 * 添加一个解析成功的参数值，注意该接口只在构造参数对象，解析命令时调用
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
 * 通过字符串解析出的一个命令实例
 */
class __declspec(dllexport) ParsedCommand
{
public:

	/**
	* 添加一个解析成功的命令名称
	*/
	void SetName(String* pName)
	{
		this->pName = pName;
	}

	/**
	* 获取当前参数解析完成的名称
	*/
	String* GetName() const
	{
		return this->pName;
	}

	/**
	 * 获取命令对应的参数列表
	 */
	std::vector<ParsedParameter*> GetParameters() const
	{
		return this->parameters;
	}

	/**
	 * 添加一个命令解析成功的参数，该方法只在解析命令时调用
	 */
	void AddParameter(ParsedParameter* pParameter)
	{
		this->parameters.push_back(pParameter);
	}

	/**
	 * 添加一条错误信息到当前解析的命令中，该方法只在解析命令时调用
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
	 * 获取当前命令的所有错误信息列表
	 */
	std::vector<ErrorInfo*> GetErrors() const
	{
		return this->errors;
	}

	/**
	 * 判断当前命令列表中是否存在错误信息
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
 * 定义一个抽象对象基类，该抽象对象可以具有多个不同名称的名字，并且可以设置是否支持大小写
 * 敏感，比如命令的名称和参数名称均有这种属性
 */
class DifferenceNamesOwner
{
public:

	///**
	// * 检查名称是否匹配当前命令
	// */
	//BOOL IsCommandNameMatch(TCHAR*) const;

	/**
	* 检查名称是否匹配当前命令
	*/
	BOOL IsCommandNameMatch(WCHAR*) const;

	/**
	* 检查名称是否匹配当前命令
	*/
	BOOL IsCommandNameMatch(const String&) const;

	/**
	* 获取当前对象支持的所有形式的名称
	*/
	std::vector<String> GetNames() const
	{
		return this->names;
	}

	/**
	* 添加一个命令名称
	*/
	void AddName(const String name)
	{
		// TODO: 检查命令名称不允许相同
		this->names.push_back(name);
	}
private:
	std::vector<String> names;
};

/**
 * 命令参数描述对象
 */
class __declspec(dllexport) ParameterDescription : public DifferenceNamesOwner
{
public:
	ParameterDescription(String description)
	{
		this->parameterDescription = description;
	}

	/**
	 * 获取参数描述，支持 command /?命令，提供参数的所有描述信息
	 */
	String GetDescription() const { return this->parameterDescription; }
private:
	String parameterDescription;
};

/**
 * 命令元数据，提供命令名称形式，说明，以及参数说明等信息
 */
class __declspec(dllexport) CommandMetadata : public DifferenceNamesOwner
{
public:

    /**
     * 获取当前命令支持的参数集合
     */
	std::vector<ParameterDescription*> GetParameterDescriptions() const
	{
		return this->parameters;
	};

	/**
	 * 添加新的参数描述项
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
 * \brief 该头文件定义控制台命令结构支持
 */
class __declspec(dllexport) DebugCommand
{
public:
	virtual ~DebugCommand() = default;

	/**
	 * 执行命令的方法，特定的命令实现该方法完成调试命令功能
	 */
	virtual void RunCommand() = 0;

	/**
	 * 获取特定的命令描述符，描述名称名称，当前的参数以及注释等等。
	 */
	CommandMetadata* GetCommandMetadata() const
	{
		return this->pMetadata;
	}

protected:

	/**
	 * 设置当前命令的元数据对象
	 */
	void SetCommandMetadata(CommandMetadata* pMetadata)
	{
		this->pMetadata = pMetadata;
	}

private:
	CommandMetadata * pMetadata = nullptr;
};

/**
 * \brief 定义命令服务，插件可以使用该服务注册新的支持命令
 */
class __declspec(dllexport) CommandService
{
public :
	/**
	 * 获取命令服务实例
	 */
	static CommandService* GetInstance();

	/**
	 * 获取调试服务实例
	 */
	DebuggerService* GetDebuggerService() const { return this->pDebuggerService; }

	/**
	 * 获取当前的调试进程
	 */
	DebuggerProcess* GetDebuggerProcess() const { return this->pDebuggerProcess; }

	/**
	 * 设置当前的调试进程
	 */
	void SetDebuggerProcess(DebuggerProcess* pDebuggerProcess)
	{
		this->pDebuggerProcess = pDebuggerProcess;
	}

	/**
	 * 根据命令名称检索调试命令
	 */
	DebugCommand* FindCommand(WCHAR*);

	/**
	 * 根据命令名称检索调试命令
	 */
	DebugCommand* FindCommand(const String&);

	/**
	 * 根据一个字符串集合查找命令，只要满足其中任何一个名称，即返回当前找到的命令对象指针
	 */
	DebugCommand* FindCommand(std::vector<String> names);

	

	/**
	 * 注册调试命令，外部命令插件可以在dllMain中获取调试服务，并注册支持的插件
	 */
	void RegisterDebugCommand(DebugCommand*);

	/**
	 * 返回当前命令服务的所有支持命令，所有的命令对象不允许修改
	 */
	std::vector<DebugCommand*> GetCommands() const
	{
		return this->commands;
	}

	/**
	 * 根据字符串解析命令序列
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
