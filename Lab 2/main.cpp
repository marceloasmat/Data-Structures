// using c++20
#include <iostream>
#include <cassert>
#include <string>
#include <cctype>

// DOCTEST NOTES
//
// This brings in the doctest testing framework. It contains its own main so creating another here will be problematic.
// You can bring the code to another project with a main if you prefer to work outside of doctest.
// To see your console output, add --no-skip as an argument to the compiled executable.
// CHECK macros take a bool. Be careful to not use the assignment operator instead of the equality operator.
//     Using = instead of == can cause some strange template errors.
// The DocTest tutorial is located here:
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace std;

template<typename T>
class StackADT {
public:
    virtual bool isEmpty() const = 0;

    virtual bool push(const T & value) = 0;
    virtual T peek() const = 0;
    virtual bool pop() = 0;
};

constexpr int MIN_ARRAY_SIZE=64;

template<typename T, int N>
class ArrayStack final : public StackADT<T> {
private:
    int topIndex;
    int array[N] {};
public:
    ArrayStack() : topIndex(-1) {
        static_assert(N >= MIN_ARRAY_SIZE);
    }

    bool isEmpty() const override {
        // first try. Not ideal, topIndex should be used as it already keeps track of Array size
        //value at index 0 if not blank   
        // int test = array[0];
       /* // if (array[0] != 0){
            // return false;
        // }
        return true;  */

        if (topIndex == -1){
            return true;
        }
        return false;
    }
// the & will reference the test case input of "10" rather than make a copy of the value 10. 
    bool push(const T & value) override {
        if(topIndex == N) {
            throw std::length_error("Max array exceeded.");
        }
        // this will fufil our test case to make sure we dont add more than what our array can take
        if(topIndex == N-1){
            return false;
        }
        // we are implementing the virtual method push. 
        // if stack is of size 3. We have 3 items and indexs of 0,1,2 
        topIndex = topIndex +1; //this will make sure the topIndex moves with each push call
        array[topIndex] = value;
        return true;
    }

    T peek() const override {
        if(isEmpty()) {
            throw std::logic_error("Peek on empty ArrayStack.");
        }

        // this returns the item at the top of the array
        return array[topIndex];
    }

    bool pop() override {
        if(isEmpty()) {
            return false;
        }
        // TODO . Similar to peek, but it also removes the object from the stack
        // originally i thought we had to store the value and return it. But pop just removes the value, also we cant returna  value to a boolean funciotn. 
        // T popValue = array[topIndex];
        array[topIndex] = T{}; //This should explicitly set the value in the array to blank. I understand its not necassary. But i wanted to do it. 
        topIndex = topIndex - 1;
        return true;

    }
};

TEST_CASE("testing the array implementation of stack") {
    ArrayStack<int, MIN_ARRAY_SIZE> stack0;
    CHECK(stack0.isEmpty()); 
    bool isPushSuccess = stack0.push(10);
    CHECK(isPushSuccess);
    CHECK(stack0.peek() == 10);
    stack0.push(20);
    CHECK(stack0.peek() == 20);
    CHECK(stack0.pop());
    CHECK(stack0.peek() == 10);
    CHECK(stack0.pop());
    CHECK(stack0.isEmpty());
}


// This is the linked list stuff
template<typename T>
class Node {
private:
    T value;
    Node* next;
// These constructors are not specific to creating a linked list, its generic. We are using it in this lab to create a linked list, but it could be used in other methods. 
public:
    Node(T value) : value(value), next(nullptr) {} // this is used for the first object in the linked list (you can also call line 130 if you want the next node to not be nullpointer)
    Node(T value, Node* next) : value(value), next(next) {} // this is used for all subsequent objects
    // for line 120 see line 14 in python code shapes.py for reference. 
      
    // getter for value of a node
    T getValue() const {
        return value;
    }
    // getter for what a node is pointing to
    Node* getNext() const {
        return next;
    }
    // setter to update what the node is pointing to
    void setNext(Node* n) {
        next = n;
    }
    // setter to update the nodes value
    void setValue(const T & v) {
        value = v;
    }
};

template<typename T>
class ListStack : public StackADT<T> {
private: // other classes and programs cant access/use this. Thats what private means
    Node<T>* top; // last item of my linked list, the * means the variable is a pointer THIS MEANS TOP IS A POINTER.
public: //other classes can!
    ListStack() : top(nullptr) {}
    ~ListStack() {
        while(pop()) {}
    }

    // Copy constructor
    ListStack(const ListStack & other) {
        top = nullptr;
        if (other.top) {
            this->top = new Node<T>(other.top->getValue()); // this represents self. in python
            Node<T>* currentThis = top;
            Node<T>* currentOther = other.top->getNext();

            while (currentOther != nullptr) {
                currentThis->setNext(new Node<T>(currentOther->getValue()));
                currentThis = currentThis->getNext();
                currentOther = currentOther->getNext();
            }

            
        }
        
    }

 /*   void addValues(const int x){
        x = 20;
    }   this is just an example of how const works to prevent changing the passed value*/

    // Move constructor, Hint: Don't forget to make a other a "hollow" data structure.
    ListStack(ListStack && other) noexcept {
        top = other.top;
        other.top = nullptr;
         
    }

    bool isEmpty() const override {
        if (top == nullptr) {
            return true;
        } else {
            return false;
        }   
    }
// TOP IS A POINTER NOT A VARIABLE OF THE NODE. TOP IS THE TOP POINTER
    bool push(const T & value) override {
        Node<T>* topNode = new Node<T>(value ,top);
        top = topNode;
        return true;
    }
// creates a variable for the front of the linked list and returns that value.
    T peek() const override {
        if(isEmpty()) {
            throw std::logic_error("Peek on empty ArrayStack.");
        }
        T topValue = top->getValue(); // this is an extra step, i could just return top.getValue()
        return {topValue};
    }

    bool pop() override {
        if(isEmpty()) {
            return false;
        }
        Node<T>* newTop = top->getNext();
        top->setNext(nullptr); 
        top = newTop;
        return true;
    }
};

TEST_CASE("testing the linked chain implementation of stack") {
    ListStack<int> stack0;
    CHECK(stack0.isEmpty());
    stack0.push(10);
    CHECK(stack0.peek() == 10);
    stack0.push(20);
    CHECK(stack0.peek() == 20);
    CHECK(stack0.pop());
    CHECK(stack0.peek() == 10);
    CHECK(stack0.pop());
    CHECK(stack0.isEmpty());

    // Test copy constructor
    stack0.push(1);
    stack0.push(2);
    stack0.push(3);

    ListStack<int> stack1(stack0);
    CHECK(!stack0.isEmpty());
    CHECK(stack0.peek() == 3);
    CHECK(!stack1.isEmpty());
    CHECK(stack1.peek() == 3);

    // Test move constructor.
    ListStack<int> stack2(std::move(stack0));
    CHECK(stack0.isEmpty());
    CHECK(!stack2.isEmpty());
    CHECK(stack2.peek() == 3);
}

bool areCurleyBracesMatched(const string & inputString) {
    ListStack<string> fakeStack;
    for (int i=0; i < inputString.length(); i++){
        char letterAtI = inputString[i];
        if (letterAtI == '{') {
            fakeStack.push("{");
        } else if (letterAtI == '}'){
            bool popResult = fakeStack.pop();
            if (popResult == false){
                return false;
            }   
        }
    }
    return fakeStack.isEmpty();
}

TEST_CASE("testing matched curly braces") {
    CHECK(areCurleyBracesMatched(""));
    CHECK(areCurleyBracesMatched("{}"));
    CHECK(areCurleyBracesMatched("a{bc}d"));
    CHECK(!areCurleyBracesMatched("{"));
    CHECK(!areCurleyBracesMatched("}"));
    CHECK(!areCurleyBracesMatched("a{b{c}"));
};



bool isPalindrome(const string & inputString) {
    ListStack<char> paliStack;
    for (int i = 0; i < inputString.length(); i++){
        paliStack.push(inputString[i]);
    }
    for( int i = 0; i < inputString.length()/2; i++){
        char topOfStack = paliStack.peek();
        if( topOfStack == inputString[i]){
            // pair matches, keep going
            paliStack.pop();
        }else{
            return false;
        }
    }
    return true;
}

TEST_CASE("testing palindrome") {
    CHECK(isPalindrome(""));
    CHECK(isPalindrome("a"));
    CHECK(isPalindrome("aa"));
    CHECK(isPalindrome("aba"));
    CHECK(isPalindrome("abba"));
    CHECK(!isPalindrome("ab"));
    CHECK(!isPalindrome("abaa"));
    CHECK(isPalindrome("aaabaaa"));
}

string reversedString(const string & inputString) {
    ListStack<char> reverseStack;
    for (int i = 0; i < inputString.length(); i++){
        reverseStack.push(inputString[i]);
    }
    string outputString = "";
    for (int i = 0; i <inputString.length(); i++){
        char topOfStack = reverseStack.peek();
        outputString += topOfStack;
        reverseStack.pop();  
    }
    return {outputString};
}

TEST_CASE("testing reversed string") {
    CHECK(reversedString("").empty());
    CHECK(reversedString("a")=="a");
    CHECK(reversedString("ab")=="ba");
    CHECK(reversedString("abc")=="cba");
}

// Helper for infixToPostFix.
int precedence(char op) {
    if(op == '*' || op == '/') {
        return 2;
    } else {
        assert(op == '+' || op == '-');
        return 1;
    }
}

// Helper for infixToPostFix.
bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

// Helper for infixToPostFix.
bool isOperand(char ch) {
    return isalpha(ch);
}


string infixToPostFix(const string & infix) {
    ListStack<char> aStack;
    string postfixExp = "";
    for(int i = 0; i<infix.length(); i++){
        char charAtI = infix[i];
        bool charIsOperand = isOperand(charAtI); 
        bool charIsOperator = isOperator(charAtI);
        if ( charIsOperand){
            postfixExp += charAtI; 
        } else if (charAtI == '(') {
            aStack.push(charAtI);
        } else if (charIsOperator){
            while (!aStack.isEmpty() && aStack.peek() != '(' &&
                precedence(charAtI) <= precedence(aStack.peek())) {
                    postfixExp += aStack.peek();
                    aStack.pop();
            }
            aStack.push(charAtI);
        } else if (charAtI == ')'){
            while (aStack.peek() != '('){
                postfixExp += aStack.peek();
                aStack.pop();
            }
            aStack.pop();
        }
    }
    while (!aStack.isEmpty()) {
        postfixExp += aStack.peek();
        aStack.pop();
    }
    return {postfixExp};
}

TEST_CASE("testing infix to postfix conversions") {
    CHECK(infixToPostFix("").empty());

    CHECK(infixToPostFix("a") == "a");
    CHECK(infixToPostFix("a+b") == "ab+");
    CHECK(infixToPostFix("a*b") == "ab*");

    CHECK(infixToPostFix("a+b*c") == "abc*+");
    CHECK(infixToPostFix("a+(b*c)") == "abc*+");
    CHECK(infixToPostFix("(a+(b*c))") == "abc*+");

    CHECK(infixToPostFix("(a+b)*c") == "ab+c*");
    CHECK(infixToPostFix("((a+b)*c)") == "ab+c*");

    CHECK(infixToPostFix("a*b+c") == "ab*c+");
    CHECK(infixToPostFix("(a*b)+c") == "ab*c+");
    CHECK(infixToPostFix("((a*b)+c)") == "ab*c+");
}
