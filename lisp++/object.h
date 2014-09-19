//
//  object.h
//  lisp++
//
//  Created by evil on 9/7/14.
//  Copyright (c) 2014 evilbinary.org. All rights reserved.
//  rootntsd@gmail.com

#ifndef __lisp____object__
#define __lisp____object__

#include <iostream>
#include <string>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

using namespace std;
#define DEBUG_INFO cout<<"["<<__FILE__<<" "<<__FUNCTION__<<" "<<__LINE__<<"]" <<endl;
#define error(X) do { fprintf(stderr, "%s\n", X); exit(1); } while (0)

class Object;

typedef Object* (*FUNCTION)(Object *args) ;

typedef enum ObjectType{
    INT,
    SYM,
    CONS,//PAIR
    PROC,
    PRIMOP,
}OType;



class Object{

public:
    static Object * eval(Object *exp,Object *evn);
    static Object * evlis(Object *exps,Object *env);
    static Object * apply(Object *prog,Object *args,Object *env);
    static Object * lookup(Object *id,Object* env);
    static Object * update(Object *id,Object *value,Object* env);
    static Object * extend(Object *vars,Object* vals,Object* env);
    static Object * extend(Object *kvpair,Object* env);
    
    
    Object(OType type,int count,...);
    Object(char *name);
    Object(OType type,void* data);
    Object();
    ~Object();
    
    
    //global object
    static Object *nil;
    static Object *symbols;
    static Object *topEnv;
    static Object *symLambda;
    static Object *symDefine;
    static Object *symIf;
    static Object *symQuote;
    static Object *symSetb;
    static Object *tee;
    static Object *fee;
    static Object *symBegin;
    
    static Object *globalEnv;
    
    
    
    //static method
    static void init();
    static bool isnil(Object *obj);
    static Object *car(Object *obj);
    static Object *cdr(Object *obj);
    static Object *setcdr(Object *obj1,Object *obj2);
    static Object *setcar(Object *obj1,Object *obj2);
    
    static Object *cons(Object* obj1,Object *obj2);
    static Object *mksym(char *name);
    static char*  symname(Object *obj);
    static Object* mkobj(OType type,int count,...);
    static Object * mkpriop(FUNCTION fun);
    static Object * mkproc(Object *args,Object *body,Object* env);
    static Object * mkproc(char* name,Object *args,Object *body,Object* env);
    
    static Object *inter(char *name);
    static Object *findsym(char* name);
    static size_t intval(Object *obj);
    static Object *mkint(size_t val);
    
    //primop buildin function
    static Object *sum(Object *args);
    static Object *sub(Object *args);
    static Object *mul(Object *args);
    static Object *div(Object *args);
    
    static Object *primnumeq(Object *args);
    static Object * primnumgt(Object *args);
    static Object * primnumlt(Object *args);
    static Object *primcons(Object *args);
    static Object *primcar(Object *args) ;
    static Object *primcdr(Object *args);
    //method
    void print();
    void tprint();
    void dprint();
    void dprint(Object *o);
    ostream& dstream(ostream &os,Object *o);
    ostream& dstream(ostream &os);
                            
    

    
    Object *car();
    Object *cdr();
    Object *cons(Object *obj1);
    bool isnil();
    Object *setcdr(Object *obj1);
    Object *setcar(Object *obj1);
    
//    Object *caar();
//    Object *cdar();
    char*  symname();

    
    friend istream& operator>>(istream &cin,Object &obj);
    friend ostream& operator<<(ostream &cout,Object&obj);
    friend ostream& operator<<(ostream &cout,Object* obj);

private:
    //data
    OType type;
    void *data;
    Object **obj;
    bool isalive;
    
};





#endif /* defined(__lisp____obj__) */
