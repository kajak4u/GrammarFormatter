#pragma once
#include <unordered_map>


template<typename _ITEM> class CObjectManager
{
	std::unordered_map<std::string, _ITEM*> memory;
	CObjectManager() {}
	~CObjectManager()
	{
		for (auto keyVal : memory)
			delete keyVal.second;
	}
	CObjectManager(const CObjectManager&) = delete;
	CObjectManager(CObjectManager&&) = delete;
	CObjectManager& operator=(const CObjectManager&) = delete;
	static CObjectManager& Instance()
	{
		static CObjectManager<_ITEM> instance;
		return instance;
	}
public:
	static _ITEM* Register(const std::string& str)
	{
		auto& instance = Instance();
		auto iter = instance.memory.find(str);
		if (iter != instance.memory.end())
			return iter->second;
		auto newItem = new _ITEM();
		instance.memory[str] = newItem;
		return newItem;
	}
	static const std::unordered_map<std::string, _ITEM*> GetMemory()
	{
		auto& instance = Instance();
		return instance.memory;
	}
	static void PrintMemory()
	{
		std::cerr << "Memory: " << std::endl;
		auto& instance = Instance();
		for (const auto& keyVal : instance.memory)
			std::cerr << keyVal.first << " -> " << *keyVal.second << std::endl;
	}
};

