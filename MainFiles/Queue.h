#include <iostream>
using namespace std;

template< typename T>
class QueueNode {
public:
	T data;
	QueueNode<T>* next;

	QueueNode<T>() {
		this->data = 0;
		this->next = NULL;
	}
	QueueNode<T>(T data, QueueNode<T>* next = NULL) {
		this->data = data;
		this->next = next;
	}
	~QueueNode<T>() {
		this->next = NULL;
	}
};
template< typename T >
class Queue {
public:
	QueueNode<T>* front;
	QueueNode<T>* rear;
	int num;

	Queue<T>() {
		this->front = this->rear = NULL;
		this->num = 0;
	}
	Queue<T>(T data) {
		this->front = new QueueNode<T>(data);
		this->front = this->rear;
		this->num = 1;
	}
	~Queue<T>() {
		this->clear();
	}
	bool is_empty() const {
		if (this->front && this->rear)
			return false;
		else
			return true;
	}
	void clear() {
		if (!this->is_empty()) {
			QueueNode<T>* curr = this->rear;
			while (curr) {
				this->rear = curr->next;
				delete curr;
				curr = this->rear;
			}
			this->rear = this->front = NULL;
			this->num = 0;
		}
	}
	int size() const {
		return this->num;
	}
	void enqueue(T data) {
		QueueNode<T>* newNode = new QueueNode<T>(data);
		++this->num;
		if (!this->is_empty()) {
			newNode->next = this->rear;
			this->rear = newNode;
		}
		else {
			this->rear = this->front = newNode;
		}
	}
	T dequeue() {
		if (!this->is_empty()) {
			T data;
			if (this->rear == this->front) {
				data = this->rear->data;
				delete this->rear;
				this->rear = this->front = NULL;
				this->num = 0;
				return data;

			}
			QueueNode<T>* prev = this->rear;
			while (prev->next != this->front) {
				prev = prev->next;
			}
			data = this->front->data;
			delete this->front;
			this->front = prev;
			this->front->next = NULL;
			--this->num;
			return data;
		}
		else {
			cout << "Queue is Empty. Returning 0" << endl;
			T temp=0;
			return temp;
		}
	}
	T peek() const {
		if (!this->is_empty())
			return this->front->data;
		else {
			cout << "Queue is Empty. Returning 0" << endl;
			T temp=0;
			return temp;
		}
	}
};