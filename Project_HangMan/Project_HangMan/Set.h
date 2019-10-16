#include "AVLTree.h"


template <typename E>
class Set {
	class SetEntry {
		friend class Set<E>;
		typedef E K;
		typedef E V;

	public:
		typedef E Key;
		typedef E Value;
		SetEntry(const K& k = K(), const V& v = V()) : _key(k) { }
		
		const K& key() const { return _key; }
		const V& value() const { return _key; }
		void setKey(const K& k) { _key = k; }
		void setValue(const V& v) { return; }

	private:
		K _key;

		friend ostream& operator<<(ostream& of, const SetEntry& se) { return of << se.key(); }
	};

protected:
	typedef SetEntry SE;
	typename typedef AVLTree<SE>::Iterator AVLIterator;

public:
	class Iterator {
		friend class Set<E>;
	public:
		Iterator(const AVLIterator& w = NULL) : iter(w) { }
		Iterator operator=(const AVLIterator& w) { iter = w; return *this; }
		E& operator*() { return iter->_key; }
		bool operator==(const Iterator& p) const { return iter == p.iter; }
		bool operator!=(const Iterator& p) const { return iter != p.iter; }
		Iterator& operator--() { --iter; return *this; }
		Iterator& operator++() { ++iter; return *this; }

	private:
		typename AVLIterator iter;
	};

	Set() : T() {}
	int size() const { return T.size(); }
	bool empty() const { return T.empty(); }
	Iterator insert(const E& k) { Iterator p = T.find(k); if (p == end()) T.insert(k, k); return p; }
	void erase(const E& k)  { T.erase(SE(k)._key); }
	void erase(const Iterator& p)  { T.erase(p.iter); }
	Iterator find(const E& k) { return Iterator(T.find(k)); }
	Iterator begin() { return T.begin(); }
	Iterator end() { return T.end(); }
	Iterator getRandElement() { return T.getRand(); }

private:
	AVLTree<SE> T;
};
