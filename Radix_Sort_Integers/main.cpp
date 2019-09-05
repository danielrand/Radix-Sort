#include <iostream>
#include <fstream>
using namespace std;

class ListNode  {
    
private:
    
    int data;
    ListNode *next;
    
public:
    
    ListNode (int d) {
        data = d;
        next = NULL;
    }
    
    ListNode(const ListNode &otherNode) {
        data = otherNode.data; next = otherNode.next;
    }
    
    ListNode* getNext () {
        return next;
    }
    
    void setNext (ListNode* other) {
        next = other;
    }
    
    int getData () {
        return data;
    }
    
};

class LinkedListStack {
    
private:
    
    ListNode *top;
    
public:
    
    LinkedListStack () {
        top = NULL;
    }
    
    ~LinkedListStack () {
        delete top;
    }
    
    void push (ListNode *n) {
        (*n).setNext(top);
        top = n;
    }
    
    ListNode pop () {
        if (top!=NULL) {
            ListNode oldTop = *top;
            ListNode *oldPointer = top;
            top = oldTop.getNext();
            delete oldPointer;
            return oldTop;
        }
        return NULL;
    }
    
    bool isEmpty () {
        return top == NULL;
    }
    
    void printS () {
        ListNode *n = top;
        cout << "Top -> ";
        while (n != NULL) {
            cout << "(" << (*n).getData() << ", ";
            n = (*n).getNext();
            if (n == NULL)
                cout << "NULL) -> NULL";
            else cout << n->getData() << ") -> ";
        }
    }
};

class LinkedListQueue {
    
private:
    // head points next to the first node
    // tail points next to the last node
    ListNode *head, *tail;
    ListNode *dummy;
    
public:
    
    LinkedListQueue () {
        dummy = new ListNode(9999);
        dummy->setNext(NULL);
        head = dummy;
        tail = dummy;
    }
    
    ~LinkedListQueue () {
        if (head != tail)
            delete tail;
        delete head;
    }
    
    void enqueue (ListNode *n) {
        (*tail).setNext(n);
        tail = n;
    }
    
    ListNode dequeue () {
        if (isEmpty())
            return NULL;
        ListNode oldFront = *(head->getNext());
        ListNode *oldFrontPointer = head->getNext();
        head->setNext(oldFront.getNext());
        // if the list is now empty point tail to the dummy again
        if (isEmpty())
            tail = dummy;
        delete oldFrontPointer;
        return oldFront;
    }
    
    bool isEmpty () {
        return head->getNext() == NULL;
    }
    
    void printQ () {
        ListNode* n = head->getNext();
        cout << "Head -> ";
        while (true) {
            if (n == NULL) {
                cout << "NULL";
                break;
            }
            cout << "(" << n->getData() << ", ";
            if (n->getNext() != NULL)
                cout << (n->getNext())->getData() << ") -> ";
            else cout << "NULL) -> ";
            n = n->getNext();
        }
        cout << "\n   Tail -> (" << tail->getData() << ", NULL) -> NULL\n\n";
    }
    void printQToFile (ofstream &file) {
        if (file.is_open()) {
            ListNode* n = head;
            file << "Head -> ";
            if (n == dummy) file << "DUMMY";
            while (true) {
                if (n == NULL) {
                    file << "NULL";
                    break;
                }
                file << "(" << n->getData() << ", ";
                if (n->getNext() != NULL)
                    file << (n->getNext())->getData() << ") -> ";
                else file << "NULL) -> ";
                n = n->getNext();
            }
            file << "\n   Tail -> (" << tail->getData() << ", NULL) -> NULL\n\n";
        }
        else
            cout << "Unable to open file argv[1], please provide an input file in argv[1]." << endl;
        
    }
    
};

class RadixSort {
    
private:
    
    const static int tableSize = 10;
    LinkedListQueue *hashTable [2][tableSize];
    LinkedListStack stack;
    int data, currentTable, previousTable, maxDigits, offSet, currentDigit;
    
    int getMaxDigits (int num) {
        if (num/10 == 0)
            return 1;
        return 1 + getMaxDigits(num/10);
    }
    
    public :
    
    RadixSort () {
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < tableSize; j++)
                hashTable [i][j] = new LinkedListQueue();
        currentDigit = 0;
        currentTable = 0;
        
    }
    void firstReading (ifstream &myfile) {
        int negativeNum = 0, positiveNum = 0;
        if (myfile.is_open()) {
            while (myfile >> data) {
                if (data < negativeNum)
                    negativeNum = data;
                if (data > positiveNum)
                    positiveNum = data;
            }
            offSet = abs(negativeNum);
            positiveNum += offSet;
            maxDigits = getMaxDigits(positiveNum);
        }
        else
            cout << "Unable to open file argv[1], please provide an input file in argv[1]." << endl;
    }
    void loadStack (ifstream &myfile) {
        //TOP = new LinkedListStack();
        if (myfile.is_open()) {
            while (myfile >> data) {
                data+= offSet;
                ListNode *newNode = new ListNode (data);
                stack.push(newNode);
            }
            cout << "Below is the constructed stack:\n";
            stack.printS();
            cout << endl;
        }
        else
            cout << "Unable to open file argv[1], please provide an input file in argv[1]." << endl;
    }
    
    void radixSort () {
        dumpStack ();
        currentDigit++;
        currentTable = 1;
        previousTable = 0;
        int currentQueue = 0;
        while (currentDigit < maxDigits) {
            while (currentQueue < tableSize) {
                while (!hashTable[previousTable][currentQueue]->isEmpty()) {
                    ListNode node = hashTable[previousTable][currentQueue]->dequeue();
                    int hashIndex = getDigit(node, currentDigit);
                    hashTable[currentTable][hashIndex]->enqueue(new ListNode(node.getData()));
                }
                currentQueue++;
            }
            printTable(hashTable[currentTable]);
            swap (currentTable, previousTable);
            currentQueue = 0;
            currentDigit++;
        }
    }
    
    void dumpStack () {
        while (!stack.isEmpty()) {
            ListNode node = stack.pop();
            int digit = getDigit (node, currentDigit);
            int hashIndex = digit;
            //cout << "hash index for " << node.getData() << " is " << hashIndex << endl;
            hashTable[currentTable][hashIndex]->enqueue(new ListNode(node.getData()));
        }
        printTable(hashTable[currentTable]);
    }
    
    int getDigit (ListNode node, int cDigit) {
        int num = node.getData();
        for (int i = 0; i < cDigit; i++)
            num /= 10;
        return num % 10;
    }
    
    void printTable (LinkedListQueue *table []) {
        cout << "\nBelow is the current hashTable for digit #" << currentDigit << endl;
        for (int i = 0; i < 10; i++) {
            if (!table[i]->isEmpty()) {
                cout << i << ": ";
                table[i]->printQ();
            }
        }
        cout << endl;
    }
    
    void outputSortedData (ofstream &outFile1, ofstream &outFile2) {
        LinkedListQueue currentQ;
        swap(currentTable, previousTable);
        if (!outFile2.is_open())
            cout << "Unable to open file argv[3], please provide an output file in argv[3]." << endl;
        else
            outFile2 << "Step 11: Here are all the non empty queues in the final hash table after sorting:\n";
        for (int i = 0; i < tableSize; i++) {
            currentQ = *(hashTable[currentTable][i]);
            if (outFile2.is_open() && !currentQ.isEmpty()) {
                outFile2 << i << ": ";
                currentQ.printQToFile(outFile2);
            }
            while (!currentQ.isEmpty()) {
                data = currentQ.dequeue().getData();
                if (offSet > 0)
                    data -= offSet;
                outFile1 << data << " ";
            }
        }
    }
};

int main (int argc, char** argv) {
    RadixSort sort;
    ifstream inFile;
    ofstream outFile1, outFile2;
    inFile.open(argv[1]);
    if (inFile.is_open()) {
        sort.firstReading(inFile);
        inFile.close();
        inFile.open(argv[1]);
        sort.loadStack(inFile);
        sort.radixSort();
        outFile1.open (argv[2]);
        outFile2.open (argv[3]);
        if (outFile1.is_open()) {
            sort.outputSortedData(outFile1, outFile2);
            cout << "The final sorted list of numbers can be found in whatever you entered for argv[2]\n";
        }
        else {
            cout << "Unable to open file argv[2], please provide an output file in argv[2]." << endl;
            cout << "Unable to open file argv[3], please provide an output file in argv[3]." << endl;
            
        }
        if (outFile1.is_open() && outFile2.is_open())
            cout << "The Debug output can be found in whatever you entered for argv[3]\n";
    }
    else
        cout << "Unable to open file argv[1], please provide an input file in argv[1]." << endl;
    inFile.close();
    outFile1.close();
    outFile2.close();
    return 0;
}
