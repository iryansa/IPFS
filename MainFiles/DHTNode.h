#pragma 
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h> 
#include "Btree.h"
#include "RoutingTable.h"
using namespace std;

// #ifndef DHTNode
// #define DHTNode

class DHTNode {
public:
    RoutingTable<DHTNode> head;
    Btree tree;
    string folderpath;
    string folderName;
    DHTNode* next;
    BigInt machineHash;
    string machineName;
    int data;
    BigInt identifier_space;
    int filecount;
    
    DHTNode(string folderName, BigInt machineHash,BigInt IS, DHTNode* next = NULL);
    ~DHTNode();
    void AdjustRT();
    void AdjustRTAfterDeletion(DHTNode* deleteNode);
   // bool InsertFile(string path, string hash);
    bool insertFile(keypair data);
    //bool InsertFileHelper(string path, BigInt hash, Btree* root);
    bool deleteFile(BigInt hash);
    //bool deletefilehelper(BigInt hash);
   
    void mergeTrees();
    void splitTrees();
    void BFStraversalForMerging(btnode* root);
    void BFStraversalForSpliting(btnode* root);
    void BFStraversalForDestrcutor(btnode* root);
    // bool removeData();

    bool makeFolder(string folderName);
    string fileExtensionExtractor(string path);
    string fileCopy(string path);
    string fileNameWithoutExtension(string path);
    bool fileDeletor(string name);
    bool deleteFolder(string name);
    DHTNode* succ(BigInt key, DHTNode* pre);
    DHTNode* previousSearch(DHTNode* node);
    DHTNode* getMachineNodeAccordingToRT(BigInt& machineHash);
    bool SearchFile(BigInt hash);
    void displayBT();

};

// #endif
//function for adjusting the routing table after insertion of the machine
void DHTNode::displayBT() {
    displayBFT(tree.root);
}
void DHTNode::AdjustRT() {


    RTNode<DHTNode>* curr = head.head;
    BigInt count("1");
    BigInt count2("1");
    BigInt count3("2");

    curr->node = succ(machineHash + count.power(count3, count - count2), this);  //the power function may return a float (when count is 0), so using ceiling ftn, converting it into a string and then converting it into a BigInt to make it compatible with the BigInt + operator.
    curr->machineHash = curr->node->machineHash;                                  //it can show some problems like 32 in decimals is not equal to 32 in hexa, but our Bigint class is storing in hexa.
    //we have to correct it
    curr = curr->next;
    count++;
    while (curr) {
        curr->node = succ(machineHash + count.power(count3, count - count2), curr->prev->node);
        curr->machineHash = curr->node->machineHash;

        count++;
        curr = curr->next;
    }
}
DHTNode* DHTNode::previousSearch(DHTNode* node) {
    DHTNode* curr = node;
    while (curr->next != node) {
        curr = curr->next;
    }
    return curr;
}

DHTNode* DHTNode::succ(BigInt key, DHTNode* pre) {
    key = key.modulus(key, identifier_space);
    DHTNode* previous = NULL;
    while (true) {
        if (pre->machineHash < key && pre->next->machineHash < pre->machineHash && pre->next->machineHash < key) {
            return pre->next;
        }
        if (pre->machineHash >= key && pre->next->machineHash < pre->machineHash && pre->next->machineHash >= key) {
            return pre->next;
        }
        previous = previousSearch(pre);
        //if (pre->machineHash >= key && !(pre->prev->machineHash >= key)) {
        if (pre->machineHash >= key && !(previous->machineHash >= key)) {
            return pre;
        }
        if (pre->next->machineHash == pre->machineHash)
            return pre;
        pre = pre->next;
    }
    return pre->next;
}

void DHTNode::AdjustRTAfterDeletion(DHTNode* deleteNode) {
    RTNode<DHTNode>* curr = head.head;
    while (curr) {
        if (curr->machineHash == deleteNode->machineHash) {
            curr->node = curr->node->next;
            curr->machineHash = curr->node->machineHash;
        }
        curr = curr->next;
    }
    return;
}
////function for adjusting the routing table after deletion of the machine
//void DHTNode::AdjustRTAfterDeletion(DHTNode* deleteNode) {}
//
////function for inserting the file
//bool DHTNode::InsertFile(string path, string hash) {}
//
//bool DHTNode::InsertFileHelper(string path, string hash, BTree* root) {}
//
////function for deleting the file 
//bool DHTNode::deleteFile(string hash) {}
//
//bool DHTNode::deletefilehelper(string hash) {}
//
////function for removing the data from a file
//bool DHTNode::removeData() {}

DHTNode::DHTNode(string folderName, BigInt machineHash, BigInt IS, DHTNode* next) : head(stoi(string(IS))) {
    this->identifier_space = IS;
    this->folderName = folderName;
    this->makeFolder(folderName);
    this->folderpath += "./";
    this->folderpath += folderName;
    this->machineHash = machineHash;
    this->next = next;
    for (int i = 0; i < stoi(string(IS)); ++i) {
        head.insert(this);
    }
    if (this->next)
        this->splitTrees();
}
//function to make a new folder
bool DHTNode::insertFile(keypair data) {
    keypair dataToBeInserted(data.hash);
    dataToBeInserted.filepath = fileCopy(data.filepath);
    if (dataToBeInserted.filepath == "\0")
        return false;
    tree.insert(dataToBeInserted);
    return true;
}

bool DHTNode::SearchFile(BigInt hash) {
    return this->tree.search(hash);
}
bool DHTNode::deleteFile(BigInt hash) {
    keypair fileToBeDeleted;
    this->tree.deletedata(hash , fileToBeDeleted);
    if (fileToBeDeleted.filepath == "\0")
        return false;
    else {
        this->fileDeletor(fileToBeDeleted.filepath);
        return true;
    }
}
bool DHTNode::makeFolder(string folderPath) {
    //will be called in the constructor
    //Works only for windows (shayad)
    //didn't use other commands because they were causing problems like datatypes and libraries
#ifdef _WIN32
    std::string command = "mkdir \"" + std::string(folderPath) + "\"";
#else
    std::string command = "mkdir -p \"" + std::string(folderPath) + "\"";
#endif

    int result = std::system(command.c_str());
    //this->folderpath = command.c_str();
    return result == 0;
}
bool DHTNode::fileDeletor(string name) {
    //will be called in the deletefile function
    const char* nameString = name.c_str(); //because the function only accepts const char pointer
    return remove(nameString);
}
bool DHTNode::deleteFolder(string name="\0") {
    //Will be called in the destructor
    //Works only for windows (shayad)
    const char* folderPath = this->folderName.c_str();
#ifdef _WIN32
    std::string command = "rmdir /s /q \"" + std::string(folderPath) + "\"";
#else
    std::string command = "rm -r \"" + std::string(folderPath) + "\"";
#endif

    int result = std::system(command.c_str());

    return result == 0;
}
// function to extract the extension of the file
string DHTNode::fileExtensionExtractor(string path) {
    int size = path.size();
    int i = 0;
    for (i = size - 1; i >= 0; --i) {
        if (path[i] == '.')
            break;
    }
    string extension = "";
    for (i; i < size; ++i) {
        extension += path[i];
    }
    return extension;
}
string DHTNode::fileNameWithoutExtension(string path) {
    string hello = "";
    int size = path.size();
    int i = 0;
    for (i = size - 1; i >= 0; --i) {
        if (path[i] == '.')
            break;
    }
    int k = 0;
    for (k = i; k > 0; --k) {
        if (path[k] == '\\' || path[k] == '/') {
            break;
        }
    }
    for (int j = k; j < i; ++j) {
        hello += path[j];
    }
    return hello;
}
//function to copy the file content to another file (supports every filetype)
//first parameter is the path of the source file, and the second paramter is the folder path
string DHTNode::fileCopy(string path) {
    //will be called in the insertfile function
    //opening the source file in binary format

    std::ifstream sourceFile(path, std::ios::binary);
    if (!sourceFile) {
        std::cout << "Error opening source file." << std::endl;
        return "\0";
    }

    string newfilepath = this->folderpath;
    newfilepath += "/";
    newfilepath += fileNameWithoutExtension(path);
    newfilepath += to_string(this->filecount);
    newfilepath += fileExtensionExtractor(path);

    this->filecount++;

    std::ofstream destinationFile(newfilepath, std::ios::binary);

    //The following function (rdbuf) reads the whole data from one file and returns it. Here it is copying
    //in binary format because of the above std::ios::binary, which allows it to read data of every filetype correctly

    destinationFile << sourceFile.rdbuf();

    if (destinationFile.fail() || sourceFile.fail()) {
        std::cerr << "Error copying contents." << std::endl;
        return "\0";
    }

    std::cout << "Contents copied successfully." << std::endl;


    sourceFile.close();
    destinationFile.close();

    return newfilepath;
}

void DHTNode::mergeTrees() {
    this->BFStraversalForMerging(this->tree.root);
}
void DHTNode::BFStraversalForMerging(btnode* root) {
    if (root == nullptr)
        return;

    Queue<btnode*> q;
    q.enqueue(root);
    while (!q.is_empty()) {

        btnode* current = q.peek();
        q.dequeue();
        // Display the values in the current node
        for (int i = 0; i < current->count; i++) {
            //cout << current->value[i + 1].getdata().hash << "\t"<< current->value[i + 1].getdata().filepath<<"\t"; // Assuming 1-indexed value array
            //current->value[i + 1].print();
            while (current->value[i + 1].isEmpty() == false) {
                keypair data = current->value[i + 1].deleteAt(0);
                if(this->next)
                    this->next->insertFile(data);
                this->fileDeletor(data.filepath);
            }
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
void DHTNode::BFStraversalForDestrcutor(btnode* root) {
    if (root == nullptr)
        return;
    Queue<btnode*> q;
    q.enqueue(root);
    while (!q.is_empty()) {

        btnode* current = q.peek();
        q.dequeue();
        // Display the values in the current node
        for (int i = 0; i < current->count; i++) {
            //cout << current->value[i + 1].getdata().hash << "\t"<< current->value[i + 1].getdata().filepath<<"\t"; // Assuming 1-indexed value array
            //current->value[i + 1].print();
            while (current->value[i + 1].isEmpty() == false) {
                keypair data = current->value[i + 1].deleteAt(0);
                //this->next->insertFile(data);
                this->fileDeletor(data.filepath);
            }
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

// returns relevant DHTNode* using routing table and given machineHash 
DHTNode* DHTNode::getMachineNodeAccordingToRT(BigInt& machineHash) {

    RTNode<DHTNode>* curr = head.head;
    
    while (curr) {
        if (curr->machineHash == machineHash)
            return curr->node;
        else if (curr->machineHash < machineHash && curr->next && curr->next->machineHash > machineHash)
            return curr->node;
        else if (curr->prev && curr->prev->machineHash > curr->machineHash)
            return curr->prev->node;
        else if (curr->next == NULL)
            return curr->node;
        else
            curr = curr->next;
    }

    return NULL;
}


void DHTNode::splitTrees() {
    if(this->next)
        this->BFStraversalForSpliting(this->next->tree.root);
}
void DHTNode:: BFStraversalForSpliting(btnode* root) {
    if (root == nullptr)
        return;

    Queue<btnode*> q;
    Queue<BigInt> deletedhashes;

    q.enqueue(root);
    //int count = 1;
    while (!q.is_empty()) {

        btnode* current = q.peek();
        q.dequeue();
        // Display the values in the current node
        for (int i = 0; i < current->count; i++) {
            //cout << current->value[i + 1].getdata().hash << "\t"<< current->value[i + 1].getdata().filepath<<"\t"; // Assuming 1-indexed value array
            //current->value[i + 1].print();
            if(current->value[i+1].getdata().hash<=this->machineHash) {
                keypair data;
                while (current->value[i + 1].getCount() >1 ) {
                    data = current->value[i + 1].deleteAt(0);
                    this->insertFile(data);
                    this->next->fileDeletor(data.filepath);
                }
                data = current->value[i + 1].getdata();
                this->insertFile(data);
                deletedhashes.enqueue(data.hash);
                //keypair deleted;
                //this->next->deleteFile(data.hash);
                //this->next->fileDeletor(data.filepath);
            }
            
        }
        cout << endl; // New line after each node's values

        // Enqueue child nodes of the current node
        for (int i = 0; i <= current->count; i++) {
            if (current->child[i] != nullptr) {
                q.enqueue(current->child[i]);
            }
        }
    }
    BigInt hash;
    while (!deletedhashes.is_empty()) {
        hash = deletedhashes.peek();
        deletedhashes.dequeue();
        this->next->deleteFile(hash);
    }

}
DHTNode::~DHTNode() {
    // implement check if there is another machine in the system or not
    if (this->next == NULL)
        // deletes the files in the associated foler
        this->BFStraversalForDestrcutor(this->tree.root);
    else
        // copy the files onto the next machine and delete this machine's files
        this->BFStraversalForMerging(this->tree.root);
    this->deleteFolder();
}

//void DHTNode::AdjustRT() {
//    RTNode* curr = head.head;
//    int count = 0;
//    curr->node = succ(hash + BigInt(to_string(ceil(pow(2, count - 1)))), this);  //the power function may return a float (when count is 0), so using ceiling ftn, converting it into a string and then converting it into a BigInt to make it compatible with the BigInt + operator.
//    curr->hash = curr->node->hash;                                  //it can show some problems like 32 in decimals is not equal to 32 in hexa, but our Bigint class is storing in hexa.
//    //we have to correct it
//    curr = curr->next;
//    count++;
//    while (curr) {
//        curr->node = succ(hash + BigInt(to_string(ceil(pow(2, count - 1)))), curr->prev->node);
//        curr->hash = curr->node->hash;
//
//        count++;
//        curr = curr->next;
//    }
//}
//DHTNode* DHTNode::succ(BigInt key, DHTNode* pre) {
//    while (true) {
//        if (pre->hash >= key) {
//            return pre;
//        }
//        pre = pre = next;
//        if (pre->hash < pre->prev->hash) {
//            break;
//        }
//    }
//    return nullptr;
//}
////function for adjusting the routing table after deletion of the machine
//void DHTNode::AdjustRTAfterDeletion(DHTNode* deleteNode) {
//    RTNode* curr = head.head;
//    while (curr) {
//        if (curr->hash == deleteNode->hash) {
//            curr->node = curr->node->next;
//            curr->hash = curr->node->hash;
//        }
//    }
//    return;
//}