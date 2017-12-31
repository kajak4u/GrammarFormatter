#pragma once
#include <map>

//global memory for associations [key] => [_ITEM]
template<typename _ITEM> class CObjectManager
{
	//memory
	_STD map <_STD string, _ITEM* > memory;
	//private constructor, destructor, deleted copying and assigning - singleton
	CObjectManager() {}
	~CObjectManager()
	{
		for (auto keyVal : memory)
			delete keyVal.second;
	}
	CObjectManager(const CObjectManager&) = delete;
	CObjectManager(CObjectManager&&) = delete;
	CObjectManager& operator=(const CObjectManager&) = delete;
	//static method returning the only object's instance
	static CObjectManager& Instance()
	{
		static CObjectManager<_ITEM> instance;
		return instance;
	}
public:
	//registers new entry or returns an existing one for key duplicate
	static _ITEM* Register(const _STD string& str)
	{
		auto& instance = Instance();
		auto iter = instance.memory.find(str);
		if (iter != instance.memory.end())
			return iter->second;
		auto newItem = new _ITEM();
		instance.memory[str] = newItem;
		return newItem;
	}
	//returns memory
	static const _STD map<_STD string, _ITEM*> GetMemory()
	{
		auto& instance = Instance();
		return instance.memory;
	}
	//prints memory onto given stream
	static void PrintMemory(_STD ostream& os)
	{
		auto& instance = Instance();
		for (const auto& keyVal : instance.memory)
			os << keyVal.first << " -> " << *keyVal.second << _STD endl;
	}
};