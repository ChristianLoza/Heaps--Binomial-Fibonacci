//
#ifndef __bheap_HPP_
#define __bheap_HPP_

#include <vector>
#include <cassert>
#include "heap.hpp"

#define MAX 999999

using std::vector;

class bHeapKey
{
public:
	int key;
	int index;

	bHeapKey(int _key) {
		key = _key;
	}
};

class bheap : public heap<bheap, bHeapKey> {
private:
	int izq(int) ;
	int der(int) ;
	int parent(int) ;
	void makeHeap();
	void heapfy(int);
	void swapParent(int);
	vector<bHeapKey *> vheap;
public:
	bheap();
	~bheap();
	bHeapKey *insert(int);
	int minim() ;
	void extractMin();
	void merge(bheap &);
	void decreaseKey(bHeapKey *, int);
	void remove(bHeapKey *);
	int size() ;

};

void bheap::remove(bHeapKey *pKey) {
	decreaseKey(pKey, -MAX);
	extractMin();
}

int bheap::size()  {
	return vheap.size();
}

int bheap::izq(int i)  {
	return 2*i + 1;
}

int bheap::der(int i)  {
	return 2*i + 2;
}

int bheap::parent(int i)  {
	return (i-1)/2;
}

void bheap::makeHeap() {
	int n = vheap.size();
	for (int i=n-1;i>=0;i--)
		heapfy(i);
}

void bheap::heapfy(int i) {
	int n = vheap.size();
	while (i < n) {
		int iz = izq(i);
		int de = der(i);
		int m = i;
		if (iz<n && vheap[iz]->key < vheap[m]->key)
			m = iz;
		if (de<n && vheap[de]->key < vheap[m]->key)
			m = de;
		if (m!=i) {
			swapParent(m);
			i = m;
		} else
			break;
	}
}

void bheap::swapParent(int i) {
	bHeapKey *temp = vheap[i];
	int pi = parent(i);

	vheap[i] = vheap[pi];
	vheap[i]->index = i;
	vheap[pi] = temp;
	vheap[pi]->index = pi;
}
bheap::bheap() {}
bheap::~bheap() {
	int n = vheap.size();
	for (int i = 0; i < n; i++)
		delete vheap[i];
}

bHeapKey *bheap::insert(int key) {
	assert(key < MAX);
	assert(key > -MAX);
	bHeapKey *newNode = new bHeapKey(MAX);
	vheap.push_back(newNode);
	newNode->index = vheap.size() - 1;
	decreaseKey(newNode, key);

	return newNode;
}

int bheap::minim()  {
	return vheap[0]->key;
}

void bheap::extractMin() {
	assert(vheap.size() > 0);
	delete vheap[0];
	int n = vheap.size();
	if (n > 1) {
		vheap[0] = vheap[n - 1];
		vheap[0]->index = 0;
		vheap.resize(n - 1);
		heapfy(0);
	} else {
		vheap.clear();
	}
}

void bheap::merge(bheap &mHeap) {
	int n = vheap.size();
	int m = mHeap.vheap.size();
	vheap.resize(n + m);

	for(int i = 0; i < m; i++) {
		vheap[n + i] = mHeap.vheap[i];
		vheap[n + i]->index = n + i;
	}

	mHeap.vheap.clear();
	makeHeap();
}

void bheap::decreaseKey(bHeapKey *pKey, int key) {
	assert(key <= pKey->key);
	pKey->key = key;
	int i = pKey->index;
	while (i > 0) {
		if (vheap[parent(i)]->key > key) {
			swapParent(i);
			i = parent(i);
		} else
			break;
	}
}
#endif
