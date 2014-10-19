//
//  gc.h
//  lisp++
//
//  Created by evil on 10/18/14.
//  Copyright (c) 2014 evilbinary.org. All rights reserved.
//

#ifndef __lisp____memory__
#define __lisp____memory__

#include <iostream>
#include "object.h"
#include <memory>
#include <map>
#include <list>

using namespace std;


#define MAX_OBJ 1000

class Object;

typedef  std::auto_ptr< Object > ObjPtr;
//#define  alloc(size) new

typedef enum MemState{
    INIT,
    CLEAR,
    MARK,
}MemState;


class Memory{
private:
    map<int *,int> mempool;
    map<Object *,int> objs;
    int threshold;
    static list<Object*> roots;
    static Memory* memory;
public:
    static Memory *instance(){if(memory==NULL) memory=new Memory();return memory;}
    static void add_root(Object *obj){roots.push_back( obj);}
    Memory();
    
    
    void init();
    void * alloc(size_t size);
    void ** allocs(size_t size, int num);
    Object * new_obj();
    Object ** new_objs(int count);
    Object * add_obj(Object *);
    Object ** add_objs(Object **o,int count);
    void del_obj(Object *obj);
    void remove_obj(Object *obj);
    void print_root();
    
    void mark_obj(Object *obj);
    void mark();
    void sweep();
    void gc();
};

#endif /* defined(__lisp____gc__) */
