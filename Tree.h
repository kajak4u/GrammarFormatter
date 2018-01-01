#pragma once
#include <map>
#include <iostream>

//Prefix tree for incremental recognizing. Each edge corresponds to single character in given word
template<typename _VALUE> class CTree
{
	const _VALUE* current = nullptr;
	_STD map<char, CTree*> children;
public:
	~CTree()
	{
		clear();
	}
	void clear()
	{
		for (auto& node : children)
			delete node.second;
		children.clear();
		if (current)
			delete current;
		current = nullptr;
	}
	void add(const _STD string& index, const _VALUE* value)
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
	const _VALUE* Predict(_STD istream& is) const
	{
		char c = is.get();
		auto child = children.find(c);
		const _VALUE* result = child == children.end() ? current : child->second->Predict(is);
		is.putback(c);
		return result;
	}
	const _VALUE* GetFromStream(_STD istream& is) const
	{
		static _STD string mem = "";
		char c = is.get();
		if (is.eof())
			return current;
		mem += c;
		auto child = children.find(c);
		const _VALUE* result;
		if (child == children.end())
		{
			result = current;
			is.putback(c);
		}
		else
			result = child->second->GetFromStream(is);
		if (result == nullptr && !is.eof())
			throw MyException("Unrecognized symbol '"+mem+"'.", 1);
		mem.pop_back();
		return result;
	}
};