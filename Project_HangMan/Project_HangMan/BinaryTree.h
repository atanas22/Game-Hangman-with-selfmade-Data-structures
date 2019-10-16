#ifndef BINARYTREE_H
#define BINARYTREE_H
#endif // !BINARYTREE_H

#include <iostream>
using namespace std;

template <typename T>
class BinaryTree {
protected:
	struct Node {
		Node() : el(), parent(NULL), left(NULL), right(NULL) {}
		T el;
		Node *parent, *left, *right;
	};

public:
	class Position {
		friend class BinaryTree<T>;
	public:
		Position() : pos(NULL) {}
		T & operator*() const { return pos->el; } 
		T * operator->() const { return &(pos->el); }
		Position left() const { return Position(pos->left); }
		Position right() const { return Position(pos->right); }
		Position parent() const { return Position(pos->parent); }
		bool isRoot() const { return pos->parent == NULL; }
		bool isExternal() const { return (pos->right == NULL) && (pos->left == NULL); }
		bool isInternal() const { return (pos->right != NULL) || (pos->left != NULL); }
		bool operator==(const Position &p) const { return pos == p.pos; }
		bool operator!=(const Position &p) const { return pos != p.pos; }
		void setLeft(const Position& p) { pos->left = p.pos; }
		void setRight(const Position& p) { pos->right = p.pos; }
		void setParent(const Position& p) { pos->parent = p.pos; }
		
	private:
		Node *pos;
		Position(Node *p) { pos = p; }
	};

public:
	typedef vector<Position> PositionList;
	BinaryTree() : _root(NULL), n(0) {}
	~BinaryTree() { destruct(_root); }
	BinaryTree(const BinaryTree &bt) { copyWidth(bt); }
	BinaryTree& operator=(const BinaryTree &bt) { if (this != &bt) return copyWidth(); }

	int size() const { return n; }
	bool empty() const { return size() == 0; }
	Position root() const { return Position(_root); }
	void addRoot() { _root = new Node; n = 1; }
	PositionList positions() const;
	void expandExternal(const Position & p);
	Position removeAboveExternal(const Position &p);
	void ArithmeticEulerTour(Position f);
	Position at(int ind) const;
	void printWidth();
	void readListElements(const list<T>& ls);

protected:
	void preorder(Node* nd, PositionList &pl);
	void eulerTour(Node* nd, PositionList &pl) const;
	void destruct(Node* nd);

private:
	BinaryTree& copyWidth(const BinaryTree &bt);
	Node *_root;
	int n;
};

template <typename T>
typename BinaryTree<T>::PositionList BinaryTree<T>::positions() const {
	PositionList pl;
	preorder(_root, pl);
	//eulerTour(_root, pl);
	return pl;
}

template <typename T>
void BinaryTree<T>::expandExternal(const Position & p) {
	if (!p.isExternal()) throw exception("expandExternal(): internal node");
	Node* pos = p.pos;
	pos->left = new Node;
	pos->right = new Node;
	pos->left->parent = pos->right->parent = pos;
	n += 2;
}


template <typename T>
typename BinaryTree<T>::Position BinaryTree<T>::removeAboveExternal(const Position &p) {
	if (p.isRoot() || !p.isExternal()) throw exception("removeAboveExternal(): internal node or root");
	Node *curr = p.pos;
	Node *par = curr->parent;
	Node *sib = (curr == par->left ? par->right : par->left);
	if (par == _root) {
		_root = sib;
		sib->parent = NULL;
	}
	else {
		Node *gpar = par->parent;
		if (par == gpar->left)
			gpar->left = sib;
		else
			gpar->right = sib;
		sib->parent = gpar;
	}
	delete curr;
	delete par;
	n -= 2;
	return Position(sib);
}

template <typename T>
void BinaryTree<T>::ArithmeticEulerTour(Position f) {
	Node *nd = f.pos;
	if (nd->left == NULL && nd->right == NULL)
		cout << nd->el << " ";
	else {
		cout << "(";
		ArithmeticEulerTour(nd->left);
		cout << nd->el << " ";
		ArithmeticEulerTour(nd->right);
		cout << ")";
	}
}

template <typename T>
typename BinaryTree<T>::Position BinaryTree<T>::at(int ind) const {
	if (ind > size()) throw exception("at(): out of range");
	list<Position> save;
	save.push_back(_root);
	int cnt = 1;
	while (cnt < ind && !save.empty()) {
		if (!save.front().isExternal()) {
			save.push_back(save.front().left());
			save.push_back(save.front().right());
			cnt += 2;
		}
		if (cnt > ind) save.pop_back();
		save.pop_front();
	}
	return save.back();
}

template <typename T>
void BinaryTree<T>::printWidth() {
	queue<Position, list<Position>>save;
	save.push(_root); int range = 1, cnt = 0;
	while (!save.empty()) {
		if (cnt == range) { cout << "\n"; range *= 2; cnt = 0; } 
		cout << *save.front() << " ";
		if (!save.front().isExternal()) {
			save.push(save.front().left());
			save.push(save.front().right());
		}
		save.pop();
		++cnt;
	}
}

template <typename T>
void BinaryTree<T>::readListElements(const list<T>& ls) {
	destruct(_root); _root = NULL;
	addRoot();
	queue<Position, list<Position>>save;
	save.push(_root); int cnt = 0;

	for (auto it = ls.begin(); it != ls.end(); ++it) {
		*save.front() = *it;
		if (save.front().isExternal()) expandExternal(save.front());
		save.push(save.front().left());
		save.push(save.front().right());
		save.pop(); ++cnt;
	}

	if (cnt % 2 == 0) save.pop();
	while (!save.empty()) {
		save.front().pos->parent->left = NULL;
		save.front().pos->parent->right = NULL;
		delete save.front().pos;
		save.pop();
	}
}

template <typename T>
void BinaryTree<T>::preorder(Node* nd, PositionList &pl) {
	if (nd->left != NULL) preorder(nd->left, pl);
	pl.push_back(Position(nd));
	if (nd->right != NULL) preorder(nd->right, pl);
}

template <typename T>
void BinaryTree<T>::eulerTour(Node* nd, PositionList &pl) const {
	if (nd->left != NULL) eulerTour(nd->left, pl);
	pl.push_back(Position(nd));
	if (nd->right != NULL) eulerTour(nd->right, pl);
	pl.push_back(Position(nd));
}

template <typename T>
void BinaryTree<T>::destruct(Node* nd) {
	if (nd != NULL && nd->left != NULL) {
		destruct(nd->left);
		destruct(nd->right);
		//cout << nd->el << "\n";
		delete nd;
	}
	else if (nd != NULL) {
		//cout << nd->el << " ";
		delete nd;
	}
}

template <typename T>
BinaryTree<T>& BinaryTree<T>::copyWidth(const BinaryTree<T> &bt) {
	queue<Position, list<Position>>save;
	save.push(bt._root); int range = 1, cnt = 0;
	list<T> ls;
	while (!save.empty()) {
		if (cnt == range) { range *= 2; cnt = 0; }
		ls.push_back(*save.front());
		if (!save.front().isExternal()) {
			save.push(save.front().left());
			save.push(save.front().right());
		}
		save.pop();
		++cnt;
	}
	readListElements(ls);
	return *this;
}
