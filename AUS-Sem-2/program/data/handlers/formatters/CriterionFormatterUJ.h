#pragma once

#include <ostream>
#include <sstream>
#include <iomanip>


#include "../criteria/CriterionUJ.h"
#include "CriterionFormatter.h"

using std::ostream;
using std::left;
using std::right;
using std::setw;
using std::setfill;

class CUJ_Formatter : public CriterionFormatter
{

public:

	void writeData(ostream& stream, void* data) final
	{
		writeData(stream, reinterpret_cast<Canton*>(data));
	}

protected:

	virtual void writeData(ostream& stream, Canton* data) = 0;

	// To const or not to const, that is the question
	//static constexpr unsigned short _widthNazov = 48;
	static constexpr const unsigned short _widthNazovObec = 48;
	static constexpr const unsigned short _widthNazovOkres = 28;
	static constexpr const unsigned short _widthNazovKraj = 17;
	static constexpr const unsigned short _widthNazovStat = 9;
	static constexpr const unsigned short _widthNazov = _widthNazovObec + _widthNazovOkres + _widthNazovKraj + _widthNazovStat;
	static constexpr const unsigned short _widthPrislusnost = 11;
	static constexpr const unsigned short _widthObyvatelia = 11;
	static constexpr const unsigned short _widthTyp = 5;
	static constexpr const unsigned short _widthPlocha = 15;
	static constexpr const unsigned short _widthZastavanost = 11;

};

class CUJ_NazovFormatter : public CUJ_Formatter
{

public:

	CUJ_NazovFormatter() : criterion() {};

	void writeInfo(ostream& stream) final
	{
		stream
			<< left
			<< setw(_widthNazov)
			<< setfill(_filler)
			<< "Názov";
	}

	void writeData(ostream& stream, Canton* data) final
	{
		std::ostringstream strstr;
		Canton* item = data;

		do
		{
			strstr
				<< left
				<< setw(getWidth(item->typ))
				<< setfill(_filler)
				<< criterion.evaluate(item);
			item = item->getParent();
		} while (item);

		stream
			<< right
			<< setw(_widthNazov)
			<< setfill(_filler)
			<< strstr.str();
	}

private:

	CUJ_Nazov criterion;

	constexpr const unsigned short getWidth(const Canton::Type type) const
	{
		switch (type)
		{
		case Canton::Type::Obec:
			return _widthNazovObec;

		case Canton::Type::Okres:
			return _widthNazovOkres;

		case Canton::Type::Kraj:
			return _widthNazovKraj;

		case Canton::Type::Stat:
			return _widthNazovStat;

		default:
			return 0;
		}
	}

};

class CUJ_TypFormatter : public CUJ_Formatter
{

public:

	CUJ_TypFormatter() : criterion() {};

	void writeInfo(ostream& stream) final
	{
		stream
			<< left
			<< setw(_widthTyp)
			<< setfill(_filler)
			<< "Typ";
	}

	void writeData(ostream& stream, Canton* data) final
	{
		Canton::Type type = criterion.evaluate(data);

		stream
			<< left
			<< setw(_widthTyp)
			<< setfill(_filler);

		switch (type)
		{
		case Canton::Type::Obec:
			stream << "Obec";
			break;

		case Canton::Type::Okres:
			stream << "Okres";
			break;

		case Canton::Type::Kraj:
			stream << "Kraj";
			break;

		case Canton::Type::Stat:
			stream << "Štát";
			break;

		default:
			throw std::logic_error("CUJ_TypFormatter::writeData() : Invalid type.");
		}
	}

private:

	CUJ_Typ criterion;

};

class CUJ_PrislusnostFormatter : public CUJ_Formatter
{

public:

	CUJ_PrislusnostFormatter() : criterion(nullptr) {};

	~CUJ_PrislusnostFormatter() { delete criterion; };

	void writeInfo(ostream& stream) final
	{
		stream
			<< left
			<< setw(_widthPrislusnost)
			<< setfill(_filler)
			<< "Príslušnos";
	}

	void writeData(ostream& stream, Canton* data) final
	{
		stream
			<< right
			<< setw(_widthPrislusnost)
			<< setfill(_filler)
			<< (criterion->evaluate(data) ? "Patrí" : "Nepatrí");
	}

	void init(ostream& stream) final
	{
		stream << "Zadaj názov príslušného vyššieho územného celku.\n";
		criterion = new CUJ_Prislusnost(ConsoleIO::textLine());
	}

private:

	CUJ_Prislusnost* criterion;

};

class CUJ_PocetMladychFormatter : public CUJ_Formatter
{

public:

	CUJ_PocetMladychFormatter() : criterion() {};

	void writeInfo(ostream& stream) final
	{
		stream
			<< right
			<< setw(_widthObyvatelia)
			<< setfill(_filler)
			<< "Mladí";
	}

	void writeData(ostream& stream, Canton* data) final
	{
		stream
			<< right
			<< setw(_widthObyvatelia)
			<< setfill(_filler)
			<< criterion.evaluate(data);
	}

private:

	CUJ_PocetMladych criterion;

};

class CUJ_PocetDospelychFormatter : public CUJ_Formatter
{

public:

	CUJ_PocetDospelychFormatter() : criterion() {};

	void writeInfo(ostream& stream) final
	{
		stream
			<< right
			<< setw(_widthObyvatelia)
			<< setfill(_filler)
			<< "Dospelí";
	}

	void writeData(ostream& stream, Canton* data) final
	{
		stream
			<< right
			<< setw(_widthObyvatelia)
			<< setfill(_filler)
			<< criterion.evaluate(data);
	}

private:

	CUJ_PocetDospelych criterion;

};

class CUJ_PocetDochodcovFormatter : public CUJ_Formatter
{

public:

	CUJ_PocetDochodcovFormatter() : criterion() {};

	void writeInfo(ostream& stream) final
	{
		stream
			<< right
			<< setw(_widthObyvatelia)
			<< setfill(_filler)
			<< "Dôchodcovia";
	}

	void writeData(ostream& stream, Canton* data) final
	{
		stream
			<< right
			<< setw(_widthObyvatelia)
			<< setfill(_filler)
			<< criterion.evaluate(data);
	}

private:

	CUJ_PocetDochodcov criterion;

};

class CUJ_PocetObyvatelovFormatter : public CUJ_Formatter
{

public:

	CUJ_PocetObyvatelovFormatter() : criterion() {};

	void writeInfo(ostream& stream) final
	{
		stream
			<< right
			<< setw(_widthObyvatelia)
			<< setfill(_filler)
			<< "Obyvatelia";
	}

	void writeData(ostream& stream, Canton* data) final
	{
		stream
			<< right
			<< setw(_widthObyvatelia)
			<< setfill(_filler)
			<< criterion.evaluate(data);
	}

private:

	CUJ_PocetObyvatelov criterion;

};

class CUJ_CelkovaVymeraFormatter : public CUJ_Formatter
{

public:

	CUJ_CelkovaVymeraFormatter() : criterion() {};

	void writeInfo(ostream& stream) final
	{
		stream
			<< right
			<< setw(_widthPlocha)
			<< setfill(_filler)
			<< "Plocha";
	}

	void writeData(ostream& stream, Canton* data) final
	{
		stream
			<< right
			<< setw(_widthPlocha - 4)
			<< setfill(_filler)
			<< criterion.evaluate(data) / 1000.0
			<< " km2";
	}

private:

	CUJ_CelkovaVymera criterion;

};

class CUJ_ZastavanaPlochaFormatter : public CUJ_Formatter
{

public:

	CUJ_ZastavanaPlochaFormatter() : criterion() {};

	void writeInfo(ostream& stream) final
	{
		stream
			<< right
			<< setw(_widthPlocha)
			<< setfill(_filler)
			<< "Zastavané";
	}

	void writeData(ostream& stream, Canton* data) final
	{
		stream
			<< right
			<< setw(_widthPlocha - 4)
			<< setfill(_filler)
			<< criterion.evaluate(data) / 1000.0
			<< " km2";
	}

private:

	CUJ_ZastavanaPlocha criterion;

};

class CUJ_ZastavanostFormatter : public CUJ_Formatter
{

public:

	CUJ_ZastavanostFormatter() : criterion() {};

	void writeInfo(ostream& stream) final
	{
		stream
			<< right
			<< setw(_widthZastavanost)
			<< setfill(_filler)
			<< "Zastavanos";
	}

	void writeData(ostream& stream, Canton* data) final
	{
		stream
			<< right
			<< setw(_widthZastavanost)
			<< setfill(_filler)
			<< criterion.evaluate(data);
	}

private:

	CUJ_Zastavanost criterion;

};

