#pragma once

#include <ostream>
#include "../../../common/ConsoleIO.h"

using std::ostream;

template <typename _Filter>
class FilterFormatter
{

public:

	_Filter* prepare(ostream& stream);

protected:

	virtual _Filter* create(ostream& stream) = 0;

	virtual void write(ostream& stream) = 0;

};

template<typename _Filter>
inline _Filter* FilterFormatter<_Filter>::prepare(ostream& stream)
{
	write(stream);
	bool input = ConsoleIO::decide("éel·te si pouûiù tento filter?");

	if (input)
	{
		_Filter* filter = create(stream);
		return filter;
	}

	return nullptr;
}
