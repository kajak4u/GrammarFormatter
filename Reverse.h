#pragma once
#include <iterator>

template <typename _T> struct Reversion
{
	_T& collection;
};

template <typename _T> auto begin(Reversion<_T> rev)
{
	return std::rbegin(rev.collection);
}

template <typename _T> auto end(Reversion<_T> rev)
{
	return std::rend(rev.collection);
}

template <typename _T> Reversion<_T> Reverse(_T&& collection)
{
	return{ collection };
}

template <typename _T> Reversion<_T> Reverse(const _T& collection)
{
	return{ collection };
}