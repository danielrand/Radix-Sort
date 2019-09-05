#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string removePadding (string s) {
    string noPadding = s;
    while (noPadding.find(' ', 1) != noPadding.npos)
        noPadding.erase(s.find(' '), 1);
    return noPadding;
}

class ListNode  {
    
private:
    
    string data;
    ListNode *next;
    
public:
    
    ListNode (string d) {
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
    
    string getData () {
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
        ListNode oldTop = *top;
        ListNode *oldPointer = top;
        top = oldTop.getNext();
        delete oldPointer;
        return oldTop;
    }
    
    bool isEmpty () {
        return top == NULL;
    }
    
    void printS (ofstream &outFile2) {
        ListNode *n = top;
        outFile2 << "Top -> ";
        while (n != NULL) {
            outFile2 << "(" << removePadding(n->getData()) << ", ";
            n = (*n).getNext();
            if (n == NULL)
                outFile2 << "NULL) -> NULL";
            else outFile2 << removePadding(n->getData()) << ") -> ";
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
        dummy = new ListNode("9999");
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
    
    void printQ(ofstream &file) {
        if (file.is_open()) {
            ListNode* n = head;
            if (n == dummy) file << "DUMMY";
            while (true) {
                if (n == NULL) {
                    file << "NULL\n\n";
                    break;
                }
                file << "(" << removePadding(n->getData()) << ", ";
                if (n->getNext() != NULL) {
                    file << removePadding(n->getNext()->getData()) << ") -> ";
                }
                else file << "NULL) -> ";
                n = n->getNext();
            }
        }
    }
    
};

class RadixSort {
    
private:
    
    const static int tableSize = 256;
    LinkedListQueue *hashTable [2][tableSize];
    LinkedListStack stack;
    string data;
    int currentTable, previousTable, longestStringLength, offSet, currentPosition;
    
    public :
    
    RadixSort () {
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < tableSize; j++)
                hashTable [i][j] = new LinkedListQueue();
        currentPosition = 0;
        currentTable = 0;
        
    }
    
    void firstReading (ifstream &myfile) {
        longestStringLength = 0;
        if (myfile.is_open()) {
            while (myfile >> data)
                if (data.length() > longestStringLength)
                    longestStringLength = data.length();
        }
        else
            cout << "Unable to open file argv[1], please provide an input file in argv[1]." << endl;
    }
    
    string padString (string s) {
        string paddedString = s;
        if (s.length() < longestStringLength)
            for (int i = 1; i <= longestStringLength-s.length(); i++)
                paddedString += " ";
        return paddedString;
    }
    
    void loadStack (ifstream &myfile, ofstream &outFile2) {
        while (myfile >> data) {
            string paddedData = padString(data);
            ListNode *newNode = new ListNode (paddedData);
            stack.push(newNode);
        }
        outFile2 << "Below is the output of the stack:\n\n";
        stack.printS(outFile2);
        outFile2 << endl << endl;
    }
    
    char getChar (ListNode n, int pos) {
        string s = n.getData();
        int index = s.length()-1-pos;
        return s[index];
    }
    
    void radixSort (ofstream &outFile2) {
        dumpStack (outFile2);
        currentPosition++;
        currentTable = 1;
        previousTable = 0;
        int currentQueue = 0;
        char chr;
        while (currentPosition < longestStringLength) {
            while (currentQueue < tableSize) {
                while (!hashTable[previousTable][currentQueue]->isEmpty()) {
                    ListNode node = hashTable[previousTable][currentQueue]->dequeue();
                    chr = getChar(node,currentPosition);
                    int hashIndex = (int) chr;
                    hashTable[currentTable][hashIndex]->enqueue(new ListNode(node.getData()));
                }
                currentQueue++;
            }
            printTable(hashTable[currentTable], outFile2);
            swap (currentTable, previousTable);
            currentQueue = 0;
            currentPosition++;
        }
    }
    
    void dumpStack (ofstream &outFile2) {
        while (!stack.isEmpty()) {
            ListNode node = stack.pop();
            char chr = getChar (node, currentPosition);
            int hashIndex = (int) chr;
            //cout << "hash index for " << node.getData() << " is " << hashIndex << endl;
            hashTable[currentTable][hashIndex]->enqueue(new ListNode(node.getData()));
        }
        printTable(hashTable[currentTable], outFile2);
    }
    
    void printTable (LinkedListQueue *table [], ofstream &outFile2) {
        if (outFile2.is_open()) {
            if (currentPosition == longestStringLength-1)
                outFile2 << "Here is the final hashtable for char #" << currentPosition << endl << endl;
            else
                outFile2 << "\nBelow is the current hashTable for char #" << currentPosition << endl << endl;
            for (int i = 0; i < tableSize; i++) {
                if (!table[i]->isEmpty()) {
                    outFile2 << "Table[" << currentTable << "][" << i << "]: ";
                    table[i]->printQ(outFile2);
                }
            }
            outFile2 << endl;
        }
    }
    
    void outputSortedData (ofstream &outFile1, ofstream &outFile2) {
        LinkedListQueue currentQ;
        swap(currentTable, previousTable);
        if (!outFile2.is_open())
            cout << "Unable to open file argv[3], please provide an output file in argv[3]." << endl;
        int count = 0;
        for (int i = 0; i < tableSize; i++) {
            currentQ = *(hashTable[currentTable][i]);
            while (!currentQ.isEmpty()) {
                data = currentQ.dequeue().getData();
                outFile1 << removePadding(data) << " ";
            }
        }
    }
};

int main (int argc, char** argv) {
    RadixSort sort;
    ifstream inFile;
    ofstream outFile1, outFile2;
    inFile.open(argv[1]);
    outFile1.open (argv[2]);
    outFile2.open (argv[3]);
    if (inFile.is_open()) {
        sort.firstReading(inFile);
        inFile.close();
        inFile.open(argv[1]);
        sort.loadStack(inFile, outFile2); // CHECK FOR OUTFILES BEING PASSED
        sort.radixSort(outFile2);
        if (outFile1.is_open()) {
            sort.outputSortedData(outFile1, outFile2);
            cout << "The final sorted list of strings can be found in whatever you entered for argv[2]\n";
        }
        else {
            cout << "Unable to open file argv[2], please provide an output file in argv[2]." << endl;
            cout << "Unable to open file argv[3], please provide an output file in argv[3]." << endl;
        }
        if (outFile1.is_open() && outFile2.is_open())
            cout << "The debug output can be found in whatever you entered for argv[3]\n";
    }
    else {
        cout << "Unable to open argv[1], please provide an input file in argv[1]." << endl;
        cout << "Unable to open argv[2], please provide an input file in argv[2]." << endl;
        cout << "Unable to open argv[3], please provide an input file in argv[3]." << endl;
    }
    inFile.close();
    outFile1.close();
    outFile2.close();
    return 0;
}
