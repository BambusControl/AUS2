#pragma once

#include "../list/array_list.h"
#include "sequence_table.h"

namespace structures
{

	/// <summary> Sekvencna neutriedena tabulka. </summary>
	/// <typeparam name = "K"> Kluc prvkov v tabulke. </typeparam>
	/// <typeparam name = "T"> Typ dat ukladanych v tabulke. </typeparam>
	template <typename K, typename T>
	class UnsortedSequenceTable : public SequenceTable<K, T>
	{
	public:
		/// <summary> Konstruktor. </summary>
		UnsortedSequenceTable();

		/// <summary> Kopirovaci konstruktor. </summary>
		/// <param name = "other"> NonortedSequenceTable, z ktorej sa prevezmu vlastnosti. </param>
		UnsortedSequenceTable(const UnsortedSequenceTable<K, T>& other);

		/// <summary> Operacia klonovania. Vytvori a vrati duplikat udajovej struktury. </summary>
		/// <returns> Ukazovatel na klon struktury. </returns>
		UnsortedSequenceTable* clone() const override;

		/// <summary> Vymeni prvky na indexoch. </summary>
		/// <param name = "indexFirst"> Index prveho prvku. </param>
		/// <param name = "indexSecond"> Index druheho prvku. </param>
		void swap(int indexFirst, int indexSecond);

		/// <summary> Vymeni prvky tabulky. </summary>
		/// <param name = "first"> Prvy prvok. </param>
		/// <param name = "second"> Druhy prvok. </param>
		static void swap(TableItem<K, T>& first, TableItem<K, T>& second);
	};

	template<typename K, typename T>
	inline UnsortedSequenceTable<K, T>::UnsortedSequenceTable() :
		SequenceTable<K, T>(new ArrayList<TableItem<K, T>*>())
	{
	}

	template<typename K, typename T>
	inline UnsortedSequenceTable<K, T>::UnsortedSequenceTable(const UnsortedSequenceTable<K, T>& other) :
		UnsortedSequenceTable()
	{
		*this = other;
	}

	template<typename K, typename T>
	inline UnsortedSequenceTable<K, T>* UnsortedSequenceTable<K, T>::clone() const
	{
		return new UnsortedSequenceTable<K, T>(*this);
	}
	template<typename K, typename T>
	inline void UnsortedSequenceTable<K, T>::swap(int indexFirst, int indexSecond)
	{
		swap(getItemAtIndex(indexFirst), getItemAtIndex(indexSecond));
	}

	template<typename K, typename T>
	inline void UnsortedSequenceTable<K, T>::swap(TableItem<K, T>& first, TableItem<K, T>& second)
	{
		TableItem<K, T> item = first;
		first = second;
		second = item;
	}
}