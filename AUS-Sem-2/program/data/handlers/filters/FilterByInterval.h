#pragma once

#include "Filter.h"
#include "../criteria/Criterion.h"

template <typename Key, typename Data, typename Value>
class FilterByInterval : public Filter<Key, Data>
{

public:

	FilterByInterval(Criterion<Data, Value>* criterion, Value min, Value max);

	virtual bool criterionMet(Data data) override;

	virtual ~FilterByInterval();

private:

	Criterion<Data, Value>* criterion;
	Value minValue;
	Value maxValue;

};

template<typename Key, typename Data, typename Value>
inline FilterByInterval<Key, Data, Value>::FilterByInterval(Criterion<Data, Value>* criterion, Value minValue, Value maxValue) :
	Filter(),
	criterion(criterion),
	minValue(minValue),
	maxValue(maxValue)
{}

template<typename Key, typename Data, typename Value>
inline bool FilterByInterval<Key, Data, Value>::criterionMet(Data data)
{
	Value value = criterion->evaluate(data);
	return value >= minValue && value <= maxValue;
}

template<typename Key, typename Data, typename Value>
inline FilterByInterval<Key, Data, Value>::~FilterByInterval()
{
	delete criterion;
	criterion = nullptr;
}
