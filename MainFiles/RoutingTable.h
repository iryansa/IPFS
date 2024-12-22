#pragma once
#include<iostream>
#include"BigInt.h"
// #include"DhtNode.h"
using namespace std;
// class T;
template <typename T>
class RTNode
{
public:
	T* node;
	//string machinemachineHash;
	BigInt machineHash;
	int index;
	RTNode<T>* next;
	RTNode<T>* prev;
	RTNode<T>()
	{
		index = 0;
		node = nullptr;
		next = NULL;
		prev = NULL;
	}

	RTNode<T>(T* node1)
	{
		this->node = node1;
		this->machineHash = node1->machineHash;
		this->index = 0;
		next = NULL;
		prev = NULL;
	}

	void insert(T* node1) {
		RTNode<T>* curr = this;
		while (curr->next) {
			curr = curr->next;
		}
		curr->next = new RTNode<T>(node1);
		curr->next->prev = curr;
		curr->next->machineHash = node1->machineHash;
	}


};
template <class T>
class RoutingTable
{
public:
	RTNode<T>* head;
	int size;

	RoutingTable<T>(int n_elements = 1) {  //size of identifier space
		this->size = n_elements-1;
		head = nullptr;
	}
	bool isEmpty()
	{
		if (head == NULL)
			return true;
		else
			return false;
	}

	void insertToHead(T* node1)
	{
		RTNode<T>* temp = new RTNode<T>(node1);
		temp->index = 0;

		if (head == NULL)
		{
			head = temp;
			return;
		}

		else
		{
			temp->next = head;
			head->prev = temp;
			head = temp;
			RTNode<T>* curr = head->next;
			while (curr) {
				curr->index++;
				curr = curr->next;
			}
			return;
		}

	}

	void insert(T* node1)
	{
		RTNode<T>* temp = new RTNode<T>(node1);

		if (head == NULL)
		{
			head = temp;
			return;
		}

		else
		{

			RTNode<T>* p = head;

			while (p->next)
			{
				p = p->next;
			}
			if (p->index >= size) {
				cout << "Routing Table Full!" << endl;
				return;
			}
			p->next = temp;
			temp->prev = p;
			temp->index = temp->prev->index + 1;
			return;
		}
	}

	RTNode<T>* operator[](int i) {
		RTNode<T>* current = head;

		for (int j = 0; j <= i && current; ++j) {
			current = current->next;
		}

		return current;
	}


	bool search(T* d)
	{
		RTNode<T>* p = head;

		while (p->next)
		{
			if (p->node == d)
			{
				return true;
			}
			p = p->next;
		}

		return false;
	}

	void update(T* c, T* d)   // update the node after deletion (c is the node to be deleted and d is the succesor node)
	{
		if (search(c))
		{
			RTNode<T>* p = head;
			while (p->next)
			{
				if (p->node == c)
				{
					p->node = d;
					p->machineHash = d->machineHash;
				}
				p = p->next;
			}
		}

		return;

	}

	void insertAtIndex(T* d, int index)
	{
		RTNode<T>* temp = new RTNode<T>(d);
		if (index == 0)
		{
			this->insertToHead(d);
			return;
		}

		else
		{
			RTNode<T>* p = head;
			RTNode<T>* q = head;


			for (int i = 0; i < index; i++)
			{
				p = q;
				q = q->next;
			}

			p->next = temp;
			temp->next = q;
			temp->prev = p;
			q->prev = temp;

			RTNode<T>* temp = head;
			for (int i = 0; temp; ++i) {
				temp->index = i;
				temp = temp->next;
			}

			return;

		}
	}

	T* MachineForFile(BigInt filemachineHash) {
		RTNode<T>* curr = head;
		while (curr && curr->next) {
			if (curr->machineHash >= filemachineHash) return curr->node;
			curr = curr->next;
		}
		return head->node;
	}

	void displayRT() {
		RTNode<T>* curr=head;
		for(int i=0;i<=size;i++){
			cout << curr->machineHash;
			if (i < size)
			{
				cout << "->";
			}

			curr = curr->next;
		}
		cout << endl<<endl;
	}

};
