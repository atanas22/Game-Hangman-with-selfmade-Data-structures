#ifndef AVLTREE_H
#define AVLTREE_H
#endif
#include "SearchTree.h"
#include <ctime>

template <typename E>
class AVLTree;

// AVL Entry
template <typename E> // v AVLEntry
class AVLEntry : public E {
	friend class AVLTree<E>;

protected:
	typename typedef E::Key K;
	typename typedef E::Value V;

public:
	AVLEntry(const K& k = K(), const V& v = V()) : E(k, v), hg(0) {}

 protected: 
	void setHeight(int h) { hg = h; }
	int getHeight() const { return hg; }

private:
	int hg;
};


// AVL Tree - Declaration
template <typename E>
class AVLTree : public SearchTree<AVLEntry<E>> {
public:
	typedef AVLEntry<E> AVLEntry;
	typename typedef SearchTree<AVLEntry>::Iterator Iterator;

protected:
	typedef SearchTree<AVLEntry> ST;
	typedef list<Iterator> ListIter;
	typename typedef AVLEntry::Key K;
	typename typedef AVLEntry::Value V;
	typename typedef ST::TPos TrPos;

public:
	AVLTree() {}
	Iterator insert(const K& k, const V& v);
	ListIter findAll(const K& key);
	void erase(const K& k);
	void erase(const Iterator& p) ;
	void eraseAll(const K& k);
	Iterator getRand();

protected:
	void setHeight(const TrPos& p);
	bool isBalanced(const TrPos &p);
	TrPos tallerGrandChild(const TrPos &p);
	void rebalance(const TrPos& P);
	TrPos restructure(const TrPos& p);
	void getAll(const K& k, TrPos nd, ListIter& pl);
	Iterator Rand(const int& h, TrPos p);
};


// AVL Tree - Definition
template <typename E>
typename AVLTree<E>::Iterator AVLTree<E>::insert(const K& k, const V& v) {
	TrPos p = ST::inserter(k, v);
	setHeight(p);
	rebalance(p);
	return Iterator(p);
}

template <typename E>
typename AVLTree<E>::ListIter AVLTree<E>::findAll(const K& key) {
	list<Iterator> all;
	getAll(key, ST::root(), all);
	return all;
}

template <typename E>
void AVLTree<E>::erase(const K& k) {
	TrPos f = ST::finder(k, ST::root());
	if (Iterator(f) == ST::end()) throw exception("erase(): nonexsistent element");
	TrPos s = ST::eraser(f);
	rebalance(s);
}

template <typename E>
void AVLTree<E>::erase(const Iterator& p) {
	TrPos s = ST::eraser(ST::retV(p));
	rebalance(s);
}

template <typename E>
void AVLTree<E>::eraseAll(const K& k) {
	list<Iterator> l;
	getAll(k, ST::root(), l);
	int sz = l.size();
	for (int i = 0; i < sz; ++i)
		erase(k);
}

template <typename E>
void AVLTree<E>::setHeight(const TrPos& p) {
	int hg = std::max(p.left()->getHeight(), p.right()->getHeight());
	p->setHeight(hg + 1);
}

template <typename E>
bool AVLTree<E>::isBalanced(const TrPos &p) {
	int lh = p.left()->getHeight();
	int rh = p.right()->getHeight();
	if ((lh - rh > 1) || (lh - rh < -1)) return false;
	return true;
}

template <typename E>
typename AVLTree<E>::TrPos AVLTree<E>::tallerGrandChild(const TrPos &p) {
	TrPos p1 = (p.left()->getHeight()) > (p.right()->getHeight()) ? p.left() : p.right();
	TrPos p2 = (p1.left()->getHeight()) > (p1.right()->getHeight()) ? p1.left() : p1.right();
	return p2;
}

template <typename E>
void AVLTree<E>::rebalance(const TrPos& P) {
	TrPos p = P;
	while (p != ST::root()) {
		p = p.parent();
		setHeight(p);
		if (!isBalanced(p)) {
			TrPos x = tallerGrandChild(p);
			TrPos b = restructure(x);
			setHeight(b.left());
			setHeight(b.right());
			setHeight(b);
		}
	}
}

template <typename E>
typename AVLTree<E>::TrPos AVLTree<E>::restructure(const TrPos& p) {
	TrPos x = p, y = x.parent(), z = y.parent();
	TrPos a, b, c, T0, T1, T2, T3, ParUnbNode;
	bool leftLeft = (z.left() == y && y.left() == x) ? true : false;       bool leftRight = (z.left() == y && y.right() == x) ? true : false;
	bool rightRight = (z.right() == y && y.right() == x) ? true : false;   bool rightLeft = (z.right() == y && y.left() == x) ? true : false;

	if (leftLeft) {
		a = x; b = y; c = z;
		T0 = a.left(); T1 = a.right();
		T2 = b.right(); T3 = c.right();
	}
	else if (leftRight) {
		a = y; b = x; c = z;
		T0 = a.left(); T1 = b.left();
		T2 = b.right(); T3 = c.right();
	}
	else if (rightRight) {
		a = z; b = y; c = x;
		T0 = a.left(); T1 = b.left();
		T2 = c.left(); T3 = c.right();
	}
	else if (rightLeft) {
		a = z; b = x; c = y;
		T0 = a.left(); T1 = b.left();
		T2 = b.right(); T3 = c.right();
	}
	if (z == ST::root()) { (z.parent()).setLeft(b); }
	else {
		if (z.parent().left() == z) z.parent().setLeft(b);
		else z.parent().setRight(b);

	}
	b.setParent(z.parent());   b.setRight(c);   b.setLeft(a);
	a.setParent(b); c.setParent(b);

	a.setLeft(T0);   a.setRight(T1);
	T0.setParent(a); T1.setParent(a);

	c.setLeft(T2);   c.setRight(T3);
	T2.setParent(c); T3.setParent(c);

	return b;
}

template <typename E>
void AVLTree<E>::getAll(const K& k, TrPos nd, ListIter& pl) {
	if (nd.left().isInternal()) getAll(k, nd.left(), pl);
	if (k == nd->key()) pl.push_back(Iterator(nd));
	if (nd.right().isInternal()) getAll(k, nd.right(), pl);
}


// getRand() - избира рандом височина (int h), която да се търси в дървото и я изпраща в Rand(h, корен) за да търси елемент с такава височина
template <typename E>
typename AVLTree<E>::Iterator AVLTree<E>::getRand() {
	Iterator rt = ST::root(); // ST = SearchTree - там се намира ф-ята root();
	int h = (rand()% rt->getHeight());
	// *** cout << "Searching element at height h = " << h << "\n";
	return Rand(h, this->retV(rt));
}

/* Rand() - търси тази височина, като отива в ляво или дясно поддърво от корена, в който се намираме, благодарение на bool dir, който
			произволно избира число 1(отива в дясно) или 0(ляво поддърво)*/
/* Може да проверите, че избира рандом посока наляво или надясно, към рандом височината h,
   за да стигне до търесената височина 'h', като освободите cout-овете в getRand() и Rand()*/
template <typename E>
typename AVLTree<E>::Iterator AVLTree<E>::Rand(const int &h, TrPos p) { // complexity - O(log(n))
	if (h == p->getHeight()) {
		// ***  cout << "found element '"<< *p << "' at height: " << p->getHeight() << "\n\n";
		return Iterator(p);
	}
	bool dir = rand() % 2; // direction: 0 - left, 1-right
	 // *** cout << "direction = " << dir << ((dir)? " - right\n":" - left\n");
	
	 /*обаче, ако е избрала дясна посока и ако там няма такава височина, тогава да сменя с
	 сменя с лява посока (dir = 0 или false) (В този случай точно лявото дете ще е с вис. h, защото разликата между височините е точно 1 ниво)*/
	if (dir && (h > p.right()->getHeight())) dir = false; 
	/* и обратно - ако в лявото дете височината е по-малка, ще отиде в дясното дете(и точно там ще се намира елемент с височина h)*/
	else if (h > p.left()->getHeight()) dir = true;

	if (dir) return Rand(h, p.right()); // отиди в дясно поддърво
	else return Rand(h, p.left()); // отиди в ляво поддърво
}
/*  При този метод шансът да се повторят елементите с по-големи височини е голям, защото елементите на по-малка височина са много повече, 
    отколкото тези на по-голямата. Проади тази причина не всички елементити са с еднаква вероятност на избор.
	За да се реши този проблем, може да се съхранят всички елементи във vector от итератори, преди да започне играта, а когато се иска
	рандом елемент - да го избира с функцията rand() - а полученето число да е индексът на рандом елемента (log(1)).
	Само че, ако думите са стотици хиляди, играта ще зарежда много бавно. Този метод също е добър, защото log(n) време за рандом елемент
	също е супер бързо. С недостатък, че не всички елементи са с еднаква вероятност на избор.
	*/
	
// end of AVL Tree Definition
