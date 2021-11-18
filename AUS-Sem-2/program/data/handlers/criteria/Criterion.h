#pragma once

enum class CriterionType {
	UJ_Nazov,
	UJ_Typ,
	UJ_Prislusnost,
	UJ_PocetMladych,
	UJ_PocetDospelych,
	UJ_PocetDochodcov,
	UJ_PocetObyvatelov,
	UJ_CelkovaVymera,
	UJ_ZastavanaPlocha,
	UJ_Zastavanost
};

template <typename In, typename Out>
class Criterion
{

public:

	Criterion(CriterionType type);

	virtual Out evaluate(In data) = 0;

	virtual ~Criterion() {};

private:

	CriterionType type;

};

template<typename In, typename Out>
inline Criterion<In, Out>::Criterion(CriterionType type) :
	type(type)
{}
