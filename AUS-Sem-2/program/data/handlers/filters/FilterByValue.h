#pragma once

#include "Filter.h"
#include "../criteria/Criterion.h"

template <typename Key,typename Data,typename Value>
class FilterByValue : public Filter<Key, Data>
{

public:

	FilterByValue(Criterion<Data, Value>* criterion, Value value);

	virtual bool criterionMet(Data data) override;

	virtual ~FilterByValue();

	Value getValue();

protected:

	Value value;

private:

	Criterion<Data, Value>* criterion;

};

template<typename Key, typename Data, typename Value>
inline FilterByValue<Key, Data, Value>::FilterByValue(Criterion<Data, Value>* criterion, Value value) :
	Filter(),
	criterion(criterion),
	value(value)
{}

template<typename Key, typename Data, typename Value>
inline bool FilterByValue<Key, Data, Value>::criterionMet(Data data)
{
	return criterion->evaluate(data) == value;
}

template<typename Key, typename Data, typename Value>
inline FilterByValue<Key, Data, Value>::~FilterByValue()
{
	delete criterion;
	criterion = nullptr;
}

template<typename Key, typename Data, typename Value>
inline Value FilterByValue<Key, Data, Value>::getValue()
{
	return value;
}
