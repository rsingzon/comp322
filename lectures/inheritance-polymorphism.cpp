/************************************/
/**** Lecture 09 - Inheritance ******/
/************************************/



/*********************************************/
// SHAPE EXAMPLE - a more complex hierarchy
/*********************************************/


#include <iostream>
using namespace std;


class shape {
public:
    shape() {} // Constructor
    ~shape() {} // Destructor
    
    double perimeter() const { return 0; }
    double area() const { return 0; }
    
};


class polygon : public shape {
protected:
    int nsides;         // Number of sides
    double *lengths;    // Lengths of each side public:
    
    polygon(double width=1.0, double height=1.0)  {
        nsides = 4;
        lengths = new double[4];
        lengths[0] = width;
        lengths[1] = height;
        lengths[2] = width;
        lengths[3] = height;
    }
    
    polygon(int n, double *len) {
        nsides = n;
        lengths = len;
    }
    
    ~polygon() { delete [] lengths; }

public:
    double perimeter() const { // Override base class
        double p = 0.0;
        for (int i = 0; i < nsides; i++) p += lengths[i];
        return p;
    }
    
};


class rectangle: public polygon {
public:
    // Constructor just calls the base class
    rectangle(double width, double length) : polygon(width, length) { }
    // Override base class
    double area() const { return lengths[0] * lengths[1];}
};


#define PI 3.14159

class ellipse: public shape {
protected:
    double semimajor, semiminor;
public:
    ellipse(double smj=1.0, double smn=1.0) {
        semimajor = smj;
        semiminor = smn;
    }
    
    double area() const {
        return PI * semimajor * semiminor;
    }
};

class circle : public ellipse {
public:
    circle(double r = 1.0) : ellipse(r, r) { }
    
    
    // Don't override area(), but provide perimeter()
    double perimeter() const {
        return 2 * PI * semimajor; // "semimajor" is protected
    }
};


void printShapeInfo(shape& s) {
    cout << s.area() << " " << s.perimeter() << endl;
}

int mainEx() {
    circle c1(1);
    rectangle r1(1, 1);
    cout << c1.area() << " " << c1.perimeter() << endl;
    cout << r1.area() << " " << r1.perimeter() << endl;
    cout << "----------------" << endl;
    printShapeInfo(c1);
    printShapeInfo(r1);
    return 0;
}


// This method should return:
//3.14159 6.28318
//1 4
//----------------
//0 0
//0 0


/**********************************************************/

int main() {
    /**
     C++ considers objects of a derived class to be
     assignment compatible with objects of their base class.
     This just makes a copy, skipping members
     that aren’t part of the base class.
     **/
    circle c1(1);
    rectangle r1(1, 1);
    cout << c1.area() << " " << c1.perimeter() << endl;
    cout << r1.area() << " " << r1.perimeter() << endl;
    
    shape sh = c1; // semimajor and semiminor are not copied!!!
    
    cout << "----------------" << endl;
    cout << sh.area() << " " << sh.perimeter() << endl;
    
    //ellipse e1 = sh; //does not compile!
    
    // SAME APPLIES TO POINTERS AND REFERENCES
    shape s2, *ps2;
    ellipse e2, *pe2;
    ps2 = &e2; // OK
    //pe2 = &s2; // Error!
    
    /** POLYMORPHISM
     The ability to use base class pointers to refer
     to any of several derived objects is a key part of polymorphism.
     **/
    
    cout << ps2->area() << endl; // Calls the function shape::area() !!!!
    
    //SOLUTION: DECLARE FUNCTIONS TO BE VIRTUAL
    /** IN class shape,
     change 'double area() const { return 0; }'
     with 'virtual double area() const { return 0; }'
     **/
    
    /**
     A virtual function allows conditional overriding : only if the type signatures match.
     - Note: to call the base 'virtual' function, explicitly:
     **/
    
    cout << ps2->shape::area() << endl; // Calls the function shape::area() explicitly!!!!
    
    /**
     - CANNOT DECLARE VIRTUAL CONSTRUCTORS OR DESTRUCTORS
     **/
    
    
    // FUNCTIONS CAN BE 'PURELY' VIRTUAL
    /** IN class shape,
     change 'virtual double area() const { return 0; }'
     with 'virtual double area() = 0;
     **/
    
    //BUT IT gets much trickier than this: see STATIC VS. DYNAMIC DISPATCH
    
    
    return 0;
}





/** STATIC VS. DYNAMIC DISPATCH **/

/**
 By default (i.e. non-virtual), the function to be called
 is done STATICALLY (i.e. at compile time)
 **/

class AA {
public:
    void f() { cout << "AA::f()"; }
};


class BB : public AA {
public:
    void f() { cout << "BB::f()"; }
};

int mainAABB() {
    BB b;
    AA *pa = &b;
    pa->f(); // Calls AA::f() because pa is of type ’AA *’
    return 0;
}

/** The call on virtual function is dispached
 DYNAMICALLY (at run-time)
 **/

class CC {
public:
    virtual void f() { cout << "CC::f()"; }
};


class DD : public CC {
public:
    void f() { cout << "DD::f()"; }
};

int mainCCDD() {
    DD d;
    CC *pc = &d;
    pc->f(); // Calls DD::f() because pc points to a ’CC *’
    return 0;
}

/**
 CONSTRUCTORS AND DESTRUCTORS ALWAYS USE _STATIC_ DISPATCH
 **/


/***********************************************************************
 A base class can invoke virtual functions in a derived class,
 with no knowledge of the derived classes.
 **/

class EE {
public:
    EE() { cerr << "EE()\n"; f(); } // Always calls EE::f()!
    ~EE() { cerr << "∼EE()\n"; g(); } // Always calls EE::g()!
    virtual void f() { cerr << "EE::f()\n"; g(); } // Depends on context
    virtual void g() { cerr << "EE::g()\n"; }
};

class FF : public EE { public:
    virtual void g() { cerr << "B::g()\n"; } };

int mainEEFF() {
    FF f;
    EE *pe = &f;
    pe->f(); // Calls EE::f(), then FF::g()!
    return 0;
}









/*******************************************************/
// Assigned readings

// * Inheritance: http://www.cplusplus.com/doc/tutorial/inheritance/
// * Polymorphism: http://www.cplusplus.com/doc/tutorial/polymorphism/
