
#ifndef __HEAP_HPP__
#define __HEAP_HPP__

template<class H, class K>
class heap {
public:
	virtual K *insert(int) = 0;
	virtual int minimum() const = 0;
	virtual void extract_min() = 0;
	virtual void merge(H &) = 0;
	virtual void decrease_key(K *, int) = 0;
	virtual void remove(K *) = 0;
	virtual int size() const = 0;
};

#endif
