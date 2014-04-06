/**********************************************************/

/**** Lecture 11 - Templates and Operation Overloading ****/

/**********************************************************/


/* 
 Templates are a feature in C++ that allows generic programming: 
 
    - using types that will be specified later. 
    - still, by "later", we don't mean at runtime...
    - "later" = compile time, when an object is instantiated, 
        or when a function is called 
        (rather than inside function/class definition)
 
 Templates allow you to:
    - pass types as arguments
    - delay type checking
    - pass constant values as arguments: compile time computation
 */


template <int N, int M> class Matrix;
template <int N> class Vector;
template <int NR, int NC> Vector<NR> multiply(Matrix<NR,NC>& m, Vector<NC>& v);


// N - size of vector
template <int N>
class Vector {
public:
    Vector() {
        data = new double[N];
    }
    
    Vector(Vector<N>& other) {
        data = new double[N];
        for(int i=0; i<N; i++)
            data[i] = other.data[i];
    }
    
    
    Vector(const Vector<N>& other) {
        data = new double[N];
        for(int i=0; i<N; i++)
            data[i] = other.data[i];
    }
    
    ~Vector() { delete [] data;}
    int size() {return N;}
    
    double& elem(int i) {return data[i];}
private:
    double* data;
};

// N - number of rows, M - number of columns
template <int N, int M>
class Matrix {
public:
    Matrix() {
        data = new double*[N];
        for (int i=0; i<N; i++) {
            data[i] = new double[M];
        }
    }
    ~Matrix() {
        for (int i=0; i<N; i++) {
            delete [] data[i];
        }
        delete [] data;
    }
    
    int rows() {return N;}
    int cols() {return M;}
    double& elem(int i, int j) { return data[i][j]; }
    
    Vector<N> operator()(const Vector<M>& v) {
        Matrix<N, M>& m = *this;
        Vector<M> v2 = v;
       return multiply(m, v2);
    }
private:
    double** data;
};


template <int R, int C>
Vector<R> multiply(Matrix<R,C>& m, Vector<C>& v) {
    Vector<R> r;
    for (int i=0; i < m.rows(); i++) {
        r.elem(i) = 0;
        for (int j=0; j < m.cols(); j++) {
            r.elem(i) += m.elem(i,j) * v.elem(j);
        }
    }
    return r;
}


#include <iostream>
using namespace std;


int main0() {
    Vector<3> v;
    Vector<2> v2;
    Matrix<4,3> m;
    Vector<4> v3 = multiply(m,v); //valid
    
    m(v); //valid as well
    //m(v2); // invalid AND does not compile
    
    Vector<8-4> v4 = v3; //valid!
    const int z = 4;
    Vector<z> v5 = v3; //valid!
    return 0;
}


/* Example 2: Compile time computation*/

template <int R>
class Circle {
    const double PI = 3.14;
public:
    Circle() {}
    ~Circle() {}
    
    const double area = PI * R * R;
    const double circ = 2 * PI * R;
};



/* Containers: taking abstraction to another level
        - how does vector<int> and vector<double> and vector<list<int>> work??
        - is it a C++ standard library construct? can we do the same??
        - we NEED to make have the type of elements in the container come as input
 */

/* Non-generic implementation for Stack of doubles */

class stack_silly {
private:
    double* storage;
    int size;
    int top;
public:
    stack_silly(int n=100);
    ~stack_silly();
    
    double pop();
    void push(double);
    
};


/* What if I want a stack of Matrices? Generic programming!*/

template <typename T>
class Stack {
private:
    T* storage;
    int size;
    int top;
public:
    Stack(int n=100) {
        storage = new T[n];
        size = n;
        top = -1;
    }
    ~Stack() {
        delete [] storage;
    }
    
    T pop() {
        if (top >= 0) {
            return storage[top--];
        }else return 0;
    }
    
    void push(T);

};

template<typename T>
void Stack<T>::push(T x) {
    if (top >= size-1) {
        T* new_storage = new T[2*size];
        for (int i=0; i<size; i++) {
            new_storage[i] = storage[i];
        }
        delete [] storage;
        storage = new_storage;
    }
    storage[++top] = x;
}



#include <string>

int main2() {

    Stack<string> sstack;
    sstack.push("world");
    sstack.push("hello");
    cout << sstack.pop() << endl; // Print hello
    
    typedef string sss;
    Stack<sss>& new_stack = sstack; //valid!!!
    
    
    return 0;
}



/*
    Class templates: these are *instantiated* : template code is generated on demand
 
        *CODE ON DEMAND*
 
        Stack<int> istack; // Generates code for integer stack
        Stack<double> istack; // Generates code for double stack
 
        - if you don't declare a Stack<Matix>, no code is generated for this specific type!!!
 
        - generates classes as demanded at COMPILE TIME
 */


/* Algorithms on containers : simplified typechecking */



template <typename C, typename P>
int count(const C& c, P pred) {
    int cnt = 0;
    for(const auto& x : c) { //we don't know if this is a container!
        if(pred(x)) ++cnt; //we don't know if pred returns a boolean!
    }
    return cnt;
}


template <int N>
bool less_than(int z) { return z < N; }





template <typename T>
class Less_than {
    const T val; //val to compare against
public:
    Less_than(const T& v) : val(v) {}
    
    bool operator()(const T& x) const { return x < val; } // call operator
};


#include <list>

int main() {
    list<int> l;
    l.push_back(3);
    l.push_back(5);
    l.push_back(6);
    l.push_back(10);
    
    cout << count(l,less_than<4>) <<  " " << count(l,less_than<7>) << endl;

    list<string> l2;
    l2.push_back("3");
    l2.push_back("5");
    l2.push_back("6");
    l2.push_back("10");
    
    cout << "-------------------" << endl;
    
    
    cout << count(l,Less_than<int>{4}) <<  " " << count(l, Less_than<int>{7}) << endl;
    
    cout << count(l2,Less_than<string> {"4"}) <<  " " << count(l2, Less_than<string>{"7"}) << endl;
    
    return 0;
}

/* Templates: other aspects
    - By default, the full template definition must be accessible from any compilation unit (source file) that uses it.
    - Often, this means the entire template definition is placed in a “.h” file.
    - This may expose the implementation, or require extra information to be included during compilation.
 
 
    - Inheritance is not preserved across templates:

template <typename T> class A { ... };

class B {  ...  };
class C : public B {  ...  };

int main5() {
    B b;
    C c;
    A<B> ab;
    A<C> ac;
    b = c; // legal, as B is derived from C
    ab = ac; // error!
    return 0;
}

*/



/*******************************************************/
// Assigned readings


// * Templates: http://www.cplusplus.com/doc/oldtutorial/templates/
// * Operator overloading : http://en.cppreference.com/w/cpp/language/operators





