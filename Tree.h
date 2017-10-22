#pragma once
#include <map>
#include <iostream>

template<typename _VALUE> class CTree
{
	const _VALUE* current = nullptr;
	std::map<char, CTree*> children;
public:
	~CTree()
	{
		if (current)
			delete current;
	}
	void add(const std::string& index, const _VALUE* value)
	{
		CTree<_VALUE>* current = this;
		for (auto i : index)
		{
			if (current->children[i] == nullptr)
				current->children[i] = new CTree<_VALUE>();
			current = current->children[i];
		}
		current->current = value;
	}
	const _VALUE* get(std::istream& is)
	{
		char c = is.get();
		auto child = children.find(c);
		const _VALUE* result = child == children.end() ? current : child->second->get(is);
		is.putback(c);
		return result;
	}
};