#pragma once

#include "FilterByValue.h"
#include "../../../common/SvkStr.h"
#include "../../holders/Canton.h"
#include "../criteria/CriterionUJ.h"

class FUJ_Nazov : public FilterByValue<SvkStr, Canton*, SvkStr>
{
public:

	FUJ_Nazov(SvkStr& nazov);

	virtual ~FUJ_Nazov();

	_Table& filter(const _Table& toFilter, _Table& output) final;

private:

};

inline FUJ_Nazov::FUJ_Nazov(SvkStr& nazov) :
	FilterByValue(new CUJ_Nazov(), nazov)
{}

inline FUJ_Nazov::~FUJ_Nazov()
{}

Table<SvkStr, Canton*>& FUJ_Nazov::filter(const _Table& toFilter, _Table& output)
{
	try
	{
		output.insert(value, toFilter[value]);
	}
	catch (std::out_of_range&)
	{}

	return output;
}