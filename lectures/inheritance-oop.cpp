/***************************************************/
/**** Lecture 08 - Classes, OOP and Inheritance ****/
/***************************************************/

// * _Inheritance_ refers to our ability to create a hierarchy of classes, in which derived classes (subclass) automatically incorporate functionality from their base classes (superclass).
// A derived class inherits all of the data and methods from its base class.
// * A derived class may override one or more of the inherited methods.
// * Most of the utility of classes and objects comes from the creation of class hierarchies.

#include <iostream>

using namespace std;


class A { // base class
private:
    int x; // Visible only within ’A’
protected:
    int y; // Visible to derived classes
public:
    int z; // Visible globally
    A(); // Constructor
    ~A(); // Destructor
    void f(); // Example method
};

class B : public A { // B is derived from A private:
private:
    int w; // Visible only within ’B’
public:
    B(); // Constructor
    ~B(); // Destructor
    void g() {
        w = z + y;  // OK
        f();        // OK
        //w = x + 1;  // Error - ’x’ is private to ’A’
    }
};

/*Implementations*/
A::A() {}
A::~A() {}
void A::f() {}
B::B() {}
B::~B() {}


/*****************************************************/
// Public inheritance
// * The use of the *public* keyword is the norm although in some rare circumstances you will use private or protected.
// * If you omit the *public* keyword, inheritance is *private*.


class C {
public:
    void f();
};

class D: C { // B inherits A privately
public:
    void g();
};


int mainCD() {
    C c;
    D d;
    c.f(); // OK
    d.g(); // OK
    // d.f(); // Illegal
    return 0;
}

/*Implementations*/
void C::f() {}
void D::g() {}

/******************************************************************/

class E {
public:
    void f_call(int x) {
        cout << "E::f_call(" << x << ")\n";
    }
};

class F : public E {
public:
    void f_call(int x) {
        E::f_call(1); // uncomment this to see another example
        cout << "- F::f_call(" << x << ")\n";
    }
};

int mainEF() {
    E e;
    cout << "--------" << endl;
    F f;
    cout << "--------" << endl;
    e.f_call(1);
    cout << "--------" << endl;
    f.f_call(2);
    cout << "--------" << endl;
    return 0;
}

// The main program should print:
// E::f_call(1)
// - F::f_call(2)


/******************************************************************/
/*********     INHERITANCE AND CONSTRUCTORS     ********/


class G {
public:
    G() { cout << "G()\n"; }
    ~G() { cout << "~G()\n"; }
    void f_call() { cerr << "G::f_call()\n"; } // Not special
};

class H: public G {
public:
    H() { cout << "- H()\n"; }
    ~H() { cout << "- ~H()\n"; }
    void f_call() { cerr << "- H::f_call()\n"; }
};

int mainGH() {
    G g;
    cout << "--------" << endl;
    g.f_call();
    cout << "--------" << endl;
    H h;
    cout << "--------" << endl;
    h.f_call();
    cout << "--------" << endl;
    return 0;
    //destructors
}


// this main function should produce the following output
//G()
//--------
//G::f_call()
//--------
//G()
//- H()
//--------
//- H::f_call()
//--------
//- ~H()
//~G()
//~G()


/***************************/
// Implicit vs. explicit constructor invocation

class I {
public:
    I() { cout << "I()\n"; }
    I(int x) { cout << "I("<< x << ")\n"; }
    ~I() { cout << "~I()\n"; }
    void f_call() { cerr << "I::f_call()\n"; } // Not special
};

class J: public I {
public:
    J() { cout << "- J()\n"; }
    //J(int x=2) I() { cout << "- J("<< x << ")\n"; } // implicit
    J(int x=2) : I(x) { cout << "- J("<< x << ")\n"; } // explicit
    ~J() { cout << "- ~J()\n"; }
    void f_call() { cerr << "- J::f_call()\n"; }
};


int main() {
    J j(3);
    return 0;
}


/*********************************************/
// SHAPE EXAMPLE

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
    
    polygon(double width=1.0, double height=1.0);
    polygon(int n, double *len);
    ~polygon() { delete [] lengths; }
public:
    double perimeter() const { // Override base class
        double p = 0.0;
        for (int i = 0; i < nsides; i++) p += lengths[i];
        return (p);
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
    // Don’t override area(), but provide perimeter()
    double perimeter() const {
        return 2 * PI * semimajor; // ‘‘semimajor’’ is protected
    }
};


void printShapeInfo(shape& s) {
    cout << s.area() << " " << s.perimeter() << endl;
}

int mainShape() {
    circle c1(1);
    rectangle r1(1, 1);
    cout << c1.area() << " " << c1.perimeter() << endl;
    cout << r1.area() << " " << r1.perimeter() << endl;
//    cout << "----------------" << endl;
//    printShapeInfo(c1);
//    printShapeInfo(r1);
    return 0;
}


// This method should return:
//3.14159 6.28318
//1 4



/**********************************************************/

int mainPoly() {
    circle c1(1);
    rectangle r1(1, 1);
    cout << c1.area() << " " << c1.perimeter() << endl;
    cout << r1.area() << " " << r1.perimeter() << endl;
    shape sh = c1;
    //ellipse e1 = sh; //does not compile!
    cout << "----------------" << endl;
    cout << sh.area() << " " << sh.perimeter() << endl;
    return 0;
}


int mainShapeVirt() {
    circle c1(1);
    rectangle r1(1, 1);
    printShapeInfo(c1);
    printShapeInfo(r1);
    cout << "----------------" << endl;
    shape sh = c1;
    cout << c1.area() << " " << c1.perimeter() << endl;
    return 0;
}







/**Method implementation**/

polygon::polygon(double width, double height) {
    nsides = 4;
    lengths = new double[4];
    lengths[0] = width;
    lengths[1] = height;
    lengths[2] = width;
    lengths[3] = height;
}

polygon::polygon(int n, double *len) {
    nsides = n;
    lengths = len;
}


/*******************************************************/
// Assigned readings

// * Inheritance: http://www.cplusplus.com/doc/tutorial/inheritance/

