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
	const _VALUE* Predict(std::istream& is) const
	{
		char c = is.get();
		auto child = children.find(c);
		const _VALUE* result = child == children.end() ? current : child->second->Predict(is);
		is.putback(c);
		return result;
	}
	const _VALUE* GetFromStream(std::istream& is) const
	{
		static std::string mem = "";
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
			throw MyException("Unrecognized symbol '"+mem+"'.", -12);
		mem.pop_back();
		return result;
	}
};