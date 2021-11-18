#pragma once

#include "../criteria/Criterion.h"
#include "../../../structures/table/table.h"

using structures::SequenceTable;
using structures::Table;
using structures::TableItem;

enum class FilterType
{
	UJNazov = 0,
	UJPocetObyvatelov = 1,
	UJZastavanost = 2,
	UJTyp = 3,
	UJPrislusnost = 4
};

template <typename Key, typename Data>
class Filter
{

public:

	typedef Table<Key, Data> _Table;

	Filter() {};
	
	virtual ~Filter() {};

	virtual _Table& filter(const _Table& toFilter, _Table& output);

	virtual bool criterionMet(Data data) = 0;

};

template<typename Key, typename Data>
inline Table<Key, Data>& Filter<Key, Data>::filter(const _Table&toFilter, _Table&output)
{
	for (TableItem<Key, Data>* item : toFilter)
	{
		if (criterionMet(item->accessData()))
		{
			output.insert(item->getKey(), item->accessData());
		}
	}

	return output;
}
