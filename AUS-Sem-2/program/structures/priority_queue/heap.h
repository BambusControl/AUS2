#pragma once

#include "priority_queue_list.h"
#include "../list/array_list.h"
#include <exception>

namespace structures
{
	/// <summary> Prioritny front implementovany haldou. Implementujucim typom haldy je struktura ArrayList. </summary>
	/// <typeparam name = "T"> Typ dat ukladanych v prioritnom fronte. </typeparam>
	template<typename T>
	class Heap : public PriorityQueueList<T>
	{
	public:
		/// <summary> Konstruktor. </summary>
		Heap();

		/// <summary> Kopirovaci konstruktor. </summary>
		/// <param name = "other"> Halda, z ktorej sa prevezmu vlastnosti. </param>
		Heap(const Heap<T>& other);

		/// <summary> Operacia klonovania. Vytvori a vrati duplikat haldy. </summary>
		/// <returns> Ukazovatel na klon struktury. </returns>
		Structure* clone() const override;

		/// <summary> Operator priradenia. </summary>
		/// <param name = "other"> Prioritny front implementovany zoznamom, z ktoreho ma prebrat vlastnosti. </param>
		/// <returns> Adresa, na ktorej sa tento prioritny front nachadza po priradeni. </returns>
		/// <exception cref="std::logic_error"> Vyhodena, ak vstupny parameter nie je halda. </exception>  
		/// <remarks> Vyuziva typovy operator priradenia. </remarks>
		virtual PriorityQueueList<T>& operator=(const PriorityQueueList<T>& other) override;

		/// <summary> Operator priradenia pre haldu. </summary>
		/// <param name = "other"> Halda, z ktorej ma prebrat vlastnosti. </param>
		/// <returns> Adresa, na ktorej sa tato halda nachadza po priradeni. </returns>
		virtual Heap<T>& operator=(const Heap<T>& other);

		/// <summary> Vlozi prvok s danou prioritou do haldy. </summary>
		/// <param name = "priority"> Priorita vkladaneho prvku. </param>
		/// <param name = "data"> Vkladany prvok. </param>
		void push(const int priority, const T& data) override;

		/// <summary> Odstrani prvok s najvacsou prioritou z haldy. </summary>
		/// <returns> Odstraneny prvok. </returns>
		/// <exception cref="std::logic_error"> Vyhodena, ak je halda prazdna. </exception>  
		T pop() override;

	protected:
		/// <summary> Vrati index v ArrayList-e, na ktorom sa nachadza prvok s najvacsou prioritou. </summary>
		/// <returns> Index prvku s najvacsou prioritou. </returns>
		/// <exception cref="std::logic_error"> Vyhodena, ak je ArrayList prazdny. </exception>  
		int indexOfPeek() const override;

	private:
		/// <summary> Vrati index v ArrayList-e, na ktorom by sa mal nachadzat otec daneho prvku. </summary>
		/// <param name = "index"> Index prvku, ktoreho otca chceme vratit. </param>
		/// <returns> Index, na ktorom ba sa mal nachadzat otec prvku. </returns>
		int getParentIndex(const int index);

		/// <summary> Vrati index v ArrayList-e, na ktorom by mal mat dany prvok syna s najvacsou prioritou. </summary>
		/// <param name = "index"> Index prvku, ktoreho syna chceme vratit. </param>
		/// <returns> Index, na ktorom by sa mal nachadzat syn prvku. </returns>
		int getGreaterSonIndex(const int index);
	};

	template<typename T>
	Heap<T>::Heap() :
		PriorityQueueList<T>(new ArrayList<PriorityQueueItem<T>*>())
	{
	}

	template<typename T>
	Heap<T>::Heap(const Heap<T>& other) :
		Heap<T>()
	{
		*this = other;
	}

	template<typename T>
	Structure* Heap<T>::clone() const
	{
		return new Heap<T>(*this);
	}

	template<typename T>
	inline PriorityQueueList<T>& Heap<T>::operator=(const PriorityQueueList<T>& other)
	{
		return *this = dynamic_cast<const Heap<T>&>(other);
	}

	template<typename T>
	inline Heap<T>& Heap<T>::operator=(const Heap<T>& other)
	{
		if (this != &other)
		{
			PriorityQueueList<T>::operator=(other);
		}
		return *this;
	}

	template<typename T>
	void Heap<T>::push(const int priority, const T& data)
	{
		list_->add(new PriorityQueueItem<T>(priority, data));

		int indexCurrent = list_->size() - 1;
		int indexParent = getParentIndex(indexCurrent);

		while (
			indexParent != -1 &&
			list_->operator[](indexCurrent)->getPriority()
			<
			list_->operator[](indexParent)->getPriority()
			)
		{
			DSRoutines::swap(
				list_->operator[](indexCurrent),
				list_->operator[](indexParent)
			);
			indexCurrent = indexParent;
			indexParent = getParentIndex(indexCurrent);
		}
	}

	template<typename T>
	T Heap<T>::pop()
	{
		// Grab highest priority item
		PriorityQueueItem<T>* item = list_->operator[](0);

		// Put last item in place of removed item
		list_->operator[](0) = list_->removeAt(list_->size() - 1);;

		if (list_->size() > 1)
		{
			int indexCurrent = 0;
			int indexSon = getGreaterSonIndex(indexCurrent);

			// Swap with son until sorted (current has lower priority than son)
			while (
				indexSon != -1 &&
				list_->operator[](indexCurrent)->getPriority()
				>
				list_->operator[](indexSon)->getPriority()
				)
			{
				DSRoutines::swap(
					list_->operator[](indexCurrent),
					list_->operator[](indexSon)
				);
				indexCurrent = indexSon;
				indexSon = getGreaterSonIndex(indexCurrent);
			}
		}

		// Handle data
		T result = item->accessData();
		delete item;
		return result;
	}

	template<typename T>
	inline int Heap<T>::getParentIndex(const int index)
	{
		return (index + 1) / 2 - 1;
	}

	template<typename T>
	inline int Heap<T>::getGreaterSonIndex(const int index)
	{
		// Calculate indexes (2i + 1) - (2i + 2)
		int indexLeft = 2 * index + 1;
		int indexRight = indexLeft + 1;

		// Retrieve children items
		PriorityQueueItem<T> *sonLeft, *sonRight;
		sonLeft = indexLeft < list_->size() ?
			list_->operator[](indexLeft) : nullptr;
		sonRight = indexRight < list_->size() ?
			list_->operator[](indexRight) : nullptr;

		if (sonLeft == nullptr && sonRight == nullptr)
		{
			// Item at index has no children
			return -1;
		}

		if (sonLeft != nullptr && sonRight != nullptr)
		{
			// Return item with the highest priority
			return
				sonLeft->getPriority() <= sonRight->getPriority() ?
				indexLeft : indexRight;
		}

		// Only one child exists, it is always the left one
		return indexLeft;
	}

	template<typename T>
	inline int Heap<T>::indexOfPeek() const
	{
		if (list_->isEmpty())
		{
			throw std::logic_error("Heap<T>::indexOfPeek() : Queue is empty!");
		}
		return 0;
	}
}