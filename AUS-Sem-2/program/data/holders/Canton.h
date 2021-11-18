#pragma once
#include <string>
#include <sstream>
#include <stack>
#include <queue>

#include "../../common/SvkStr.h"
#include "../../structures/table/table.h"
#include "../../structures/table/sorted_sequence_table.h"
#include "../../structures/structure_iterator.h"

using std::string;
using structures::Table;
using structures::TableItem;
using structures::SortedSequenceTable;
using structures::Iterable;
using structures::Iterator;

template <typename T>
using Queue = std::queue<T>;
template <typename T>
using Stack = std::stack<T>;

typedef unsigned long long ullong;

class Canton
{

public:

	typedef Table<SvkStr, Canton*> _Table;
	typedef TableItem<SvkStr, Canton*> TItem;
	typedef SortedSequenceTable<SvkStr, Canton*> SortedTable;


	enum class Type {
		Obec = 0, Okres = 1, Kraj = 2, Stat = 3
	};

	class CantonTypeIterator : public Iterator<Canton*>
	{

	public:
		CantonTypeIterator(Canton* const canton = nullptr, Type type = Type::Stat);

		virtual ~CantonTypeIterator();

		/// <summary> Operator priradenia. Priradi do seba hodnotu druheho iteratora. </summary>
		/// <param name = "other"> Druhy iterator. </param>
		/// <returns> Vrati seba po priradeni. </returns>
		virtual CantonTypeIterator& operator=(const Iterator& other) override;

		/// <summary> Porovna sa s druhym iteratorom na nerovnost. </summary>
		/// <param name = "other"> Druhy iterator. </param>
		/// <returns> True, ak sa iteratory nerovnaju, false inak. </returns>
		virtual bool operator!=(const Iterator& other) override;

		/// <summary> Vrati data, na ktore aktualne ukazuje iterator. </summary>
		/// <returns> Data, na ktore aktualne ukazuje iterator. </returns>
		virtual Canton* const operator*() override;

		/// <summary> Posunie iterator na dalsi prvok v strukture. </summary>
		/// <returns> Iterator na dalsi prvok v strukture. </returns>
		/// <remarks> Zvycajne vrati seba. Ak vrati iny iterator, povodny bude automaticky zruseny. </remarks>
		virtual CantonTypeIterator& operator++() override;

	protected:

		Stack<Canton*>* parents;
		Iterator<TItem*>* parentIt;
		Iterator<TItem*>* parentItEnd;

	};

	Canton(SvkStr nazov,Type typ, Canton* parent = nullptr, ullong pocetMladych = 0, ullong pocetDospelych = 0, ullong pocetDochodcov = 0, double celkovaVymera = 0, double zastavanaPlocha = 0);
	~Canton();

	static Canton* parse(string line[6], Canton::Type type = Canton::Type::Obec, Canton* parent = nullptr);
	Canton& operator+=(const Canton& other);

	bool trySetParent(Canton* newParent);
	bool addChild(Canton* child);

	Canton* levelFind(SvkStr& name);
	const SortedTable* const getChildren() const;
	Canton* getParent();

	_Table& toTable(_Table& table);

	SvkStr nazov;
	Type typ;
	ullong pocetMladych;
	ullong pocetDospelych;
	ullong pocetDochodcov;
	double celkovaVymera;
	double zastavanaPlocha;


private:

	SortedTable* children;
	Canton* parent;

};

Canton::Canton(
	SvkStr nazov,
	Type typ,
	Canton* parent,
	ullong pocetMladych,
	ullong pocetDospelych,
	ullong pocetDochodcov,
	double celkovaVymera,
	double zastavanaPlocha
) :
	nazov(nazov),
	typ(typ),
	parent(parent),
	pocetMladych(pocetMladych),
	pocetDospelych(pocetDospelych),
	pocetDochodcov(pocetDochodcov),
	celkovaVymera(celkovaVymera),
	zastavanaPlocha(zastavanaPlocha),
	children(typ == Type::Obec ? nullptr : new SortedTable())
{
	if (parent)
	{
		parent->addChild(this);
	}
}

inline Canton::~Canton()
{
	if (children)
	{
		for (size_t i = 0; i < children->size(); i++)
		{
			auto item = children->getItemAtIndex(i);
			auto data = item.accessData();
			delete data;
		}

		delete children;
		children = nullptr;
	}

	parent = nullptr;
}

inline Canton& Canton::operator+=(const Canton& other)
{
	pocetMladych += other.pocetMladych;
	pocetDospelych += other.pocetDospelych;
	pocetDochodcov += other.pocetDochodcov;
	celkovaVymera += other.celkovaVymera;
	zastavanaPlocha += other.zastavanaPlocha;

	return *this;
}

Canton* Canton::parse(string line[6], Canton::Type type, Canton* parent)
{
	return new Canton(
		line[0],
		type,
		parent,
		std::stoul(line[1]),
		std::stoul(line[2]),
		std::stoul(line[3]),
		std::stod(line[4]),
		std::stod(line[5])
	);
}

inline bool Canton::trySetParent(Canton* newParent)
{
	if (newParent == nullptr || parent)
	{
		return false;
	}
	
	parent = newParent;
	return true;
}

inline bool Canton::addChild(Canton* child)
{
	if (child)
	{
		if (!children->containsKey(child->nazov))
		{
			children->insert(child->nazov, child);
			return true;
		}
	}

	return false;
}

inline Canton* Canton::levelFind(SvkStr& name)
{
	if (nazov == name)
	{
		return this;
	}
	else if (typ == Type::Stat)
	{
		return nullptr;
	}

	Canton* item = this;
	while (item->parent)
	{
		// Climb the tree
		item = item->parent;
	}

	// Level Order Traversing

	Queue<Canton*> queue;
	queue.push(item);
	Canton* data = nullptr;

	while (!queue.empty())
	{
		item = queue.front();
		queue.pop();

		if (item->children->tryFind(name, data))
		{
			// Item found in table
			return data;
		}

		for (TItem* tit : *item->children)
		{
			// Item not found, push table item data
			if (tit->accessData()->children)
			{
				queue.push(tit->accessData());
			}
		}
	}

	return nullptr;
}

inline Table<SvkStr, Canton*>& Canton::toTable(_Table& table)
{
	table.insert(nazov, this);

	if (children)
	{
		for (auto item : *children)
		{
			item->accessData()->toTable(table);
		}
	}

	return table;
}

inline const SortedSequenceTable<SvkStr, Canton*>* const Canton::getChildren() const
{
	return children;
}

inline Canton* Canton::getParent()
{
	return parent;
}

Canton::CantonTypeIterator::CantonTypeIterator(Canton* const canton, Type type) :
	parents(new Stack<Canton*>())
{
	if (canton == nullptr || canton->typ <= type)
	{
		// Empty iterator
		parentIt = nullptr;
		parentItEnd = nullptr;
		return;
	}

	// I just need to remember parents to iterate through
	type = static_cast<Type>(static_cast<int>(type) + 1);

	Canton* item = canton;
	Stack<Canton*> path;
	path.push(item);

	while (!path.empty())
	{
		item = path.top();
		path.pop();

		if (item->typ == type)
		{
			parents->push(item);
		}
		else if (item->children)
		{
			for (auto c : *item->children)
			{
				path.push(c->accessData());
			}
		}
	}

	// Init
	item = parents->top();
	parents->pop();
	parentIt = item->children->getBeginIterator();
	parentItEnd = item->children->getEndIterator();

}

inline Canton::CantonTypeIterator::~CantonTypeIterator()
{
	delete parents;
	delete parentIt;
	delete parentItEnd;

	parents = nullptr;
	parentIt = nullptr;
	parentItEnd = nullptr;
}

inline Canton::CantonTypeIterator& Canton::CantonTypeIterator::operator=(const Iterator& other)
{

	auto o = dynamic_cast<const CantonTypeIterator&>(other);
	*parents = *o.parents;
	*parentIt = *o.parentIt;
	*parentItEnd = *o.parentItEnd;

	return *this;
}

inline bool Canton::CantonTypeIterator::operator!=(const Iterator& other)
{
	const CantonTypeIterator& o = dynamic_cast<const CantonTypeIterator&>(other);

	if (parentIt && o.parentIt)
	{
		if (!(*parentIt != *o.parentIt))
		{
			return false;
		}
	}
	else
	{
		return parentIt != o.parentIt;
	}

	if (parentItEnd && o.parentItEnd)
	{
		if (!(*parentItEnd != *o.parentItEnd))
		{
			return false;
		}
	}
	else
	{
		return parentIt != o.parentIt;
	}

	return *parents != *o.parents;
}

inline Canton* const Canton::CantonTypeIterator::operator*()
{
	return (**parentIt)->accessData();
}

inline Canton::CantonTypeIterator& Canton::CantonTypeIterator::operator++()
{
	parentIt->operator++();

	if (*parentIt != *parentItEnd)
	{
		return *this;
	}
	else
	{
		delete parentIt;
		delete parentItEnd;

		if (!parents->empty())
		{
			Canton* item = parents->top();
			parents->pop();

			parentIt = item->children->getBeginIterator();
			parentItEnd = item->children->getEndIterator();
		}
		else
		{
			parentIt = nullptr;
			parentItEnd = nullptr;
		}
	}

	return *this;
}
