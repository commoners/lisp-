//
//  function.cpp
//  lisp++
//
//  Created by evil on 10/17/14.
//  Copyright (c) 2014 evilbinary.org. All rights reserved.
//

#include "function.h"

void resist_functions(){
    Object::add_procedure("display", display);
    Object::add_procedure("newline", newline);
    
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
Object *trace(Object *trace){
    
    return Object::nil;
}
