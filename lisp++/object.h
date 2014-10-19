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
#include <memory>
#include "memory.h"


using namespace std;
#define DEBUG_INFO cout<<"["<<__FILE__<<" "<<__FUNCTION__<<" "<<__LINE__<<"]" <<endl;
#define error(X) do { fprintf(stderr, "%s\n", X); exit(1); } while (0)

#define BUFFER_SIZE 256

class Object;
class Memory;

typedef Object* (*FUNCTION)(Object *args) ;

typedef enum ObjectType{
    INT,//1
    SYM,//
    CONS,//PAIR 2
    PROC,//
    PRIMOP,//
    STRING,//
}OType;


class Object{

public:
     Object * eval_tail_call(Object *exp,Object *env);
//    static std::auto_ptr < IAnswer<Object*> >  eval2(Object *exp,Object *evn);
//    static std::auto_ptr < IAnswer<Object*> > evlis2(Object *exps,Object *env);
//    static std::auto_ptr < IAnswer<Object*> > apply2(Object *prog,Object *args,Object *env);
//    

     Object * eval2(Object *exp,Object *evn);
     Object * evlis2(Object *exps,Object *env);
     Object * apply2(Object *prog,Object *args,Object *env);

    
    Object * eval3(Object *exp,Object *evn);
    Object * evlis3(Object *exps,Object *env);
    Object * apply3(Object *proc,Object *args,Object *env);
    
    //Object * apply2(Object *proc,Object *args,Object *env);
    
    static Object * eval(Object *exp,Object *evn);
    static Object * evlis(Object *exps,Object *env);
    static Object * apply(Object *prog,Object *args,Object *env);
    static Object * lookup(Object *id,Object* env);
    static Object * values(Object *id,Object *env);
    
    static Object * update(Object *id,Object *value,Object* env);
    static Object * extend(Object *vars,Object* vals,Object* env);
    static Object * extend(Object *kvpair,Object* env);
    
    static Object *expand_clauses(Object *clauses);
    static Object *sequence_to_exp(Object *seq);

    
    static Object * assoc(Object *x,Object *y);
    
    
    
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
    static Object *symAtom;
    static Object *symSetb;
    static Object *tee;
    static Object *fee;
    static Object *symBegin;
    static Object *none;
    static Object *symCond;
    static Object *symElse;
    static Object *symEq;
    static Object *symCar;
    static Object *symCdr;
    
    static Object *symLf;
    static Object *symCr;
    static Object *symCrLf;
    
    static Object *symCons;
    static Object *symLabel;
    static Object *symAnd;
    static Object *symOr;
    static Object *symLet;
    static Object *symStatus;

    
    
    static Object *globalEnv;
    static Object *callExp;
    static Object *callEnv;

    static Memory *memory;

    
    
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
 
    static Object * mkobj(OType type,int count,...);
    static Object * mkobj(char*name);
    static Object * mkobj(OType type,void* data);

    static Object * mkpriop(FUNCTION fun);
    static Object * mkproc(Object *args,Object *body,Object* env);
    static Object * mkproc(char* name,Object *args,Object *body,Object* env);
    
    static Object *inter(char *name);
    static Object *findsym(char* name);
    static size_t intval(Object *obj);
    static Object *mkint(size_t val);
    static Object *mkstring(char* name);
    
    //primop buildin function
    static Object *sum(Object *args);
    static Object *sub(Object *args);
    static Object *mul(Object *args);
    static Object *div(Object *args);
    
    static Object *prim_numeq(Object *args);
    static Object * prim_numgt(Object *args);
    static Object * prim_numlt(Object *args);
    static Object *prim_cons(Object *args);
    static Object *prim_car(Object *args) ;
    static Object *prim_cdr(Object *args);
    static Object *prim_or(Object *args);
    static Object *prim_and(Object *args);

    
    static Object *add_procedure(char *name,FUNCTION function);

    //method
    void print();
    void tprint();
    void dprint();
    void dprint(Object *o);
    ostream& dstream(ostream &os,Object *o);
    ostream& dstream(ostream &os);
                            
    

    Object *eval(Object *env=NULL);
    Object *cond(Object *env=NULL);
    
    Object * execute(Object *env=NULL);
    
    Object *atom();
    Object *car();
    Object *cdr();
    Object *cons(Object *obj1);
    bool isnil();
    bool is_pair();
    bool is_atom();
    bool is_null();
    bool is_nil();
    bool is_alive();
 
    
    Object *setcdr(Object *obj1);
    Object *setcar(Object *obj1);
    
    ObjectType gettype(){ return this->type;}
    void settype(ObjectType t){ this->type=t;}
    
//    Object *caar();
//    Object *cdar();
    char*  symname();
 
    
    friend istream& operator>>(istream &cin,Object &obj);
    friend ostream& operator<<(ostream &cout,Object&obj);
    friend ostream& operator<<(ostream &cout,Object* obj);
    
    int flag;// for mark gc

    
private:
    //data
    void *data;
    Object **obj;
    bool isalive;
    OType type;
};





#endif /* defined(__lisp____obj__) */
