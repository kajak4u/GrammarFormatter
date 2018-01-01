#pragma once
#include <iterator>

//wrapper structure for reverse range-based for loop
template <typename _T> struct Reversion
{
	_T& collection;
};
//begin returning reverse begin
template <typename _T> auto begin(Reversion<_T> rev)
{
	return std::rbegin(rev.collection);
}
//end returning reverse end
template <typename _T> auto end(Reversion<_T> rev)
{
	return std::rend(rev.collection);
}
//inline move conversion
template <typename _T> Reversion<_T> Reverse(_T&& collection)
{
	return{ collection };
}
//inline copy conversion
template <typename _T> Reversion<_T> Reverse(const _T& collection)
{
	return{ collection };
}