#pragma once

#include "../structure.h"
#include "../structure_iterator.h"
#include <queue>

namespace structures
{
	/// <summary> Vrchol stromu. </summary>
	/// <typeparam name = "T"> Typ dat ukladanych vo vrchole stromu. </typeparam>
	template <typename T>
	class TreeNode : public DataItem<T>
	{
	public:
		/// <summary> Vytvori plytku kopiu vrchola (teda skopiruje iba data a smerniky na synov). </summary>
		/// <returns> Vystvorena plytka kopia vrcholu. </returns>
		virtual TreeNode<T>* shallowCopy() = 0;

		/// <summary> Vytvori a vrati hlboku kopiu vrchola (teda skopiruje data a skopiruje celu stromovu strukturu). </summary>
		/// <returns> Hlboka kopia vrchola. </returns>
		virtual TreeNode<T>* deepCopy();

		/// <summary> Test, ci je koren. </summary>
		/// <returns> true, ak je koren, false inak. </returns>
		bool isRoot();
    
		/// <summary> Test, ci je list. </summary>
		/// <returns> true, ak je list, false inak. </returns>
		virtual bool isLeaf() = 0;
  
		/// <summary> Spristupni rodica vrcholu. </summary>
		/// <returns> Rodic vrcholu. </returns>
		TreeNode<T>* getParent() const;

		/// <summary> Nastavi rodica na nullptr. </summary>
		void resetParent();
    
		/// <summary> Zmeni rodica vrcholu. </summary>
		/// <param name = "parent"> Novy rodic vrcholu. </param>
		void setParent(TreeNode<T>* parent);

		/// <summary> Spristupni brata vrcholu podla jeho pozicie u spolocneho rodica. </summary>
		/// <param name = "brothersOrder"> Poradie brata u rodica. </param>
		/// <exception cref=""> Vyhodena, ak je korenom. </exception>  
		/// <returns> Brat vrcholu. </returns>
		virtual TreeNode<T>* getBrother(int brothersOrder);
    
		/// <summary> Spristupni syna vrcholu podla jeho pozicie v tomto vrchole. </summary>
		/// <param name = "order"> Poradie syna. </param>
		/// <exception cref="std::out_of_range"> Vyhodena, ak neexistuje syn s takym poradim. </exception>
		/// <returns> Syn vrcholu. </returns>
		virtual TreeNode<T>* getSon(int order) const = 0;
    
		/// <summary> Vlozi noveho syna vrcholu na dane miesto. </summary>
		/// <param name = "order"> Poradie syna. </param>
		/// <exception cref="std::out_of_range"> Vyhodena, ak neexistuje syn s takym poradim. </exception>
		/// <remarks> 
		/// Spravanie sa lisi podla toho, ci sa jedna o vrchol s pevnym alebo variabilnym poctom synov. 
		/// Vsetky smerniky zucastnenych vrcholov budu spravne nastavene. 
		/// </remarks>
		virtual void insertSon(TreeNode<T>* son, int order) = 0;
    
		/// <summary> Nahradi syna vrcholu na danom mieste. </summary>
		/// <param name = "order"> Poradie syna. </param>
		/// <exception cref="std::out_of_range"> Vyhodena, ak je poradie neplatne. </exception>
		/// <returns> Nahradeny syn vrcholu. </returns>
		/// <remarks> Vsetky smerniky zucastnenych vrcholov budu spravne nastavene. </remarks>
		virtual TreeNode<T>* replaceSon(TreeNode<T>* son, int order) = 0;

		/// <summary> Odstrani syna na danom mieste. </summary>
		/// <param name = "order"> Poradie syna. </param>
		/// <exception cref="std::out_of_range"> Vyhodena, ak neexistuje syn s takym poradim. </exception>
		/// <returns> Odstraneny syn vrcholu. </returns>
		/// <remarks> 
		/// Spravanie sa lisi podla toho, ci sa jedna o vrchol s pevnym alebo variabilnym poctom synov. 
		/// Vsetky smerniky zucastnenych vrcholov budu spravne nastavene. 
		/// </remarks>
		virtual TreeNode<T>* removeSon(int order) = 0;
    
		/// <summary> Vrati stupen vrcholu. </summary>
		/// <returns> Stupen vrcholu. </returns>
		virtual int degree() = 0;   

		/// <summary> Vrati pocet vrcholov v podstrome. </summary>
		/// <returns> Pocet vrcholov v podstrome. </returns>
		virtual size_t sizeOfSubtree();
	protected:
		/// <summary> Konstruktor. </summary>
		/// <param name = "data"> Data, ktore uchovava. </param>
		TreeNode(T data);

		/// <summary> Kopirovaci konstruktor. </summary>
		/// <param name = "other"> Vrchol stromu, z ktoreho sa prevezmu vlastnosti. </param>
		TreeNode(const TreeNode<T>& other);
    
		/// <summary> Odkaz na rodica. </summary>
		TreeNode<T>* parent_;  
	};

	/// <summary> Strom. </summary>
	/// <typeparam name = "T"> Typ dat ukladanych v strome. </typeparam>
	template <typename T> 
	class Tree : public Structure, public Iterable<T>
	{
	public:
		/// <summary> Destruktor. </summary>
		~Tree();
  
		/// <summary> Operacia klonovania. Vytvori a vrati duplikat udajovej struktury. </summary>
		/// <returns> Ukazovatel na klon struktury. </returns>
		virtual Structure* clone() const = 0;
    
		/// <summary> Zisti, ci je struktura prazdna. </summary>
		/// <returns> true, ak je struktura prazdna, false inak. </returns>
		bool isEmpty() const override; 

		/// <summary> Vrati pocet vrcholov v strome. </summary>
		/// <returns> Pocet vrcholov v strome. </returns>
		virtual size_t size() const;

		/// <summary> Operator priradenia. </summary>
		/// <param name = "other"> Struktura (strom), z ktorej ma prebrat vlastnosti. </param>
		/// <returns> Adresa, na ktorej sa tento strom nachadza po priradeni. </returns>
		/// <exception cref="std::logic_error"> Vyhodena, vstupny parameter nie je strom. </exception>  
		/// <remarks> Vyuziva typovy operator priradenia. </remarks>
		Structure& operator=(const Structure& other) override;

		/// <summary> Operator priradenia. </summary>
		/// <param name = "other"> Strom, z ktoreho ma prebrat vlastnosti. </param>
		/// <returns> Adresa, na ktorej sa tento strom nachadza po priradeni. </returns>
		virtual Tree<T>& operator=(const Tree<T>& other);
		
		/// <summary> Vymaze strom. </summary>
		virtual void clear();
    
		/// <summary> Spristupni koren stromu. </summary>
		/// <returns> Koren stromu. </returns>
		TreeNode<T>* getRoot();
    
		/// <summary> Nahradi koren stromu. </summary>
		/// <param name = "newRoot">  Novy koren stromu. </param>
		/// <returns> Povodny koren stromu. </returns>
		virtual TreeNode<T>* replaceRoot(TreeNode<T>* newRoot);

		/// <summary> Vytvori a vrati instanciu vrcholu stromu. </summary>
		/// <returns> Vytvorena instancia vrcholu stromu. </returns>
		/// <remarks> 
		/// Kazdy strom spravuje vrcholy ineho typu, mali by vsak byt jednotne v celom strome. 
		/// Tato tovarenska metoda by mala byt pretazena v kazdom potomkovi stromu tak, aby vracala vrcholy, ktore ten strom ocakava.
		/// Instance vrcholov by mali byt vytvarane vyhradne pomocou tejto metody.
		/// </remarks>
		virtual TreeNode<T>* createTreeNodeInstance() = 0;

		/// <summary> Vrati skutocny iterator na zaciatok struktury </summary>
		/// <returns> Iterator na zaciatok struktury. </returns>
		/// <remarks> Zabezpecuje polymorfizmus. </remarks>
		Iterator<T>* getBeginIterator() const override;

		/// <summary> Vrati skutocny iterator na koniec struktury </summary>
		/// <returns> Iterator na koniec struktury. </returns>
		/// <remarks> Zabezpecuje polymorfizmus. </remarks>
		Iterator<T>* getEndIterator() const override;
	protected:
		/// <summary> Konstruktor. </summary>
		Tree();

		/// <summary> Kopirovaci konstruktor. </summary>
		/// <param name = "other"> Stromu, z ktoreho sa prevezmu vlastnosti. </param>
		Tree(const Tree<T>& other);
    
		/// <summary> Koren stromu. </summary>
		TreeNode<T>* root_;

	protected:
		/// <summary> Iterator pre Tree. </summary>
		class TreeIterator : public Iterator<T>
		{
		public:
			/// <summary> Konstruktor. </summary>
			TreeIterator();

			/// <summary> Destruktor. </summary>
			~TreeIterator();

			/// <summary> Operator priradenia. Priradi do seba hodnotu druheho iteratora. </summary>
			/// <param name = "other"> Druhy iterator. </param>
			/// <returns> Vrati seba po priradeni. </returns>
			Iterator<T>& operator= (const Iterator<T>& other) override;

			/// <summary> Porovna sa s druhym iteratorom na nerovnost. </summary>
			/// <param name = "other"> Druhy iterator. </param>
			/// <returns> True, ak sa iteratory nerovnaju, false inak. </returns>
			bool operator!=(const Iterator<T>& other) override;

			/// <summary> Vrati data, na ktore aktualne ukazuje iterator. </summary>
			/// <returns> Data, na ktore aktualne ukazuje iterator. </returns>
			const T operator*() override;

			/// <summary> Posunie iterator na dalsi prvok v strukture. </summary>
			/// <returns> Iterator na dalsi prvok v strukture. </returns>
			/// <remarks> Zvycajne vrati seba. Ak vrati iny iterator, povodny bude automaticky zruseny. </remarks>
			Iterator<T>& operator++() override;
		protected:
			/// <summary> Prehliadka stromu. </summary>
			std::queue<TreeNode<T>*>* path_;
		};

	public:
		class PreOrderTreeIterator : public TreeIterator
		{
		public:
			/// <summary> Konstruktor. </summary>
			/// <param name = "startNode"> Vrchol podstromu, cez ktory ma iterovat. </param>
			PreOrderTreeIterator(TreeNode<T>* const startNode);
		private:
			/// <summary> Zostavi do path_ prehliadku stromu v poradi preorder. </summary>
			/// <param name = "current"> Aktualny spracovavany vrchol stromu. </param>
			void populatePath(TreeNode<T>* const current);
		};

		class PostOrderTreeIterator: public TreeIterator
		{
		public:
			/// <summary> Konstruktor. </summary>
			/// <param name = "startNode"> Vrchol podstromu, cez ktory ma iterovat. </param>
			PostOrderTreeIterator(TreeNode<T>* const startNode);
		private:
			/// <summary> Zostavi do path_ prehliadku stromu v poradi postorder. </summary>
			/// <param name = "path"> Cesta stromom, ktoru zostavuje. </param>
			/// <param name = "current"> Aktualny spracovavany vrchol stromu. </param>
			void populatePath(TreeNode<T>* const current);
		};

		class LevelOrderTreeIterator : public TreeIterator
		{
		public:
			/// <summary> Konstruktor. </summary>
			/// <param name = "startNode"> Vrchol podstromu, cez ktory ma iterovat. </param>
			LevelOrderTreeIterator(TreeNode<T>* const startNode);
		private:
			/// <summary> Zostavi do path_ prehliadku stromu v poradi levelorder. </summary>
			/// <param name = "current"> Aktualny spracovavany vrchol stromu. </param>
			void populatePath(TreeNode<T>* const current);
		};
	};

	template<typename T>
	inline TreeNode<T>* TreeNode<T>::deepCopy()
	{
		// Sprav akciu s vrcholom, nad kazdym synom zavolaj prehliadku
		TreeNode<T>* result = shallowCopy();
		TreeNode<T>* son;

		// Prejdi vsetkych synov
		for (int i = 0; i < degree(); i++)
		{
			son = getSon(i);
			if (son)
			{
				// Syn existuje, nahrad ho jeho kopiou
				result->replaceSon(son->deepCopy(), i);
			}
		}

		return result;
	}

	template<typename T>
	inline bool TreeNode<T>::isRoot()
	{
		return parent_ == nullptr;
	}

	template<typename T>
	inline TreeNode<T>* TreeNode<T>::getParent() const
	{
		return parent_;
	}

	template<typename T>
	inline void TreeNode<T>::resetParent()
	{
		parent_ = nullptr;
	}

	template<typename T>
	inline void TreeNode<T>::setParent(TreeNode<T>* parent)
	{
		parent_ = parent;
	}

	template<typename T>
	inline TreeNode<T>* TreeNode<T>::getBrother(int brothersOrder)
	{
		if (isRoot())
		{
			throw std::logic_error("TreeNode<T>::getBrother : Node is root!");
		}

		return parent_->getSon(brothersOrder);
	}

	template<typename T>
	inline size_t TreeNode<T>::sizeOfSubtree()
	{
		// Ratam sameho seba
		size_t result = 1;
		TreeNode<T>* son;

		// Prejdi vsetkych synov
		for (int i = 0; i < degree(); i++)
		{
			son = getSon(i);
			if (son)
			{
				result += son->sizeOfSubtree();
			}
		}

		return result;
	}

	template<typename T>
	inline TreeNode<T>::TreeNode(T data):
		DataItem<T>(data),
		parent_(nullptr)
	{
	}

	template<typename T>
	inline TreeNode<T>::TreeNode(const TreeNode<T>& other):
		DataItem<T>(other),
		parent_(other.parent_)
	{
	}
  
	template<typename T>
	inline Tree<T>::~Tree()
	{
		clear();
	}

	template<typename T>
	inline bool Tree<T>::isEmpty() const
	{
		// Strom je prazdny ked nema koren - ziadny node
		return root_ == nullptr;
	}

	template<typename T>
	inline size_t Tree<T>::size() const
	{
		// Ak mam koren vrat velkost, inak 0
		return root_ ? root_->sizeOfSubtree() : 0;
	}

	template<typename T>
	inline Iterator<T>* Tree<T>::getBeginIterator() const
	{
		return new PreOrderTreeIterator(root_);
	}

	template<typename T>
	inline Iterator<T>* Tree<T>::getEndIterator() const
	{
		return new PreOrderTreeIterator(nullptr);
	}

	template<typename T>
	inline Tree<T>::Tree():
		Structure(),
		Iterable<T>(),
		root_(nullptr)    
	{
	}

	template<typename T>
	inline Tree<T>::Tree(const Tree<T>& other):
		Tree<T>()
	{
		*this = other;
	}

	template<typename T>
	inline Structure & Tree<T>::operator=(const Structure & other)
	{
		if (this != &other)
		{
			*this = dynamic_cast<const Tree<T>&>(other);
		}
		return *this;
	}
  
	template<typename T>
	inline Tree<T>& Tree<T>::operator=(const Tree<T>& other)
	{
		if (this != &other)
		{
			// Vymaz sa
			clear();
			if (other.root_)
			{	
				// Ak ma koren, skopiruj
				root_ = other.root_->deepCopy();
			}
		}
		return *this;
	}

	template<typename T>
	inline void Tree<T>::clear()
	{
		// Node sa postara o odstranenie synov
		// delete nullptr je platna operacia - nevyhodi exception
		delete root_;
		root_ = nullptr;
	}
  
	template<typename T>
	inline TreeNode<T>* Tree<T>::getRoot()
	{
		return root_;
	}
  
	template<typename T>             
	inline TreeNode<T>* Tree<T>::replaceRoot(TreeNode<T>* newRoot)
	{
		TreeNode<T>* oldRoot = root_;
		root_ = newRoot;
		return oldRoot;
	}

	template<typename T>
	inline Tree<T>::TreeIterator::TreeIterator():
		Iterator<T>(),
		path_(new std::queue<TreeNode<T>*>())
	{
	}

	template<typename T>
	inline Tree<T>::TreeIterator::~TreeIterator()
	{
		delete path_;
		path_ = nullptr;
	}

	template<typename T>
	inline Iterator<T>& Tree<T>::TreeIterator::operator=(const Iterator<T>& other)
	{
		*path_ = *dynamic_cast<const TreeIterator&>(other).path_;
		return *this;
	}

	template<typename T>
	inline bool Tree<T>::TreeIterator::operator!=(const Iterator<T>& other)
	{
		return *path_ != *dynamic_cast<const TreeIterator&>(other).path_;
	}

	template<typename T>
	inline const T Tree<T>::TreeIterator::operator*()
	{
		// Toto je peek -> front() = peek()
		return path_->front()->accessData();
	}

	template<typename T>
	inline Iterator<T>& Tree<T>::TreeIterator::operator++()
	{
		path_->pop();
		return *this;
	}

	template<typename T>
	inline Tree<T>::PreOrderTreeIterator::PreOrderTreeIterator(TreeNode<T>* const startNode):
		TreeIterator()
	{
		populatePath(startNode);
	}

	template<typename T>
	inline void Tree<T>::PreOrderTreeIterator::populatePath(TreeNode<T>* const current)
	{
		// Iba ak nie je nullptr
		if (current)
		{
			// Pridaj current do frontu
			path_->push(current);

			// Pridaj vsetkych synov
			for (int i = 0; i < current->degree(); i++)
			{
				// To iste sprav pre vsetkych synov current
				populatePath(current->getSon(i));
			}
		}
	}

	template<typename T>
	inline Tree<T>::PostOrderTreeIterator::PostOrderTreeIterator(TreeNode<T>* const startNode) :
		TreeIterator()
	{
		// TODO?????
		// Iba ak nie je nullptr
		if (startNode)
		{
			TreeNode<T>* current = startNode;

			// Pridaj vsetkych synov
			for (int i = 0; i < current->degree(); i++)
			{
				// To iste sprav pre vsetkych synov current
				populatePath(current->getSon(i));
			}

			// Pridaj current do frontu
			path_->push(current);
		}
	}

	template<typename T>
	inline void Tree<T>::PostOrderTreeIterator::populatePath(TreeNode<T>* const current)
	{
		//TODO 08: Tree<T>::PostOrderTreeIterator
		throw std::exception("Tree<T>::PostOrderTreeIterator::populatePath: Not implemented yet.");
	}

	template<typename T>
	inline Tree<T>::LevelOrderTreeIterator::LevelOrderTreeIterator(TreeNode<T>* const startNode) :
		TreeIterator()
	{
		populatePath(startNode);
	}

	template<typename T>
	inline void Tree<T>::LevelOrderTreeIterator::populatePath(TreeNode<T>* const current)
	{
		//TODO 08: Tree<T>::LevelOrderTreeIterator
		throw std::exception("Tree<T>::LevelOrderTreeIterator::populatePath: Not implemented yet.");
	}

}

