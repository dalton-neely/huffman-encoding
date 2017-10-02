#ifndef HEAPCLASS_HPP
#define HEAPCLASS_HPP

#include <iostream>
#include "nodeClass.hpp"

using namespace std;

class heapClass{
	public:
		nodeClass * nodes[500];
		int size;
		heapClass();
		void insert(nodeClass *);
		void sort(int);
		void print();
		nodeClass * extract();
};

heapClass::heapClass(){
	for(int i = 0; i < 500; i++)
		nodes[i] = nullptr;
	size = 0;
}

void heapClass::insert(nodeClass * _node){
	nodes[size] = _node;
	size++;
	for(int i = (size - 2) / 2; i >= 0; i--)
		sort(i);
}

void heapClass::sort(int _index){
	if(_index >= 0){
		int small = _index;
		int left = (2 * _index) + 1;
		int right = (2 * _index) + 2;
		if(left < size && nodes[left]->freq < nodes[small]->freq)
			small = left;
		if(right < size && nodes[right]->freq < nodes[small]->freq)
			small = right;
		if(small != _index){
			swap(nodes[small], nodes[_index]);
			sort(small);
		}
	}
}

void heapClass::print(){
	for(int i = 0; i < size; i++)
		cout << "index: " << i << " [" << nodes[i]->freq << "," << nodes[i]->ascii << "]" << endl;
}

nodeClass * heapClass::extract(){
	nodeClass * ptr;
	ptr = new nodeClass;
	ptr->set(nodes[0]->freq, nodes[0]->ascii, nodes[0]->L, nodes[0]->R);
	swap(nodes[0], nodes[--size]);
	for(int i = (size - 2) / 2; i >= 0; i--)
		sort(i);
	return ptr;
}

#endif
