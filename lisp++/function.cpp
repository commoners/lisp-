//
//  function.cpp
//  lisp++
//
//  Created by evil on 10/17/14.
//  Copyright (c) 2014 evilbinary.org. All rights reserved.
//

#include "function.h"
#include <sstream>
#include <iostream>
#include "parser.h"

using namespace std;

void resist_functions(){
    Object::add_procedure("display", display);
    Object::add_procedure("newline", newline);
//    Object::add_procedure("trace", trace);
    Object::add_procedure("not", notf);
    Object::add_procedure("pair?", is_pair);
    
    Object::add_procedure("sys:top-env", get_top_env);
    Object::add_procedure("sys:lookup", lookup);
    Object::add_procedure("sys:find", lookup);
    Object::add_procedure("sys:update", update);
    
    Object::add_procedure("load", load);
    Object::add_procedure("symbol?", is_symbol);
    Object::add_procedure("procedure?", is_procedure);
    Object::add_procedure("null?", is_null);
    Object::add_procedure("integer?", is_interger);
    Object::add_procedure("boolean?", is_boolean);
    Object::add_procedure("real?", is_real);
    Object::add_procedure("eq?", is_eq);
    Object::add_procedure("atom?", is_atom);

    



}

Object *load(Object *args){
//    cout<<"   load args:";args->dprint();

    Object *file=args->car();
    char *name=file->symname();
    
    ifstream istrm(name);
    if(istrm.fail()){
        cout<<"file "<<file->symname()<<" not exit."<<endl;
        return Object::none;
    }
    Object::init();
    Parser p;
    while(true){
        Object *o;
        Object *ret;
        o=p.parse(istrm);
        //Memory::add_root(o);
        //Memory::add_root(env);
        //                cout<<"     @"<<o<<endl;
        if(istrm.peek()==EOF&&o==Object::nil){
            break;
        }
        ret=o->eval(o,Object::nil);
        //delete ret;
    }
    istrm.close();
    
    return Object::none;
}

Object *get_top_env(Object*args){
    return Object::topEnv;
}
Object *lookup(Object *args){
    Object *id=Object::car(args);
    Object *env=args->cdr()->car();
    return Object::lookup(id, env);
}
Object *update(Object *args){
    Object *id=Object::car(args);
    Object *val=args->cdr()->car();
    Object *env=args->cdr()->cdr()->car();
    return Object::update(id, val, env );
}

Object *is_pair(Object *args){
//    cout<<"   is_pair args:";args->dprint();
//    cout<<"args:"<<args<<endl;
//    cout<<"args car:"<<args->car()<<endl;
    if(args->car()->gettype()==CONS&&args->car()->car()!=Object::nil){
//        cout<<"is_pair tee"<<endl;
        return Object::tee;
    }
    return Object::fee;
}

Object *is_number(Object *args){
    if(args->car()->gettype()==FLOAT||args->car()->gettype()==INT){
        return Object::tee;
    }
    return Object::fee;
 }
Object *is_interger(Object *args){
    if(args->car()->gettype()==INT){
        return Object::tee;
    }
    return Object::fee;
}
Object *is_boolean(Object *args){
    if(args->car()->gettype()==BOOL){
        return Object::tee;
    }
    return Object::fee;
}
Object *is_real(Object *args){
    if(args->car()->gettype()==FLOAT||args->car()->gettype()==INT){
        return Object::tee;
    }
    return Object::fee;
}
Object *is_eq(Object *args){
    if(args->car()==args->cdr()->car()){
        return Object::tee;
    }
    return Object::fee;
}
Object *is_zero(Object *args){
    Object *obj=args->car();
    int type=obj->gettype();
    if(type==INT ){
        return Object::intval(obj)==0? Object::tee :Object::fee ;
    }else if(type==FLOAT){
        return Object::floatval(obj)==0.0? Object::tee :Object::fee ;
    }else{
        cout<<"type not support."<<endl;
    }
    return Object::fee;
}

Object *is_symbol(Object *args){
//    cout<<__FUNCTION__<<"  args:";args->dprint();

    if(args->car()->gettype()==SYM){
        return Object::tee;
    }
    return Object::fee;
}Object *is_string(Object *args){
    if(args->car()->gettype()==STRING){
        return Object::tee;
    }
    return Object::fee;
}Object *is_char(Object *args){
    
    return Object::fee;
}
Object *is_atom(Object *args){
    if(args->car()->gettype()!=CONS){
        return Object::tee;
    }
    return Object::fee;
}
Object *is_procedure(Object *args){
    if(args->car()->gettype()==PROC||args->car()->gettype()==PRIMOP){
        return Object::tee;
    }
    return Object::fee;
}
Object *is_null(Object *args){
//    cout<<__FUNCTION__<<"  args:";args->dprint();
    if(args->car()==Object::el){
        return Object::tee;
    }
    return Object::fee;
}


Object *display(Object *args){
//  cout<<"   display args:";args->dprint();
    //if(args!=Object::nil)
    cout<<Object::car(args);
    return Object::symStatus;
}

Object *newline(Object *args){
     //cout<<"   newline args:";args->dprint();
    cout<<endl;
    return Object::symStatus;
//    return Object::symLf;
    return Object::symCr;
//    return Object::symCrLf;
//    return Object::cons(Object::symCr,Object::symLf);
}
Object *trace(Object *args){
    cout<<"   trace args:";args->dprint();

    Object *procedure=Object::car(args);
    cout<<"procedure:"<< procedure<<endl;
    if(procedure==Object::nil){
//        cout<<"no find."<<endl;
        //topEnv=extend(def_var,def_val,topEnv);
        return Object::nil;
    }else{
        if(procedure->gettype()==SYM){
            cout<<"     topEnv:"<<Object::topEnv<<endl;
            Object *proc=Object::lookup(procedure, Object::topEnv);
            if(proc!=Object::nil){
                cout<<"find"<<endl;
                Object *proc_parms=Object::car(proc);//arg
                Object *proc_body=Object::cdr(proc);//body
                //        Object *proc_env=procedure->obj[2];
                cout<<"proc:"<<procedure->symname()<<" args:"<<proc_parms<<" body:"<< proc_body<<endl;
                proc_body=Object::cons(Object::symBegin,Object::cons(proc_parms,proc_body ));
                Object *newproc=Object::mkproc(procedure->symname(),proc_parms, proc_body, Object::topEnv);
                
                Object *ret=Object::update(procedure,newproc, Object::topEnv);
//            proc_body->setcar(Object::cons(Object::symLambda,proc_body));
                
                cout<<"proc:"<<proc<<"     topEnv:"<<Object::topEnv<<endl;
            }
        }
    }
    return Object::none;
}

Object *notf(Object *args){
//    cout<<"   notf args:";args->dprint();

    if(args->car()==Object::fee){
        return Object::tee;
    }
    return Object::fee;
}

