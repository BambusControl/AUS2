#pragma once

#include <iostream>
using std::ostream;

class CriterionFormatter
{

public:

	virtual void writeInfo(ostream& stream) = 0;

	virtual void writeData(ostream& stream, void* data) = 0;

	virtual ~CriterionFormatter() {};

	virtual void init(ostream& stream) {};

protected:

	static constexpr char _filler = ' ';

};

