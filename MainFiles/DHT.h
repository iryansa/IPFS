#pragma once
#include<iostream>
#include"DHTNode.h"
#include<queue>
#include<sstream>
#include<fstream>

// including library for hashing
#include"sha1.h"

using namespace std;
void visualizeTree(const string& dotCode);
string generatecircular(DHTNode* head);
// implementation of DHT class using Circular Linked List ADT
class CircularLinkedList {
private:
	char assigneeLetter = 'A';			// use assigneeLetter and assigneeCounter to
	int assigneeCounter = 0;			// generate Assignee IDs for each new insertion
	// generates a new name for the automatic insertion of a folder
	string generateID() {
		(++this->assigneeCounter) % 1000;
		if (this->assigneeCounter == 0)			// update the starting letter if 1000 
			++this->assigneeLetter;				// assignee ID's start with the current letter
		string assigneeID = "\0";
		assigneeID += this->assigneeLetter;
		assigneeID += "000";
		int temp1 = this->assigneeCounter, temp2 = 0, count = 3;
		while (temp1) {
			temp2 = temp1 % 10;
			temp1 /= 10;
			assigneeID[count] = temp2 + 48;
			--count;
		}
		return assigneeID;
	}
public:
	
	DHTNode* head;
	DHTNode* tail;
	BigInt machineCount, maxNoOfMachines, identifierSpace;

	
	CircularLinkedList(BigInt identifierSpace) {
		head = NULL;
		tail = NULL;
		this->identifierSpace = identifierSpace;
		BigInt base("2");
		this->maxNoOfMachines = this->maxNoOfMachines.power(base, identifierSpace);
	}
	// Defualt Constructor
	CircularLinkedList() {
		this->head = NULL;
		this->tail = NULL;
		string IS;

		cout << " ENTER THE INDENTIFIER SPACE IN BITS ." << endl;
		cout <<"It must be in range of 1 to 160" << endl;
		cin >> IS;
		this->identifierSpace = IS;
		BigInt base("2");
		this->maxNoOfMachines = this->maxNoOfMachines.power(base, identifierSpace);
	}
	// destructor
	~CircularLinkedList() {
		if (this->head)
			this->clear();
	}
	void clear() {
		DHTNode* curr = this->head;
		if (curr) {
			this->tail->next = NULL;
			DHTNode* temp = NULL;
			while (curr != NULL) {
				temp = curr;
				curr = curr->next;
				temp->next = NULL;
				delete temp;
				temp = NULL;
			}
		}
	}

	bool isEmpty()
	{
		if (head == NULL)
			return true;
		else
			return false;
	}

	DHTNode* getHead()
	{
		return head;
	}

	bool validateFilePath(string& filePath) {
		if (filePath[0] == '"') {
			if (filePath[filePath.length() - 1] == '"')
				filePath = filePath.substr(1, filePath.length() - 2);
			else
				filePath = filePath.substr(1, filePath.length() - 1);
		}
		
		for (int i = 0; i < filePath.length(); ++i) {
			if (filePath[i] == ' ') {
				cout << "File Path contains space which is invalid\n";
				return false;
			}
			else if (filePath[i] == '\\') {
				if (i < filePath.length() - 1) {
					if (filePath[i + 1] == '\\')
						++i;
					else
						filePath[i] = '/';
				}
				else
					filePath[i] = '/';
			}
		}
		return true;
	}


	DHTNode* getMachineNode(BigInt& machineHash) {
		
		// if there is a single machine in the system
		BigInt count;
		if (this->machineCount == BigInt(1) ) {
			if (this->head->machineHash == machineHash)
				return this->head;
			else
				return NULL;
		}
		// if the machineHash is greater than the biggest machineHash present in the system 
		if (this->tail->machineHash < machineHash) 
			return NULL;
		// if the machineHash corresponds to the machine being pointed by the tail
		if (this->tail->machineHash == machineHash)
			return this->tail;
		// return relevant DHTNode* or NULL 
		DHTNode* curr = this->head;
		while (true && count < this->machineCount) {
			count = count.decimalAddition(1);
			if (curr->machineHash == machineHash)
				return curr;
			else if (curr->machineHash > machineHash)
				return NULL;
			else if (curr->next->machineHash == machineHash)
				return curr->next;
			else if (curr->machineHash < machineHash && curr->next->machineHash > machineHash)
				return NULL;
			else
				curr = curr->getMachineNodeAccordingToRT(machineHash);
		}
		return NULL;
	}


	// insert file into the relevant machine
	// first find the machine to start the insertion process from
	// use the insertion point, to navigate to the machine at which to insert the file at
	void insertFile() {
		cout << "Initiating the File Insertion Process" << endl;
		int inputCheck = 0;
		// prompt user from where to start the insertion process from
		// relevant input validation
		do {
			cout << "From which machine would you like to start the insertion process from:\n";
			cout << "Enter 1 to provide machine name\n";
			cout << "Enter 2 to provide machine hash\n";
			cout << "Enter 3 to start the insertion automatically from the head\n";
			cin >> inputCheck;
		} while ( inputCheck < 1 || inputCheck > 3);
		
		string macHash;
		BigInt machineHash;
		// points to the machine from which to start the insertion process from
		string* machineName = NULL;
		DHTNode* startPoint = NULL;
		if (inputCheck == 1) {
			machineName = new string;
			cout << "Enter the Machine's Name from which to start the insertion process from: ";
			cin >> (*machineName);
			this->generateMachineHash(*machineName, machineHash);
			startPoint = this->getMachineNode(machineHash);
		}
		// start the insertion process from the machine having the input hash
		else if (inputCheck == 2) {
			cout << "Enter the Machine's Hash from which to start the insertion process from: ";
			cin >> macHash;
			machineHash = macHash;
			// validating provided machine hash
			machineHash = machineHash.modulus(machineHash, this->identifierSpace);
			startPoint = this->getMachineNode(machineHash);
		}
		// start the insertion process from head
		else 
			startPoint = this->head;
		// if no relevant machine has been found
		if (startPoint == NULL) {
			if (this->head == NULL)
				cout << "No Machine in the System\n";
			else if (inputCheck == 1) {
				cout << "No Machine in the System having name: " << *machineName << endl;
				delete machineName;
				machineName = NULL;
			}
			else
				cout << "No Machine in the System having hash: " << machineHash << endl;
		}
		// if the relevant machine is found
		else {
			// clear up redundant memory
			if (machineName) {
				delete machineName;
				machineName = NULL;
			}
			cout << "The Machine having hash: " << startPoint->machineHash << " has been found\n\n";
			cout << "Enter the path of the file which you wish to insert in the system:\n";
			cout << "NOTE: \n";
			cout << "\t1.The file path must not contain \\ and space.\n";
			cout<<"\t2.It should either be replaced with / or \\\\.\n";
			cout << "\t3.Failure to do so will result in error in file copying process\n\n";
			string filePath;
			cout << "Enter File Path: ";
			cin >> filePath;
			if (this->validateFilePath(filePath) == false) {
				cout << "\nIncorrect File Path Provided. Stopping file insertion process\n";
				return;
			}
			BigInt fileHash;
			//this->generateFileHash(filePath, fileHash);
			// generating file hash
			if (this->generateFileHash(filePath, fileHash)) {
				startPoint = this->findMachineNodeAccordingToFileHash(fileHash, startPoint);
				// no machine found to insert the machine at
				if (startPoint == NULL) {
					cout << "Error in machine search. No relevant machine to insert the file";
					cout << "having filePath: " << filePath << " and hash: " << fileHash << endl;
					cout << "\nDiscontinuing File Insertion Process" << endl;
				}
				else {
					cout << "The file will be inserted at the machine: " << startPoint->machineName << endl;
					cout << "having machine hash: " << startPoint->machineHash << endl;
					// inserting file in the found machine
					if (startPoint->insertFile(keypair(fileHash, filePath))) {
						cout << "File Insertion is Succesful\n";
						cout << "File having path: " << filePath << " and hash: " << fileHash << endl;
						cout << "has been inserted at machine: " << startPoint->machineName << " having hash: " << startPoint->machineHash << endl;
					}
					// file insertion error
					else
						cout << "File Insertion Failed\n";
				}
			}
			// no file hash generated
			else 
				cout << "Insertion Process discontinued\n";
		}
		//cout << "Enter Machine Hash: ";
		//string macHash;
		//cin >> macHash;
		//BigInt machineHash(macHash);
		//DHTNode* startPoint= this->getMachineNode(machineHash);
		//if (startPoint == NULL) {
		//	cout << "There is no machine in the system having hash: " << machineHash << endl;
		//	return;
		//} 
		//cout << "The machine having hash " << startPoint->machineHash << " has been found" << endl;
		//
		//cout << "Enter the file hash: ";
		//cin >> macHash;
		//machineHash = macHash;
		//startPoint = this->findMachineNodeAccordingToFileHash(machineHash, startPoint);
		//if (startPoint == NULL) {
		//	cout << "Machine Searching Failure. Returning back\n";
		//	return;
		//}
		//cout << "The file having hash: " << machineHash << " will be inserted at machine having: " << startPoint->machineHash << endl;
	}
	void deleteFile() {
		cout << "Initiating the File Deletion Process" << endl;
		int inputCheck = 0;
		// prompt user from where to start the insertion process from
		// relevant input validation
		do {
			cout << "From which machine would you like to start the deletion process from:\n";
			cout << "Enter 1 to provide machine name\n";
			cout << "Enter 2 to provide machine hash\n";
			cout << "Enter 3 to start the insertion automatically from the head\n";
			cin >> inputCheck;
		} while (inputCheck < 1 || inputCheck > 3);

		string macHash;
		BigInt machineHash;
		// points to the machine from which to start the insertion process from
		string* machineName = NULL;
		DHTNode* startPoint = NULL;
		if (inputCheck == 1) {
			machineName = new string;
			cout << "Enter the Machine's Name from which to start the deletion process from: ";
			cin >> (*machineName);
			this->generateMachineHash(*machineName, machineHash);
			startPoint = this->getMachineNode(machineHash);
		}
		// start the insertion process from the machine having the input hash
		else if (inputCheck == 2) {
			cout << "Enter the Machine's Hash from which to start the deletion process from: ";
			cin >> macHash;
			machineHash = macHash;
			// validating provided machine hash
			machineHash = machineHash.modulus(machineHash, this->identifierSpace);
			startPoint = this->getMachineNode(machineHash);
		}
		// start the insertion process from head
		else
			startPoint = this->head;
		// if no relevant machine has been found
		if (startPoint == NULL) {
			if (this->head == NULL)
				cout << "No Machine in the System\n";
			else if (inputCheck == 1) {
				cout << "No Machine in the System having name: " << *machineName << endl;
				delete machineName;
				machineName = NULL;
			}
			else
				cout << "No Machine in the System having hash: " << machineHash << endl;
		}
		// if the relevant machine is found
		else {
			// clear up redundant memory
			if (machineName) {
				delete machineName;
				machineName = NULL;
			}
			cout << "The Machine having hash: " << startPoint->machineHash << " has been found\n\n";
			cout << "Enter the hash of the file which you wish to delete from the system:\n";
			string filHash;
			cout << "Enter File Hash: ";
			cin >> filHash;
			cout <<endl;
			BigInt fileHash;
			fileHash = filHash;
			fileHash = fileHash.modulus(fileHash, this->identifierSpace);

			startPoint = this->findMachineNodeAccordingToFileHash(fileHash, startPoint);
			// no machine found to delete the machine at
			if (startPoint == NULL) {
				cout << "Error in machine search. No relevant machine to delete the file ";
				cout << "having file hash: " << fileHash << endl;
				cout << "Discontinuing File Deletion Process" << endl;
			}
			else {
				cout << "The file will be deleted from the machine: " << startPoint->machineName << endl;
				cout << "having machine hash: " << startPoint->machineHash << endl;
				// deleting file in the found machine
				if (startPoint->deleteFile(fileHash)) {
					cout << "File Deletion Succesful";
					cout << "File having hash: " << fileHash << endl;
					cout << "has been deleted from the machine: " << startPoint->machineName << " having hash: " << startPoint->machineHash << endl;
				}
				// file deletion error
				else
					cout << "File Deletion Failed\n";
			}
		}
	}
	// returns the DHTNode* at which to insert the file having the corresponding fileHash
	DHTNode* findMachineNodeAccordingToFileHash(BigInt& fileHash, DHTNode* startPoint) {
		cout <<"hello"<< endl << endl;
		if (startPoint) {
			if (this->tail->machineHash == fileHash) {
				cout << startPoint->machineHash << " -> " << this->tail->machineHash<<endl;
				return this->tail;
			}
			else if (this->tail->machineHash < fileHash || this->head->machineHash > fileHash) {
				cout << startPoint->machineHash << " -> " << this->head->machineHash << endl;
				return this->head;
			}
			else if (startPoint->machineHash == fileHash)
			{
				cout << startPoint->machineHash << endl;
				return startPoint;
			}
			else {
				cout << startPoint->machineHash<<"->";
				BigInt count;
				if (startPoint->machineHash < fileHash) {
					while (true && count < this->machineCount) {
						count = count.decimalAddition(1);
						if (startPoint->machineHash == fileHash || startPoint->machineHash > fileHash) {
							cout << startPoint->machineHash << endl;
							return startPoint;
						}
						//else if (startPo->machineHash > Hash)
						//	return NULL;
						else if (startPoint->machineHash < fileHash && startPoint->next->machineHash > fileHash) {
							cout << startPoint->next->machineHash << endl;
							return startPoint->next;
						}
						else {
							cout << startPoint->machineHash << " -> ";
							startPoint = startPoint->getMachineNodeAccordingToRT(fileHash);
						}
					}
					return NULL;
				}
				// file is to be inserted in-between head and startPoint
				else {

					startPoint = this->head;
					cout << startPoint->machineHash << "->";
					while (true && count < this->machineCount) {
						count = count.decimalAddition(1);
						if (startPoint->machineHash == fileHash || startPoint->machineHash > fileHash) {
							cout << startPoint->machineHash << endl;
							return startPoint;
					}
						else if (startPoint->machineHash < fileHash && startPoint->next->machineHash > fileHash) {
							cout << startPoint->next->machineHash << endl;
							return startPoint->next;
						}
						else {
							cout << startPoint->machineHash << " -> ";
							startPoint = startPoint->getMachineNodeAccordingToRT(fileHash);
						}
					}
					return NULL;
				}
			}
		}
		else {
			cout << "No machine to start the insertion process from" << endl;
			return NULL;
		}
	}

	// insert machine automatically using predefined naming convention
	void insertMachineAutomatically() {
		string machineName = this->generateID();
		BigInt machineHash;
		this->generateMachineHash(machineName, machineHash);
		this->addNode(machineName, machineHash);
	}

	// insert machine on the hash generated by the machine name provided by user
	void insertMachineManually() {
		cout << "Entering Machine Manually. Enter machine name: ";
		string machineName;
		cin >> machineName;
		BigInt machineHash;
		this->generateMachineHash(machineName, machineHash);
		this->addNode(machineName, machineHash);
		this->recalculateRT();
	}

	void DeleteMachineByName() {
		string Mname;
		cout << "Enter the machine name which you want to delete " << endl;
		cin >> Mname;
		BigInt deletedhash;
		this->generateMachineHash(Mname, deletedhash);
		this->deleteNode(deletedhash);
	}

	void DeleteMachineByHash() {
		cout << "Enter the Machine Hash you want to be deleted" << endl;
		string deletedHash;
		cin >> deletedHash;

		BigInt B1(deletedHash);

		this->deleteNode(B1);
	}
	// generates hash based upon the machineName provided
	void generateMachineHash(const string machineName, BigInt& machineHash) {
		
		SHA1 checkSum;
		checkSum.update(machineName);
		const string hash = checkSum.final();
		cout << "The hash generated with the machine name: " << machineName << " is: ";
		machineHash = hash;
		machineHash = machineHash.modulus(machineHash, this->identifierSpace);
		cout << machineHash << endl;


	}

	// generates hash based upon the content of the file provided 
	bool generateFileHash(const string filePath, BigInt& fileHash) {
		SHA1 checkSum;
		const string hash = checkSum.from_file(filePath);
		if (hash == "\0") {
			cout << "Error in generating file hash. Discontinuing file insertion process\n";
			return false;
		}
		else {
			fileHash = hash;
			fileHash = fileHash.modulus(fileHash, this->identifierSpace);
			cout << "The hash of the file having path: " << filePath << " is: " << fileHash;
			return true;
		}
		return false;
	}
	
	void addNode(string machineName, BigInt& machineHash) {
		if (this->machineCount == this->maxNoOfMachines) {
			cout << "System is Full. Machine has not been inserted\n";
			return;
		}
		this->machineCount = this->machineCount.decimalAddition(1);
		DHTNode* newNode = new DHTNode(machineName, machineHash, identifierSpace);
		if (head == nullptr) {
			head = newNode;
			tail = newNode;
			newNode->next = head;
		//	newNode->prev = head;
		}
		else {
			DHTNode* current = head;
			while (current->next != head && current->next->machineHash < machineHash) {
				if (newNode->machineHash == current->machineHash)
				{
					this->machineCount = this->machineCount.decimalSubtraction(1);
					delete newNode;
					//this->machineCount = this->machineCount.decimalSubtraction(1);
					cout << "duplicates machine cannot be inserted " << endl;
					return;
				}
				current = current->next;
			}
			
			if (current->next == head && current->next->machineHash > machineHash) {
				newNode->next = head;
				//newNode->prev = tail;
				//head->prev = newNode;
				tail->next = newNode;
				head = newNode;
			}
			else if (current == head && current->machineHash > newNode->machineHash) {
				newNode->next = head;
				//newNode->prev = tail;
				//head->prev = newNode;
				tail->next = newNode;
				head = newNode;
			}
			else {
				newNode->next = current->next;
				//newNode->prev = current;
				//current->next->prev = newNode;
				current->next = newNode;
				if (newNode->next == head) {
					tail = newNode;
				}
			}
			newNode->splitTrees();
			recalculateRT();
		}

		/*DHTNode* newNode = new DHTNode(machineName, machineHash);
		newNode->machineHash = machineHash;
		if (head == nullptr) {
			head = newNode;
			tail = newNode;
			newNode->next = head;
		}
		else {
			DHTNode* current = head;
			while (current->next != head && current->next->machineHash < machineHash) {
				current = current->next;
			}*/
			
			// doubly commented
			/*if (newNode->machineHash == current->machineHash)
			{
				delete newNode;
				this->machineCount = this->machineCount.decimalSubtraction(1);
				cout << "duplicates machine cannot be inserted " << endl;
				return;
			}
			if (current->next == this->head) {
				if (current->machineHash < newNode->machineHash) {
					current->next = newNode;
					newNode->next = head;
					tail = newNode;
				}
				else {
					this->head = newNode;
					newNode->next = current;
					current->next = newNode;
				}
			}*/
			//

			/*if (current->next == head && current->next->machineHash > machineHash) {
				newNode->next = head;
				head = newNode;
				tail->next = newNode;
			}
			else if (current == head && current->machineHash > newNode->machineHash) {
				newNode->next = head;
				head = newNode;
				tail->next = newNode;
			}
			else {
				newNode->next = current->next;
				current->next = newNode;
				if (newNode->next == head) {
					tail = newNode;
				}
			}
		}*/
	}

	void recalculateRT() {
		DHTNode* curr = head;
		if (curr != NULL) {
			curr->AdjustRT();
		}
		else {
			return;
		}
		curr=curr->next;
		while (curr != head) {
			curr->AdjustRT();
			curr = curr->next;
		}

	}

	void displayBT() {
		cout << "ENTER MACHINE HASH FOR WHICH YOU WANT BTREE " << endl;
		string hash;
		cin >> hash;
		BigInt hash1(hash);
		DHTNode* curr = getMachineNode(hash1);
		if (curr == NULL) {
			cout << "NO MACHINE EXIST WITH FOLLOWING HASH " << endl;
		}
		else
		{
			curr->displayBT();
		}
	}

	void displayRT() {

		int choice1;
		do {
			cout << "1. PRINT ROUTING TABLE OF ALL MACHINES " << endl;
			cout << "2. PRINT ROUTING TABLE FOR SPECIFIC MACHINE " << endl;
			cin >>choice1;
		} while (choice1 < 1 || choice1 > 2);

		if (choice1 == 1) {
			DHTNode* curr = head;
			if (curr != NULL) {
				cout << "This is RT of machine having hash " << curr->machineHash << " and name: " << curr->machineName << endl;
				curr->head.displayRT();
			}
			else {
				return;
			}
			curr = curr->next;
			while (curr != head) {
				cout << "This is RT of machine having hash " << curr->machineHash << " and name: " << curr->machineName << endl;
				curr->head.displayRT();
				curr = curr->next;
			}
		}
		else {
			string hash;
			//visualizeTree(generatecircular(head));
			visualizeTree(generatecircular(this->head));
			cout << "ENTER MACHINE HASH FROM FOLLOWING MACHINES "<<endl;
			cin >> hash;
			BigInt hash1(hash);
			DHTNode* curr= getMachineNode(hash1);
			if (curr == NULL) {
				cout << "NO MACHINE EXIST WITH FOLLOWING HASH " << endl;
			}
			else
			{
				curr->head.displayRT();
			}
		}
	}
	//void insertMachine(string machineName, BigInt& machineHash) {
	//	if (machineCount == this->maxNoOfMachines) {
	//		cout << "No more space in the identifier space" << endl;
	//	return;
	//	}
	//	this->machineCount = this->machineCount.decimalAddition(1);
	//	DHTNode* temp = new DHTNode(machineName, machineHash,indeti);
	//	DHTNode* curr = this->head;
	//	if (curr) {
	//		while (curr->next != this->head && temp->machineHash > curr->next->machineHash)
	//			curr = curr->next;

	//		if (temp->machineHash == curr->machineHash)
	//		{
	//			delete temp;
	//			this->machineCount = this->machineCount.decimalSubtraction(1);
	//			cout << "duplicates machine cannot be inserted " << endl;
	//			return;
	//		}
	//		if (curr->next == this->head) {
	//			if (curr->machineHash < temp->machineHash) {
	//				curr->next = temp;
	//				temp->next = head;
	//			}
	//			else {
	//				this->head = temp;
	//				temp->next = curr;
	//				curr->next = temp;
	//			}
	//		}
	//		else {
	//			if (curr == this->head) {
	//				DHTNode* temp2 = curr;
	//				DHTNode* prev = temp2;
	//				while (temp2->next != this->head && temp->machineHash < temp2->next->machineHash) {
	//					prev = temp2;
	//					temp2 = temp2->next;
	//				}
	//				if (temp2->next == this->head) {
	//					temp->next = prev->next;
	//					prev->next = temp;
	//					//this->head = temp;
	//				}
	//				else
	//				{
	//					temp->next = temp2->next;
	//					temp2->next = temp;
	//				}
	//			}
	//			else {
	//				temp->next = curr->next;
	//				curr->next = temp;
	//			}
	//		}
	//	}
	//	else {
	//		this->head = temp;
	//		this->head->next = head;
	//	}
	//	//// create a machine
	//	//DHTNode* temp = new DHTNode(machineName, machineHash);
	//	////temp->data = d;
	//	////temp->next = NULL;

	//	//// insert the machine at the head
	//	//if (head == NULL) {
	//	//	head = temp;
	//	//	head->next = temp;
	//	//	noofmachine++;
	//	//	return;
	//	//}
	//	//// find the position to insert the DHTNode at
	//	//else {

	//	//	DHTNode* p = head;
	//	//	BigInt counter;
	//	//	DHTNode* prev = NULL;
	//	//	do{				
	//	//		prev = p;
	//	//		++counter;
	//	//		p = p->next;
	//	//	} while (p != head && temp->machineHash > p->machineHash);

	//	//		this->insertAtIndex(temp, counter);

	//	//	//p->next = temp;
	//	//	//temp->next = head;
	//	//	return;
	//	//}
	//}

	/*bool search(BigInt d) {
		DHTNode* p = head;
		BigInt num = p->machineHash;

		while (p->machineHash != d)
		{
			p = p->next;
			if (p->machineHash == num)
				return false;
		}

		return true;
	}*/

	/*void update(int c, int d)
	{
		if (search(c))
		{

			DHTNode* p = head;
			int num = p->data;

			while (p->data != c)
			{
				p = p->next;
				if (p->data == num)
					return;
			}

			p->data = d;

			return;
		}
		else
		{
			return;
		}
	}*/

	void insertAtIndex(DHTNode* nodeToAdd, BigInt& index) {
		DHTNode* temp = nodeToAdd;
		//temp->data = d;
		temp->next = NULL;

		if (index == "0") {
			temp->next = head;
			head = temp;
			return;
		}
		else {
			DHTNode* p = head;
			DHTNode* q = head;

			for (BigInt i; i < index; i++) {
				p = q;
				q = q->next;
			}
			p->next = temp;
			temp->next = q;
			return;
		}
	}


	/*void deleteData(BigInt d)
	{
		if (search(d))
		{
			this->machineCount = this->machineCount.decimalSubtraction(1);
			if (head->machineHash == d)
			{
				DHTNode* p = head;

				while (true)
				{
					if (p->next->machineHash == head->machineHash)
						break;

					p = p->next;

				}

				p->next = head->next;
				DHTNode* temp = head;
				head = head->next;
				delete temp;

				return;
			}

			DHTNode* p = head;

			while (true)
			{
				if (p->next->machineHash == d)
				{
					DHTNode* temp = p->next;
					p->next = p->next->next;
					delete temp;
					return;
				}
				p = p->next;
				if (p->machineHash == head->machineHash)
					return;
			}
		}
	}*/

	void deleteNode(BigInt val) {
		if (head == nullptr) {
			cout << "List is empty" << endl;
			return;
		}
		if (head->machineHash == val) {
			this->machineCount = this->machineCount.decimalSubtraction(1);
			if (head == tail) {
				delete head;
				head = NULL;
				tail = NULL;
			}
			else {
				tail->next = head->next;
				DHTNode* curr = head;
				head = head->next;
				//head->prev = tail;
				delete curr;
				curr = NULL;
			}
			return;
		}
		DHTNode* current = head;
		while (current->next != head && current->next->machineHash != val) {
			current = current->next;
		}
		if (current->next != head) {
			DHTNode* temp = current->next;
			current->next = current->next->next;
			if (temp == tail) {
				tail = current;
			}
			//current->next->prev = current;
			delete temp;
			this->machineCount = this->machineCount.decimalSubtraction(1);
		}
		recalculateRT();
	}
	void print(DHTNode* head)
	{
		DHTNode* p = head;
		int num = p->data;

		while (true)
		{
			cout << p->data << "  ";

			p = p->next;

			if (p->data == num)
			{
				cout << endl;
				return;
			}
		}

	}

	bool searchfile() {
		cout << "Initiating the File Searching Process" << endl;
		int inputCheck = 0;
		// prompt user from where to start the insertion process from
		// relevant input validation
		do {
			cout << "From which machine would you like to start the searching process from:\n";
			cout << "Enter 1 to provide machine name\n";
			cout << "Enter 2 to provide machine hash\n";
			cout << "Enter 3 to start the searching automatically from the head\n";
			cin >> inputCheck;
		} while (inputCheck < 1 || inputCheck > 3);

		string macHash;
		BigInt machineHash;
		// points to the machine from which to start the searching process from
		string* machineName = NULL;
		DHTNode* startPoint = NULL;
		if (inputCheck == 1) {
			machineName = new string;
			cout << "Enter the Machine's Name from which to start the searching process from: ";
			cin >> (*machineName);
			this->generateMachineHash(*machineName, machineHash);
			startPoint = this->getMachineNode(machineHash);
		}
		// start the searching process from the machine having the input hash
		else if (inputCheck == 2) {
			cout << "Enter the Machine's Hash from which to start the searching process from: ";
			cin >> macHash;
			machineHash = macHash;
			// validating provided machine hash
			machineHash = machineHash.modulus(machineHash, this->identifierSpace);
			startPoint = this->getMachineNode(machineHash);
		}
		// start the insertion process from head
		else
			startPoint = this->head;
		// if no relevant machine has been found
		if (startPoint == NULL) {
			if (this->head == NULL)
				cout << "No Machine in the System\n";
			else if (inputCheck == 1) {
				cout << "No Machine in the System having name: " << *machineName << endl;
				delete machineName;
				machineName = NULL;
			}
			else
				cout << "No Machine in the System having hash: " << machineHash << endl;
		}
		// if the relevant machine is found
		else {
			// clear up redundant memory
			if (machineName) {
				delete machineName;
				machineName = NULL;
			}
			cout << "The Machine having hash: " << startPoint->machineHash << " has been found\n";

			cout << "Enter the file hash which you want to search from the system:\n";
			string filHash;
			cin >> filHash;

			BigInt fileHash;
			fileHash = filHash;
			fileHash = fileHash.modulus(fileHash, this->identifierSpace);

			startPoint = this->findMachineNodeAccordingToFileHash(fileHash, startPoint);
			// no machine found to delete the machine at
			if (startPoint == NULL) {
				cout << "Error in machine search. No relevant machine has ";
				cout << " file hash: " << fileHash << endl;
				//cout << "Discontinuing File Deletion Process" << endl;
			}
			else {
				cout << "The file will be searched from the machine: " << startPoint->machineName << endl;
				cout << "having machine hash: " << startPoint->machineHash << endl;
				// deleting file in the found machine
				if (startPoint->SearchFile(fileHash)) {
				
					//cout << "has been deleted from the machine: " << startPoint->machineName << " having hash: " << startPoint->machineHash << endl;
				}
				// file deletion error
				else
					cout << "File search Failed\n";
			}
		}
	}

};
void visualizeTree(const string& dotCode) {
	ofstream dotFile("btree.dot");
	dotFile << dotCode;
	dotFile.close();
	string command = "dot -Tpng btree.dot -o btree.png";
	system(command.c_str());
	system("start btree.png");
}

string generatecircular(DHTNode* head) {
	std::ostringstream dotCode;
	dotCode << "digraph CircularLinkedList {\n";
	dotCode << "layout = circo\n";
	if (head != nullptr) {
		DHTNode* current = head;
		do {
			dotCode << "  \"" << current->machineHash << "\" [label=\"" << current->machineHash << "\", shape=circle];" << std::endl;
			dotCode << "  \"" << current->machineHash << "\" -> \"" << current->next->machineHash << "\";" << std::endl;
			dotCode << "  \"" << current->machineHash << "\" -> \"" << current->next->machineHash << "\"[style=invis];" << std::endl;
			dotCode << "  \"" << current->machineHash << "\" -> \"" << current->next->machineHash << "\"[style=invis];" << std::endl;
			dotCode << "  \"" << current->machineHash << "\" -> \"" << current->next->machineHash << "\"[style=invis];" << std::endl;

			current = current->next;
		} while (current != head);

		// Connect the last node back to the head to form a circle
		//dotCode += std::to_string(head->data) + " [label=\"" + std::to_string(head->data) + "\", shape=circle];\n";
		dotCode << "}\n";
	}
	else {
		dotCode << "}\n";
	}

	return dotCode.str();
}