#pragma once

#include <ostream>
#include <limits.h>
#include <float.h>

#include "../../../common/SvkStr.h"
#include "../../../common/ConsoleIO.h"
#include "../../holders/Canton.h"
#include "../criteria/CriterionUJ.h"
#include "../filters/FilterByValue.h"
#include "../filters/FilterByInterval.h"
#include "../filters/FUJ_Nazov.h"
#include "FilterFormatter.h"

using std::ostream;

typedef signed long long llong;
typedef FUJ_Nazov FilterNazov;
typedef FilterByInterval<SvkStr, Canton*, ullong> FilterPocetObyvatelov;
typedef FilterByInterval<SvkStr, Canton*, double> FilterZastavanost;
typedef FilterByValue<SvkStr, Canton*, Canton::Type> FilterTyp;
typedef FilterByValue<SvkStr, Canton*, bool> FilterPrislusnost;

class FUJ_NazovFormatter : public FilterFormatter<FilterNazov>
{

protected:

	FilterNazov* create(ostream& stream) final
	{
		stream << "Zadajte názov územnej jednotky.\n";
		SvkStr input = ConsoleIO::textLine();

		return new FilterNazov(input);
	}

	void write(ostream& stream) final
	{
		stream << "Filtrovanie pod¾a názvu územnej jednotky.\n";
	}

};

class FUJ_PocetObyvatelovFormatter : public FilterFormatter<FilterPocetObyvatelov>
{

protected:

	FilterPocetObyvatelov* create(ostream& stream) final
	{
		stream << "Zadajte minimálny poèet obyvate¾ov.\n";
		auto min = ConsoleIO::numberAtLeast<llong>(LLONG_MAX);

		stream << "\nZadajte maximálny poèet obyvate¾ov.\n";
		auto max = ConsoleIO::numberAtLeast<llong>(LLONG_MAX, min);

		CUJ_PocetObyvatelov* criterion = new CUJ_PocetObyvatelov();
		return new FilterPocetObyvatelov(criterion, min, max);
	}

	void write(ostream& stream) final
	{
		stream << "Filtrovanie pod¾a poètu obyvate¾ov územnej jednotky.\n";
	}

};

class FUJ_ZastavanostFormatter : public FilterFormatter<FilterZastavanost>
{

protected:

	FilterZastavanost* create(ostream& stream) final
	{
		stream << "Zadajte minimálnu zastavanos.\n";
		double min = ConsoleIO::numberBetween<double>(100.0, 0.0);

		stream << "\nZadajte maximálnu zastavanos.\n";
		double max = ConsoleIO::numberBetween<double>(100.0, min);

		CUJ_Zastavanost* criterion = new CUJ_Zastavanost();
		return new FilterZastavanost(criterion, min, max);
	}

	void write(ostream& stream) final
	{
		stream << "Filtrovanie pod¾a percentuálnej zastavanosti územnej jednotky.\n";
	}

};

class FUJ_TypFormatter : public FilterFormatter<FilterTyp>
{

protected:

	FilterTyp* create(ostream& stream) final
	{
		double input = ConsoleIO::chooseFrom(
			{"Obec", "Okres", "Kraj", "Slovensko"},
			"Vyberte typ územnej jednotky:",
			-1
		);
		Canton::Type type = static_cast<Canton::Type>(input);

		CUJ_Typ* criterion = new CUJ_Typ();
		return new FilterTyp(criterion, type);
	}

	void write(ostream& stream) final
	{
		stream << "Filtrovanie pod¾a typu územnej jednotky.\n";
	}

};

class FUJ_PrislusnostFormatter : public FilterFormatter<FilterPrislusnost>
{

protected:

	FilterPrislusnost* create(ostream& stream) final
	{
		stream << "Zadajte názov príslušnej územnej jednotky.\n";
		SvkStr input = ConsoleIO::textLine();

		CUJ_Prislusnost* criterion = new CUJ_Prislusnost(input);
		return new FilterPrislusnost(criterion, true);
	}

	void write(ostream& stream) final
	{
		stream << "Filtrovanie pod¾a príslušnosti územnej jednotky.\n";
	}

};

