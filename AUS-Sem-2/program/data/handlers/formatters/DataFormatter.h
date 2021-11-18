#pragma once

#include <initializer_list>
#include <ostream>
#include <windows.h>
#include <wincon.h>

#include "CriterionFormatterUJ.h"
#include "FilterFormatterUJ.h"
#include "../filters/FilterComposite.h"
#include "../../holders/Canton.h"
#include "../../../structures/table/sorted_sequence_table.h"
#include "../../../structures/table/unsorted_sequence_table.h"

using std::initializer_list;
using std::ostream;
using std::cout;
using std::endl;
using structures::Table;
using structures::SortedSequenceTable;
using structures::UnsortedSequenceTable;

class DataFormatter
{

public:

	static SortedSequenceTable<SvkStr, Canton*>* filterCanton(
		Canton& item, 
		initializer_list<FilterType> types, 
		ostream& stream = cout
	);

	template<typename Key, typename Data>
	static void writeTable(
		Table<Key, Data>& dataTable, 
		initializer_list<CriterionType> criteria, 
		ostream& stream = cout
	);

	template<typename Key, typename Data>
	static void sort(UnsortedSequenceTable<Key, Data>& dataTable, CriterionType criterionType);

private:

	static CriterionFormatter* createCriterionFormatter(CriterionType type);

	template<typename Key, typename Data>
	static Filter<Key, Data>* prepareFilter(FilterType type, ostream& stream);

	static Table<SvkStr, Canton*>& filterCanton(
		Canton& item,
		initializer_list<FilterType>& types,
		Table<SvkStr, Canton*>& output,
		ostream& stream = cout
	);
};

inline CriterionFormatter* DataFormatter::createCriterionFormatter(CriterionType type)
{
	switch (type)
	{
	case CriterionType::UJ_Nazov:
		return new CUJ_NazovFormatter();

	case CriterionType::UJ_Typ:
		return new CUJ_TypFormatter();

	case CriterionType::UJ_Prislusnost:
		return new CUJ_PrislusnostFormatter();

	case CriterionType::UJ_PocetMladych:
		return new CUJ_PocetMladychFormatter();

	case CriterionType::UJ_PocetDospelych:
		return new CUJ_PocetDospelychFormatter();

	case CriterionType::UJ_PocetDochodcov:
		return new CUJ_PocetDochodcovFormatter();

	case CriterionType::UJ_PocetObyvatelov:
		return new CUJ_PocetObyvatelovFormatter();

	case CriterionType::UJ_CelkovaVymera:
		return new CUJ_CelkovaVymeraFormatter();

	case CriterionType::UJ_ZastavanaPlocha:
		return new CUJ_ZastavanaPlochaFormatter();

	case CriterionType::UJ_Zastavanost:
		return new CUJ_ZastavanostFormatter();

	default:
		throw std::invalid_argument("FormatterFactory::createCriterion() :: invalid criterion type.");
	}
}

template<typename Key, typename Data>
inline Filter<Key, Data>* DataFormatter::prepareFilter(FilterType type, ostream& stream)
{
	switch (type)
	{
	case FilterType::UJNazov:
	{
		FUJ_NazovFormatter ff;
		return ff.prepare(stream);
	}

	case FilterType::UJPocetObyvatelov:
	{
		FUJ_PocetObyvatelovFormatter ff;
		return ff.prepare(stream);
	}

	case FilterType::UJZastavanost:
	{
		FUJ_ZastavanostFormatter ff;
		return ff.prepare(stream);
	}

	case FilterType::UJTyp:
	{
		FUJ_TypFormatter ff;
		return ff.prepare(stream);
	}

	case FilterType::UJPrislusnost:
	{
		FUJ_PrislusnostFormatter ff;
		return ff.prepare(stream);
	}

	default:
		throw std::invalid_argument("DataFormatter::prepareFilter() : invalid FilterType.");
	}
}

inline Table<SvkStr, Canton*>& DataFormatter::filterCanton(Canton& item, initializer_list<FilterType>& types, Table<SvkStr, Canton*>& output, ostream& stream)
{

	FilterComposite<SvkStr, Canton*> composite;
	Filter<SvkStr, Canton*>* filter;

	bool filterUJTypeSet = false;
	Canton::Type filterUJType = Canton::Type::Stat;

	for (FilterType type : types)
	{
		filter = prepareFilter<SvkStr, Canton*>(type, stream);

		if (type == FilterType::UJTyp)
		{
			//using _FilterTyp = FilterByValue<SvkStr, Canton*, Canton::Type>;
			if (filter && !filterUJTypeSet)
			{
				filterUJType = reinterpret_cast<FilterTyp*>(filter)->getValue();
				delete filter;
				filter = nullptr;
				filterUJTypeSet = true;
			}
		}
		else
		{
			composite.add(filter);
		}
	}

	if (filterUJTypeSet)
	{
		using Citer = Canton::CantonTypeIterator;
		using Ctype = Canton::Type;

		if (filterUJType == Ctype::Stat && item.typ == Ctype::Stat)
		{
			if (composite.criterionMet(&item))
			{
				output.insert(item.nazov, &item);
			}
		}
		else if (filterUJType == Ctype::Kraj)
		{
			composite.filter(*item.getChildren(), output);
		}
		else
		{
			Citer it(&item, (Canton::Type)((int)filterUJType + 1));
			Citer itEnd;
			Canton* canton;

			for (; it != itEnd; ++it)
			{
				canton = *it;
				composite.filter(*canton->getChildren(), output);
			}
		}
	}
	else
	{
		SortedSequenceTable<SvkStr, Canton*> table;
		item.toTable(table);
		composite.filter(table, output);
	}

	return output;
}

inline SortedSequenceTable<SvkStr, Canton*>* DataFormatter::filterCanton(Canton& item, initializer_list<FilterType> types, ostream& stream)
{
	auto table = new SortedSequenceTable<SvkStr, Canton*>();
	filterCanton(item, types, *table, stream);
	return table;
}

template<typename Key, typename Data>
inline void DataFormatter::writeTable(Table<Key, Data>& dataTable, initializer_list<CriterionType> criteria, ostream& stream)
{
	using CF = CriterionFormatter;
	using CT = CriterionType;
	using TItem = TableItem<Key, Data>;
	constexpr char* delimeter = " | ";

	std::list<CF*> formatters;

	// Initialise formatters
	for (CT type : criteria)
	{
		auto formatter = createCriterionFormatter(type);
		formatter->init(stream);
		formatters.push_back(formatter);
	}

	// Write formatter header
	stream << delimeter;
	for (CF* formatter : formatters)
	{
		formatter->writeInfo(stream);
		stream << delimeter;
	}
	stream << endl;

	// Write data
	for (TItem* item : dataTable)
	{
		stream << delimeter;
		for (CF* formatter : formatters)
		{
			formatter->writeData(stream, item->accessData());
			stream << delimeter;
		}
		stream << endl;
	}

	// Release resources
	for (CF* formatter : formatters)
	{
		delete formatter;
	}
}

template<typename Key, typename Data>
inline void DataFormatter::sort(UnsortedSequenceTable<Key, Data>& dataTable, CriterionType criterionType)
{
	bool ascending = ConsoleIO::decide(
		"Vzostupne", "Zostupne", "Zoradenie prvkov:"
	);

	switch (criterionType)
	{

	case CriterionType::UJ_Nazov:
	{
		if (!ascending)
		{
			CUJ_Nazov criterion;
			Sequencer::reverse(dataTable);
		}
		return; 
	}

	case CriterionType::UJ_Typ:
	{
		CUJ_Typ criterion;
		Sequencer::heapSort(dataTable, criterion, ascending);
		return;
	}

	case CriterionType::UJ_PocetMladych:
	{
		CUJ_PocetMladych criterion;
		Sequencer::heapSort(dataTable, criterion, ascending);
		return;
	}

	case CriterionType::UJ_PocetDospelych:
	{
		CUJ_PocetDospelych criterion;
		Sequencer::heapSort(dataTable, criterion, ascending);
		return;
	}

	case CriterionType::UJ_PocetDochodcov:
	{
		CUJ_PocetDochodcov criterion;
		Sequencer::heapSort(dataTable, criterion, ascending);
		return;
	}

	case CriterionType::UJ_PocetObyvatelov:
	{
		CUJ_PocetObyvatelov criterion;
		Sequencer::heapSort(dataTable, criterion, ascending);
		return;
	}

	case CriterionType::UJ_CelkovaVymera:
	{
		CUJ_CelkovaVymera criterion;
		Sequencer::heapSort(dataTable, criterion, ascending);
		return;
	}

	case CriterionType::UJ_ZastavanaPlocha:
	{
		CUJ_ZastavanaPlocha criterion;
		Sequencer::heapSort(dataTable, criterion, ascending);
		return;
	}

	case CriterionType::UJ_Zastavanost:
	{
		CUJ_Zastavanost criterion;
		Sequencer::heapSort(dataTable, criterion, ascending);
		return;
	}

	default:
		throw std::invalid_argument("FormatterFactory::sort() : Invalid CriterionType value.");
	}
}
