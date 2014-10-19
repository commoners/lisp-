//
//  gc.cpp
//  lisp++
//
//  Created by evil on 10/18/14.
//  Copyright (c) 2014 evilbinary.org. All rights reserved.
//

#include "memory.h"
#include <memory>

Memory * Memory::memory=NULL;
list<Object*> Memory::roots;

Memory::Memory(){
    threshold=MAX_OBJ;
}


void Memory::init(){

}
Object *Memory::new_obj(){
    Object *o=new Object();
//    auto_ptr<Object> ap(o);
//    Object *ret=&*ap.get();
//    return ret;
    //add_obj(o);
    
    //gc();
    return o;
}
Object ** Memory::new_objs(int count){
    Object** o=new Object*[count]();
    add_objs(o, count);
    return o;
}
Object * Memory::add_obj(Object *o){
    if(o==NULL) return NULL;
    if(o->flag==INIT){
        //printf("ptr:%p:\n",o);
        objs.insert(std::pair<Object*, int >(o,INIT) );
    }else{
        cout<<"error flag"<<o->flag<<endl;
    }
    return o;
}
Object ** Memory::add_objs(Object **o,int count){
    for(int i=0;i<count;i++){
        //cout<<"i:"<<i<<" "<<(int*)o[i]<<endl;
        add_obj(o[i]);
    }
    return o;
}
void Memory::del_obj(Object *obj){
    if(obj==NULL)
        return;
    objs.erase(obj);
    cout<<"delete "<<obj<<endl;
    delete obj;
    obj=NULL;
}
void Memory::remove_obj(Object* obj){
    cout<<"     remove:"<<obj<<endl;
    objs.erase(obj);
}

void Memory::gc(){
    //cout<<__FUNCTION__<<endl;
    if(objs.size()>threshold){//gc
        cout<<"[+"<<objs.size()<<"+]"<<endl;
        mark();
        sweep();
        cout<<"[-"<<objs.size()<<"-]"<<endl;
//        print_root();
    }
}
void Memory::sweep(){
    //cout<<__FUNCTION__<<endl;
//
//    if(obj==NULL||obj->is_null()) return ;
//    Object *o=obj;
//    while(o!=NULL&&!o->is_null()){
//        if(o->flag!=MARK&&o->flag!=INIT){
//            o->flag=CLEAR;
//            int *p=(int*)o;
//            cout<<"CLEAR:"<<o<<p<<endl;
//            delete o;
//        }
//        //cout<<"mark======="<<endl;
//        o=o->car();
//    }
//    //cout<<"makr end"<<endl;
//    sweep(obj->cdr());
    map<Object *,int>::iterator iter;
    for(iter=objs.begin();iter!=objs.end();){
        if(iter->first->flag!=MARK&&iter->first->flag!=CLEAR){
            
            Object *o=iter->first;
            //int *p=(int*)o;
            o->flag=CLEAR;
            //cout<<"     CLEAR:"<<o<<" " <<p<<endl;
            objs.erase(iter++);
            delete o;
            //iter++;
        }else{
            iter++;
        }
    }
}
void Memory::mark_obj(Object*obj){
    //cout<<__FUNCTION__<<endl;
    if(obj==NULL||obj->is_null()) return ;
    //cout<<__FUNCTION__<<endl;
    Object *o=obj;
    while(o!=NULL&&!o->is_null()){
        o->flag=MARK;
        //cout<<"mark======="<<endl;
        o=o->car();
    }
    mark_obj(obj->cdr());
}
void Memory::mark(){
    list<Object*>::iterator iter;
    for(iter=roots.begin();iter!=roots.end();iter++){
  //      mark_obj(*iter);
      mark_obj((*iter)->car());
        mark_obj((*iter)->cdr());

    }
}
void Memory::print_root(){
    list<Object*>::iterator iter;
    for(iter=roots.begin();iter!=roots.end();iter++){
        cout<<"root:"<<*iter<<endl;
        
    }
}
