/*********************************/
/**** Lecture 10 - Exceptions ****/
/*********************************/



/* Two aspects that one has in C++, but not in Java. Avoid them at all costs!*/

/** Multiple inheritance */


class D {
public:
    int dd;
    void ff() {};
};



class A : public D { // base class 1
    int x;
public:
    void f() {}
};

class B : public D { // base class 2
    int y;
public:
    void f() {} // source of ambiguity
    void g() {}
};



class C : public A, public B { // C inherits from A & B
    int z; // Visible only within ’C’
public: // ..
};


int main2() {
    A a;
    B b;
    C c;
    a = c; // OK
    b = c; // OK
    //c = a; // Error
    A *pa = &c;
    B *pb = &c;
     //In general , (int) pa != (int) pb
    // This is because the data of c is organized as follows: | A | B | C |
    // data of class A, followed by data of class B, followed by data of class C
    // taking the address of c either as type pointer to A or pointer to B, it will
    // get the address at the memory of c associated with class A, resp. class B
    
    
    // c.f(); // does not compile: which f are we calling?
    c.A::f();
    c.B::f();
    
    // diamond structure : add another class D which derives both A and B
    // declaring the object of type C will allocate memory using the class D, twice!
    // one for the A-inheritace, and one for the B-inheritance: | D | A | D | B | C |
    
    //D d = c; //error : ambiguous, which of the two will go to d?
    D d = (B) c;
    d = (A) c;
    return 0;
}





/* gotos */

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

bool f() {
    int *pc = NULL;
    FILE *fp = NULL;
    pc = (int *) malloc(sizeof(int)*100);
    if (pc == NULL) {
        goto error1; }
    fp = fopen("outfile", "w");
    if (fp == NULL) {
        goto error2; }
    // ...
    free(pc); fclose(fp);
    return true;
error1:
    if (pc != NULL) free(fp);
    if (fp != NULL) fclose(fp);
    // ...
    return false;
error2:
    if (pc != NULL) free(fp);
    if (fp != NULL) fclose(fp);
    // ...
    return false;
    
}

//rewrite a for loop using gotosr
void z() {
    for (int i=0; i<3; i++) {
        int j=4;
    }
    
    
    {
        int i=0;
    loopStarts:
        if(i<3) {
            int j=4;
            i++;
            goto loopStarts;
        }
    }
    
}



/* EXCEPTIONS */

/**
 - HANDLE exceptional conditions
 - PASS error information
 - GIVE control to a caller when no action has any meaning locally
 - STOPS the use of print messages and gotos for debugging
**/


/** Syntax : try, catch, throw **/

#include <iostream>

void g() { // etc.
    int x;
    if ( x == 3 /* something goes wrong */) { throw 2.0; }
}


void ff() {
    try {
        // ...
        g();
        //..
        //..
    }
    catch (int code) { // Handle int exceptions
        std::cerr << "Caught exception " << code << std::endl;
    }
    catch (...) { // Default handler
        std::cerr << "Caught unknown exception" << std::endl; }
}


/** C++ vs. Java 
    - C++ has no *finally* block
    - C++ can throw any type
    - C++ methods are not required to have throw specifications - if specified, other types will exit the program upon being thrown
    - in C++, only give a name to the type of the exception if the data will 
            be used.
 **/




/**
    Exceptions "unwind" the stack! 
        - all objects going out of scope are destroyed.
        - objects created with new will NOT be destroyed
 **/

#include <string>

void gg() {
    // ...
    int* p = new int[100];
    std::string s("a string");
    throw 13; // s is destroyed, but NOT p
}

void fff() {
    try {
        gg();
    }
    catch (int& e) { // Will be caught here...
        std::cerr << "fff " << e << std::endl;
        e = 15;
        throw e;
    }
}

//alternative to fff()
void ffff() try {
    gg();
}
catch (int e) { // Will be caught here...
    std::cerr << "fff " << e << std::endl;
}


int main() {
    try {
        fff();
    }
    catch (int e) { // ...not here.
        std::cerr << "main " << e << std::endl;
    }
    return 0;
}

/** Re-throw
    - If your exception handler cannot completely 
        handle the exception, it can “re-throw” 
        the exception for the benefit of a caller.
    - The exception will be passed upwards. 
        If you the exception is received by 
        non-const reference or pointer, any modification 
        will be passed to the next handler.
 **/


/**
 The order of exception handlers matters. When an exception occurs, C++ scans through the list of eligible exception handlers and selects the first one that is compatible. Therefore we often list catch blocks in order of increasing generality:
 **/

int main3() {
    try {
        fff();
    }
    catch( C& c ) { /* ... */ } // Least general
    catch( B& b ) { /* ... */ } // More general
    catch ( ... ) { /* ... */ } // Most general
    return 0;
}

/**
 In hierachical classes, throwing/catching is equivalent to calling a method    
        - carefully choose the type of the parameters
 
 Passing exceptions by pointers is somewhat dangerous, 
 as it may be unclear when and if to delete the exception.
 **/


/**
    EXCEPTION GUIDELINES
 
    - in large programs, you should define a set of exception classes 
        (even though C++ allows you to throw any type
    - catch by reference is the norm
    - don't throw exceptions in a destructor
    - standard library methods may throw a number of possible exceptions. 
        These are in <stdexcept>
    - The "root" exception class is std::exception
    - It is common to have new exceptions inherit from std::runtime error
 
    - "Exception safe" code:
        - Restore modified structures to standard values
        - Release resources
 **/


    
    
    
    
/*******************************************************/
// Assigned readings
    
// * Exceptions :   http://www.cplusplus.com/doc/tutorial/exceptions/
//                  http://www.cplusplus.com/reference/exception/exception/
// * Operator overloading : http://en.cppreference.com/w/cpp/language/operators
    
    
    
