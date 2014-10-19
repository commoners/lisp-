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
    this->type=CONS;
    this->data=NULL;
    this->obj=NULL;
    isalive=true;
    this->flag=INIT;
//    cout<<__FUNCTION__<<" "<<(int *)this<<endl;
    memory->add_obj(this);
}
Object* Object::mkobj(char* name){
    Object *thiz=memory->new_obj();
    size_t len=strlen(name);len++;
    thiz->type=SYM;
    thiz->data=new char(len);
    memset(thiz->data,0,len);
    strcpy((char*)thiz->data,(const char*)name);
    thiz->obj=NULL;
    thiz->isalive=true;
    thiz->flag=INIT;
    return thiz;
}


Object *Object::mkobj(OType type,void* data){
    Object* thiz=memory->new_obj();
    thiz->type=type;
    thiz->data=data;
    thiz->obj=NULL;
    thiz->isalive=true;
    thiz->flag=INIT;
    return thiz;
}

Object* Object::mkobj(OType type,int count,...){
    Object *thiz=memory->new_obj();
    va_list ap;
    int i;
    va_start(ap, count);
    //    this->obj = (Object**) new Object[count]();
    thiz->obj=memory->new_objs(count);
    thiz->type = type;
    thiz->isalive=true;
    for(i = 0; i < count; i++){
       thiz->obj[i] =(Object *)va_arg(ap,Object*);
        //        cout<<"obj["<<i<<"]="<<*(obj[i])<<endl;
    }
    va_end(ap);
    thiz->flag=INIT;
    //    cout<<"======"<<this<<endl;
    return thiz;
}



Object *Object::cons(Object* obj1,Object *obj2){
    Object *o;
    //cout<<"obj1:";obj1->dprint();
//    cout<<"obj2:";obj2->dprint();
    //o=mkobj(CONS,2,obj1,obj2);
    o=mkobj(CONS,2,obj1,obj2);
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
Object *Object::mkint(size_t val){
    size_t *p=(size_t*) new int(sizeof(size_t));
    *p=val;
    Object *o=mkobj(INT,p);
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
    Object *o=  mkobj(PROC,3,args,body,env);
    return o;
}
Object * Object::mkproc(char* name,Object *args,Object *body,Object* env){
    //cout<<"############"<<name<<endl;
    Object *o=mkobj(PROC,3,args,body,env);
    size_t len=strlen(name);
    len++;
    o->data=malloc(len);
    memset(o->data,0,len);
    strcpy((char*)o->data,(const char*)name);
    //    cout<<"@@@@@@@@@mkproc name:"<<(char*)o->data<<endl;
    return o;
}






char*  Object::symname(Object *obj){
    char* ret="";
    if(obj==NULL||obj->data==NULL){
        return ret;
    }
    ret=(char *)(obj->data);
    //    cout<<ret<<endl;
    
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
    nil=inter("nil");
    symbols=cons(nil,nil);
    
    tee=inter("#t");
    fee=inter("#f");
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
    add_procedure("display", display);
    add_procedure("newline", newline);
    
    //    cout<<"topEnv:";topEnv->dprint();
}
//for expression  symbol and make callable function
Object* Object::eval(Object *exp,Object *env){
    if(exp==nil) return nil;
    switch (exp->type) {
        case INT:
            return exp;
        case SYM:{
            //            cout<<"-SYM"<<endl;
            Object *vals=lookup(exp, env);;
            if(vals!=nil){
                //                cout<<"  Found SYM "<<exp->symname()<<endl;
                return vals;
            }else{
                vals=lookup(exp,topEnv);
                if(vals!=nil){
                    //                    cout<<"  Found G SYM "<<exp->symname()<<endl;
                    return vals;
                }else{
                    //                    cout<<"  No found SYM "<<exp->symname()<<endl;
                    
                }
                
            }
            return exp;
            
        }
        case CONS:{
            //            cout<<"exp:";exp->dprint();
            //exp->tprint();
            Object *ex=car(exp);
            if(ex==symDefine){
                Object * vals=cddr(exp);
                Object * vars=cadr(exp);
                if(cadr(exp)->type==SYM){
                    vals=eval(car(cdr(cdr(exp))),env);
                    //vals=car(cdr(cdr(exp)));
                    //                    cout<<" FIRST DEFILE"<<endl;
                    //                    cout<<" val=";
                    //                    vals->dprint();
                    // avals=eval(car(cdr(cdr(exp))),env);
                    topEnv=extend(vars,vals,topEnv);
                    //topEnv->dprint();
                    //                    cout<<" define:";vals->dprint();
                    return vals;
                }else{
                    vars=caadr(exp);
                    //                    cout<<" mk--proc"<<endl;
                    //                    vars->dprint();
                    //                    vals->dprint();
                    //                    cout<<" cdadr(exp):";cdadr(exp)->dprint();
                    //cout<<vars<<endl;
                    //cout<<*vars<<endl;
                    Object *proc;
                    //env=extend(vars,proc,env);
                    proc=mkproc(car(exp)->symname(), cdadr(exp),vals,env);
                    topEnv=extend(vars,proc,topEnv);
                    //cout<<proc<<endl;
                    return proc;
                }
                
            }else if(ex==symLambda){
                Object *proc=mkproc(car(exp)->symname(),cadr(exp),cddr(exp),topEnv);
                topEnv=extend(caadr(exp),proc,topEnv);
                //                cout<<" mkproc:";proc->dprint();
                return proc;
            }else if(ex==symQuote){
                //                cout<<" quote:";exp->dprint();
                //                cdr(exp)->dprint() ;
                return cdr(exp);
            }else if(ex==symIf){
                if(eval(car(cdr(exp)), env) != nil)
                    exp = car(cdr(cdr(exp)));
                else
                    exp = car(cdr(cdr(cdr(exp))));
                return eval(exp,env);
            }else if(ex==symSetb){
                //                cout<<" =====setb"<<endl;
                //                cout<<" ======env:";env->dprint();
                //                cadr(exp)->dprint();
                //                cout<<"cddr:";cddr(exp)->dprint();
                //cout<<" ee:";eval(cddr(exp), env)->dprint();
                Object *vals=update(cadr(exp),eval(cddr(exp), env),env);
                //                cout<<" ======#env:";env->dprint();
                if(vals==nil){
                    //                    cout<<"No find, "<<*cadr(exp)<<endl;
                }
                return vals;
            }else if(ex==symBegin){
                Object *ret=nil;
                for(exp=cdr(exp);exp!=nil;exp=cdr(exp)){
                    //                    cout<<"============================";car(exp)->dprint();
                    //                    cout<<"#############################env";env->dprint();
                    ret=eval(car(exp),env);
                    //                    cout<<"ret:";ret->dprint();
                }
                return ret;
                
            }
            //            cout<<"  exp car:";ex->dprint();
            //            cout<<"  exp cdr:";exp->cdr()->dprint();
            
            if(exp->car()->type==PRIMOP){
                //                cout<<"    PRIMOP"<<endl;
                return apply(eval(car(exp),env),cdr(exp),env);
            }else if(exp->car()->type==CONS){
                //                cout<<"    CONS"<<endl;
                //                return exp;
                //                return cons(eval(car(exp),env),eval(cdr(exp),env));
                //                return eval(cons(eval(car(exp),env),eval(cdr(exp),env)),env);
                return apply(eval(car(exp),env),cdr(exp),env);
            }else if(exp->car()->type==SYM){
                //                cout<<"    SYM"<<endl;
                return apply(eval(car(exp),env),cdr(exp),env);
                //                return eval(car(exp),env),cdr(exp);
                //return eval(cons(eval(car(exp),env),cdr(exp)),env);
            }else if(exp->car()->type==PROC){
                //                cout<<"    PROC"<<endl;
                return exp;
            }else{
                //                cout<<"    ELSE"<<endl;
                //return cons(car(exp),eval(cdr(exp),env));
                //return apply(eval(car(exp),env),cdr(exp),env);
                //return cons(eval(car(exp),env),eval(cdr(exp),env));
                return exp;
            }
        }case PROC:{
            //            cout<<"PROC---";exp->dprint();
        }case PRIMOP:{
            
        }default:{
            break;
        }
    }
    //    cout<<"ret exp:";exp->dprint();
    return exp;
}
//call of the function with a given set of arguments (apply f (list x))
Object * Object::apply(Object *proc,Object *args,Object *env){
    //    cout<<endl;
    if(proc==nil)
        return nil;
    //    cout<<"proc:";proc->print();
    Object *vals=evlis(args, env);
    //     cout<<"  eval vals:";vals->dprint();
    Object *fn=eval(proc, env);;
    //    cout<<"  eval fn:";fn->dprint();
    if(proc->type==PRIMOP) {//内建函数
        //        cout<<"+++++++++PRIMOP"<<endl;
        return ((FUNCTION)(fn->data))(vals);
    }else if(proc->type==PROC){
        //        cout<<"+++++++++PROC"<<endl;
        //cout<<proc<<endl;
        Object* e=extend(car(proc),vals,proc->obj[2]);
        //        car(proc)->dprint();
        //        vals->dprint();
        //        e->dprint();
        return eval(cdr(proc),e);
    }else{
        //cout<<"no apply."<<endl;
        return proc;
    }
    return nil;
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
                                cout<<" ==============else cdar:"<<cdar(e)<<endl;

                                exp=cdar(e); //->eval2(cdar(e),env);
                                cout<<" ==============else ret:"<<exp<<endl;
                                //return exp;
                                goto call_eval_start;

                            }else{
                                cout<<"test"<<endl;
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
                            cout<<"ret exp:"<<exp<<endl;
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
        else {
            Object* if_cond=car(first);
            Object *if_seq=sequence_to_exp(cdr(first));
            Object *if_else=expand_clauses(rest);
            return cons(symIf,cons(  if_cond, cons(if_seq,cons(if_else,nil))  ));
        }
    }
}


Object * Object::eval3(Object *exp,Object *env){
    Object *procedure;
    Object *arguments;
    Object *result;
calltail:
//   cout<<"#exp:"<<exp<<" #car(exp):"<<car(exp)<<" #cdr(exp):"<<cdr(exp)<<endl;
//    exp->dprint();
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
    
    
    Object *sym=car(exp);
    //cout<<"     sym:"<<sym<<endl;
    if(exp->type==INT||exp->type==STRING){//to do string boolean and more data type
//        cout<<"ret:"<<exp<<endl;
        return exp;
    }else if(exp->type==SYM){
//        cout<<"     lookpu:"<<exp<<endl;
        Object* symbol=lookup(exp, env);
        if(symbol==nil){
            symbol=lookup(exp,topEnv);
            if(symbol==nil) symbol=exp;
        }
        return symbol;
    }else if(exp->type==CONS&&car(exp)->type==SYM&&sym==symQuote){
//        cout<<"============="<<cdr(exp)<<endl;
        return cdr(exp);
    }else if(exp->type==CONS&&car(exp)->type==SYM&&sym==symSetb){
        Object *set_var=cadr(exp);
        Object *set_val=caddr(exp);
//        cout<<"     set_var:"<<set_var<<" set_val:"<<set_val<<endl;
        set_val=set_val->eval3(set_val,env);
//        cout<<"     =set_var:"<<set_var<<" =set_val:"<<set_val<<endl;

        Object *ret=update(set_var, set_val, env);
        if(ret==nil)
            ret=update(set_var, set_val, topEnv);
        return symStatus;
    }else if(exp->type==CONS&&car(exp)->type==SYM&&sym==symDefine){
        Object *def_var;
        Object *def_val;
        
        if(cadr(exp)->type==SYM){
            def_var=cadr(exp);
            def_val=caddr(exp);
            def_val=def_val->eval3(def_val,env);
            Object *findret=lookup(def_var, topEnv);;
            if(findret==nil){
                topEnv=extend(def_var,def_val,topEnv);
            }else{
                update(def_var,def_val,topEnv);
            }
            //cout<<"hhehe"<<def_var<<" val"<<def_val<<endl;
        }else{
            def_var=caadr(exp);
            Object *params=cdadr(exp);
            Object *body=cddr(exp);
            //def_val=cons(symLambda,cons(params,body));//make lambda
            //cout<<"def_var:"<<def_var<<" params:"<<params<<" body:"<<body<<endl;
            Object *proc;
            proc=mkproc(car(exp)->symname(), params,body,env);
            topEnv=extend(def_var,proc,topEnv);
            
        }
        
        return symStatus;
    }else if(exp->type==CONS&&car(exp)->type==SYM&&sym==symIf){
        Object* if_test=cadr(exp);
        Object* if_consequent=caddr(exp);
        Object * if_alternate=cadddr(exp);
        Object *test=if_test->eval3(if_test,env);
//        cout<<"     exp:"<<exp<<endl;
//        cout<<"         if_test:"<<if_test<<"="<<test<<endl;
//        cout<<"         if_consequent:"<<if_consequent<<endl;
//        cout<<"         if_alternate:"<<if_alternate<<endl;
//        cout<<"         cdddr(exp):"<<cdddr(exp)<<endl;
        if(test==tee||(test!=nil&&test!=fee)){
            exp=if_consequent;//first exp
        }else{//else exp
            if(cdddr(exp)==nil){
                //cout<<"if_alternate is nil"<<endl;
                exp=fee;
            }else{
                //cout<<"      =====if_alternate:"<<if_alternate<<endl;
                exp=if_alternate;
            }
        }
//        cout<<"     ret:"<<exp<<endl;
//        cout<<endl;

        goto calltail;
    }else if(exp->type==CONS&&car(exp)->type==SYM&&sym==symLambda){
        Object *lambda_parms=cadr(exp);
        Object *lambda_body=cddr(exp);
        return mkproc(lambda_parms, lambda_body, env);
    }else if(exp->type==CONS&&car(exp)->type==SYM&&sym==symBegin){
        exp=cdr(exp);
        while(cdr(exp)!=nil){//is the last
            exp->eval3(car(exp),env);
            exp= cdr(exp);
        }
        exp=car(exp);
        goto calltail;
    }else if(exp->type==CONS&&car(exp)->type==SYM&&sym==symCond){
        Object *the_clauses=cdr(exp);
        if(cdr(the_clauses)==nil){
            return fee;
        }else{
//            cout<<"             clauses:"<<the_clauses<<endl;
            exp=expand_clauses(the_clauses);
//            cout<<"             clauses="<<exp<<endl;
        }
        goto calltail;
    }else if(exp->type==CONS&&car(exp)->type==SYM&&sym==symLet){
        
    }else if(exp->type==CONS){//proc to execute
//        cout<<"     app:"<<exp<<endl;
        Object *op=car(exp);
        Object *ol=cdr(exp);
        
//        cout<<"         op:"<<op<<" type:"<<op->type<<" ol:"<<ol<<endl;
        procedure=op->eval3(op,env);
        arguments=ol->evlis3(ol, env);
//        cout<<"         =op:"<<op<<" type:"<<procedure->type<<" =ol:"<<arguments<<endl;
//        cout<<"     app:"<<exp<<" ["<<op<<" "<<arguments<<"]"<<endl;
    
        if(procedure->type==PRIMOP){
//            cout<<"         "<<((FUNCTION)(procedure->data))(arguments)<<endl;
            return ((FUNCTION)(procedure->data))(arguments);
        }else if(procedure->type==PROC){
            Object *proc_args=car(procedure);//arg
            Object *proc_body=cdr(procedure);//body
            Object *proc_env=procedure->obj[2];
            env=extend(proc_args,arguments,proc_env);
//            exp=cons(symBegin,proc_body);//make begin
            exp=proc_body;
            goto calltail;
        }else if(procedure->type==SYM||procedure->type==INT||procedure->type==STRING){
            exp=procedure;
            goto calltail;
        }else{
            cout<<"erro procedure type"<<procedure->type<<" "<<procedure<<"."<<endl;
        }
    }else{
        cout<<" eval erro "<<exp<<endl;
    }
    cout<<"eval unknow state"<<endl;
    return exp;
    
}
Object * Object::evlis3(Object *exps,Object *env){
    if(exps == nil) return nil;
    //    cout<<"exps:"<<exps<<" env:"<<env<<endl;
//       cout<<"     evlis3 exps:";exps->dprint(); cout<<"    env:"<<env<<endl;
    
    //    Object *ret=cons(eval2(car(exps), env),evlis2(cdr(exps), env));
    Object* t=cons(car(exps),nil);
//    cout<<"     t:"<<endl;
    Object*r=t;
    Object*l=nil;
    for(;exps->type==CONS;exps=cdr(exps)){
//               cout<<" ****exps:"<<exps<<endl;
        Object *te=exps->eval3(car(exps),env);
//        cout<<" ****vals:"<<te<<endl;
        t->setcar(te);
        t->setcdr(cons(nil,nil));
        //            cout<<" ********t:"<<t<<endl;
        l=t;
        t=t->cdr();
        
    }
    l->setcdr(nil);
//    cout<<"     ret:"<<r<<endl;
    return r;
}
Object * Object::apply3(Object *proc,Object *args,Object *env){
    args=proc->evlis3(args,env);
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
    if(env==NULL||env==nil){
        return nil;
    }else if(env->type==CONS){
        //cout<<"cmp id:"<<id<<" "<<caar(env)<<" :::"<< (id==caar(env))<<endl;

        if((id==caar(env))||id->symname()==caar(env)->symname()){
            //cout<<"find"<<endl;
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




Object* Object::evlis(Object *exps,Object *env){
    if(exps == nil) return nil;
    return cons(eval(car(exps), env),
                evlis(cdr(exps), env));
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
            //cout<<"get:"<<name<<" symlist"<<(*symlist)<<endl;
            return symlist;
        }
    }
    return nil;
}
size_t Object::intval(Object *obj){
    if(obj->type==INT){
        return *(int*)obj->data;
    }else{
        return -1;
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
    if(this->type==INT||this->type==SYM){
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
            cout<<*(long*)o->data;
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
    }else if(o->type==SYM){
        //cout<<"SYM:";
        os<<(char*)o->data;
    }else if(o->type==INT){
        os<<*(long*)o->data;
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
        
    }else{
        os<<"(";
        dstream(os,o->car());
        os<<" . ";
        dstream(os,o->cdr());
        os<<")";
    }
    return os;
}
istream& operator>>(istream &cin,Object &obj){
    return cin;
}
ostream& operator<<(ostream &cout,Object* obj){
    
    if(obj==NULL){
        cout<<"NULL";
        return cout;
    }
    else if(obj==obj->none){
        return cout;
    }
    if(obj->type==SYM){
        if((obj->isnil())){
            cout<<"nil";
            return cout;
        }
        if(obj->data!=NULL)
            cout<<(obj->symname());
    }else if(obj->type==INT){
        cout<<*(int*)obj->data;
    }else if(obj->type==STRING){
        string str((const char*)obj->data);
                cout<<str;
        //cout<<str.substr(1,str.length()-2);
    }else if(obj->type==CONS){
        cout<<"(";
        while(true) {
            cout<<(obj->car());
            if((obj->cdr()->isnil())){
                cout<<")";
                break;
            }else{
                cout<<" ";
            }
            obj=(obj->cdr());
            if(obj->type!=CONS){
                cout<<" . ";
                cout<<obj;
                cout<<")";
                break;
            }
        }
        //cout<<")";
        
    }else if(obj->type==PROC){
        cout<<"#<PROC "<<obj->data<<">";
        
    }else if(obj->type==PRIMOP){
        cout<<"#<PRIMOP "<<obj->data<<">";
    }
    else {
        cout<<"UNDEFINE";
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
        cout<<"UNDEFINE";
    }
    //cout<<endl;
    return cout;
}




//build in function
Object *Object::sum(Object *args){
    size_t sum=0;
//        cout<<"   sum args:";args->dprint();
    for(sum=0;!isnil(args);args=cdr(args)){
        //        cout<<"    car(args):";car(args)->dprint();
        sum+=intval(car(args));
    }
//        cout<<"    sum="<<sum<<endl;
    return Object::mkint(sum);
}
Object *Object::sub(Object *args){
    size_t sub=0;
//    cout<<"   sub args:";args->dprint();
    if(cdr(args)==nil){//only firt args
        sub=-intval(car(args));
    }else{
        sub=intval(car(args));
    }
    for(args=cdr(args);!isnil(args);args=cdr(args)){
        //        cout<<"    car(args):";car(args)->dprint();
        sub-=intval(car(args));
    }
//       cout<<"   ="<<sub<<endl;
    return Object::mkint(sub);
}

Object *Object::mul(Object *args){
    size_t mul=0;
    //    cout<<"   mul args:";args->dprint();
    for(mul=intval(car(args)),args=cdr(args);!isnil(args);args=cdr(args)){
        //        cout<<"    car(args):";car(args)->dprint();
        mul*=intval(car(args));
    }
    //    cout<<"    mul:"<<mul<<endl;
    return Object::mkint(mul);
}
Object *Object::div(Object *args){
    size_t div=0;
    //    cout<<"   div args:";args->dprint();
    for(div=intval(car(args)),args=cdr(args);!isnil(args);args=cdr(args)){
        //        cout<<"    car(args):";car(args)->dprint();
        div/=intval(car(args));
    }
    //    cout<<"    div:"<<div<<endl;
    return Object::mkint(div);
}

Object *Object::prim_numeq(Object *args) {
//    cout<<"     primnumeq";args->dprint();
    Object *o= intval(car(args)) == intval(car(cdr(args))) ? tee : fee;
//    cout<<"     ="<<o<<endl;
    return o;
}
Object *Object::prim_numgt(Object *args) {
    //    cout<<"primnumgt";args->dprint();
    return intval(car(args)) > intval(car(cdr(args))) ? tee : fee;
}
Object *Object::prim_numlt(Object *args) {
//    cout<<"     primnumlt";args->dprint();
    
    Object *ret= intval(car(args)) < intval(car(cdr(args))) ? tee : fee;
//    cout<<"     ="<<ret<<endl;
    return ret;
}

Object *Object::prim_cons(Object *args) {
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

