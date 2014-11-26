//
//  obj.cpp
//  lisp++
//
//  Created by evil on 9/7/14.
//  Copyright (c) 2014 evilbinary.org. All rights reserved.
//  rootntsd@gmail.com

#include "object.h"
#include <iomanip>
#include "function.h"
#include "memory.h"
#include <iomanip>

#define caar(obj)   car(car(obj))
#define cadr(obj)   car(cdr(obj))
#define cdar(obj)   cdr(car(obj))
#define cddr(obj)   cdr(cdr(obj))
#define caaar(obj)  car(car(car(obj)))
#define caadr(obj)  car(car(cdr(obj)))
#define cadar(obj)  car(cdr(car(obj)))
#define caddr(obj)  car(cdr(cdr(obj)))
#define cdaar(obj)  cdr(car(car(obj)))
#define cdadr(obj)  cdr(car(cdr(obj)))
#define cddar(obj)  cdr(cdr(car(obj)))
#define cdddr(obj)  cdr(cdr(cdr(obj)))
#define caaaar(obj) car(car(car(car(obj))))
#define caaadr(obj) car(car(car(cdr(obj))))
#define caadar(obj) car(car(cdr(car(obj))))
#define caaddr(obj) car(car(cdr(cdr(obj))))
#define cadaar(obj) car(cdr(car(car(obj))))
#define cadadr(obj) car(cdr(car(cdr(obj))))
#define caddar(obj) car(cdr(cdr(car(obj))))
#define cadddr(obj) car(cdr(cdr(cdr(obj))))
#define cdaaar(obj) cdr(car(car(car(obj))))
#define cdaadr(obj) cdr(car(car(cdr(obj))))
#define cdadar(obj) cdr(car(cdr(car(obj))))
#define cdaddr(obj) cdr(car(cdr(cdr(obj))))
#define cddaar(obj) cdr(cdr(car(car(obj))))
#define cddadr(obj) cdr(cdr(car(cdr(obj))))
#define cdddar(obj) cdr(cdr(cdr(car(obj))))
#define cddddr(obj) cdr(cdr(cdr(cdr(obj))))

//Object *nil;
Object *Object::nil=NULL;
Object *Object::el=NULL;
Object *Object::eob=NULL;


Object *Object::symbols=NULL;
Object *Object::topEnv=NULL;
Object *Object::symLambda=NULL;
Object *Object::symDefine=NULL;
Object *Object::symIf=NULL;
Object *Object::symQuote=NULL;
Object *Object::symAtom=NULL;

Object *Object::symSetb=NULL;
Object *Object::tee=NULL;
Object *Object::fee=NULL;
Object *Object::symBegin=NULL;
Object *Object::none=NULL;
Object *Object::symCond=NULL;
Object *Object::symElse=NULL;
Object *Object::symEq=NULL;


Object *Object::symCar=NULL;
Object *Object::symCdr=NULL;
Object *Object::symLabel=NULL;
Object *Object::symStatus=NULL;


Object *Object::symLf=NULL;
Object *Object::symCr=NULL;
Object *Object::symCrLf=NULL;



Object *Object::symOr=NULL;
Object *Object::symAnd=NULL;
Object *Object::symLet=NULL;


Object *Object::callExp=NULL;
Object *Object::callEnv=NULL;

Memory *Object::memory=Memory::instance();




Object::Object(){
    this->type=EL;
    this->data=NULL;
    this->obj=NULL;
    isalive=true;
    this->flag=NULL;
//    cout<<__FUNCTION__<<" "<<(int *)this<<endl;
//    memory->add_obj(this);
}
Object* Object::mkobj(char* name){
    Object *thiz=memory->new_obj();
    size_t len=strlen(name);
    len++;
    thiz->type=SYM;
    thiz->data=new char(len);
    memset(thiz->data,0,len);
    strcpy((char*)thiz->data,(const char*)name);
    thiz->obj=NULL;
    thiz->isalive=true;
    thiz->flag=INIT;
//    cout<<(char *)thiz->data<<endl;
    return thiz;
}


Object *Object::mkobj(ObjType type,void* data){
    Object* thiz=memory->new_obj();
    thiz->type=type;
    thiz->data=data;
//    cout<<(char*)thiz->data<<endl;
    thiz->obj=NULL;
    thiz->isalive=true;
    thiz->flag=INIT;
    return thiz;
}

Object* Object::mknobj(ObjType type,int count,...){
    Object *thiz=memory->new_obj();
    va_list ap;
    int i;
    va_start(ap,count);
    //    this->obj = (Object**) new Object[count]();
    thiz->obj=memory->new_objs(count);
    thiz->type = type;
    thiz->isalive=true;
    thiz->data=NULL;
    for(i = 0; i < count; i++){
        Object *o=(Object *)va_arg(ap,Object*);
       thiz->obj[i]=o ;
        //        cout<<"obj["<<i<<"]="<<*(obj[i])<<endl;
//        cout<<"         obj["<<i<<"]="<<o<<" "<<thiz->obj[i]<<endl;

    }
    va_end(ap);
    thiz->flag=INIT;
    //    cout<<"======"<<this<<endl;
    return thiz;
}



Object *Object::cons(Object* obj1,Object *obj2){
    Object *o;
//    cout<<" #obj1:";obj1->dprint();
//    cout<<" #obj2:";obj2->dprint();
    //o=mkobj(CONS,2,obj1,obj2);
//    cout<<" obj1:"<<obj1<<endl;
//    cout<<" obj2:"<<obj2<<endl;
    o=mknobj(CONS,2,obj1,obj2);
//    cout<<"     obj1:"<<obj1<<endl;
    o->setcar(obj1);
    o->setcdr(obj2);
//    cout<<"     cons ret:"<<o<<endl;
    
 //    cout<<" scons:";
//    cout<<" car:";o->car()->dprint();
//    cout<<" cdr:";o->cdr()->dprint();
//    o->dprint();
    //cout<<endl;
    //o->tprint();
    //cout<<endl;
    return  o;
}

Object *Object::mksym(char *name){
    //DEBUG_INFO;
    return mkobj(name);
}
Object *Object::mkint(int val){
    int *p=(int*) new int(sizeof(int));
    *p=val;
    Object *o=mkobj(INT,p);
     return o;
}
Object *Object::mkfloat(double val){
    double *p=(double*) new double(sizeof(double));
    *p=val;
    Object *o=mkobj(FLOAT,p);
    return o;
}
Object *Object::mkstring(char* name){
    Object *o=mkobj(name);
    o->type=STRING;
    return o;
}

Object * Object::mkpriop(FUNCTION fun){
    Object *o=mkobj(PRIMOP,(void*)fun);
    return o;
}

Object * Object::mkproc(Object *args,Object *body,Object* env){
    Object *o=  mknobj(PROC,3,args,body,env);
    return o;
}
Object * Object::mkproc(char* name,Object *args,Object *body,Object* env){
    //cout<<"############"<<name<<endl;
    Object *o=mknobj(PROC,3,args,body,env);
    size_t len=strlen(name);
    len++;
    o->data=malloc(len);
    memset(o->data,0,len);
    strcpy((char*)o->data,(const char*)name);
    //    cout<<"@@@@@@@@@mkproc name:"<<(char*)o->data<<endl;
    return o;
}






char*  Object::symname(Object *obj){
//    cout<<__FILE__<<__LINE__<<endl;
    char* ret=NULL;
    if(obj==NULL||obj->data==NULL){
        return ret;
    }else if(obj->type==SYM||obj->type==STRING){
        ret=(char *)(obj->data);
//        cout<<obj->data<<endl;
//        cout<<"sym==="<<ret<<endl;
    }else if(obj->type==BOOL){
        ret=(char *)(obj->data);
    }else if(obj->type==PROC){
        if(obj->data!=NULL)
            ret=(char*)obj->data;
    }else{
        cout<<__FUNCTION__<<" get "<<obj<<" symname erro."<<endl;
    }
    
    return ret;
}



Object::~Object(){
    int *p=(int*)this;
    cout<<"~Object "<<p<<" isalive:"<<isalive<<" obj:"<<this<<endl;
    //    cout<<this<<endl;
    isalive=false;
    //if(isalive){
        //cout<<" release"<<endl;
    //isalive=false;
        /*if(this->data!=NULL){
            free(data);
            this->data=NULL;
        }
        if(this->obj!=NULL) {
            this->obj=NULL;
        }*/
//    }else{
//        //cout<<" do nothing."<<endl;
//    }
    
    
}


Object *Object::inter(char *name){
    Object *op=findsym(name);
    if(!isnil(op)){
        //cout<<"find symbol:"<<car(op)<<endl;
        //printf("%p\n",op);
        return car(op);
    }

    //DEBUG_INFO;
    op=mksym(name);
    symbols=cons(op,symbols);
    //printf("%p\n",op);

    return op;
}

void Object::init(){
    if(topEnv==NULL){
        nil=inter("nil");
        el=mkobj(EL,(void*)NULL);
        eob=mkobj(EOB,(void*)NULL);

        
        symbols=cons(nil,nil);
        
        tee=inter("#t");
        tee->type=BOOL;
        fee=inter("#f");
        fee->type=BOOL;
        none=inter("");
        topEnv=cons(tee,fee);
        topEnv=cons(topEnv,nil);
        
        Memory::add_root(symbols);
        Memory::add_root(topEnv);
        
        
        Object *add=mkpriop(Object::sum);
        Object *sub=mkpriop(Object::sub);
        Object *mul=mkpriop(Object::mul);
        Object *div=mkpriop(Object::div);
        
        //    cout<<"add:";add->dprint();
        topEnv=extend(inter("+"),add,topEnv);
        topEnv=extend(inter("-"),sub,topEnv);
        topEnv=extend(inter("*"),mul,topEnv);
        topEnv=extend(inter("/"),div,topEnv);
        
        Object *mnumeq=mkpriop(Object::prim_numeq);
        Object *cons=mkpriop(Object::prim_cons);
        Object *car=mkpriop(Object::prim_car);
        Object *cdr=mkpriop(Object::prim_cdr);
        Object *mnumgt=mkpriop(Object::prim_numgt);
        Object *mnumlt=mkpriop(Object::prim_numlt);
        Object *primor=mkpriop(Object::prim_or);
        Object *primand=mkpriop(Object::prim_and);
        
        
        topEnv=extend(inter("="),mnumeq,topEnv);
        topEnv=extend(inter("cons"),cons,topEnv);
        topEnv=extend(inter("car"),car,topEnv);
        topEnv=extend(inter("cdr"),cdr,topEnv);
        
        topEnv=extend(inter(">"),mnumgt,topEnv);
        topEnv=extend(inter("<"),mnumlt,topEnv);
        topEnv=extend(inter("or"),primor,topEnv);
        topEnv=extend(inter("and"),primand,topEnv);
        
        
        symCond=inter("cond");
        symIf=inter("if");
        symQuote=inter("quote");
        symDefine=inter("define");
        symSetb=inter("set!");
        symLambda=inter("lambda");
        symBegin=inter("begin");
        symElse=inter("else");
        symAtom=inter("atom");
        symEq=inter("eq");
        symCar=inter("car");
        symCdr=inter("cdr");
        symLabel=inter("label");
        symStatus=inter("");
        symAnd=inter("and");
        symOr=inter("or");
        symLet=inter("let");
        symCr=inter("\r");
        symLf=inter("\n");
        symCrLf=inter("\n\r");
        
        //function.cpp
        resist_functions();
    }
    
    //    cout<<"topEnv:";topEnv->dprint();
}


//###############################################################################
//for tail call implements
//###############################################################################
//for expression  symbol and make callable function


Object * Object::eval_tail_call(Object *exp,Object *env){
    return exp->eval2(exp,env);
}

Object* Object::eval2(Object *exp,Object *env){
    
    if(exp==nil||exp==NULL) return nil;
call_eval_start:
//    cout<<"##exp:"<<exp<<endl;
    switch (exp->type) {
        case INT:
            return exp;
        case SYM:{
            //            cout<<"-SYM"<<endl;
            Object *vals=lookup(exp, env);;
            if(vals!=nil){
//                                            cout<<"  Found SYM "<<exp->symname()<<endl;
//                cout<<env<<endl;
                return vals;
            }else{
                vals=lookup(exp,topEnv);
                if(vals!=nil){
//                                                       cout<<"  Found G SYM "<<exp->symname()<<endl;
                    return vals;
                }else{
//                                                            cout<<"  No found SYM "<<exp->symname()<<endl;
                    //return exp;
                }
                
            }
            return exp;
            
        }
        case CONS:{
            Object *ex=car(exp);
//            cout<<"CONS exp:"<<exp <<" exp->car:"<<car(exp)<< " addr:"<<car(exp)->type<<endl;
//            cout<<"CONS:"<<CONS<<" addr:"<<symDefine <<" "<<ex<<endl;
//            printf("ex:%p car(exp):%p cdr(exp):%p\n",ex,car(exp),cdr(exp));

            //exp->tprint();
            if(ex==symQuote){
                //cout<<"$$$$$$$$$$$$$$$$$quote:";exp->dprint();
//                cdr(exp)->dprint() ;
                return cdr(exp);
            }else if(ex==symIf){
                if(ex->eval2(car(cdr(exp)), env) != nil)
                    exp = car(cdr(cdr(exp)));
                else
                    exp = car(cdr(cdr(cdr(exp))));
                goto call_eval_start;
            }else if(ex==symCond){
//                cout<<"cond:"<<exp<<endl;
                Object *e=cdr(exp);
//                cout<<"  @@@exp:"<<e<<endl;
                for(;e!=nil;e=cdr(e)){
                    cout<<"     #e:"<<car(e)<<"     #cdr(e):"<<cdr(e)<<endl;
                    if(cdr(e)==nil){//last element
                        cout<<" last else car(e):"<<car(e)<<endl;
                        if(caar(e)==symElse){
                            cout<<" ###else:"<<cdar(e)<<endl;
                            //cout<<" ============###else:"<<cddar(e)<<endl;
                            if(cddar(e)==nil){
//                                cout<<" ==============else cdar:"<<cdar(e)<<endl;

                                exp=cdar(e); //->eval2(cdar(e),env);
//                                cout<<" ==============else ret:"<<exp<<endl;
                                //return exp;
                                goto call_eval_start;

                            }else{
//                                cout<<"test"<<endl;
                                exp=cons(symBegin,cdar(e));
                            }
                        }else{
//                            cout<<" eee:"<<e<<endl;
                            exp=e;
                        }
                        goto call_eval_start;
                    }else{
                        Object *cond=car(e)->eval2(car(e),env);
                        if(cond!=nil){
                            exp=cdar(e);
//                            cout<<"ret exp:"<<exp<<endl;
                            if(exp==nil){
                                exp=cond;
                            }
                            //cout<<"     ret:"<<exp<<endl;
                            goto call_eval_start;
                        }
                    }
                }
                
            }else if(ex==symSetb){
                //                 cout<<" =====setb"<<endl;
                //                cout<<" ======env:";env->dprint();
                //                cadr(exp)->dprint();
                //                cout<<"cddr:";cddr(exp)->dprint();
                //cout<<" ee:";eval(cddr(exp), env)->dprint();
                Object *vals=update(cadr(exp),cddr(exp)->eval2(cddr(exp), env),env);
                //                cout<<" ======#env:";env->dprint();
                if(vals==nil){
                    //cout<<"～～～～～～No find, "<<*cadr(exp)<<endl;
//                    cout<<"###topEvn:";topEnv->dprint();
//                    cout<<"#######topEnv:"<<topEnv<<endl;
                    
                    vals=update(cadr(exp),cddr(exp)->eval2(cddr(exp), env),topEnv);
//                    cout<<"###"<<vals<<" topEvn:"<<topEnv<<endl;
                    //topEnv->dprint();
                }
                return none;
            }
            else if(ex==symBegin){
                Object *ret=nil;
                Object *e=cdr(exp);
                for(;e!=nil;e=cdr(e)){
                    Object *i=car(e);
//                    cout<<"====begin:"<<i<<endl;
                    ret=i->eval2(i,env);
//                    cout<<"cddr(e):"<<cddr(e)<<endl;
                    if(cddr(e)==nil){
//                        cout<<"@@@@@@@@@@@@@@@e:"<<cdr(e)<<endl;
                        exp=cdr(e);
                        goto call_eval_start;
                        break;
                    }
                }
                //cout<<"@@@@@@@@@@@#####e:"<<e<<endl;
                
                return ret;
                
            }else if(ex==symDefine){
                Object * vars=cadr(exp);
//                Object * vals=eval2(cddr(exp),env);
                Object * vals=cddr(exp);

//                cout<<"define vals:"<<vals<<" vars:"<<vars<<endl;
                if(cadr(exp)->type==SYM){
                    vals=car(cdr(cdr(exp)));
//                    cout<<"SYM-----##vars:"<<vars<<" exp:"<<cadr(exp)<<" vals:"<<vals <<" eval-vals:"<<eval2(vals,env)<<endl;
//                    Object *proc=mkproc(car(exp)->symname(),vars,eval2(vals,env),env);
//                    topEnv=extend(vars,vals,topEnv);
                    vals=vals->eval2(vals,env);
//                    cout<<"==topEnv:"<<topEnv<<endl;
                    Object *findret=lookup(vars, topEnv);;
                    if(findret==nil){
                        topEnv=extend(vars,vals,topEnv);
                    }else{
                        update(vars,vals,topEnv);
                    }
//                    cout<<"topEnv:"<<topEnv<<endl;
                    //exp=vals;
                    //goto call_eval_start;
                    return none;
                    
                }else{
                    vars=caadr(exp);
                    Object *proc;
                        //env=extend(vars,proc,env);
//                        cout<<"update "<<vars<<" "<<endl;
                        proc=mkproc(car(exp)->symname(), cdadr(exp),vals,env);
                        topEnv=extend(vars,proc,topEnv);
                    
                    return none;
                    
                }
                
            }else if(ex==symLambda||(ex->car()!=nil&&ex->car()==symLambda)){
                if(ex==symLambda){
                    Object *proc=mkproc(car(exp)->symname(),cadr(exp),cddr(exp),topEnv);
                    topEnv=extend(caadr(exp),proc,topEnv);
//                    cout<<" mkproc:";proc->dprint();
                    //                exp->setcar(proc);
                    //                cout<<"exp:"<<exp<<" ex:"<<ex<<endl;
                    return proc;
                }else{
                    Object *proc=mkproc(car(ex)->symname(),cadr(ex),cddr(ex),topEnv);
                    topEnv=extend(caadr(ex),proc,topEnv);
//                    cout<<"topEnv:"<<topEnv<<endl;
                    exp->setcar(proc);
//                    cout<<"  mkproc exp:"<<exp<<" proc:"<<proc<<endl;
                    goto call_eval_start;
                }
            }else if(ex==nil){
                return nil;
            }else {
//                cout<<"else:"<<ex<<endl;
                Object *args=exp->cdr();
                Object *proc=exp->car();
                if(exp->car()->type==SYM){
                    proc=lookup(exp->car(), env);;
                    if(proc==nil){
                        proc=lookup(exp->car(),topEnv);
                        if(proc==nil){
//                            cout<<"  No found  SYM  and G SYM"<<ex<<endl;
                            //proc=eval2(exp->car(),env);
                            //exp=exp->car();
                            goto call_eval_start;
                            //return exp;
                        }else{
//                            cout<<"$$$$$$$$fond G SYM:"<<proc<<endl;
                        }
                    }else{
//                        cout<<"$$$$$$$$fond  SYM:"<<proc<<endl;

                    }
                }
//                cout<<"$$$$$$$$$$$type:"<<proc->type<<" proc:"<<proc<<endl;
                if(proc->type==PRIMOP){
                    cout<<"###PRIMOP "<<exp<<endl;
//                    cout<<"###proc:"<<proc<<" args:"<<args <<endl;
                    //cout<<" r:"<<r<<endl;
                    //                   cout<<proc<<"" <<r<< ":"<<result<<endl;
                    return apply2(proc,args,env);
                    //args=proc->evlis2(args,env);
                    //return ((FUNCTION)(proc->data))(args);
                    
//                    {
//                        Object *exps=args;
//                        if(exps == nil) return nil;
//                        for(;exps->type==CONS&&exps!=nil;exps=cdr(exps)){
////                                  cout<<" ****exps:"<<exps<<" car(exp):"<<car(exps)<<" ="<<car(exps)->eval2(car(exps),env)<<endl;
//                            if(car(exps)->type==INT){
//                                continue;
//                            }else if(car(exps)->type==SYM){
//                                Object *v=lookup(car(exps),env);
//                                if(v!=nil){
//                                    exps->setcar( v);
//                                    continue;
//                                }
//                            }else{
//                                //Object* e=extend(proc,exps,proc->obj[2]);
//                                exps->setcar( proc->eval2(cons(  car(exps),env) );
//                                exp=cons(proc,exps);
//                                goto call_eval_start;
//                                break;
//                            }
//                        }
//                        //Object* e=extend(car(proc),vals,proc->obj[2]);
                        //exp=cdr(proc);
                        
                        //cout<<"====args:"<<args<<endl;
                        cout<<"exp :"<<exp<<" args:"<<args<<endl;
                        return ((FUNCTION)(proc->data))(args);
                        
                        //    cout<<"exps:"<<exps<<" env:"<<env<<endl;
                        //    cout<<"exps:";exps->dprint(); cout<<" env:"<<endl;
                        
                        //    Object *ret=cons(eval2(car(exps), env),evlis2(cdr(exps), env));
                        
                        /*Object* t=cons(nil,nil);
                        Object*r=t;
                        Object*l=nil;
                        for(;exps->type==CONS&&exps!=nil;exps=cdr(exps)){
                            //        cout<<" ****exps:"<<exps<<endl;
                            Object *te=nil;
                            //Object *ett=new Object();
                                te=car(te)->eval2(car(exps),env);
                            //        cout<<" ****vals:"<<te<<endl;
                            t->setcar(te);
                            t->setcdr(cons(nil,nil));
                            //            cout<<" ********t:"<<t<<endl;
                            l=t;
                            t=t->cdr();
                            
                        }
                        l->setcdr(nil);
                        //    cout<<"##ret:"<<ret<<endl;
                        //    cout<<"$$r:"<<r<<endl;
                        
                        //    ret->dprint();
                        //    r->dprint();
//                        return r;
 
                        return ((FUNCTION)(proc->data))(r);*/

//                    }
                    
                    
                    goto call_eval_start;
                    
                }else if(proc->type==PROC){
                    cout<<"###PROC "<<exp<<endl;
                    Object *vals=args->evlis2(args, env);
                    cout<<"###proc:"<<proc<<" args:"<<args <<" vals:"<<vals<<endl;
                    Object* e=extend(car(proc),vals,proc->obj[2]);
                    cout<<"===:cdr(proc):"<<cdr(proc)<<endl;
                    exp=cons(symBegin,cdr(proc));
                    //exp=cdr(proc);
                    //                        exp=proc;
                    env=e;
                    goto call_eval_start;
                }else if(proc->type==CONS){
//                    cout<<"######CONS:"<<proc<<" exp:"<<exp<<endl;
//                    proc=eval2(exp->car(),env);
                    //Object *vals=evlis2(args, env);
                    
                    //exp=proc;
                    //exp=cons(proc,args);
                    //env=proc;
                    exp=proc;
                    goto call_eval_start;
                    //return proc->eval2(proc,env);
                }else{
//                    cout<<"xxx else exp:"<<exp<<" exp->car:"<<exp->car()<<endl;
                    //exp=cons(proc,args);
                    exp=exp->car();
                    goto call_eval_start;
                }
            }
//
        }
            
        default:{
            break;
        }
    }
    cout<<"exp end:"<<exp<<endl;
    return exp;
}

Object * Object::apply2(Object *proc,Object *args,Object *env){
    args=proc->evlis2(args,env);
    return ((FUNCTION)(proc->data))(args);
}


Object* Object::evlis2(Object *exps,Object *env){
    if(exps == nil) return nil;
//    cout<<"exps:"<<exps<<" env:"<<env<<endl;
//    cout<<"exps:";exps->dprint(); cout<<" env:"<<endl;

//    Object *ret=cons(eval2(car(exps), env),evlis2(cdr(exps), env));
    
    Object* t=cons(nil,nil);
    Object*r=t;
    Object*l=nil;
    for(;exps->type==CONS;exps=cdr(exps)){
//        cout<<" ****exps:"<<exps<<endl;
        Object *te=exps->eval2(car(exps),env);
//        cout<<" ****vals:"<<te<<endl;
        t->setcar(te);
        t->setcdr(cons(nil,nil));
//            cout<<" ********t:"<<t<<endl;
        l=t;
        t=t->cdr();

    }
    l->setcdr(nil);
//    cout<<"##ret:"<<ret<<endl;
//    cout<<"$$r:"<<r<<endl;

//    ret->dprint();
//    r->dprint();
    return r;
//    return ret;

}



//=========================================================

Object *Object::sequence_to_exp(Object *seq) {
    if(cddr(seq)==nil){
        return seq;
    }else if(cdddr(seq)==nil){
        return cadr(seq);
    }else{
        return cons(symBegin,seq);
    }
}
Object *Object::expand_clauses(Object *clauses) {
    Object *first;
    Object *rest;
//    cout<<"=====XX:"<<cdr(clauses)<<endl;
    if (car(clauses)==nil) {
        return nil;
    }else {
        first = car(clauses);
        rest  = cdr(clauses);
//        cout<<"first:"<<first<<" rest:"<<rest<<endl;
        if (car(first)==symElse) {
//            cout<<"=====else:"<<cdr(first)<<"  #:"<<cddr(rest)<<endl;
            if (cddr(rest)==nil) {
//                cout<<"seq:"<< sequence_to_exp(cdr(first))<<endl;
                return sequence_to_exp(cdr(first));
            }
            else {
                cout<<"else clause isn't last cond->if"<<endl;
                exit(1);
            }
        }
//        else if(rest==nil){
//            return first;
//        }
        else {
            Object* if_cond=car(first);
            Object *if_seq=sequence_to_exp(cdr(first));
            Object *if_else=expand_clauses(rest);
            return cons(symIf,cons(  if_cond, cons(if_seq,cons(if_else,nil))  ));
        }
    }
}


Object * Object::eval(Object *exp,Object *env){
    Object *procedure;
    Object *arguments;
    Object *result;
calltail:
//       cout<<"  #exp:"<<exp<<" #car(exp):"<<car(exp)<<" #cdr(exp):"<<cdr(exp)<<endl;
//        exp->dprint();
//    exp->tprint();
    /* cout<<"         exp: "<<exp<<" ";
     if(exp->type==SYM||exp->type==INT){
     Object* symbol=lookup(exp, env);
     if(symbol==nil) symbol=lookup(exp,topEnv);
     if(symbol!=nil)
     cout<<" ==> "<<symbol<<endl;
     else cout<<endl;
     }else{
     cout<<endl;
     }*/
    
    
    
    if(exp->type==INT||exp->type==STRING||exp->type==FLOAT||exp->type==BOOL||exp->type==EOB){//to do string boolean and more data type
//        cout<<"ret:"<<exp<<endl;
        return exp;
    }else if(exp->type==SYM){
//        cout<<"     lookup:"<<exp<<endl;
        if(exp==tee||exp==fee||exp==nil)
            return exp;
        Object* symbol=lookup(exp, env);
        if(symbol==nil){
            symbol=lookup(exp,topEnv);
            if(symbol==nil){
//                cout<<"can't find symbol "<<exp<<endl;
                //                cout<<topEnv<<endl;
                //                cout<<"exp->type:"<<exp->type<<endl;
                symbol=nil;
            }
        }
        return symbol;
    }else{
        Object *sym=car(exp);
//        cout<<"     sym:"<<sym<<endl;
        if(sym==symQuote){
            //        cout<<"============="<<cdr(exp)<<endl;
            return cdr(exp);
        }else if(sym==symSetb){
            Object *set_var=cadr(exp);
            Object *set_val=caddr(exp);
            //        cout<<"     set_var:"<<set_var<<" set_val:"<<set_val<<endl;
            set_val=set_val->eval(set_val,env);
            //        cout<<"     =set_var:"<<set_var<<" =set_val:"<<set_val<<endl;
            
            Object *ret=update(set_var, set_val, env);
            if(ret==nil)
                ret=update(set_var, set_val, topEnv);
            return symStatus;
        }else if(sym==symDefine){
            Object *def_var;
            Object *def_val;
            
            if(cadr(exp)->type==SYM){
                def_var=cadr(exp);
                def_val=caddr(exp);
                def_val=def_val->eval(def_val,env);
                Object *findret=lookup(def_var, topEnv);;
                if(findret==nil){
                    topEnv=extend(def_var,def_val,topEnv);
                }else{
                    update(def_var,def_val,topEnv);
                }
                //            cout<<"hhehe"<<def_var<<" val"<<def_val<<endl;
            }else{
                def_var=caadr(exp);
                Object *params=cdadr(exp);
                Object *body=cddr(exp);
                
                
                //def_val=cons(symLambda,cons(params,body));//make lambda
//                cout<<"def_var:"<<def_var<<" params:"<<params<<" body:"<<body<<endl;
                Object *proc;
                //            cout<<" mkpro:"<<def_var->symname()<<" parm:"<<params<<" body:"<<body<<endl;
                proc=mkproc(def_var->symname(), params,body,env);
                topEnv=extend(def_var,proc,topEnv);
                
            }
            
            return symStatus;
        }else if(sym==symIf){
            Object* if_test=cadr(exp);
            Object* if_consequent=caddr(exp);
            Object * if_alternate=cadddr(exp);
            Object *test=if_test->eval(if_test,env);
//                    cout<<"     exp:"<<exp<<endl;
//                    cout<<"         if_test:"<<if_test<<"="<<test<<endl;
//                    cout<<"         if_consequent:"<<if_consequent<<endl;
//                    cout<<"         if_alternate:"<<if_alternate<<endl;
            //        cout<<"         cdddr(exp):"<<cdddr(exp)<<endl;
            if(test==tee||(test!=nil&&test!=fee)){
                exp=if_consequent;//first exp
            }else{//else exp
//                cout<<"ee==exp:";
//                write(cout, cdddr(exp));

                if(cdddr(exp)==nil){
//                    cout<<"if_alternate is nil"<<endl;
                    exp=fee;
                }else if(cdddr(exp)==el){//no else and is ()
//                    return NULL;
                    exp=eob;
                }else{
//                    cout<<"      =====if_alternate:"<<if_alternate<<endl;
                    exp=if_alternate;
                }
            }
            //        cout<<"     ret:"<<exp<<endl;
            //        cout<<endl;
            
            goto calltail;
        }else if(sym==symLambda){
            Object *lambda_parms=cadr(exp);
            Object *lambda_body=cddr(exp);
            //cout<<"p:"<<lambda_parms<<endl;
            //cout<<"b:"<<lambda_body<<endl;
            return mkproc(lambda_parms, lambda_body, env);
        }else if(sym==symBegin){
            exp=cdr(exp);
            while(cdr(exp)!=el&&cdr(exp)!=nil){//is the last
//                cout<<"     on exp:"<<exp<<endl;
//                cout<<"cdr(exp):"<<cdr(exp)<<endl;
                exp->eval(car(exp),env);
                exp= cdr(exp);
            }
//            cout<<"l=="<<cdr(exp)<<" e:"<<exp<<endl;
            exp=car(exp);
//            cout<<"last===="<<exp<<endl;
            goto calltail;
        }else if(sym==symCond){
            Object *the_clauses=cdr(exp);
            if(cdr(the_clauses)==nil){
                return fee;
            }else{
//                            cout<<"             clauses:"<<the_clauses<<endl;
                exp=expand_clauses(the_clauses);
//                            cout<<"             clauses="<<exp<<endl;
            }
            goto calltail;
        }else if(sym==symAtom){
            //cout<<"============="<<cadr(exp)<<endl;
            exp=cadr(exp);
            exp=exp->eval(exp,env);
            return exp->atom();
        }else if(sym==symLet){
//            cout<<"===exp:"<<exp<<endl;
//            cout<<"     binds:"<<cadr(exp)<<endl;
//            cout<<"     body:"<<cddr(exp)<<endl;
            Object *let_parms=NULL;
            Object *let_body=cddr(exp);
            Object *let_args=NULL;
            
            
            Object* let_parms_t=cons(nil,nil);
            Object* let_args_t=cons(nil,nil);
            let_parms=let_parms_t;
            let_args=let_args_t;
            
            for(Object *e=cadr(exp);e!=nil;e=cdr(e)){
                Object* tmp=car(e);
//                cout<<"     "<<car(tmp)<<" "<<cadr(tmp)<<endl;
                
                let_parms_t->setcar(car(tmp));
                let_parms_t->setcdr(cons(nil,nil));
                let_parms_t=let_parms_t->cdr();

                let_args_t->setcar(cadr(tmp));
                let_args_t->setcdr(cons(nil,nil));
                let_args_t=let_args_t->cdr();
                
//                env=extend(car(tmp),cadr(tmp),env);

            }
//            cout<<"     let_params:"<<let_parms<<endl;
//            cout<<"     let_args:"<<let_args<<endl;
//            cout<<"     let_body:"<<let_body<<endl;

//            env=extend(let_parms,let_args,env);

            Object *lambda=mkproc(let_parms, let_body, env);
//            cout<<"     lambda:"<<lambda<<endl;
            exp=cons(lambda,let_args);
//            exp=lambda;
//            exp=cons(symBegin,lambda);
//            exp=cons(lambda,nil);
//            cout<<"     env:"<<env<<endl;
//            cout<<"     exp:"<<exp<<endl;
//            exp->dprint();
            
            goto calltail;
        }else if(exp->type==CONS){//proc to execute
//            cout<<"     =app:"<<exp<<endl;
            Object *op=car(exp);
            Object *ol=cdr(exp);
            
//            cout<<"         op:"<<op<<" type:"<<op->type<<" ol:"<<ol<<endl;
            if(op->type==PROC){
                procedure=op;
            }else{
                procedure=op->eval(op,env);
            }
//            cout<<"         procedure:"<<procedure<<"  type:" <<procedure->type<<endl;
            if(procedure==nil){//can't find procedure
                cout<<"can't find procedure '"<<op<<"'."<<endl;
                return symStatus;
            }
            
            arguments=ol->evlis(ol, env);
//            cout<<"         =op:"<<op<<" type:"<<procedure->type<<" =ol:"<<arguments<<endl;
//            cout<<"     app:"<<exp<<" ["<<op<<" "<<arguments<<"]"<<endl;
            
            if(procedure->type==PRIMOP){
//                            cout<<"         "<<((FUNCTION)(procedure->data))(arguments)<<endl;
                Object *ret= ((FUNCTION)(procedure->data))(arguments);
//                cout<<"         call fun:"<<procedure<<" arg:"<<arguments<<"  ret:"<<ret<<endl;
//                cout<<endl;
                return ret;
            }else if(procedure->type==PROC){
                Object *proc_args=car(procedure);//arg
                Object *proc_body=cdr(procedure);//body
                Object *proc_env=procedure->obj[2];
//                            cout<<"     ##"<<" proc_args:"<<proc_args<<" arguments:" <<arguments<<" proc_body:"<<proc_body<<" proc_env:"<<proc_env<<endl;
                env=extend(proc_args,arguments,proc_env);
                exp=cons(symBegin,proc_body);//make begin this should avoid tail call stack overflow.
//                cout<<"exp:"<<exp<<endl;
//                exp=proc_body;
                goto calltail;
            }else if(procedure->type==SYM){
                exp=procedure;
                goto calltail;
            }else if(procedure->type==INT||procedure->type==STRING||procedure->type==FLOAT){
                return procedure;
            }else{
                cout<<"erro procedure type"<<procedure->type<<" "<<procedure<<"."<<endl;
            }
        }else{
            cout<<"eval erro on exp "<<exp<<"."<<endl;
//            exp->tprint();
        }
    }
//    cout<<"eval unknow state."<<endl;
    return exp;
    
}
Object * Object::evlis(Object *exps,Object *env){
//    cout<<"exps:"<<exps<<" env:"<<env<<endl;
//    write(cout, exps);
//    cout<<endl;

    if(exps == nil||exps->car()==nil) return nil;
//       cout<<"     evlis3 exps:";exps->dprint(); cout<<"    env:"<<env<<endl;
    
    //    Object *ret=cons(eval2(car(exps), env),evlis2(cdr(exps), env));
    Object* t=cons(car(exps),nil);
//    cout<<"     t:"<<endl;
    Object*r=t;
    Object*l=nil;
    for(;exps->type==CONS;exps=cdr(exps)){
//      cout<<" ****exps:"<<car(exps)<<endl;
        Object *te=exps->eval(car(exps),env);
//        cout<<" ****vals:"<<te<<endl;
        t->setcar(te);
        t->setcdr(cons(nil,nil));
//                    cout<<" ********t:"<<t<<endl;
        l=t;
        t=t->cdr();
        
    }
//    cout<<" l:"<<l<<endl;
    l->setcdr(nil);
//    cout<<" l:"<<l<<endl;
//    cout<<"     ret:"<<r<<" "<<endl;
//    r->write(cout, r);
    return r;
}
Object * Object::apply(Object *proc,Object *args,Object *env){
    args=proc->evlis(args,env);
    return ((FUNCTION)(proc->data))(args);
}


//=========================================================





//###############################################################################
//tail call implement end
//###############################################################################

Object * Object::values(Object *exp,Object* env){
    if(exp==nil||exp==NULL) return nil;
    switch (exp->type) {
        case INT:
            return exp;
        case SYM:{
            //            cout<<"-SYM"<<endl;
            Object *vals=lookup(exp, env);;
            if(vals!=nil){
                //                                            cout<<"  Found SYM "<<exp->symname()<<endl;
                //                cout<<env<<endl;
                return vals;
            }else{
                vals=lookup(exp,topEnv);
                if(vals!=nil){
                    //                                                       cout<<"  Found G SYM "<<exp->symname()<<endl;
                    return vals;
                }else{
                    //                                                            cout<<"  No found SYM "<<exp->symname()<<endl;
                    //return exp;
                    return nil;
                }
                
            }
            return nil;
            
        }
    }
    return nil;
}

Object * Object::lookup(Object *id,Object* env){
//    cout<<" id:"<<id<<" env:"<<env<<endl;
//    cout<<__FILE__<<" id:"<<id<<endl;
    if(env==NULL||env==nil||env==el){
        return nil;
    }else if(env->type==CONS){
//        cout<<"cmp id:"<<id<<" "<<caar(env)<<" :::"<< (id==caar(env))<<" ="<<(id->symname()==caar(env)->symname())<<" --:"<<
//        strcmp(id->symname(),caar(env)->symname())<<" cmp:"<<id->symname()<<" "<<caar(env)->symname()<<endl;

        if((id==caar(env))||id->symname()==caar(env)->symname()||strcmp(id->symname(),caar(env)->symname())==0){
//            cout<<"find"<<endl;
            return cdar(env);
        }else{
            return lookup(id, cdr(env));
        }
    }else{
        //cout<<"No such look binding "<<id->symname(id)<<endl;
        return nil;
    }
}
Object * Object::update(Object *id,Object *value,Object* env){
    //cout<<"env"<<env<<endl;
    if(env==NULL||env==nil){
        return nil;
    }else if(env->type==CONS){
        if(id==caar(env)||id->symname()==caar(env)->symname()){
//            cout<<" ###update env:"<<*env<<endl;
            //             cout<<"    env:";env->dprint();
            //            cout<<"     value:"; value->dprint();
            //            cout<<"     car env:"; car(env)->dprint();
            //            cout<<"     cdar env:"<<*cdar(env)<<endl;
//            cout<<"      #ENV:"<<env<<endl;
            setcdr(car(env),value);
            //            cout<<"     car env:"; car(env)->dprint();
            //            cout<<"     env:";env->dprint();
//            cout<<"      ENV:"<<env<<endl;
            return cdar(env);
        }else{
            return update(id,value,cdr(env));
        }
    }else{
        cout<<"No such binding "<<id->symname(id)<<endl;
        return nil;
    }
}

//input as (a b c) (1 2 3) ,then as env list ((a 1) (b 2) (c 3))
Object * Object::extend(Object *vars,Object* vals,Object* env){
//    cout<<"exentd vars:";vars->dprint();
//    cout<<"exentd vals:";vals->dprint();
    if(vals->car()==symQuote){
        vals=cdr(vals);
    }
    //env->dprint();
    if(vars==NULL||vars==nil){
        if(vals==nil){
            return env;
        }else{
//            cout<<"Too much values."<<endl;
        }
    }else if(vars->type==CONS){
        if(vals->type==CONS){
            env=cons(cons(car(vars),car(vals)),extend(cdr(vars),cdr(vals),env));
        }else{
            cout<<"Too less values."<<endl;
        }
    }else if(vars->type==SYM){
//        cout<<"xx:"<<cons(vars,vals)<<endl;
//        cout<<" ##env:"<<env<<endl;
        env=cons(cons(vars,vals),env);
//        cout<<" env:"<<env<<endl;
    }
    return env;
}


Object * Object::assoc(Object *x,Object *y){
    if(x==NULL||y==NULL)
        return nil;
    if(y->car()->car()==x){
        return y->car()->cdr()->cdr();
    }else{
        return assoc(x,y->cdr());
    }
}



Object *Object::findsym(char* name){
    Object *symlist;
    //DEBUG_INFO;
    for(symlist=symbols;symlist!=NULL&&!isnil(symlist);symlist=cdr(symlist)){
        char *name2=symname(car(symlist));
        //cout<<"=>"<<*symlist;
        //DEBUG_INFO;
        //if(name2!=NULL) cout<<"name2:"<<name2<<endl;
        if(name2!=NULL&&0==strcmp(name,name2)){
//            cout<<"get:"<<name<<" symlist"<<(*symlist)<<endl;
//            cout<<"get:"<<name <<" aadr:"<<(int*)(symlist)<<endl;
            return symlist;
        }
    }
    return nil;
}
int Object::intval(Object *obj){
    if(obj->type==INT){
        return *(int*)obj->data;
    }else if(obj->type==FLOAT){
        int d;
        d=(int)*(double*)obj->data;
        return d;
    }else{
        cout<<"erro obj "<<obj<<" type intval."<<endl;
        return -1;
    }
}
double Object::floatval(Object *obj){
    if(obj->type==FLOAT){
        return *(double*)obj->data;
    }else if(obj->type==INT){
        double d;
        d=(double)*(int*)obj->data;
        return d;
    }else{
        cout<<"erro obj "<<obj<<"  type floatval."<<endl;
        return -1.0;
    }
}




bool Object::isnil(Object *obj){
    if(obj==nil)
        return true;
    return false;
}

Object *Object::car(Object *obj){
    if(obj!=NULL&&(obj->obj!=NULL)&&!isnil(obj)){
        //cout<<obj->obj[0]<<endl;
        return obj->obj[0];
    }
    //return obj;
    return nil;
}

Object *Object::cdr(Object *obj){
    
    if(obj!=NULL&&obj->obj!=NULL&&!isnil(obj))
        return obj->obj[1];
    return nil;
    
}

Object *Object::setcdr(Object *obj1,Object *obj2){
    if(obj1!=NULL&&obj1->obj!=NULL&&!isnil(obj1)){
        obj1->obj[1]=obj2;
        return obj2;
    }
    return nil;
}
Object *Object::setcar(Object *obj1,Object *obj2){
    if(obj1!=NULL&&obj1->obj!=NULL&&!isnil(obj1)){
        obj1->obj[0]=obj2;
//        cout<<"===================obj1:"<<*obj1<<" obj2:"<<obj2<<endl;

//        if(obj2->symname()!=NULL)
//        cout<<__FILE__<<" "<<__LINE__<<obj2->symname()<<endl;
        return obj2;
    }
    return nil;
}




//method

Object *Object::eval(Object *env){
    Object *ret;
    if(this==nil)
        return nil;
    cout<<this<<endl;
    if(this->atom()!=nil){
        return env=assoc(this,env);
    }else{
        if(this==symQuote){
            return cadr(this);
        }else if(this==symAtom){
            return cadr(this)->eval(env)->atom();
        }else if(this==symEq){
            return cadr(this)->eval();
        }else if(this==symCar){
            return this->eval(env)->car();
        }else if(this==symCdr){
            this->eval(env)->cdr();
        }else {
            return (this->car()->eval(env)->cons(this->cdr()));
        }
        
    }
    return ret;
}
Object *Object::cond(Object *env){
    Object *ret;
    
    return ret;
}
Object *Object::execute(Object *env){
    Object *e=this;
    while(e!=NULL&&!(e->isnil()) ){
        cout<<"exp:"<<e<<endl;
        e=e->eval();
        cout<<"e:"<<e<<endl;
    }
    return e;
}


Object *Object::atom(){
//    cout<<"type:"<<type<<endl;
    if(this->type==INT||this->type==SYM||this->type==STRING||this->type==FLOAT){
        return tee;
    }
    return nil;
}




Object *Object::car(){
    return this->car(this);
    //return (this->obj[0]);
}
Object *Object::cdr(){
    return this->cdr(this);
    //return this->obj[1];
}
Object *Object::cons(Object *obj){
    return this->cons(this,obj);
}
//Object *Object::caar(){
//    return this->car()->car();
//}
//Object *Object::cdar(){
//    return this->car()->cdr();
//}
char * Object::symname(){
    return this->symname(this);
}

bool Object::isnil(){
    return  isnil(this);
}
bool Object::is_nil(){
    return  isnil(this);
}
bool Object::is_alive(){
    return this->isalive;
}

bool Object::is_pair(){
    if(this->type==CONS){
        return true;
    }
    return false;
}
bool Object::is_atom(){
    if(this->type==INT||type==STRING||type==SYM){
        return true;
    }
    return false;
}
bool Object::is_null(){
    if(this->obj==NULL||(this==nil&&this->car()==NULL&&this->cdr()==NULL) ){
        //cout<<"ret->this;"<<this<<endl;
        return true;
    }
    return false;
}

Object *Object::setcdr(Object *obj1){
    return setcdr(this,obj1);
}
Object *Object::setcar(Object *obj1){
//    cout<<"==============@@@@@obj1:"<<*this<<" obj2:"<<obj1<<endl;

    return setcar(this,obj1);
}

void Object::print(){
    cout<<*this;
    if(this==NULL)
        return;
    if(this->type==CONS){
        cout<<" "<<*this->car()<<" . "<<*this->cdr();
    }
    else{
        cout<<" "<<*this->car()<<" "<<*this->cdr();
    }
    cout<<endl;
    
}
void Object::tprint(){
    Object *o;
    static int i=0;
    int count=0;
    
    for(o=this;!isnil(o);o=o->cdr()){
        i+=4;
        cout<<setw(4)<<"["<<count<<*o<<"]"<<endl;
        cout<<setw(i )<<"["<< *o->car()<<"]";
        //cout<<setw(4)<<"["<<*o->cdr()<<"]"<<endl;
        count++;
    }
}
void Object::dprint(){
    dprint(this);
    cout<<endl;
}
void Object::dprint(Object *o){
    if(o==NULL){
        cout<<"NIL";
    }else if(o->type==SYM){
        //cout<<"SYM:";
        cout<<(char*)o->data;
    }else if(o->type==INT){
        if(o->data!=NULL)
            cout<<*(int*)o->data;
        else
            cout<<"data nil";
    }else if(o->type==FLOAT){
        if(o->data!=NULL)
            cout<<*(double*)o->data;
        else
            cout<<"data nil";
    }else if(o->type==STRING){
        string str((const char*)o->data);
//        cout<<"======================="<<endl;
//        cout<<str.substr(1,str.length()-1);
        cout<<str;
    }else if(o->type==PRIMOP||o->type==PROC){
        cout<<"#<PROC "<<o->data<<">";
    }else if(o->type==PROC){
        cout<<"#<PROC "<<(char*)o->data<<">";
    }else if(o==nil){
        cout<<"nil";
    }else if(o==el){
        
    }else{
        cout<<"(";
        dprint(o->car());
        cout<<" . ";
        dprint(o->cdr());
        cout<<")";
    }
}
ostream& Object::dstream(ostream &os){
    dstream(os,this);
    return os;
}
ostream& Object::dstream(ostream &os,Object *o){
    
    if(o==NULL){
        os<<"NIL";
    }else if(o->type==SYM||o->type==BOOL){
        //cout<<"SYM:";
        os<<(char*)o->data;
    }else if(o->type==INT){
        os<<*(long*)o->data;
    }else if(o->type==FLOAT){
        os<<setprecision(10)<<*(double*)o->data;
    }else if(o->type==STRING){
        string str((const char*)o->data);
        os<<str.substr(1,str.length()-2);
    }else if(o->type==PRIMOP||o->type==PROC){
        os<<"#<PROC "<<o->data<<">";
    }else if(o->type==PROC){
        os<<"#<PROC "<<(char*)o->data<<">";
    }else if(o==nil){
        os<<"nil";
    }else if(o==none){
        
    }else if(o==el){
        os<<"nil";
    }else{
        os<<"(";
        dstream(os,o->car());
        os<<" . ";
        dstream(os,o->cdr());
        os<<")";
    }
//    write(os,o);
    return os;
}
istream& operator>>(istream &cin,Object &obj){
    return cin;
}

ostream& Object::write(ostream &os,Object *obj){
//    obj->print();
//    obj->dprint();
    switch (obj->type) {
        case EOB:{
            string str((const char*)obj->data);
            os<<str;
            break;
        }
        case EL:
            os<<"()";
            break;
        case INT:
            os<<*(int*)obj->data;
            break;
        case FLOAT:
            os<<setprecision(10)<<*(double*)obj->data;
            break;
        case STRING:{
            string str((const char*)obj->data);
            os<<str;
            break;
        }
        case SYM:{
//            if((obj->isnil())){
//                os<<"nil";
//            }else{
            if(obj->data!=NULL)
                os<<(obj->symname());
//            }
            break;
        }
        case BOOL:{
            string str((const char*)obj->data);
            os<<str;
        
            break;}
        case CONS:
            os<<"(";
            write_pair(os,obj);
            os<<")";
            break;
        case PRIMOP:
            os<<"#<primop "<<obj->data<<">";
            break;
        case PROC:{
            os<<"#<procedure ";
            if(obj->data!=NULL)
                os<<(char*)obj->data;
            os<<">";
            break;
        }
        default:
            os<<"undefine ";
            break;
    }
    return os;
}
ostream& Object::write_pair(ostream &os,Object *obj){
    Object *first=obj->car();
    Object *rest=obj->cdr();
    Object::write(cout,first);
    if(first->type==CONS){
        cout<<" ";
        Object::write(cout,rest);
    }else if(rest->type==EL){
        
    }else{
        cout<<" . ";
        Object::write(cout,rest);
    }
    return cout;
}

ostream& operator<<(ostream &cout,Object* obj){
//    return   Object::write(cout,obj);
//    cout<<"write:";
//    Object::write(std::cout,obj);
//    cout<<endl;

//    cout<<" ";
//    std::cout<<*obj<<endl;
    
    if(obj==NULL){
        cout<<"NULL";
        return cout;
    }
    else if(obj==obj->none){
        return cout;
    }else if(obj->type==EOB){
//        cout<<"eob"<<endl;
        if(obj->symname()!=NULL){
//            cout<<"eob:"<<obj->symname();
        }
        return cout;
    }
    else if(obj->type==EL){
        cout<<"()";
        return cout;
    }else if(obj->type==SYM||obj->type==BOOL){
        if(obj->data!=NULL)
            cout<<(obj->symname());
    } else if(obj->type==INT){
        cout<<*(int*)obj->data;
    }else if(obj->type==FLOAT){
        cout<<setprecision(10)<<*(double*)obj->data;
    }else if(obj->type==STRING){
        string str((const char*)obj->data);
                cout<<str;
//        printf("===========%s\n",(char*)obj->data);
        //cout<<str.substr(1,str.length()-2);
    }else if(obj->type==CONS){
        cout<<"(";
        while(true) {
            cout<<(obj->car());
            if(obj->cdr()==Object::el){
                cout<<")";
                break;
            }else if(obj->cdr()==Object::nil){
                
            }else{
                cout<<" ";
            }
            obj=(obj->cdr());
             if(obj->type==EL){
//                        cout<<"()";
             }else if(obj==Object::nil){
                 cout<<")";
                 break;
             }else if(obj->type!=CONS){
                cout<<". ";
                cout<<obj;
                cout<<")";
                break;
            }
        }
        //cout<<")";
        
    }else if(obj->type==PROC){
        cout<<"#<procedure ";
        if(obj->data!=NULL)
            cout<<(char*)obj->data;
        cout<<">";
//        cout<<obj->symname();
        
    }else if(obj->type==PRIMOP){
        cout<<"#<primop "<<obj->data<<">";
    }
    else {
        cout<<"undefine type:"<<obj->type<<".";
    }
    return cout;
}

ostream& operator<<(ostream &cout,Object&obj){
    if(&obj==NULL){
        cout<<"NULL";
        return cout;
    }else if(obj.isnil(&obj)){
        cout<<"nil";
        return cout;
    }else if(&obj==Object::none){
        return cout;
    }
    //cout<<"type:";
    if(obj.type==SYM){
        cout<<"SYM";
        if(obj.data!=NULL)
            cout<<" "<<obj.symname(&obj);
    }else if(obj.type==INT){
        cout<<"INT";
        cout<<" "<<*(int*)obj.data;
    }else if(obj.type==FLOAT){
        cout<<"FLOAT";
        cout<<" "<<*(double*)obj.data;
    }else if(obj.type==STRING){
        cout<<"STRING";
        string str((const char*)obj.data);
        cout<<" "<<str;
    }else if(obj.type==CONS){
        cout<<"CONS";
        //cout<<" "<<*obj.car()<<" . ";obj.print();
    }else if(obj.type==PROC){
        cout<<"PROC";
    }else if(obj.type==PRIMOP){
        cout<<"PRIMOP";
    }else {
        cout<<"UNDEFINE TYPE:"<<obj.type<<".";
    }
    //cout<<endl;
    return cout;
}




//build in function

//数学运算、字符串运算(todo)
Object *Object::sum(Object *args){
//    cout<<"   sum args:";args->dprint();
    double fsum=0.0;
    int isum=0;
    bool isfloat=false;
    
    for(;!isnil(args);args=cdr(args)){
        //        cout<<"    car(args):";car(args)->dprint();
        Object *num=car(args);
        if(num->type==INT){
            isum+=intval(num);
        }else if(num->type==FLOAT){
            isfloat=true;
            fsum+=floatval(num);
        }else{
           cout<<num<<" unkown type to sum."<<endl;
        }
    }
//    cout<<"       "<<fsum+isum<<endl;
    if(isfloat){
        return Object::mkfloat(fsum+isum);
    }
    return Object::mkint(isum);
}
Object *Object::sub(Object *args){
    int isub=0;
    double fsub=0.0;
    bool isfloat=false;
    
//    cout<<"   sub args:";args->dprint();
    Object *first=(car(args));
    if(cdr(args)==nil){
        if(first->type==INT){
            return Object::mkint(-intval(first));
        }else if(first->type==FLOAT){
            return Object::mkfloat(-floatval(first));
        }
    }else{
        
        if(first->type==INT){
            isub=intval(first);
        }else if(first->type==FLOAT){
            fsub=floatval(first);
            isfloat=true;
        }

        for(args=cdr(args);!isnil(args);args=cdr(args)){
            //        cout<<"    car(args):";car(args)->dprint();
            Object *num=car(args);
//            cout<<num<<endl;
            if(num->type==INT){
                isub-=intval(num);
            }else if(num->type==FLOAT){
                isfloat=true;
                fsub-=floatval(num);
            }else{
                cout<<num<<" unkown type to sub."<<endl;
            }
        }
    }
//    cout<<"       "<<fsub+isub<<endl;

    if(isfloat){
        return Object::mkfloat(fsub+isub);
    }
    return Object::mkint(isub);
}

Object *Object::mul(Object *args){
//    cout<<"   mul args:";args->dprint();
    int imul=1;
    double fmul=1.0;
    bool isfloat=false;
    
    //cout<<"   sub args:";args->dprint();
    Object *first=(car(args));
    if(cdr(args)==nil){
        if(first->type==INT){
            return Object::mkint(intval(first));
        }else if(first->type==FLOAT){
            return Object::mkfloat(floatval(first));
        }
    }else{
        for(;!isnil(args);args=cdr(args)){
            //        cout<<"    car(args):";car(args)->dprint();
            Object *num=car(args);
//                        cout<<num<<endl;
            if(num->type==INT){
                imul*=intval(num);
            }else if(num->type==FLOAT){
                isfloat=true;
                fmul*=floatval(num);
            }else{
                cout<<num<<" unkown type to mul."<<endl;
            }
        }
    }
//    cout<<"       "<<fmul*imul<<endl;

    if(isfloat){
        return Object::mkfloat(fmul*imul);
    }
    return Object::mkint(imul);
    
    
}
Object *Object::div(Object *args){
    
    int idiv=1;
    double fdiv=1.0;
    bool isfloat=false;
    
//    cout<<"   div args:";args->dprint();
    Object *first=(car(args));
    if(cdr(args)==nil){
        if(first->type==INT){
            return Object::mkint(intval(first));
        }else if(first->type==FLOAT){
            return Object::mkfloat(floatval(first));
        }
    }else{
//        if(first->type==INT){
//            idiv=intval(first);
//        }else if(first->type==FLOAT){
            fdiv=floatval(first);
//            isfloat=true;
//        }
//        if(first->type==FLOAT){
//            isfloat=true;
//        }
        for(args=cdr(args);!isnil(args);args=cdr(args)){
//            cout<<"    car(args):";car(args)->dprint();
            Object *num=car(args);
//            cout<<num<<endl;
            if(num->type==INT){
//                idiv/=intval(num);
//                cout<<"idiv:"<<idiv<<endl;
            }else if(num->type==FLOAT){
                isfloat=true;
            
            }else{
//                cout<<"unkown type to div."<<endl;
            }
            fdiv/=floatval(num);
        }
    }
//    cout<<"       "<<fdiv<<endl;

//    if(isfloat){
        return Object::mkfloat(fdiv);
//    }
//    return Object::mkint(fdiv);
    
}

Object *Object::prim_numeq(Object *args) {
//    cout<<"     primnumeq";args->dprint();
//    cout<<"     ="<<o<<endl;
    Object *first=(car(args));
    if(cdr(args)==nil){
        cout<<"error = require two argument at least."<<endl;
        return nil;
    }else{
        int icur=intval(first);
        double fcur=floatval(first);
        
        for(args=cdr(args);!isnil(args);args=cdr(args)){
            Object *num=car(args);
            if(num->type==INT){
                if(icur!=intval(num)){
                    return fee;
                }else{
                    icur=intval(num);
                    fcur=floatval(num);
                }
            }else if(num->type==FLOAT){
                if(fcur!=floatval(num)){
                    return fee;
                }else{
                    fcur=floatval(num);
                    icur=fcur;
                }
            }else{
                cout<<num<<" other type not support"<<endl;
                return nil;
            }
        }
        return tee;
    }
    
}

Object *Object::prim_numgt(Object *args) {
//    cout<<"primnumgt";args->dprint();
    Object *first=(car(args));
    if(cdr(args)==nil){
        cout<<"error > require two argument at least."<<endl;
        return nil;
    }else{
        int icur=intval(first);
        double fcur=floatval(first);
        
        for(args=cdr(args);!isnil(args);args=cdr(args)){
            Object *num=car(args);
            if(num->type==INT){
                if(icur<=intval(num)){
                    return fee;
                }else{
                    icur=intval(num);
                    fcur=floatval(num);
                }
            }else if(num->type==FLOAT){
                if(fcur<=floatval(num)){
//                    cout<<"     "<<fee<<endl;
                    return fee;
                }else{
                    fcur=floatval(num);
                    icur=fcur;
                }
            }else{
                cout<<num<<" other type not support"<<endl;
                return nil;
            }
        }
//        cout<<"     "<<tee<<endl;
        return tee;
    }

}
Object *Object::prim_numlt(Object *args) {
//    cout<<"     primnumlt";args->dprint();
    Object *first=(car(args));
    if(cdr(args)==nil){
        cout<<"error < require two argument at least."<<endl;
        return nil;
    }else{
        int icur=intval(first);
        double fcur=floatval(first);
        
        for(args=cdr(args);!isnil(args);args=cdr(args)){
            Object *num=car(args);
//            cout<<"         num:"<<num<<endl;
            if(num->type==INT){
                if(icur>=intval(num)){
//                    cout<<"     3:"<<fee<<endl;

                    return fee;
                }else{
                    icur=intval(num);
                    fcur=floatval(num);
                }
            }else if(num->type==FLOAT){
                if(fcur>=floatval(num)){
//                    cout<<"     "<<fee<<endl;

                    return fee;
                }else{
                    fcur=floatval(num);
                    icur=intval(num);
                }
            }else{
                cout<<num<<" other type not support"<<endl;
                return nil;
            }
        }
//        cout<<"     ee:"<<tee<<endl;

        return tee;
    }
}
//end of数学运算


Object *Object::prim_cons(Object *args) {
//    cout<<"prim_cons";args->dprint();

    return cons(car(args), car(cdr(args)));
}
Object *Object::prim_car(Object *args)  {
    //    cout<<"primcar";args->dprint();
    return car(car(args));
}
Object *Object::prim_cdr(Object *args)  {
    //cout<<"primcdr";args->dprint();
    //cout<<args<<endl;
    Object *ret=(cdr(car(args)));
    if(ret==nil){
        ret=cons(ret,nil);
    }
    return ret;
}
Object *Object::prim_or(Object *args)  {
//   cout<<"   primor:";args->dprint();
//    cout<<args<<endl;
    Object *ret=fee;
    for(;!isnil(args);args=cdr(args)){
        if(car(args)==tee){
//            cout<<"     ="<<car(args)<<endl;
            return car(args);
        }
    }
//    cout<<"     ="<<ret<<endl;

    return ret;
}
Object *Object::prim_and(Object *args)  {
//    cout<<"   primand:";args->dprint();
    //    cout<<args<<endl;
    Object *ret=fee;
    for(;!isnil(args);args=cdr(args)){
        if(car(args)!=tee){
//            cout<<"     ="<<car(args)<<endl;
            return car(args);
        }
    }
//    cout<<"     ="<<ret<<endl;
    
    return ret;
}

Object *Object::add_procedure(char *name,FUNCTION function){
    Object* funob=mkpriop(function);
    topEnv=extend(inter(name),funob,topEnv);
    return funob;
}

