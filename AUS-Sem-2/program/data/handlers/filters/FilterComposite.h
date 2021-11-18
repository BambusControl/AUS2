#pragma once

#include <list>

#include "Filter.h"
#include "FUJ_Nazov.h"

template <typename Key, typename Data>
class FilterComposite : public Filter<Key, Data>
{

	typedef Filter<Key, Data> _Filter;

public:
	
	FilterComposite();
	
	~FilterComposite();

	_Table& filter(const _Table& toFilter, _Table& output) final;

	void add(_Filter* filter);

	bool criterionMet(Data data) final;

private:

	std::list<_Filter*>* filters;
	FUJ_Nazov* nazovFitler;

};

template<typename Key, typename Data>
inline FilterComposite<Key, Data>::FilterComposite() :
	Filter<Key, Data>(),
	filters(new std::list<_Filter*>()),
	nazovFitler(nullptr)
{}

template<typename Key, typename Data>
inline FilterComposite<Key, Data>::~FilterComposite()
{
	for (_Filter* filter : *filters)
	{
		delete filter;
		filter = nullptr;
	}

	delete filters;
	filters = nullptr;
	delete nazovFitler;
	nazovFitler = nullptr;
}

template<typename Key, typename Data>
inline Table<Key, Data>& FilterComposite<Key, Data>::filter(const _Table& toFilter, _Table& output)
{
	if (nazovFitler)
	{
		_Table* temp = output.clone();
		nazovFitler->filter(toFilter, *temp);
		_Filter::filter(*temp, output);
		delete temp;
	}
	else
	{
		_Filter::filter(toFilter, output);
	}

	return output;
}


template<typename Key, typename Data>
inline void FilterComposite<Key, Data>::add(_Filter* filter)
{
	if (filter == nullptr)
	{
		return;
	}

	if (nazovFitler)
	{
		filters->push_back(filter);
	}
	else
	{
		nazovFitler = dynamic_cast<FUJ_Nazov*>(filter);

		if (nazovFitler == nullptr)
		{
			filters->push_back(filter);
		}
	}
}

template<typename Key, typename Data>
inline bool FilterComposite<Key, Data>::criterionMet(Data data)
{
	for (_Filter* filter : *filters)
	{
		if (!filter->criterionMet(data))
		{
			return false;
		}
	}

	return true;
}
