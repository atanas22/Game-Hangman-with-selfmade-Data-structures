#ifndef SEARCHTREE_H
#define SEARCHTREE_H
#endif

#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <exception>
#include <queue>
#include <ctime>
#include <map>
#include <string>
#include <set>
#include "BinaryTree.h"
using namespace std;

// Pair
template <typename K, typename V>
class Entry {
public:
	typedef K Key;
	typedef V Value;

public:
	Entry(const K& k = K(), const V& v = V()) : _key(k), _value(v) { }
	const K& key() const { return _key; }
	const V& value() const { return _value; }
	void setKey(const K& k) { _key = k; }
	void setValue(const V& v) { _value = v; }

private:
	K _key;
	V _value;
};

// v SearchTree
// ---- Search Tree ---- ☺☻♥♦♣♠•◘○»Θ
template <typename E>
class SearchTree {
public:
	typename typedef E::Key K;
	typename typedef E::Value V;
	class Iterator;

	SearchTree() : n(0) { T.addRoot(); T.expandExternal(T.root()); }
	int size() const { return n; }
	bool empty() const { return n == 0; }
	Iterator insert(const K& k, const V& x) { return Iterator(inserter(k, x)); }
	Iterator find(const K& k);
	list<Iterator> findAll(const K &key);
	void erase(const K& k) ;
	void erase(const Iterator& p) ;
	void eraseAll(const K& k);
	Iterator begin();
	Iterator end();

protected:
	typedef BinaryTree<E> BinaryTree;
	typedef typename BinaryTree::Position TPos;
	typedef list<Iterator> ListIter;
	
	TPos root() const { return T.root().left(); }
	TPos finder(const K& k, const TPos& v);
	TPos inserter(const K& k, const V& x);
	TPos eraser(const TPos& v);
	inline const TPos& retV(const Iterator & pos) const { return pos.v; }

private: 
	BinaryTree T;
	int n;

public:
	class Iterator {
		friend class SearchTree;

	private:
		TPos v;

	public:
		Iterator(const TPos& w) : v(w) { }
		const E& operator*() const { return *v; }
		E* operator->() const { return v.operator->(); }
		E& operator*() { return *v; }
		bool operator==(const Iterator& p) const { return v == p.v; }
		bool operator!=(const Iterator& p) const { return v != p.v; }
		Iterator& operator--() ;
		Iterator& operator++();
	};
};

// Search Tree
template <typename E>
typename SearchTree<E>::Iterator SearchTree<E>::find(const K& k) {
	TPos v = finder(k, root());
	if (v.isExternal()) return end();
	return Iterator(v);
}


template <typename E>
typename SearchTree<E>::ListIter SearchTree<E>::findAll(const K& key) {
	list<Iterator> all;
	TPos it = finder(key, root());
	while (it.isInternal()) {
		all.push_back(Iterator(it));
		it = finder(key, it.right());
	}
	return all;
}

template <typename E>
void SearchTree<E>::erase(const K& k) {
	TPos v = finder(k, root());
	if (v.isExternal()) throw exception("erase: no such an element");
	eraser(v);
}

template <typename E>
void SearchTree<E>::erase(const Iterator& p)  {
	list<Iterator> f = findAll((*p).key());
	if (f.size() == 0) return;

	auto it = f.begin();
	bool found = false;
	for (int i = 0; i < f.size(); ++i, ++it)
		if (*it == p) found = true;
	if (found) eraser(p.v);
	else throw exception("the tree doesn't contain such an iterator");
}

template <typename E>
void SearchTree<E>::eraseAll(const K& k) {
	list<Iterator> f = findAll(k);
	int sz = f.size();
	for (int i = 0; i < sz; ++i) {
		erase(*f.begin());
		f.pop_front();
	}
}

template <typename E>
typename SearchTree<E>::Iterator SearchTree<E>::begin() {
	TPos w = root();
	while (w.isInternal()) w = w.left();
	return Iterator(w.parent());
}

template <typename E>
typename SearchTree<E>::Iterator SearchTree<E>::end() { return Iterator(T.root()); }

template <typename E>
typename SearchTree<E>::TPos SearchTree<E>::finder(const K& k, const TPos& v) {
	if (v.isInternal()) {
		if (v->key() < k) return finder(k, v.right());
		else if (k < (*v).key()) return finder(k, v.left());
	}
	return v;
}

template <typename E>
typename SearchTree<E>::TPos  SearchTree<E>::inserter(const K& k, const V& x) {
	TPos v = finder(k, root());
	while (v.isInternal())
		v = finder(k, v.right());
	T.expandExternal(v);
	(*v).setKey(k);
	(*v).setValue(x);
	++n;
	return v;

}

template <typename E>
typename  SearchTree<E>::TPos  SearchTree<E>::eraser(const TPos& v) {
	TPos w;
	if (v.left().isExternal()) w = v.left();
	else if (v.right().isExternal()) w = v.right();
	else {
		w = v.right();
		while (w.isInternal())
			w = w.left();
		TPos par = w.parent();
		(*v).setKey((*par).key());
		(*v).setValue((*par).value());
	}
	--n;
	return T.removeAboveExternal(w);
}


// --- SearchTree::Iterator ---
template <typename E>
typename  SearchTree<E>::Iterator&  SearchTree<E>::Iterator::operator--()  {
	TPos w = v.left();
	if (w.isInternal()) { // ima lqvo dete
		while (w.isInternal()) w = w.right();
		v = w.parent();
	}
	else { // nqma lqvo dete
		if (v.isRight()) v = v.parent();
		else {
			while (!v.isRoot() && v.isLeft()) v = v.parent();
			if (!v.isRoot()) v = v.parent();
			else throw exception("out of range iterator");
		}
	}
	return *this;
}

template <typename E>
typename  SearchTree<E>::Iterator&  SearchTree<E>::Iterator::operator++()  {
	TPos w = v.right();
	if (w.isInternal()) {
		while (w.isInternal()) {
			v = w; w = v.left();
		}
	}
	else {
		w = v.parent();
		while (w.right() == v) {
			v = w; w = v.parent();
		}
		v = w;
	}
	return *this;
}

