/**/
// Item 3. Prefer new and delete to malloc and free

string *stringarray1 = static_cast<string*>(malloc(10*sizeof(string)));//allocate enough memory for 10 string objects, but none of them is constructed. What is worse is that we have no way to initialize these ten objects.
string *stringarray2 = new string[10];//10 constructed string objects are allocated.

delete stringarray1; // no destructor has been called and the memory alloceted in the object will be lost.
delete[] stringarray2; //

//Mixing new and delete with malloc and free is usually a bad idea.


char * strdup(const char* ps);//return copy of what ps points to

// new and delete inside and outside of clesses give rise to all kinds of opportunities to make mistake

// Item 5. Use the same form in corresponding uses of new and delete
string *stringarray = new string[100];
delete[] stringarray;
string *stringarray2 = new string;
delete stringarray2;

typedef string AddressLines[4];// a person's address has four lines, each of which is a string.
string *pal = new AddressLines;// retrns a "string *", just like "new string[4]" would
delete[] pal;
// to avoid such confusion, you're probably best off abstaining from typedefs for array types. A good solution is that replacing something like AddressLines with a vector of strings;

// Item 6. Use delete on pointer members in destructors
// for the pointer member defined in the class, we are required to do each of these three things:
// 1. Initializion of the pointer in each of the constructors, if no memory is to be allocated to the pointer in a particular constructor, the pointer should be initialized to 0
// 2. Deletion of the existing memory and assignment of new memory in the assignment operator
// 3. Deletion of the pointer in the destructor.

// an alternative solution to this case is that using smart pointer.

// Item 7. Be prepared for out-of-memory conditions
// Defining a Macro for new in this way, like
#define NEW(PTR, TYPE) \
try { (PTR) = new TYPE;} \
catch (std::bad_alloc&) {assert(0);}
// the assert is a macro, which suffers from the common errors of using assert to test condition that might occur in production code.

// three basic ways to use the new operator:
new T;
new T(constructor arguments);
new T[size];
// program can define his own version of "new"

typedef void(*new_handler)(); // define a function pointer
new_handler set_new_handler(new_handler p) throw();

// the way to use set_new_handler
void noMoreMemory(){
	cerr << "Unable to satisfy request for memory";
	abort();
}
int main(){
	set_new_handler(nonMoreMeory);
	int *pBigDataArray = new int[100000000];
}

// when operator new can cannot satisfy a request for memeory, it calls the new_handler function not once, but repeatedly until it can find enough memory. There are several method to solve or avoid this bad situation.

/*
1. Make more memory available. This may allow operator new's next attempt to allocate the memory to succeed. One way to implement this strategy is to allocate a large block of memory at program start-up, the release it the first time the new_handler is invoked. Such a release is often accompanied by some kind of warning to the user that memeory is low and that future requests may fail unless more memory is somehow make available.
2. Install a different new-handler. If the current new-handler can't make any more memor available, perhaps it knows of a different new-handler that is more resourceful. If so, the current new-hanlder can in stall the other new-handler in its place(by calling set_new_handler). The next time operator new calls the new-handler functoin, it will get the one most recently installed. (A variation on this theme is for new-handler to modify its own behavior, so the next time it's invoked, it does some thing different. One way to achieve this is to have the new-handler modify static or global data that affects the new-handler's behavior).
3. Deinstall the new-handler, i.e., pass the null pointer to set_new_handler. With no new_handler installed, operator new will throw an exception of type std::bad_alloc when its attempt to allocate memory is unsuccessful.
4. Throw an exception of type std::bad_alloc or some type derived from std::bad_alloc. Such exceptions wil not be caught by operator new, so they will propagate to the site originating the request for memory.(Throwing an exception of a different type will violate operator new's exception specification. The default action when that happens is to call abort, so if your new-handler is going to throw an exception, you definitely want to make sure it's from the std::bad_alloc hierarchy.)
5. Not Return, typically by calling abort or exit, both of which are found in standard C library and thus in the standard C++ library.
*/

//class specific new-handlers
class X{
public:
	static new_handler set_new_handler(new_handler p){
		new_handler oldHandler = currentHandler;
		currentHandler = p;
		return oldHandler;
	};
	static void * operator new(size_t size);
private: 
	static new_handler currentHandler;
}

//Static class members must be defined out side the class definition
new_handler X::currentHandler;// sets currentHandler to 0 (i.e., null) by default
void * X::operator new(size_t size){
	new_handler globalHandler = std::set_new_handler(currentHandler);
	void *memory;
	try{
		memory = ::operator new(size);
	}
	catch (std::bad_alloc&){
		std::set_new_handler(globalHandler);
		throw;//propagte exception
	}
	std::set_new_handler(globalHandler);
	return memeory;
}

//Clients of class X use its new-handling capacities like this:
void noMoreMemory();
X::set_new_handler(noMoreMemory);
X *px1 = new X;
string *ps = new string;
X::set_new_handler(0);
X *px2 = new X;

template<class T>
class NewHandlerSupport{
public:
	static new_handler set_new_handler(new_handler p);
	static void * operator new(size_t size);
private:
	static new_handler currentHandler;
}

template<class T>
new_handler NewHandlerSupport<T>::set_new_handler(new_handler p){
	new_handler oldHandler = currentHandler;
	currentHandler = p;
	return oldHandler;
}

template<class T>
new_handler void * new(size_t size){
	new_handler globalHandler = std::set_new_handler(currentHandler);
	void *memory;
	try{
		memory = ::operator new(size);
	}
	catch (std::bad_alloc){
		std::set_new_handler(globalHandler);
		throw;
	}
	std::set_new_handler(globalHandler);
	return memory;
}
template<T> new_handler NewHandlerSupport<X>::currentHandler;

class X: public NewHandlerSupport<X>{
}

// normal new and nonthrow new
new (nothrow) BaseClass;

// Item 8: Adhere to convetion when writing operator new and operator delete
void * operator new(size_t size){
	if(size == 0){
		size = 1;
	}
	while(1){
		//attempt to allocate size bytes;
		if(the allocation was successful)
			return (a pointer to the memory);
		
		// allocation is unsuccessful; find out what the current error-handling function is (see Item7)
		new_handler globalHandler = set_new_handler(0);
		set_new_handler(globalHandler);
		
		if(globalHandler) (*globalHandler)();
		else throw std::bad_alloc();
	}
}


// class-specific operator new in the base class will be inherited by its derived class
void * Base::operator new(size_t size){
	if(size != sizeof(Base)){
		return ::operator new(size);
	}
}

// non-member operator
void delete(void * rawMemoryS){
	if(rawMemory == 0) return;
	//deallocate the memory pointed to by rawMemory
	
	return;
}


class Base{
public:
	static void *operator new(size_t size);
	static void operator delete(void* rawMemorys, size_t size);
}

void Base::operator delete(void *rawMemory, size_t size){
	if(rawMemory == 0) return;
	
	if(size != sizeof(Base)){
		::operator delete(rawMemory);
		return;
	}
	
	//deallocate the memory pointed to by rawMemory
	
	return;
}


// The conventions, then, for operator new and delete(and their array counterparts) are not particularly onerous, but it is important that you obey them. If your allocation routines support new handlers functions and correctly deal with zero-sized requests, you're all but finished, and if your deallocation routines cope with null pointers, there's little more than to do. Add support for inheritance in member versions of the functions, and presto!? you're done.


// Item 9. Avoid hiding the "normal" form of new
//one solution to write a class-specific operator new that support the "normal" invocation form.
class X{
public:
	void f();
	static void* operator new(size_t size, new_handler p); 
	static void* operator new(size_t size){
		return ::operator new(size);
	}
};
X *px1 = new (specialErrorHandler) X;
X *px2 = new X;

//an alternative way to provide a default parameter value for each additional parameter you add to operator new
class X{
public:
	void f();
	static void* operator new(size_t size, new_handler p = 0); 
	static void* operator new(size_t size){
		return ::operator new(size);
	}
};
X *px1 = new (specialErrorHandler) X;
X *px2 = new X;

//Item 10. Write operator delete if you write operator new
class Airplane{
public: 
	static void* operator new(size_t size);
	static void operator delete(void *deadObject, size_t size);
private:
	union{
		AirplaneRep *p;
		Airplane *next;
	}
	static const int BLOCK_SIZE;
	static Airplane *headOfFreeList;
};

void *Airplane::operator new(size_t size){
	if(size != sizeof(Airplane))
		return ::operator new(size);
		
	Airplane *p = headOfFreeList;
	if(p)
		headOfFreeList = p->next;
	else{
		Airplane *newBlock = static_cast<Airplane*>(::operator new(BLOCK_SIZE*sizeof(Airplane)));
		for(int i = 0; i < BLOCK_SIZE-1; i++)
			newBlock[i].next = &newBlock[i+1];
		newBlock[BLOCK_SIZE-1].next = 0;
		p = newBlock;
		headOfFreeList = &newBlock[1];
	}
	return p;
};

void Airplane::operator delete(void *deadObject, size_t size){
	if(deadObject == 0) return;
	if(size != sizeof(Airplane)){
		::operator delete(deadObject);
		return;
	}
	Airplane *carcass = static_cast<Airplane*>(deadOjbect);
	carcass->next = headOfFreeList;
	headOfFreeList = carcass;
};

Airplane *Airplane::headOfFreeList;
const int Airplane::BLOCK_SIZE = 512;

class Pool{
public:
	Pool(size_t n);
	void *alloc(size_t n);
	void free(void* p, size_t n);
	~Pool();
private:
	static Airplane* headOfTotalList;
	static Airplane* headOfFreeList;
	static const void* SIZE;
	size_t block_size;
};

Pool::Pool(size_t n){
	if(headOfTotalList != 0)
		return;
	block_size = n;
	void *memBlock
	headOfTotalList = ::operator new(block_size*SIZE);
	
}

Pool::~Pool(){
	::operator delete(headOfTotalList);
	headOfTotalList = 0;
}

void *Pool::alloc(size_t n){
	if(headOfFreeList == NULL){
		void* p = ::operator new(n);
		p->next = headOfFreeList;
	}
}

void Pool::free(void *p, size_t n){
	if(p == NULL) return;
	memBlock *carcass = static_cast<memBlock*>(p);
	carcass->next = headOfFreeList;
};

Airplane * Pool::headOfFreeList;
Airplane * Pool::headOfTotalList;
const int Pool::SIZE = 512;

class Airplane{
public:
	static void* operator new(size_t size);
	static void operator delete(Airplane* p, size_t size);
private:
	AirplaneRep * rep;
	static Pool memPool;
};
inline void* Airplane::operator new(size_t size){
	return memPool.alloc(size);
};
inline void Airplane::operator delete(Airplane *p, size_t size){
	memPool.free(p, size);
}

Pool Airplane::memPool(sizeof(Airplane));
















































#ifndef __MEMPOOL
#define __MEMPOOL
#include <iostream>
#include <list>
#include <string>

#ifndef _DEBUG
#define _DEBUG
#endif
//#undef _DEBUG
using namespace std;
class airplane;

class MemPool
{
public:;
    explicit MemPool(int size = 4, char *name = NULL, int num = 100);    
    ~MemPool();   
    void * alloc(int n);  
    void free(void *p, int n);    
                                                   
   
    void debugInfo(void);
private:;
    string poolName;
    int blockSize; 
    int blockScale;  
    list<void *>freeList;
    list<void *>usedList;
private:;
    MemPool(MemPool &a);   
    MemPool &operator=(const MemPool &a);
    void allocNewFreeList(int _size);
};
#endif
 
MemPool.cpp文件:
 
#include "MemPool.h"
#include <algorithm>


MemPool::MemPool(int size, char *name, int num)
:blockSize(size), poolName(name), blockScale(num)
{
    #ifdef _DEBUG
    printf("MemPool::MemPool called: blockSize: %d, blockScale: %d\n", blockSize, blockScale);
    #endif
   
    allocNewFreeList(blockSize);
}
MemPool::~MemPool()
{
    #ifdef _DEBUG
    printf("MemPool::~MemPool called\n");
    #endif
    list<void *>::const_iterator it = freeList.begin();
   
    for (; it != freeList.end(); ++it)
    {
        ::operator delete(*it);
    }
    freeList.clear();
    if (!usedList.empty())
    {
       
        printf("Be careful! Some errors occures!\n");
        for (; it != usedList.end(); ++it)
        {
            ::operator delete(*it);
        } 
        freeList.clear();
    }
}
void * MemPool::alloc(int size)
{
    #ifdef _DEBUG
    printf("MemPool::alloc called: size: %d\n", size);
    #endif
   
    if (size != blockSize)
    {
        return ::operator new(size);
    }
   
    if (freeList.empty())
    {
       
        allocNewFreeList(blockSize);
    }
    list<void *>::iterator it = freeList.begin();
    void *p = *it;
    freeList.pop_front();
    usedList.push_back(p);
   
    return p;
}
void MemPool::free(void *deadobject, int size)
{
    #ifdef _DEBUG
    printf("MemPool::free called: size: %d\n", size);
    #endif
   
    if (NULL == deadobject)
    {
        return;         // 见条款 8
    }
    if (size != blockSize)
    {
        ::operator delete(deadobject);      // 见条款 8
        return;
    }
    list<void *>::iterator it = find(usedList.begin(), usedList.end(), deadobject);
    if (it != usedList.end())
    {
        usedList.erase(it);
        freeList.push_back(deadobject);
    }
    else
    {
       
    }
   
}
void MemPool::debugInfo(void)
{
    cout << "pool name: " << poolName << ", ";
    cout << "block size: " << blockSize<< ", ";
    cout << "total num: " << freeList.size() + usedList.size() << ", ";
    cout << "used num: " << usedList.size() << endl;
}
void MemPool::allocNewFreeList(int _size)
{
    void *newBlock;
    for (int i = 0; i < blockScale; i++)
    {
        newBlock = ::operator new(_size);
        freeList.push_back(newBlock);
    }
}
 
测试文件test.cpp:
#include "MemPool.h"
class airplane
{
public:;
    // 普通airplane功能
                                  
    airplane(){}
    virtual ~airplane(){}
    static void * operator new(size_t size);
    static void operator delete(void *p, size_t size);
   
private:
    char *rep;                 // 指向实际描述的指针
    string test;
    static MemPool memMemPool;              // airplanes的内存池
};
inline void * airplane::operator new(size_t size)
{
    memMemPool.debugInfo();
    void *tmp = memMemPool.alloc(size);
    return  tmp;
}
inline void airplane::operator delete(void *p, size_t size)
{
    memMemPool.free(p, size);
    memMemPool.debugInfo();
}
MemPool airplane::memMemPool(sizeof(airplane), "airplane", 4);
int main(void)
{
    #define TIMES 12
    airplane *pAir[TIMES];
    //printf("main: size = %d\n", sizeof(airplane));
    for (int i = 0; i < TIMES; i++)
    {
        //printf("times: %d    ", i);
        pAir[i] = new airplane;
    }
    for (i = 0; i < TIMES; i++)
    {
        //printf("times: %d    ", i);
        delete pAir[i];
    }
    for (i = 0; i < TIMES; i++)
    {
        //printf("times: %d    ", i);
        pAir[i] = new airplane;
    }
    for (i = 0; i < TIMES; i++)
    {
        //printf("times: %d    ", i);
        