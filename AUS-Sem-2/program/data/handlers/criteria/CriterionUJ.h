#pragma once

#include "../../holders/Canton.h"
#include "../../../common/SvkStr.h"
#include "Criterion.h"

template <typename Out>
class CriterionUJ : public Criterion<Canton*, Out>
{

public:

	CriterionUJ(CriterionType type) : Criterion(type) {}

};

class CUJ_Nazov : public CriterionUJ<SvkStr>
{

public:
	CUJ_Nazov() : CriterionUJ(CriterionType::UJ_Nazov)
	{}

	// Inherited via CriteriumUJ
	SvkStr evaluate(Canton* data) final
	{
		return data->nazov;
	}

};

class CUJ_Typ: public CriterionUJ<Canton::Type>
{

public:
	CUJ_Typ() : CriterionUJ(CriterionType::UJ_Typ)
	{}

	// Inherited via CriteriumUJ
	Canton::Type evaluate(Canton* data) final
	{
		return data->typ;
	}

};

class CUJ_Prislusnost : public CriterionUJ<bool>
{

public:
	CUJ_Prislusnost(SvkStr upperCantonName) :
		CriterionUJ(CriterionType::UJ_Prislusnost),
		upperCantonName(upperCantonName)
	{}

	// Inherited via CriteriumUJ
	bool evaluate(Canton* data) final
	{
		Canton* item = data;
		while (item->getParent())
		{
			item = item->getParent();
			if (item->nazov == upperCantonName)
			{
				return true;
			}
		}
		return false;
	}

private:
	SvkStr upperCantonName;

};

class CUJ_PocetMladych : public CriterionUJ<ullong>
{

public:
	CUJ_PocetMladych() : CriterionUJ(CriterionType::UJ_PocetMladych)
	{}

	// Inherited via CriteriumUJ
	ullong evaluate(Canton* data) final
	{
		return data->pocetMladych;
	}

};

class CUJ_PocetDospelych : public CriterionUJ<ullong>
{

public:
	CUJ_PocetDospelych() : CriterionUJ(CriterionType::UJ_PocetDospelych)
	{}

	// Inherited via CriteriumUJ
	ullong evaluate(Canton* data) final
	{
		return data->pocetDospelych;
	}

};

class CUJ_PocetDochodcov : public CriterionUJ<ullong>
{

public:
	CUJ_PocetDochodcov() : CriterionUJ(CriterionType::UJ_PocetDochodcov)
	{}

	// Inherited via CriteriumUJ
	ullong evaluate(Canton* data) final
	{
		return data->pocetDochodcov;
	}

};

class CUJ_PocetObyvatelov : public CriterionUJ<ullong>
{

public:
	CUJ_PocetObyvatelov() : CriterionUJ(CriterionType::UJ_PocetObyvatelov)
	{}

	// Inherited via CriteriumUJ
	ullong evaluate(Canton* data) final
	{
		return 
			data->pocetMladych +
			data->pocetDospelych +
			data->pocetDochodcov;
	}

};

class CUJ_CelkovaVymera : public CriterionUJ<double>
{

public:
	CUJ_CelkovaVymera() : CriterionUJ(CriterionType::UJ_CelkovaVymera)
	{}

	// Inherited via CriteriumUJ
	double evaluate(Canton* data) final
	{
		return data->celkovaVymera;
	}

};

class CUJ_ZastavanaPlocha : public CriterionUJ<double>
{

public:
	CUJ_ZastavanaPlocha() : CriterionUJ(CriterionType::UJ_ZastavanaPlocha)
	{}

	// Inherited via CriteriumUJ
	double evaluate(Canton* data) final
	{
		return data->zastavanaPlocha;
	}

};

class CUJ_Zastavanost : public CriterionUJ<double>
{

public:
	CUJ_Zastavanost() : CriterionUJ(CriterionType::UJ_Zastavanost)
	{}

	// Inherited via CriteriumUJ
	double evaluate(Canton* data) final
	{
		return 100 * data->zastavanaPlocha / data->celkovaVymera;
	}

};
