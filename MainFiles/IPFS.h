#pragma once
#include<iostream>
#include<stdlib.h>
#include"DHT.h"
using namespace std;
class IPFS {
public:
	CircularLinkedList *d;
	int choice;
    bool flagforstart = 1;
    IPFS() {
        this->d = NULL;
    }
	void menu() 
    {
        char alpha;

        cout << "                  WELCOME TO InterPlanetary File System (IPFS)" << endl<<endl;

        if (d == NULL) {
            d = new CircularLinkedList();
        }
        if (flagforstart) 
        {
            flagforstart = 0;
            cout << "1. SPECIFY THE NUMBER OF MACHINES THAT YOU WANTS TO INSERT IN START " << endl;
            int n;
            cin >> n;

            for (int i = 0; i < n; i++)
            {
                do
                {
                    cout << "1. ENTER MACHINE MANUALLY " << endl;
                    cout << "2. ENTER MACHINE AUTOMATICALLY " << endl;
                    cin >> choice;
                    if (choice == 1) {
                        d->insertMachineManually();
                    }
                    else if (choice == 2)
                    {
                        d->insertMachineAutomatically();
                    }
                } while (choice < 1 || choice>2);
                cout << endl;
            }
            visualizeTree(generatecircular(d->head));
        }
        system("CLS");
        cout << "                    WELCOME TO InterPlanetary File System (IPFS)" << endl << endl;
        cout << "1. INSERT FILE " << endl;
        cout << "2. DELETE FILE " << endl;
        cout << "3. PRINT ROUTING TABLE " << endl;
        cout << "4. PRINT BTREE TABLE " << endl;
        cout << "5. PRINT DISTRIBUTED HASHING SYSTEM " << endl;
        cout << "6. ADD NEW MACHINE " << endl;
        cout << "7. DELETE MACHINE " << endl;
        cout << "8. EXIT " << endl;
        cin >> choice;
        cout << endl<<endl;
        if (choice == 1) 
        {
            d->insertFile();
        }
        else if (choice == 2)
        {
            d->deleteFile();
        }
        else if (choice == 3)
        {
            d->displayRT();
        }
        else if (choice == 4)
        {
            d->displayBT();
        }
        else if (choice == 5) 
        {
            visualizeTree(generatecircular(d->head));
        }
        else if (choice == 6)
        {
            do
            {
                cout << "1. ENTER MACHINE MANUALLY " << endl;
                cout << "2. ENTER MACHINE AUTOMATICALLY " << endl;
                cin >> choice;
                if (choice == 1) {
                    d->insertMachineManually();
                }
                else if (choice == 2)
                {
                    d->insertMachineAutomatically();
                }
            } while (choice < 1 || choice > 2);
            cout << endl;
        }
        else if (choice == 7)
        {
            string hash;
            cout << "Enter Machine Hash FOR THE MACHINE THAT YOU WANT TO DELETE " << endl;
            cin>>hash;
            d->deleteNode(BigInt(hash));
        }
        else if (choice == 8)
        {
            cout<<"THANKYOU FOR USING IPFS "<<endl;
            delete d;
            d = NULL;
            return;
        }
        cout << endl<<"-----------------PRESS ANY KEY TO CONTINUE-----------------"<<endl;
        cin >> alpha;

        menu();
	}
};