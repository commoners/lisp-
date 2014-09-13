//
//  obj.cpp
//  lisp++
//
//  Created by evil on 9/7/14.
//  Copyright (c) 2014 evilbinary.org. All rights reserved.
//  rootntsd@gmail.com

#include "object.h"
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
Object *Object::symbols=NULL;
Object *Object::topEnv=NULL;
Object *Object::symLambda=NULL;
Object *Object::symDefine=NULL;
Object *Object::symIf=NULL;
Object *Object::symQuote=NULL;
Object *Object::symSetb=NULL;
Object *Object::tee=NULL;
Object *Object::fee=NULL;
Object *Object::symBegin=NULL;


Object::Object(){
    this->type=CONS;
    this->data=NULL;
    this->obj=NULL;
    //this->obj = (Object**) malloc(sizeof(Object*)+1);
//    this->obj[0]=nil;
//    this->obj[1]=nil;
}
Object::Object(char *name){
    size_t len=strlen(name);len++;
    this->type=SYM;
    this->data=malloc(len);
    memset(this->data,'\0',len);
    strcpy((char*)this->data,(const char*)name);
    this->obj=NULL;
//    this->obj = (Object**) new Object[2]();
//    this->obj[0]=nil;
//    this->obj[1]=nil;
}
Object::Object(OType type,void* data){
    this->type=type;
    this->data=data;
    this->obj=NULL;
//    this->obj = (Object**) new Object[2]();
//    this->obj[0]=nil;
//    this->obj[1]=nil;
}

Object::Object(OType type,int count,...){
    va_list ap;
    int i;
    va_start(ap, count);
    this->obj = (Object**) new Object[count]();
    this->type = type;
    for(i = 0; i < count; i++){
        obj[i] =(Object *)va_arg(ap,Object*);
        //cout<<"obj["<<i<<"]="<<*(obj[i])<<endl;
    }
    va_end(ap);
    
}
Object::~Object(){
    if(this->data!=NULL){
        free(this->data);
        this->data=NULL;
    }
    if(this->obj!=NULL) {
        delete [] obj;
        this->obj=NULL;
    }
}


Object *Object::inter(char *name){
    Object *op=findsym(name);
    if(!isnil(op)){
        return car(op);
    }
    //DEBUG_INFO;
    op=mksym(name);
    symbols=cons(op,symbols);
    return op;
}

void Object::init(){
    nil=inter("nil");
    symbols=cons(nil,nil);
    tee=inter("t");
    fee=inter("f");
    topEnv=cons(tee,fee);
    topEnv=cons(topEnv,nil);

    Object *add=mkpriop(Object::sum);
    Object *sub=mkpriop(Object::sub);
    Object *mul=mkpriop(Object::mul);
    Object *div=mkpriop(Object::div);
   
//    cout<<"add:";add->dprint();
    topEnv=extend(inter("+"),add,topEnv);
    topEnv=extend(inter("-"),sub,topEnv);
    topEnv=extend(inter("*"),mul,topEnv);
    topEnv=extend(inter("/"),div,topEnv);

    Object *mnumeq=mkpriop(Object::primnumeq);
    Object *cons=mkpriop(Object::primcons);
    Object *car=mkpriop(Object::primcar);
    Object *cdr=mkpriop(Object::primcdr);
    Object *mnumgt=mkpriop(Object::primnumgt);
     Object *mnumlt=mkpriop(Object::primnumlt);
    
    topEnv=extend(inter("="),mnumeq,topEnv);
    topEnv=extend(inter("cons"),cons,topEnv);
    topEnv=extend(inter("car"),car,topEnv);
    topEnv=extend(inter("cdr"),cdr,topEnv);
    
    topEnv=extend(inter(">"),mnumgt,topEnv);
    topEnv=extend(inter("<"),mnumlt,topEnv);


    
    symIf=inter("if");
    symQuote=inter("quote");
    symDefine=inter("define");
    symSetb=inter("set!");
    symLambda=inter("lambda");
    symBegin=inter("begin");
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
                    Object *proc=mkproc(car(exp)->symname(), cdadr(exp),vals,env);
                    topEnv=extend(vars,proc,topEnv);
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

Object * Object::lookup(Object *id,Object* env){
    if(env==NULL||env==nil){
        return nil;
    }else if(env->type==CONS){
        if(id->symname()==caar(env)->symname()){
            return cdar(env);
        }else{
            return lookup(id, cdr(env));
        }
    }else{
        cout<<"No such binding "<<id->symname(id)<<endl;
        return nil;
    }
}
Object * Object::update(Object *id,Object *value,Object* env){
    if(env==NULL||env==nil){
        return nil;
    }else if(env->type==CONS){
        if(id->symname()==caar(env)->symname()){
//            cout<<" ###update env:"<<*env<<endl;
//             cout<<"  env:";env->dprint();
//            cout<<" value:"; value->dprint();
//            cout<<" car env:"; car(env)->dprint();
//            cout<<" cdar env:"<<*cdar(env)<<endl;
            setcdr(car(env),value);
//            cout<<" car env:"; car(env)->dprint();
//            cout<<"   env:";env->dprint();
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
    //cout<<"exentd vars:";vars->dprint();
    //cout<<"exentd vals:";vals->dprint();
    //env->dprint();
     if(vars==NULL||vars==nil){
        if(vals==nil){
            return env;
        }else{
            cout<<"Too much values."<<endl;
        }
    }else if(vars->type==CONS){
        if(vals->type==CONS){
            env=cons(cons(car(vars),car(vals)),extend(cdr(vars),cdr(vals),env));
        }else{
            cout<<"Too less values."<<endl;
        }
    }else if(vars->type==SYM){
        return cons(cons(vars,vals),env);
    }
    return env;
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
Object *Object::mkint(size_t val){
    size_t *p=(size_t*)malloc(sizeof(size_t));
    *p=val;
    Object *o=new Object(INT,p);
    return o;
}
Object * Object::mkpriop(FUNCTION fun){
    Object *o=new Object(PRIMOP,(void*)fun);
    return o;
}
Object * Object::mkproc(Object *args,Object *body,Object* env){
    Object *o=new Object(PROC,3,args,body,env);
    return o;
}
Object * Object::mkproc(char* name,Object *args,Object *body,Object* env){
    //cout<<"############"<<name<<endl;
    Object *o=new Object(PROC,3,args,body,env);
    size_t len=strlen(name);len++;
    o->data=malloc(len);
    memset(o->data,'\0',len);
    strcpy((char*)o->data,(const char*)name);
    
    return o;
}





bool Object::isnil(Object *obj){
    return (obj==nil);
}

Object *Object::car(Object *obj){
    if(obj!=NULL&&(obj->obj!=NULL)&&!isnil(obj))
        return obj->obj[0];
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

Object *Object::cons(Object* obj1,Object *obj2){
    Object *o;
    //cout<<"obj1:";obj1->dprint();
    //cout<<" obj2:";obj2->dprint();
    //o=mkobj(CONS,2,obj1,obj2);
    o=new Object(CONS,2,obj1,obj2);
    //cout<<" scons:";
    //o->dprint();
    //cout<<endl;
    //o->tprint();
    //cout<<endl;
    return  o;
}

Object *Object::mksym(char *name){
    //DEBUG_INFO;
    return new Object(name);
}

char*  Object::symname(Object *obj){
    char* ret="";
    if(obj==NULL){
        return ret;
    }
    ret=(char *)obj->data;
    return ret;
}

//Object* Object::mkobj(OType type,int count,...){
//    Object *ret=new Object();
//    va_list ap;
//    int i;
//    ret->type=type;
//    va_start(ap, count);
//    ret->obj= (Object**) new Object[count]();
//    for(i = 0; i < count; i++){
//        ret->obj[i] =(Object *)va_arg(ap,Object*);
//    }
//    va_end(ap);
//    return ret;
//}



istream& operator>>(istream &cin,Object &obj){
    return cin;
}
ostream& operator<<(ostream &cout,Object* obj){

    if(obj==NULL){
        cout<<"NULL";
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
    }else {
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
    }
    //cout<<"type:";
    if(obj.type==SYM){
        cout<<"SYM";
        if(obj.data!=NULL)
            cout<<" "<<obj.symname(&obj);
    }else if(obj.type==INT){
        cout<<"INT";
        cout<<" "<<*(int*)obj.data;
    }else if(obj.type==INT){
        cout<<"INT";
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
//method

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
        cout<<*(size_t*)o->data;
        else
            cout<<"data nil";
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
        cout<<*(size_t*)o->data;
    }else if(o->type==PRIMOP||o->type==PROC){
        cout<<"#<PROC "<<o->data<<">";
    }else if(o->type==PROC){
        cout<<"#<PROC "<<(char*)o->data<<">";
    }else if(o==nil){
        os<<"nil";
    }else{
        os<<"(";
        dstream(os,o->car());
        os<<" . ";
        dstream(os,o->cdr());
        os<<")";
    }
    return os;
}

//build in function
Object *Object::sum(Object *args){
    size_t sum=0;
//    cout<<"   sum args:";args->dprint();
    for(sum=0;!isnil(args);args=cdr(args)){
//        cout<<"    car(args):";car(args)->dprint();
        sum+=intval(car(args));
    }
//    cout<<"    sum:"<<sum<<endl;
    return Object::mkint(sum);
}
Object *Object::sub(Object *args){
    size_t sub=0;
//    cout<<"   sub args:";args->dprint();
    for(sub=intval(car(args)),args=cdr(args);!isnil(args);args=cdr(args)){
//        cout<<"    car(args):";car(args)->dprint();
        sub-=intval(car(args));
    }
//    cout<<"    sub:"<<sub<<endl;
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

Object *Object::primnumeq(Object *args) {
//    cout<<"primnumeq";args->dprint();
    return intval(car(args)) == intval(car(cdr(args))) ? tee : nil;
}
Object *Object::primnumgt(Object *args) {
//    cout<<"primnumgt";args->dprint();
    return intval(car(args)) > intval(car(cdr(args))) ? tee : nil;
}
Object *Object::primnumlt(Object *args) {
    //    cout<<"primnumeq";args->dprint();
    return intval(car(args)) < intval(car(cdr(args))) ? tee : nil;
}

Object *Object::primcons(Object *args) {
    return cons(car(args), car(cdr(args)));
}
Object *Object::primcar(Object *args)  {
//    cout<<"primcar";args->dprint();
    return car(car(args));
}
Object *Object::primcdr(Object *args)  {
     //cout<<"primcdr";args->dprint();
    return cdr(car(args));
}


