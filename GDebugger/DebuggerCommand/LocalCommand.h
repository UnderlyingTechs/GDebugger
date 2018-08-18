#pragma once
#include "../DebuggerCore/DebuggerService.h"
#include <list>

/**
 * 命令参数描述对象
 */
class CommandParameterDescription
{
public:
	/**
	 * 获取参数名称
	 */
	std::string GetName() const { return this->parameterName; }

	/**
	 * 获取参数描述，支持 command /?命令，提供参数的所有描述信息
	 */
	std::string GetDescription() const { return this->parameterDescription; }

	/**
	 * 根据指定的参数名称和参数描述创建一个参数描述对象
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
 * 命令元数据，提供命令名称形式，说明，以及参数说明等信息
 */
class CommandMetadata
{
public:

    /**
     * 获取当前命令支持的参数集合
     */
	std::list<CommandParameterDescription*> GetParameterDescriptions() const
	{
		return this->parameters;
	};

	/**
	 * 获取当前命令支持的所有形式的名称
	 */
	std::list<std::string*> GetCommandNames() const
	{
		return this->names;
	}

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
	BOOL IsCommandNameMatch(const std::string&) const;

	/**
	 * 添加新的参数描述项
	 */
	void AddParameterDescription(CommandParameterDescription* parameter)
	{
		this->parameters.push_back(parameter);
	};

	/**
	 * 添加一个命令名称
	 */
	void AddCommandName(std::string* name)
	{
		// TODO: 检查命令名称不允许相同
		this->names.push_back(name);
	}

private:
	std::list<CommandParameterDescription*> parameters;
	std::list<std::string*> names;
};

/**
 * \brief 该头文件定义控制台命令结构支持
 */
class DebugCommand
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
class CommandService
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
	DebugCommand* FindCommand(const std::string&);

	/**
	 * 根据一个字符串集合查找命令，只要满足其中任何一个名称，即返回当前找到的命令对象指针
	 */
	DebugCommand* FindCommand(std::list<std::string*> names);

	/**
	 * 注册调试命令，外部命令插件可以在dllMain中获取调试服务，并注册支持的插件
	 */
	void RegisterDebugCommand(DebugCommand*);

	/**
	 * 返回当前命令服务的所有支持命令，所有的命令对象不允许修改
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
