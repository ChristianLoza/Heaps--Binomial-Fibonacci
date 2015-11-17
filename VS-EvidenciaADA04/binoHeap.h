#ifndef __BINOMIAL_HEAP_HPP__
#define __BINOMIAL_HEAP_HPP__

#include "heap.hpp"

#define MAX 999999

#include <cstddef>
#include <vector>
#include <cassert>
using namespace std;

class binoHeapNode;

class binoHeapKey {
public:
	int key;
	binoHeapNode *node;
	binoHeapKey(int _key) {
		key = _key;
		node = NULL;
	}
};


class binoHeapNode {
public:
	binoHeapKey *elem;
	int degree;
	binoHeapNode *parent;
	binoHeapNode *sibling;
	binoHeapNode *child;

	binoHeapNode(binoHeapKey *_elem) {
		elem = _elem;
		degree = 0;
		parent = sibling = child = NULL;
	}
};

class binomial_heap : public heap<binomial_heap, binoHeapKey> {
private:
	binoHeapNode *linker(binoHeapNode *, binoHeapNode *);
	void swapParent(binoHeapNode *);
	void destruction(binoHeapNode *);
	int _size;
public:
	binoHeapNode *Header;
	binomial_heap();
	~binomial_heap();
	binoHeapKey *insert(int);
	int minimum() const;
	void extract_min();
	void merge(binomial_heap &);
	void decrease_key(binoHeapKey *, int);
	void remove(binoHeapKey *);
	int size() const;

};

binomial_heap::binomial_heap() {
	Header = NULL;
	_size = 0;
}

binomial_heap::~binomial_heap() {
	destruction(Header);
}

binoHeapKey *binomial_heap::insert(int key) {

	binoHeapKey *newKey = new binoHeapKey(key);
	binoHeapNode *newNode = new binoHeapNode(newKey);
	newKey->node = newNode;
	binomial_heap newHeap;
	newHeap.Header = newNode;
	newHeap._size = 1;
	merge(newHeap);
	return newKey;
}

int binomial_heap::minimum() const {
	binoHeapNode *node = Header;
	int min = MAX;
	while (node != NULL) {
		if (node->elem->key < min)
			min = node->elem->key;
		node = node->sibling;
	}

	return min;
}

void binomial_heap::extract_min() {
	assert(_size > 0);
	
	binoHeapNode *node = Header;
	binoHeapNode *min_node = NULL;
	int min = MAX;

	while(node != NULL) {
		if (node->elem->key < min) {
			min = node->elem->key;
			min_node = node;
		}
		node = node->sibling;
	}
	node = min_node->child;
	binoHeapNode **aux = &Header;
	while(*aux != min_node) {
		aux = &((*aux)->sibling);
	}
	*aux = (*aux)->sibling;
	delete min_node->elem;
	delete min_node;
	_size--;

	if (node != NULL) {
		binomial_heap children;
		while(node != NULL) {
			node->parent = NULL;
			binoHeapNode *next = node->sibling;
			node->sibling = children.Header;
			children.Header = node;
			children._size += 1 << node->degree;
			node = next;
		}
		_size -= children._size;
		merge(children);
	}
}


void binomial_heap::merge(binomial_heap &bheap) {
	binoHeapNode *NodeA = Header;
	binoHeapNode *NodeB = bheap.Header;
	binoHeapNode *sgteA=0;
	binoHeapNode *sgteB=0;	
	binoHeapNode *hAux = NULL;
	int auxA, auxB, auxC;

	binoHeapNode *merge_Header = NULL;
	binoHeapNode **next = &(merge_Header);

	while (NodeA != NULL || NodeB != NULL) {
		if (NodeA != NULL) {
			auxA = NodeA->degree;
			sgteA = NodeA->sibling;
		} else
			auxA = MAX;
		
		if (NodeB != NULL) {
			auxB = NodeB->degree;
			sgteB = NodeB->sibling;
		} else
			auxB = MAX;

		if (hAux == NULL) {
			if (auxA<auxB) {
				NodeA->sibling = NULL;
				*next = NodeA;
				next = &(NodeA->sibling);
				NodeA = sgteA;
			} else if (auxA>auxB) {
				NodeB->sibling = NULL;
				*next = NodeB;
				next = &(NodeB->sibling);
				NodeB = sgteB;
			} else {
				hAux = linker(NodeA, NodeB);
				NodeA = sgteA;
				NodeB = sgteB;
			}
		} else {
			auxC = hAux->degree;

			if (auxC < auxA && auxC < auxB) {
				hAux->sibling = NULL;
				*next = hAux;
				next = &(hAux->sibling);
				hAux = NULL;
			} else {
				if (auxA < auxB) {
					hAux = linker(NodeA, hAux);
					NodeA = sgteA;
				} else if (auxA > auxB) {
					hAux = linker(NodeB, hAux);
					NodeB = sgteB;
				} else {
					hAux->sibling = NULL;
					*next = hAux;
					next = &(hAux->sibling);
					hAux = NULL;
				}
			}
		}
	}

	if (hAux != NULL) {
		hAux->sibling = NULL;
		*next = hAux;
		hAux = NULL;
	}
	Header = merge_Header;
	_size += bheap._size;
	bheap.Header = NULL;
	bheap._size = 0;
}

void binomial_heap::decrease_key(binoHeapKey *ptKey, int key) {
	assert(key < ptKey->key);

	ptKey->key = key;
	binoHeapNode *node = ptKey->node;
	while (node->parent != NULL) {
		if (node->elem->key < node->parent->elem->key) {
			swapParent(node);
			node = node->parent;
		} else
			break;
	}
}

void binomial_heap::remove(binoHeapKey *ptKey) {
	decrease_key(ptKey, -MAX);
	extract_min();
}

int binomial_heap::size() const {
	return _size;
}

binoHeapNode *binomial_heap::linker(binoHeapNode *NodeA, binoHeapNode *NodeB) {
	int ka = NodeA->elem->key, kb = NodeB->elem->key;

	binoHeapNode *parent, *child;
	if (ka < kb) {
		parent = NodeA;
		child = NodeB;
	} else {
		parent = NodeB;
		child = NodeA;
	}

	child->parent = parent;
	child->sibling = parent->child;
	parent->child = child;
	parent->degree++;

	return parent;
}

void binomial_heap::swapParent(binoHeapNode *node) {
	binoHeapKey *temp = node->elem;

	node->elem = node->parent->elem;
	node->elem->node = node;
	node->parent->elem = temp;
	node->parent->elem->node = node->parent;
}

void binomial_heap::destruction(binoHeapNode *node) {
	while(node != NULL) {
		binoHeapNode *next = node->sibling;
		destruction(node->child);
		delete node->elem;
		delete node;
		node = next;
	}
}

#endif
