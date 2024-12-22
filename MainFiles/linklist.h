#pragma once
#include<iostream>
using namespace std;
//#include"pch.h"
template<typename T>
class Node {
public:
	Node* next;
	T data;
	Node() {
		this->next = NULL;
	}
	Node(Node* Next , T Data ) {
		next = Next;
		data = Data;
	}
	Node* GetNext() {
		return next;
	}
	Node GetData() {
		return data;
	}
	void SetNext(Node* Next) {
		next = Next;
	}
	void SetData(T Data) {
		data = Data;
	}
};
template<typename T>
class SLinkedList {
public:
	Node<T>* head;
	int count = 0;
	SLinkedList<T>() {
		head = NULL;
	}

	SLinkedList<T>(const SLinkedList<T>& copy) {
		this->head = NULL;
		this->count = 0;
		Node<T>* copyHead = copy.head;
		while (copyHead != NULL) {
			this->insert(copyHead->GetData());
			copyHead = copyHead->GetNext();
		}
	}
	~SLinkedList<T>() {
		this->clear();
	}
 	T getdata() 
	{
		if (head)
		{
			return head->data;
		}
		else
		{
			T temp;
			return temp;
		}
	}
	void insert(T value) {
		Node<T>* newNode = new Node<T>();
		newNode->next = NULL;
		newNode->data = value;
		Node<T>* temp = head;
		if (!head) {
			head = newNode;
		}
		else {
			while (temp->next) {
				temp = temp->next;
			}
			temp->next = newNode;
		}

		count++;
	}
	void insertAtHead(T value) {
		Node<T>* newNode = new Node<T>();
		newNode->next = head;
		newNode->data = value;
		head = newNode;

		count++;

	}
	bool isEmpty() {
		if (head == nullptr)
			return 1;
		else
			return 0;
	}
	void InsertAtIndex(T val, int k) {
		Node<T>* temp = head;
		if (k < 0) {
			return;
		}
		for (int i = 0; i < k && temp->next; i++)
		{
			temp = temp->next;
		}
		Node<T>* newNode = new Node<T>();
		newNode->next = temp->next;
		temp->next = newNode;
		newNode->data = val;
		count++;

	}
	int search(T val) {
		int index = 1;
		Node<T>* temp = head;
		while (temp) {
			if (temp->data == val) {
				return index;
			}
			index++;
			temp = temp->next;
		}
		return index;
	}
	void update(int k, T val) {
		int index = 1;
		Node<T>* temp = head;
		while (index > k && temp) {
			temp = temp->next;
			index++;
		}
		temp->data = val;
	}
	void remove(T n) {
		//T returnValue;
		if (this->head) {
			Node<T>*temp = head;
		Node<T>* temp1 = head->next;
		while (temp1) {
			if (temp1->data == n) {
				break;
			}
			temp1 = temp1->next;
			temp = temp->next;
		}
		//	temp->next = temp->next->next;
		count--;
		//returnvalue = temp1->GetData();
		delete temp1;
		}
		//return returnValue;
	}
	void print() {
		Node<T>* temp = head;
		int count = 1;
		while (temp) {
			cout << count<<" . File Hash: "<<temp->data.hash << " File Path: " << temp->data.filepath << endl;
			temp = temp->next;
			++count;
		}
	}
	void specialPrint() {
		Node<T>* temp = head;
		cout << "The files stored with the corresponding hash " << this->head->data.hash << " are:\n";
	
		int counter = 1;
		while (temp) {
			cout << counter << ". " << temp->data.filepath << endl;
			temp = temp->next;
			++counter;
		}
		cout << "Which file would you like to delete. Enter the corresponding index number\n";
	}
	T deleteAt(int index) {
		T deleted;
		if (index < 0)
			return deleted;
		if (index > count)
			return deleted;
		Node<T>* curr = this->head;
		if (curr != NULL) {
			Node<T>* temp = NULL;
			int counter = 0;
			while (curr != NULL && counter < index) {
				temp = curr;
				curr = curr->GetNext();
				counter++;
			}
			if (temp == NULL) {
				this->head = curr->GetNext();
				deleted.hash = curr->data.hash;
				deleted.filepath = curr->data.filepath;
				delete curr;
				count--;
				curr = NULL;
			}
			else if(curr != NULL) {
				temp->SetNext(curr->GetNext());
				deleted.hash = curr->data.hash;
				deleted.filepath = curr->data.filepath;
				count--;
				delete curr;
				curr = NULL;
			}
		}
		return deleted;
	}
	int getCount() {
		return this->count;
	}
	void clear() {
		if (this->head) {
			Node<T>* curr = this->head;
			Node<T>* temp = NULL;
			while (curr != NULL) {
				temp = curr;
				curr = curr->GetNext();
				delete temp;
				temp = NULL;
			}
			this->count = 0;
		}
		else
			this->count = 0;
	}
	void operator=(const SLinkedList<T>& copy) {
		this->clear();
		this->head = NULL;
		this->count = 0;
		Node<T>* copyHead = copy.head;
		while (copyHead != NULL) {
			this->insert(copyHead->data);
			copyHead = copyHead->GetNext();
		}
	}
	
};