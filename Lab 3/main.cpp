#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>

// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace std;

template<typename T>
class ListADT {
public:
    virtual bool isEmpty() const = 0;
    virtual int getLength() const = 0;
    virtual bool insert(int newPosition, const T &newEntry) = 0;
    virtual bool remove(int position) = 0;
    virtual void clear() = 0;
    virtual T getEntry(int position) const = 0;
    virtual void replace(int position, const T &newEntry) = 0;

    virtual ~ListADT() = default;
};

constexpr int MAX_ARRAY_SIZE = 64;

// **** PART 1 *****

template<typename T, int N>
class ArrayList final : public ListADT<T> {
private:
    int itemCount;
    int maxItems;
    int items[N]{};
public:
    ArrayList() : itemCount(0), maxItems(N) {
        static_assert(N >= MAX_ARRAY_SIZE);
    }

    bool isEmpty() const {
        return itemCount == 0;
    }

    int getLength() const {
        return itemCount;
    };

    bool insert(int newPosition, const T &newEntry) {
        bool ableToInsert = (newPosition >= 1) &&
                            (newPosition <= itemCount + 1) &&
                            (itemCount < maxItems);
        if (ableToInsert) {
            // Make room for new entry by shifting all entries at
            // positions >= newPosition toward the end of the array
            // (no shift if newPosition == itemCount + 1)
            for (int pos = itemCount; pos >= newPosition; pos--)
                items[pos] = items[pos - 1];
            // Insert new entry
            items[newPosition - 1] = newEntry;
            itemCount++; // Increase count of entries
        } // end if
        return ableToInsert;
    }

    bool remove(int position) {
        bool ableToRemove = (position >= 1) && (position <= itemCount);
        if (ableToRemove) {
            // Remove entry by shifting all entries after the one at
            // position toward the beginning of the array
            // (no shift if position == itemCount)
            for (int fromIndex = position, toIndex = fromIndex - 1;
                 fromIndex < itemCount; fromIndex++, toIndex++)
                items[toIndex] = items[fromIndex];
            itemCount--; // Decrease count of entries
        } // end if

        return ableToRemove;
    };

    void clear() {
        itemCount = 0;
    };

    T getEntry(int position) const {
        // Enforce precondition
        bool ableToGet = (position >= 1) && (position <= itemCount);
        if (ableToGet)
            return items[position - 1];
        else {
            string message = "getEntry() called with an empty list or ";
            message = message + "invalid position.";
            throw (std::invalid_argument(message));
        } // end if
    } // end getEntry

    void replace(int position, const T &newEntry) {
        // Enforce precondition
        bool ableToSet = (position >= 1) && (position <= itemCount);
        if (ableToSet)
            items[position - 1] = newEntry;
        else {
            string message = "setEntry() called with an empty list or ";
            message = message + "invalid position.";
            throw (std::invalid_argument(message));
        } // end if
    }
};

template<class T>
void testListADT(ListADT<T>& list) {
    CHECK(list.isEmpty());
    CHECK(list.getLength() == 0);
    CHECK_THROWS(list.getEntry(1));
    CHECK(list.insert(1, 99));
    CHECK(list.getLength() == 1);
    CHECK(!list.isEmpty());
    CHECK(list.getEntry(1) == 99);
    list.replace(1, 219);
    CHECK(list.getEntry(1) == 219);
    CHECK(list.remove(1));
    CHECK(list.getLength() == 0);
    CHECK(list.isEmpty());
    CHECK(list.insert(1, 21));
    CHECK(list.insert(2, 24));
    list.clear();
    CHECK(list.getLength() == 0);
// used this to just keep my place in code.    CHECK(false);
}

TEST_CASE("testing array implementation of list adt") {
    ArrayList<int, MAX_ARRAY_SIZE> array0;

    testListADT(array0);
}

template<typename T>
class Node {
private:
    T value;
    std::shared_ptr<Node<T>> next;
    //Node *next;

public:
    Node(T value) : value(value), next(nullptr) {}

    Node(T value, std::shared_ptr<Node<T>> next) : value(value), next(next) {}

    T getItem() const {
        return value;
    }

    auto getNext() const {
        return next;
    }
   /* Node *getNext() const {
        return next;
    } */

    void setNext (std::shared_ptr<Node<T>> nextNodeptr){
        next = nextNodeptr;
    }    
   /* void setNext(Node *n) {
        next = n;
    } */

    void setItem(const T &v) {
        value = v;
    }
};

template<class ItemType>
class LinkedList : public ListADT<ItemType> {
private :
    // Pointer to first node in the chain (contains the first entry in the list)
    std::shared_ptr<Node<ItemType>> headPtr; // converted to a shared pointer

    // Current count of list items
    int itemCount;

    std::shared_ptr<Node<ItemType>> getNodeAt(int position) const {
        // Debugging check of precondition
        if (position < 1 || position > itemCount) {
            throw std::out_of_range("Position out of range.");
        }

        // Count from the beginning of the chain
        std::shared_ptr<Node<ItemType>> curPtr = headPtr;
        for (int skip = 1; skip < position; skip++)
            curPtr = curPtr->getNext();
        return curPtr;
    }

public :
    LinkedList() : headPtr(nullptr), itemCount(0) {}

    ~LinkedList() {
        clear();
    };

    bool isEmpty() const {
        return itemCount == 0;
    }

    int getLength() const {
        return itemCount;
    }

    bool insert(int newPosition, const ItemType &newEntry) {
        bool ableToInsert = (newPosition >= 1) &&
                            (newPosition <= itemCount + 1);
        if (ableToInsert) {
            // Create a new node containing the new entry
            std::shared_ptr<Node<ItemType>> newNodePtr = std::make_shared<Node<ItemType>>(newEntry);
            // Attach new node to chain
            if (newPosition == 1) {
                // Insert new node at beginning of chain
                newNodePtr->setNext(headPtr);
                headPtr = newNodePtr;
            } else {
                // Find node that will be before new node
                std::shared_ptr<Node<ItemType>> prevPtr = getNodeAt(newPosition - 1);
                // Insert new node after node to which prevPtr points
                newNodePtr->setNext(prevPtr->getNext());
                prevPtr->setNext(newNodePtr);
            } // end if
            itemCount++; // Increase count of entries
        } // end if
        return ableToInsert;
    }

    bool remove(int position) {
        bool ableToRemove = (position >= 1) && (position <= itemCount);
        if (ableToRemove) {
            std::shared_ptr<Node<ItemType>> curPtr = nullptr;
            if (position == 1) {
                // Remove the first node in the chain
                curPtr = headPtr; // Save pointer to node
                headPtr = headPtr->getNext();
            } else {
                // Find node that is before the one to delete
                std::shared_ptr<Node<ItemType>> prevPtr = getNodeAt(position - 1);
                // Point to node to delete
                curPtr = prevPtr->getNext();
                // Disconnect indicated node from chain by connecting the
                // prior node with the one after
                prevPtr->setNext(curPtr->getNext());
            } // end if
            // Return node to system
            //curPtr->setNext(nullptr);
            //delete curPtr;
            //curPtr = nullptr;
            itemCount--; // Decrease count of entries
        } // end if
        return ableToRemove;
    }

    void clear() {
        while (!isEmpty())
            remove(1);
    }

    ItemType getEntry(int position) const {
        // Enforce precondition
        bool ableToGet = (position >= 1) && (position <= itemCount);
        if (ableToGet) {
            std::shared_ptr<Node<ItemType>> nodePtr = getNodeAt(position);
            return nodePtr->getItem();
        } else {
            string message = "getEntry() called with an empty list or ";
            message = message + "invalid position.";
            throw (std::invalid_argument(message));
        }
    }

    void replace(int position, const ItemType &newEntry) {
        std::shared_ptr<Node<ItemType>> n = getNodeAt(position);
        n->setItem(newEntry);
    }
}; // end LinkedList

TEST_CASE("test linked chain implementation of list adt") {
    LinkedList<int> list0;

    testListADT(list0);
}

// ***** PART 2 *****

template<typename ItemType>
void insertionSort(ListADT<ItemType> & list) {
    for (int i = 2; i<=list.getLength(); i++){ // for loop starts at 2 because the first item in the array is already "sorted"
        ItemType tempVariable = list.getEntry(i);
        int j = 1; 
        while (tempVariable > list.getEntry(j)){
            j++;
        }
        list.remove(i);
        list.insert(j, tempVariable);
    }

}

void fillRandom(ListADT<int> & list, int n) {
    for(int i=0; i<n; ++i) {
        int j = std::rand();
        list.insert(1, j);
    }
}

bool isSorted(const ListADT<int> & list) {
    int n = list.getLength();
    if(n <= 1) {
        return true;
    }

    for(int i=1; i<n; ++i) {
        if(list.getEntry(i) > list.getEntry(i+1)) {
            return false;
        }
    }

    return true;
}

TEST_CASE("testing linked chain insertion sort") {
    LinkedList<int> list0;
    insertionSort(list0);
    CHECK(list0.isEmpty());

    LinkedList<int> list1;
    CHECK(list1.insert(1, 10));
    insertionSort(list1);
    CHECK(list1.getLength() == 1);

    LinkedList<int> list2;
    CHECK(list2.insert(1, 10));
    CHECK(list2.insert(1, 20));
    insertionSort(list2);
    CHECK(list2.getEntry(1) == 10);
    CHECK(list2.getEntry(2) == 20);

    LinkedList<int> list3;
    CHECK(list3.insert(1, 10));
    CHECK(list3.insert(1, 20));
    CHECK(list3.insert(1, 30));

    insertionSort(list3);

    CHECK(list3.getEntry(1) == 10);
    CHECK(list3.getEntry(2) == 20);
    CHECK(list3.getEntry(3) == 30);

    LinkedList<int> listRandom;
    fillRandom(listRandom, 32);
    insertionSort(listRandom);
    CHECK(isSorted(listRandom));
}

TEST_CASE("testing array insertion sort") {
    ArrayList<int, MAX_ARRAY_SIZE> list0;
    insertionSort(list0);
    CHECK(list0.isEmpty());

    ArrayList<int, MAX_ARRAY_SIZE> list1;
    CHECK(list1.insert(1, 10));
    insertionSort(list1);
    CHECK(list1.getLength() == 1);

    ArrayList<int, MAX_ARRAY_SIZE> list2;
    CHECK(list2.insert(1, 10));
    CHECK(list2.insert(1, 20));
    insertionSort(list2);
    CHECK(list2.getEntry(1) == 10);
    CHECK(list2.getEntry(2) == 20);

    ArrayList<int, MAX_ARRAY_SIZE> list3;
    CHECK(list3.insert(1, 10));
    CHECK(list3.insert(1, 20));
    CHECK(list3.insert(1, 30));

    insertionSort(list3);

    CHECK(list3.getEntry(1) == 10);
    CHECK(list3.getEntry(2) == 20);
    CHECK(list3.getEntry(3) == 30);

    ArrayList<int, MAX_ARRAY_SIZE> listRandom;
    fillRandom(listRandom, 32);
    insertionSort(listRandom);
    CHECK(isSorted(listRandom));
}
