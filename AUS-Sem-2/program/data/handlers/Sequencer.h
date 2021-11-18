#pragma once

#include "../../structures/table/unsorted_sequence_table.h"
#include "criteria/Criterion.h"

using structures::UnsortedSequenceTable;


class Sequencer
{

public:

	template <typename Key, typename Data, typename Value>
	static UnsortedSequenceTable<Key, Data>& bubbleSort(UnsortedSequenceTable<Key, Data>& toSort, Criterion<Data, Value>& criterion, bool ascending = true);

	template <typename Key, typename Data, typename Value>
	static UnsortedSequenceTable<Key, Data>& selectSort(UnsortedSequenceTable<Key, Data>& toSort, Criterion<Data, Value>& criterion, bool ascending = true);

	template <typename Key, typename Data, typename Value>
	static UnsortedSequenceTable<Key, Data>& quickSort(UnsortedSequenceTable<Key, Data>& toSort, Criterion<Data, Value>& criterion, bool ascending = true);

	template <typename Key, typename Data, typename Value>
	static UnsortedSequenceTable<Key, Data>& heapSort(UnsortedSequenceTable<Key, Data>& toSort, Criterion<Data, Value>& criterion, bool ascending = true);
	
	template <typename Key, typename Data>
	static UnsortedSequenceTable<Key, Data>& reverse(UnsortedSequenceTable<Key, Data>& toReverse);

private:

	template <typename Key, typename Data, typename Value>
	static inline Value evaluateIndex(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t index);

	template <typename Key, typename Data, typename Value>
	static inline bool smaller(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t left, const size_t right);

	template <typename Key, typename Data, typename Value>
	static inline bool greater(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t left, const size_t right);

	template <typename Key, typename Data, typename Value>
	static inline bool smallerEqual(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t left, const size_t right);

	template <typename Key, typename Data, typename Value>
	static inline bool greaterEqual(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t left, const size_t right);

	template <typename Key, typename Data, typename Value>
	static inline bool equal(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t left, const size_t right);

	template <typename Key, typename Data, typename Value>
	static inline bool notEqual(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t left, const size_t right);

};

template<typename Key, typename Data, typename Value>
inline UnsortedSequenceTable<Key, Data>& Sequencer::bubbleSort(UnsortedSequenceTable<Key, Data>& toSort, Criterion<Data, Value>& criterion, bool ascending)
{
	// Compare function
	const auto cmp = ascending ? greater<Key, Data, Value> : smaller<Key, Data, Value>;

	bool swap = true;
	size_t size = toSort.size();

	do
	{
		swap = false;

		for (size_t i = 0; i < size - 1; i++)
		{
			if (cmp(toSort, criterion, i, i + 1))
			{
				toSort.swap(i, i + 1);
				swap = true;
			}
		}

	} while (swap);

	return toSort;
}

template<typename Key, typename Data, typename Value>
inline UnsortedSequenceTable<Key, Data>& Sequencer::selectSort(UnsortedSequenceTable<Key, Data>& toSort, Criterion<Data, Value>& criterion, bool ascending)
{
	// Compare function
	const auto cmp = ascending ? smaller<Key, Data, Value> : greater<Key, Data, Value>;

	size_t min;
	size_t size = toSort.size();

	for (size_t i = 0; i < size - 2; i++)
	{
		min = i;

		for (size_t j = i + 1; j < size - 1; j++)
		{
			if (cmp(toSort, criterion, j, min))
			{
				min = j;
			}
		}

		toSort.swap(i, min);
	}

	return toSort;
}

template<typename Key, typename Data, typename Value>
inline UnsortedSequenceTable<Key, Data>& Sequencer::quickSort(UnsortedSequenceTable<Key, Data>& toSort, Criterion<Data, Value>& criterion, bool ascending)
{
	bool repeat;
	Value pivot;
	size_t
		min = 0,
		max = toSort.size() - 1,
		left, right;

	do {
		repeat = false;
		left = min,
		right = max;

		pivot = criterion.evaluate(toSort.getItemAtIndex((min + max) / 2).accessData());

		// TODO Index out of range, may be table fault {list::operator[] error}
		do
		{
			while (criterion.evaluate(toSort.getItemAtIndex(left).accessData()) < pivot)
			{
				left++;
			}

			while (criterion.evaluate(toSort.getItemAtIndex(right).accessData()) > pivot)
			{
				right--;
			}

			if (left <= right)
			{
				toSort.swap(left, right);
				left++;
				right--;
			}
		} while (left <= right);

		if (left < max)
		{
			repeat = true;
			min = left;
			//quickSort(toSort, left, max);
		}

		if (right > min)
		{
			repeat = true;
			max = right;
			//quickSort(toSort, min, right);
		}

	} while (repeat);

	return toSort;
}

template<typename Key, typename Data, typename Value>
inline UnsortedSequenceTable<Key, Data>& Sequencer::heapSort(UnsortedSequenceTable<Key, Data>& toSort, Criterion<Data, Value>& criterion, bool ascending)
{
	// Compare function
	const auto cmp = ascending ? greater<Key, Data, Value> : smaller<Key, Data, Value>;

	bool swap = false;
	const size_t size = toSort.size();
	size_t current, father, left, right, max;

	if (size == 0)
	{
		return toSort;
	}

	for (current = 1; current < size; current++)
	{
		do
		{
			swap = false;
			father = (current - 1) / 2;

			if ((current > 0) && cmp(toSort, criterion, current, father))
			{
				toSort.swap(current, father);
				current = father;
				swap = true;
			}
		} while (swap);
	}

	for (size_t i = size - 1; i > 0; i--)
	{
		toSort.swap(0, i);
		current = 0;
		do
		{
			swap = false;
			left = current * 2 + 1;
			right = current * 2 + 2;

			// One of the best operators there is
			max =
				(left < i) && (right < i)
				?
				cmp(toSort, criterion, left, right) ? left : right
				:
				left < i ? left : right;

			if ((max < i) && cmp(toSort, criterion, max, current))
			{
				toSort.swap(max, current);
				current = max;
				swap = true;
			}
		} while (swap);
	}

	return toSort;
}

template<typename Key, typename Data>
inline UnsortedSequenceTable<Key, Data>& Sequencer::reverse(UnsortedSequenceTable<Key, Data>& toReverse)
{
	const size_t size = toReverse.size();
	const size_t halfsize = size / 2;

	for (size_t i = 0; i < halfsize; i++)
	{
		toReverse.swap(i, size - i - 1);
	}

	return toReverse;
}

template<typename Key, typename Data, typename Value>
inline Value Sequencer::evaluateIndex(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t index)
{
	return criterion.evaluate(table.getItemAtIndex(index).accessData());
}

template<typename Key, typename Data, typename Value>
inline bool Sequencer::smaller(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t left, const size_t right)
{
	return evaluateIndex(table, criterion, left) < evaluateIndex(table, criterion, right);
}

template<typename Key, typename Data, typename Value>
inline bool Sequencer::greater(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t left, const size_t right)
{
	return evaluateIndex(table, criterion, left) > evaluateIndex(table, criterion, right);
}

template<typename Key, typename Data, typename Value>
inline bool Sequencer::smallerEqual(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t left, const size_t right)
{
	return evaluateIndex(table, criterion, left) <= evaluateIndex(table, criterion, right);
}

template<typename Key, typename Data, typename Value>
inline bool Sequencer::greaterEqual(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t left, const size_t right)
{
	return evaluateIndex(table, criterion, left) >= evaluateIndex(table, criterion, right);
}

template<typename Key, typename Data, typename Value>
inline bool Sequencer::equal(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t left, const size_t right)
{
	return evaluateIndex(table, criterion, left) == evaluateIndex(table, criterion, right);
}

template<typename Key, typename Data, typename Value>
inline bool Sequencer::notEqual(UnsortedSequenceTable<Key, Data>& table, Criterion<Data, Value>& criterion, const size_t left, const size_t right)
{
	return evaluateIndex(table, criterion, left) != evaluateIndex(table, criterion, right);
}

