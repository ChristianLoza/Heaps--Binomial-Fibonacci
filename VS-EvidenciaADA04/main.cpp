// VS-EvidenciaADA04.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "bheap.h"
#include "binoHeap.h"
#include "fiboHeap.h"


#define HEAPFIB fibonacci_heap
#define HEAPBIN binomial_heap


using namespace std;

/*
void binoInsert(){
	HEAPBIN hbino;
	hbino.insert(104);
	hbino.insert(104);
	hbino.insert(124);
	hbino.insert(13);
	hbino.insert(132);
	hbino.insert(123);hbino.insert(41);
}
void binoMin(){
	HEAPBIN hbino;
	cout<<"Binomial Heap - MINIMUM"<<endl;
	cout<<hbino.insert(12)<<endl;
	cout<<hbino.insert(39)<<endl;
	cout<<hbino.insert(129)<<endl;
	cout<<hbino.insert(190)<<endl;
	cout<<hbino.insert(129)<<endl;
	cout<<hbino.insert(10)<<endl;
	cout<<hbino.insert(34)<<endl;
	cout<<hbino.minimum()<<endl;;
}
void merge() {
	HEAPFIB hf1, hf2;

	hf1.insert(354);hf1.insert(483);
	hf1.insert(1);hf1.insert(345);
	hf1.insert(12);hf1.insert(32);
	hf1.insert(5);hf1.insert(90);
	hf1.insert(-100);hf1.insert(125);
	hf1.insert(130);
	hf1.extract_min();
	hf1.insert(12);
	hf1.insert(-2);
	hf1.insert(3);
	hf2.insert(45);
	hf2.insert(1);
	hf2.insert(10);
	hf2.extract_min();
	hf2.insert(100);
	hf2.insert(50);
	hf2.insert(-50);
	hf2.insert(-20);
	hf2.insert(70);
	hf2.insert(-15);
	hf2.insert(50);
	hf2.extract_min();
	hf2. insert(3);
	hf2.extract_min();
	hf1.merge(hf2);
}
	*/
void insertar(){
	HEAPBIN hf;

/*	hf.insert(12);hf.insert(423);
	hf.insert(43);hf.insert(12);
	hf.insert(16);hf.insert(490);
	hf.insert(23);hf.insert(128);
	hf.insert(63);hf.insert(14);
	hf.insert(19);hf.insert(10);
	hf.insert(87);hf.insert(30);
	hf.insert(54);hf.insert(49);
	hf.insert(45);hf.insert(123);*/

	for(int i=0;i<10;i++)
  	   hf.insert(i);
	for(int i=0;i<9;i++)
	{
	   hf.extract_min();
	   cout<<hf.Header->elem->key<<endl;
	}

}
int _tmain(int argc, _TCHAR* argv[]){
{
	insertar();
	system("pause");
	return 0;
	}
}