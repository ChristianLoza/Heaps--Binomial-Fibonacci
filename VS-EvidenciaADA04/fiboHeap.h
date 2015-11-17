#ifndef __fiboHeap_HPP__
#define __fiboHeap_HPP__

#include "heap.hpp"

#define MAX 999999

#include <cstddef>
#include <vector>
#include <cassert>
using namespace std;

class fiboHeapkey;


class fiboHeapKey {
public:
	int key;
	fiboHeapkey *node;

	fiboHeapKey(int _key) {
		key = _key;
		node = NULL;
	}
};

class fiboHeapkey {
public:
	fiboHeapKey *elem;
	int grade;
	bool mark;
	fiboHeapkey *parent;
	fiboHeapkey *prev;
	fiboHeapkey *next;
	fiboHeapkey *child;

	fiboHeapkey(fiboHeapKey *_elem) {
		elem = _elem;
		grade = 0;
		mark = false;
		parent = prev = next = child = NULL;
	}
};

class fiboHeap : public heap<fiboHeap, fiboHeapKey> {
private:
	void consolidate();
	fiboHeapkey *linker(fiboHeapkey *, fiboHeapkey *);
	void addNode(fiboHeapkey *, fiboHeapkey **);
	void removeNode(fiboHeapkey *, fiboHeapkey **);
	void concatList(fiboHeapkey *, fiboHeapkey **);
	void cut(fiboHeapkey *);
	void cascadeCut(fiboHeapkey *);
	void destructionList(fiboHeapkey *);
	int lg(int n) const;
	
	int _size;
public:
	fiboHeapkey *min;
	fiboHeap();
	~fiboHeap();
	fiboHeapKey *insert(int key);
	int minimum() const;
	void extract_min();
	void merge(fiboHeap &fheap);
	void decrease_key(fiboHeapKey *ptr, int key);
	void remove(fiboHeapKey *);
	int size() const;
	void show();

};

void fiboHeap::show(){
}

fiboHeap::fiboHeap() {
	min = NULL;
	_size = 0;
}

fiboHeap::~fiboHeap() {
	destructionList(min);
}

fiboHeapKey *fiboHeap::insert(int key) {
	fiboHeapKey *newKey = new fiboHeapKey(key);
	fiboHeapkey *newNode = new fiboHeapkey(newKey);
	newKey->node = newNode;
	

	addNode(newNode, &min);
	if (newNode->elem->key < min->elem->key)
		min = newNode;
	_size++;
	return newKey;
}

int fiboHeap::minimum() const {
	assert(_size > 0);
	return min->elem->key;
}

void fiboHeap::extract_min() {
	assert(_size > 0);
	if (min != NULL) {
		fiboHeapkey *child = min->child;
		if (min->child != NULL) {	
			do {
				child->parent = NULL;
				child->mark = false;
				child = child->next;
			} while(child != min->child);
			
			concatList(min->child, &min);
		}
		fiboHeapkey *temp = min;
		removeNode(min, &min);
		
		delete temp->elem;
		delete temp;
		if (min != NULL)
			consolidate();
		_size--;
	}
}

void fiboHeap::merge(fiboHeap &fheap) {
	concatList(fheap.min, &min);
	_size = _size + fheap._size;
	fheap.min = NULL;
	fheap._size = 0;
}

void fiboHeap::decrease_key(fiboHeapKey *ptr, int key) {
	assert(key <= ptr->key);
	
	ptr->key = key;
	fiboHeapkey *node = ptr->node;
	fiboHeapkey *p = node->parent;
	if (p != NULL && node->elem->key < p->elem->key) {
		cut(node);		
		cascadeCut(p);
	}
	if (node->elem->key < min->elem->key)
		min = node;
}
void fiboHeap::remove(fiboHeapKey *ptr) {
	decrease_key(ptr, -MAX);
	extract_min();
}
int fiboHeap::size() const {
	return _size;
}
void fiboHeap::consolidate() {
	vector<fiboHeapkey *> newRoots(2 * lg(_size) + 1, NULL);
	
	fiboHeapkey *aux = min;
	do {
		fiboHeapkey *next = aux->next;
		int data = aux->grade;
		while (newRoots[data] != NULL) {
			fiboHeapkey *y = newRoots[data];
			aux = linker(aux, y);
			newRoots[data] = NULL;
			data++;
		}
		newRoots[data] = aux;
		aux = next;
	} while(aux != min);
	min = NULL;
	for (int i = 0; i < (int)newRoots.size(); i++) {
		if (newRoots[i] != NULL) {
			addNode(newRoots[i], &min);
			if (newRoots[i]->elem->key < min->elem->key) {
				min = newRoots[i];
			}
		}
	}
}

fiboHeapkey *fiboHeap::linker(fiboHeapkey *NodeA, fiboHeapkey *NodeB) {
	int ka = NodeA->elem->key, kb = NodeB->elem->key;

	fiboHeapkey *parent, *child;
	if (ka < kb) {
		parent = NodeA;
		child = NodeB;
	} else {
		parent = NodeB;
		child = NodeA;
	}
	child->parent = parent;
	child->mark = false;
	addNode(child, &(parent->child));
	parent->grade++;

	return parent;
}

void fiboHeap::addNode(fiboHeapkey *newNode, fiboHeapkey **list) {
	newNode->next = newNode->prev = newNode;
	concatList(newNode, list);
}


void fiboHeap::removeNode(fiboHeapkey *node, fiboHeapkey **list) {
	node->next->prev = node->prev;
	node->prev->next = node->next;
	
	if (node->next == node)
		*list = NULL;
	else if (*list == node)
		*list = node->next;
}


void fiboHeap::concatList(fiboHeapkey *fheap, fiboHeapkey **list) {
	if (fheap == NULL)
		return;
	if (*list == NULL)
		*list = fheap;
	else {
		fiboHeapkey *our = *list;
		fiboHeapkey *our_prev = our->prev;
		our->prev = fheap->prev;
		our_prev->next = fheap;
		fheap->prev->next = our;
		fheap->prev = our_prev;
	}
}

void fiboHeap::cut(fiboHeapkey *node) {
	fiboHeapkey *p = node->parent;
	removeNode(node, &(p->child));
	p->grade--;
	addNode(node, &min);
	node->parent = NULL;
	node->mark = false;
}
	
void fiboHeap::cascadeCut(fiboHeapkey *node) {
	fiboHeapkey *p = node->parent;
	if (p != NULL) {
		if (!node->mark){
			node->mark = true;
		}else {
			cut(node);
			cascadeCut(p);
		}
	}
}

void fiboHeap::destructionList(fiboHeapkey *node) {
	if (node != NULL) {
		if (node->next != node) {
			fiboHeapkey *first = node;
			node = node->next;
			while (node != first) {
				fiboHeapkey *next = node->next;
				destructionList(node->child);
				delete node->elem;
				delete node;
				node = next;
			}
		}
		destructionList(node->child);
		delete node->elem;
		delete node;
	}
}

int fiboHeap::lg(int n) const {
	int res = 0;
	while (n > 1) {
		n >>= 1;
		res++;
	}
	return res;
}

#endif
