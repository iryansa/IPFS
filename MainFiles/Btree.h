#include <iostream>
//#include"DHTNode.h"
#include "linklist.h"
#include "BigInt.h"
#include "Queue.h"
#include<sstream>
#include<fstream>

using namespace std;
using namespace std;
#define MAX 4
#define MIN 2

struct keypair {
	BigInt hash;
	string filepath;
	keypair(string hash1, string path1 = "\0") {
		this->hash = hash1;
		this->filepath = path1;
	}
	keypair(BigInt hash, string path = "\0") {
		this->hash = hash;
		this->filepath = path;
	}
	keypair() {
		this->hash;
		this->filepath = "\0";
	}

};
class btnode {

public:
	int count;
	/*int MAX;
	int MIN;*/
	//int value[MAX + 1];
	SLinkedList<keypair>value[MAX + 1];
	btnode* child[MAX + 1]{};
	int noOfChilds();
};
int btnode::noOfChilds() {
	return MAX + 1;
}

class Btree
{
public:
	btnode* root = NULL;
	btnode* insert(keypair val, btnode* r);
	void insert(keypair val);
	int setval(keypair val, btnode* n, keypair* p, btnode** c);
	int searchnode(keypair val, btnode* n, int* pos, bool specialCase = true);
	int searchnode2(keypair val, btnode* n, int* pos, keypair& deleted, bool& flag1, bool specialCase = true);
	int searchnode3(keypair val, btnode* n, int* pos);
	void deletedata(BigInt hash, keypair& deleted);
	void fillnode(keypair val, btnode* c, btnode* n, int k);
	void split(keypair val, btnode* c, btnode* n, int k, keypair* y, btnode** newnode);
	btnode* del(BigInt val, btnode* r, keypair& deleted);
	int delhelp(keypair val, btnode* r, keypair& deleted, bool& flag1);
	void clear(btnode* node, int k);
	void copysucc(btnode* node, int i);
	void restore(btnode* node, int i);
	void rightshift(btnode* node, int k);
	void leftshift(btnode* node, int k);
	void merge(btnode* node, int k);
	btnode* search(keypair val, btnode* r, int* pos);
	bool search(BigInt fileHash);
	void display(btnode* r);
};
void Btree::deletedata(BigInt hash, keypair& deleted) {
	root = del(hash, root, deleted);
}

void Btree::insert(keypair val) {
	this->root = this->insert(val, this->root);
}
/*inserts a value in the B - tree */
btnode* Btree::insert(keypair val, btnode* root)
{
	keypair i;
	btnode* c, * n;
	int flag;
	flag = setval(val, root, &i, &c);
	if (flag)
	{
		n = new btnode;
		n->count = 1;
		n->value[1].insert(i);
		n->child[0] = root;
		n->child[1] = c;
		return n;
	}
	return root;
}
/*I sets the value in the node*/
int Btree::setval(keypair val, btnode* n, keypair* p, btnode** c)
{
	int k;
	if (n == NULL)
	{
		*p = val;
		*c = NULL;
		return 1;
	}
	else
	{
		if (searchnode(val, n, &k)) {
			cout << "Key value already exists. \n";
			return 0;
		}
		if (setval(val, n->child[k], p, c))
		{
			if (n->count < MAX)
			{
				fillnode(*p, *c, n, k);
				return 0;
			}
			else
			{
				split(*p, *c, n, k, p, c);
				return 1;
			}
		}
		return 0;
	}
}

btnode* Btree::search(keypair val,btnode* root, int* pos) {
	if (root == NULL)
		return NULL;
	else
	{
		if (searchnode3(val, root, pos))
			return root;
		else
			return search(val, root->child[*pos], pos);
	}
}
bool Btree::search(BigInt fileHash) {
	int pos;
	if (this->search(keypair(fileHash), this->root, &pos))
		return true;
	else
		return false;
}
/*searches for the node */
int Btree::searchnode(keypair val, btnode* n, int* pos, bool specialCase)
{
	if (val.hash < n->value[1].getdata().hash)
	{
		*pos = 0;
		return 0;
	}
	else
	{
		*pos = n->count;
		while ((val.hash < n->value[*pos].getdata().hash) && *pos > 1)
			(*pos)--;

		if (val.hash == n->value[*pos].getdata().hash) {

			if (specialCase)
				n->value[*pos].insert(val);

			return 1;
		}
		else
			return 0;
	}
}
int Btree::searchnode2(keypair val, btnode* n, int* pos, keypair& deleted, bool& flag1, bool specialCase)
{
	if (val.hash < n->value[1].getdata().hash)
	{
		*pos = 0;
		return 0;
	}
	else
	{
		*pos = n->count;
		while ((val.hash < n->value[*pos].getdata().hash) && *pos > 1)
			(*pos)--;

		if (val.hash == n->value[*pos].getdata().hash) {
			if (deleted.filepath == "\0")
			{
				if (n->value[*pos].getCount() <= 1)
				{
					deleted = n->value[*pos].getdata();
					return 1;
				}
				else {
					n->value[*pos].specialPrint();
					int index = -1;
					cin >> index;
					deleted = n->value[*pos].deleteAt(index - 1);
					flag1 = true;
					return 0;
				}

			}

			return 1;
		}
		else
			return 0;
	}
}

int Btree::searchnode3(keypair val, btnode* n, int* pos)
{
	if (val.hash < n->value[1].getdata().hash)
	{
		*pos = 0;
		return 0;
	}
	else
	{
		*pos = n->count;
		while ((val.hash < n->value[*pos].getdata().hash) && *pos > 1)
			(*pos)--;

		if (val.hash == n->value[*pos].getdata().hash) {
				n->value[*pos].print();
			return 1;
		}
		else
			return 0;
	}
}

void Btree::fillnode(keypair val, btnode* c, btnode* n, int k)
{
	int i;
	for (i = n->count; i > k; i--)
	{
		n->value[i + 1] = n->value[i];
		n->child[i + 1] = n->child[i];
	}
	n->value[k + 1].insert(val);
	n->child[k + 1] = c;
	n->count++;
}
/*splits the node*/
void Btree::split(keypair val, btnode* c, btnode* n, int k, keypair* y, btnode** newnode)
{
	int i, mid;

	if (k <= MIN)
		mid = MIN;
	else
		mid = MIN + 1;

	*newnode = new btnode;
	for (i = mid + 1; i <= MAX; i++)
	{
		(*newnode)->value[i - mid] = n->value[i];
		(*newnode)->child[i - mid] = n->child[i];
	}
	(*newnode)->count = MAX - mid;
	n->count = mid;
	if (k <= MIN)
		fillnode(val, c, n, k);
	else
		fillnode(val, c, *newnode, k - mid);

	*y = n->value[n->count].getdata();
	(*newnode)->child[0] = n->child[n->count];
	n->count--;
}
/*deletes value from the node*/

btnode* Btree::del(BigInt val, btnode* root, keypair& deleted)
{
	bool flag1=false;
	btnode* temp;
	keypair val1(val);
	if (!delhelp(val1, root, deleted,flag1))
	{
		if (flag1 == false) {
			cout << "\n";
			cout << "Value not found.\n" << val;
		}
	}
	else
	{
		if (root->count == 0)
		{
			temp = root;
			root = root->child[0];
			delete temp;
		}
	}
	return root;
}
/* helper function for del() */

int Btree::delhelp(keypair val, btnode* root, keypair& deleted,bool& flag1)
{
	int i;
	int flag;
	if (root == NULL)
		return 0;
	else
	{
		flag = searchnode2(val, root, &i, deleted,flag1, false );
		if (flag1 == true)
			return 0;
		/*if (flag && deleted.filepath == "\0" && deleted.hash == "\0")
			deleted = root->value[i].getdata();*/
		if (flag)
		{
			if (root->child[i - 1])
			{
				copysucc(root, i);
				flag = delhelp(root->value[i].getdata(), root->child[i], deleted,flag1);
				if (!flag)
				{
					cout << "\n";
					cout << "Hash " << val.hash << " not found. \n";
				}
			}
			else {
				// if there is only one value present
				//if( root->value[i].getCount() <= 1 || deleted.filepath != "\0" || deleted.hash != "\0" )
				clear(root, i);
				/*else
				{
					root->value[i].specialPrint();
					int index = -1;
					cin >> index;

					deleted = root->value[i].deleteAt(index - 1);


				}*/
				// if there are multiple values present
			}
		}
		else
			flag = delhelp(val, root->child[i], deleted,flag1);

		if (root->child[i] != NULL)
		{
			if (root->child[i]->count < MIN)
				restore(root, i);
		}
		return flag;
	}
}
/*removes the value from the node and adjusts the values* I*/
void Btree::clear(btnode* node, int k)
{
	int i;
	for (i = k + 1; i <= node->count; i++)
	{
		node->value[i - 1] = node->value[i];
		node->child[i - 1] = node->child[i];
	}
	node->count--;
}
/*copies the successor of the value that is to be deleted */
void Btree::copysucc(btnode* node, int i)
{
	btnode* temp;
	temp = node->child[i];
	while (temp->child[0])
	{
		temp = temp->child[0];
	}
	node->value[i] = temp->value[1];
}
/*adjusts the node */
void Btree::restore(btnode* node, int i)
{
	if (i == 0)
	{
		if (node->child[1]->count > MIN)
			leftshift(node, 1);
		else
			merge(node, 1);
	}
	else
	{
		if (i == node->count)
		{
			if (node->child[i - 1]->count > MIN)
				rightshift(node, i);
			else
				merge(node, i);
		}
		else
		{
			if (node->child[i - 1]->count > MIN)
				rightshift(node, i);
			else
			{
				if (node->child[i + 1]->count > MIN)
					leftshift(node, i + 1);
				else
					merge(node, i);
			}
		}
	}
}
/*adjusts the values and ohildren while shitng te valoe fiom parent to Pertchild */
void Btree::rightshift(btnode* node, int k)
{
	int i;
	btnode* temp;
	temp = node->child[k];

	for (i = temp->count; i > 0; i--)
	{
		temp->value[i + 1] = temp->value[i];
		temp->child[i + 1] = temp->child[i];
	}
	temp->child[1] = temp->child[0];
	temp->count++;
	temp->value[1] = node->value[k];
	temp = node->child[k - 1];
	node->value[k] = temp->value[temp->count];
	node->child[k]->child[0] = temp->child[temp->count];
	temp->count--;
}

void Btree::leftshift(btnode* node, int k)
{
	int i;
	btnode* temp;

	temp = node->child[k - 1];
	temp->count++;
	temp->value[temp->count] = node->value[k];
	temp->child[temp->count] = node->child[k]->child[0];
	temp = node->child[k];
	node->value[k] = temp->value[1];
	temp->child[0] = temp->child[1];
	temp->count--;
	for (i = 1; i <= temp->count; i++)
	{
		temp->value[i] = temp->value[i + 1];
		temp->child[i] = temp->child[i + 1];
	}
}
/*merges two nodes*/
void Btree::merge(btnode* node, int k)
{
	int i;
	btnode* temp1, * temp2;
	temp1 = node->child[k];
	temp2 = node->child[k - 1];
	temp2->count++;
	temp2->value[temp2->count] = node->value[k];

	if (temp2->child[0] == NULL)
		temp2->child[temp2->count] = temp1->child[0];

	for (i = 1; i <= temp1->count; i++)
	{
		temp2->count++;
		temp2->value[temp2->count] = temp1->value[i];
		temp2->child[temp2->count] = temp1->child[i];
	}
	for (i = k; i < node->count; i++)
	{
		node->value[i] = node->value[i + 1];
		node->child[i] = node->child[i + 1];
	}
	node->count--;
	delete temp1;
}

/*displays the B - tree*/
void Btree::display(btnode* root)
{
	int i;
	if (root != NULL)
	{
		for (i = 0; i < root->count; i++)
		{
			display(root->child[i]);
			cout << root->value[i + 1].getdata().hash << "\t" << root->value[i + 1].getdata().filepath;
		}
		display(root->child[i]);
	}
}

void displayBFT(btnode* root) {
	if (root == nullptr)
		return;

	Queue<btnode*> q;
	q.enqueue(root);
	int count = 1;
	while (!q.is_empty()) {

		btnode* current = q.peek();
		q.dequeue();
		cout << "level: " << count << "\t";
		count++;
		// Display the values in the current node
		for (int i = 0; i < current->count; i++) {
			//cout << current->value[i + 1].getdata().hash << "\t"<< current->value[i + 1].getdata().filepath<<"\t"; // Assuming 1-indexed value array
			current->value[i + 1].print();
		}
		cout << endl; // New line after each node's values

		// Enqueue child nodes of the current node
		for (int i = 0; i <= current->count; i++) {
			if (current->child[i] != nullptr) {
				q.enqueue(current->child[i]);
			}
		}
	}
}
