//
//  main.cpp
//  lisp++
//
//  Created by evil on 9/7/14.
//  Copyright (c) 2014 evilbinary.org. All rights reserved.
//  rootntsd@gmail.com

#include <iostream>
#include "object.h"
#include <string>
#include "parser.h"
#include <sstream>
#include <iomanip>

#define FAILE cout<<"[FAILED] "<<__FUNCTION__<<" "<<__FILE__<<" "<<__LINE__<<endl;
#define PASS cout<<"[PASS] "<<__FUNCTION__<<" "<<__FILE__<<" "<<__LINE__<<endl;


int test0(){
    Object::init();
    Object o("hello");
    cout<<Object::nil;
    cout<<o<<endl;
    Object* t=o.car(&o);
    cout<<*t<<endl;
    cout<<"=====find if symbol."<<endl;
    Object * tmp=t->findsym("if");
    cout<<"=====find if symbol end."<<endl;
    cout<<(*tmp)<<endl;
    cout<<*(tmp->car())<<endl;
    cout<<*(tmp->cdr())<<endl;
    cout<<*(tmp->car(tmp))<<endl;
    tmp->print();
    
    cout<<"======find labmda."<<endl;
    Object * lambda=t->findsym("lambda");
    cout<<"======find labmda end."<<endl;
    lambda->tprint();
    
    lambda->print();
    
    {
        Object& a=o;
    }
    cout<<*o.car()<<endl;;
    o.dprint();
    cout<<endl;
    Object::symbols->dprint();
    cout<<endl;
    //o.eval(&o,&o);
    return 0;
}

int test_parse(int number,char *test,char *result){
    Parser p;
    Object *obj=p.parse(test);
    stringstream os;
    obj->dstream(os);
    if(os.str()!=result){
        cout<<"[FAILED] test"<<number<<" input:"<<test<<" result:"<<result<<" ret:"<<os.str()<<endl;
        return 0;
    }else{
        cout<<"[PASS] test"<<number<<" input:"<<test<<" result:"<<result<<" ret:"<<os.str()<<endl;;
        return 1;
    }
    
}
int test_eval(int number,char *test,char *result,int type){
    Parser p;
    Object *env=NULL;
    Object *obj=p.parse(test);
    Object *ret;
    if(type==0){
        ret=obj->eval(obj,env);
    }else {
        ret=obj->eval_tail_call(obj, env);
    }
    stringstream os;
    ret->dstream(os);
    
    if(os.str()!=result){
        cout<<"[FAILED] test"<<number<<" input:"<<test<<" result:"<<result<<" ret:"<<os.str()<<endl;
        return 0;
    }else{
        cout<<"[PASS] test"<<number<<" input:"<<test<<" result:"<<result<<" ret:"<<os.str()<<endl;;
        return 1;
    }
    
}



int test1(){
    char *test_string[]={"(A B C)",
        "((A) B C)",
        "((A B) C D)",
        "(A (B) C)",
        "(A B (C))",
        "((A) (B) (C))",
        "(((A B)))",
        "(abc efg)",
        "(a . b)"
    };
    char *test_result[]={"(A . (B . (C . nil)))",
        "((A . nil) . (B . (C . nil)))",
        "((A . (B . nil)) . (C . (D . nil)))",
        "(A . ((B . nil) . (C . nil)))",
        "(A . (B . ((C . nil) . nil)))",
        "((A . nil) . ((B . nil) . ((C . nil) . nil)))",
        "(((A . (B . nil)) . nil) . nil)",
        "(abc . (efg . nil))",
        "(a . b)"
    };
    int count=0;
    for(int i=0;i<sizeof(test_string)/sizeof(char*);i++){
        count=test_parse(i,test_string[i],test_result[i]);
    }

    return 0;
}
int test2(){
    Object::init();
    Parser p;
    Object *env=NULL;
    Object *o[5]={NULL};
    char *test[]={
        "(define count 0)",
        "(define jj (lambda () (if (< count 10) (begin (set! count (+ count 1)) (jj)) count)))",
        "(jj)",
        "(define count 0)",
        "(jj)",
    };
    for(int i=0;i<5;i++){
        o[i]=p.parse(test[i]);
        
        cout<<"["<<i<<"]"<<"<<<<<<===============#################################  "<<test[i]<<endl;
        Object *ret=o[i]->eval_tail_call(o[i],env);
        
        cout<<">>>>>>>===============################################# result=";ret->dprint();
        cout<<endl;
    }
    return 0;
}
int test_eval(int type){
    //test1();
    //    Object *o=p.parse("((A) B C)");
    //    o->dprint();
    //    o->car()->dprint();
    //    o->cdr()->car()->dprint();
    //    o->cdr()->cdr()->dprint();
    Object::init();
    Parser p;
    Object *env;
    char *test_primop="(/ (+ (+ 1 2) (+ 3)  (+ 4 (+ 1 2)) (+ 1 2) (- 5 3) (* 19 2 (+ 1 3))) 3)";
    char *test_lambda="((lambda (a b c) (+ 1 a b c)) 2 3 4)";
    char *test_lambda2="(+ ((lambda (a b c) (+ 1 a b c)) 2 3 4) ((lambda (a b c) (+ 1 a b c)) 2 1 1114))";
    char *test_define="(define x 100)";
    char *test_define2="(define f (lambda (x) (+ 1 x)))";
    char *test_define3="(define (f x y) (+ 1 x y))";
    char *test_if="(if (= 12 13) 1 2)";
    char *test_car="(car '(a b c d))";
    char *test_cdr="(cdr '(a b c d))";
    char *test_cons="(cons '(a)  'c))";
    char *test_cons2="(car (cdr (car (cons (cons '(a b c d e f g) '(c d e f g)) '(aaaa bbbbb cccc)))))";
    char *test_define_set="(define x nil)";
    char *test_setb="(set! x '(a b c d e f))";
    char *test_setcar="(cdr x)";
    char *test_begin="(begin (+ 1 2) (+ 3 4) (+ 9 8))";
    
    char *test_mix="(define add3 (lambda (x) (+ x 3)))";
    char *test_mix1=" (define f1 (+ 1))";
    char *test_mix2="(begin  f1 add3 (add3 10) )";
    char *test_gt="(> 10 2)";
    char *test_lt="(< 10 2 )";
    
    //    char test_define_add[]="(define (add exp) (+ exp))";
    //    char test_call_define[]="(add 2)";
    //
    char *test_string[]={
        test_primop,
        test_lambda,
        test_lambda2,
        test_define,
        test_define2,
        test_define3,
        test_if,
        test_car,
        test_cdr,
        test_cons,
        test_cons2,
        test_define_set,
        test_setb,
        test_setcar,
        test_begin,
        test_mix,
        test_mix1,
        test_mix2,
        test_gt,
        test_lt
    };
    char *test_result[]={
        "56",//test_primop
        "10",//test_lambda
        "1128",//test_lambda2
        "",//test_define
        "", //test_define2
        "",//test_define3
        "2",//(if (= 12 13) 1 2)
        "a",//(car '(a b c d))
        "(b . (c . (d . nil)))",//test_cdr
        "((a . nil) . c)",//test_cons
        "c",//test_cons2
        "",//test_define_set
        "",//test_setb
        "(b . (c . (d . (e . (f . nil)))))",//test_setcar
        "17",//test_begin
        "",//test_mix
        "",//test_mix1
        "13",//test_mix2
        "t",//test_gt
        "nil",//test_lt
    };
    for(int i=0;i<sizeof(test_string)/sizeof(char*);i++){
        test_eval(i,test_string[i],test_result[i],type);
    }
    return 0;
}


int test();
int test_1();
int test_2();
int test_3(int a,int b);

int test4(){
    return test_eval(1);
}

int test3(){
    return test_eval(0);
}

void print_state (const std::ios& stream) {
    std::cout << " good()=" << stream.good();
    std::cout << " eof()=" << stream.eof();
    std::cout << " fail()=" << stream.fail();
    std::cout << " bad()=" << stream.bad();
}
typedef int (*FUN)();

int main(int argc, const char * argv[])
{
    Object::init();
    Parser p;
    Object *env;
    Object *o;
    FUN tests[]={test0,test1,test2,test3,test4,};
    
    
    if(argc>=1){
        bool argh=false; //for help
        bool argt=false;//for test
        string argf="";//for file
        bool arginfo=false;
        for(int i=1;i<argc;i++){
            string arg=string(argv[i]);
            if(arg[0]=='-'){
                //cout<<argv[i]<<endl;
                if(arg=="-t"){
                    //                    argt=string(argv[i]);
                    argt=true;
                }else if(arg=="-info"){
                    arginfo=true;
                }else if(arg=="--test"){
                    if((i+1)<argc){
                        int num=atoi(argv[++i]);
                        if(num>=0&&num<sizeof(tests)/sizeof(FUN)){
                            return tests[num]();
                        }
                    }
                    
                }else if(arg=="-h"||arg=="--h"||arg=="--help"||arg=="-help"){
                    argh=true;
                }
            }else{
                argf=string(argv[i]);
                //                cout<<"argf:"<<argf<<endl;
            }
        }
        if(argh){
            cout<<""
            
            <<"Lisp++"<<endl
            <<"    Created by evil on 9/7/14."<<endl
            <<"    Copyright (c) 2014 evilbinary.org. All rights reserved."<<endl
            <<"    rootntsd@gmail.com"<<endl;
            cout<<"Help info:"<<endl;
            cout<<"  "<<argv[0]<<" (direct to run interactive mode.)"<<endl;;
            cout<<"  "<<argv[0]<<" [file] (run lisp with file.)"<<endl;;
            
            return 0;
        }
        if(argf!=""){
            string line;
            ifstream istrm(argf);
//            print_state(istrm);
            if(istrm.fail()){
                cout<<argf<<" file not exit."<<endl;
                return 0;
            }
            while(true){
                Object *o;
                Object *ret;
                o=p.parse(istrm);
                if(istrm.peek()==EOF){
                    break;
                }
                if(arginfo){
                    cout<<"#input:"<<o<<endl;
                }
                if(argt==true){
                    //cout<<"artg=========="<<endl;
                    ret=o->eval(o,env);
                }else{
                    ret=o->eval2(o,env);
                }
                if(arginfo){
                    cout<<"#result:";
                }
                if(ret!=Object::none){
                    cout<<ret<<endl;
                }
                if(arginfo){
                    cout<<endl;
                }
                
                //delete ret;
            }
            istrm.close();
            
            //            }
            
        }else{
            cout<<">";
            char ch;
            string line;
            while (true)
            {
                try
                {
                    char ch;
                    //std::getline(std::cin, line);
                    while( (ch=cin.get())!=EOF&&ch!='\n'){
                        line+=ch;
                    }
                    if(line!=""){
                        Object *o=p.parse(line.c_str());
                        //cout<<"input=";o->dprint();
//                        Object *ret=o->eval(o,env);
                        Object *ret=Object::nil;
                        if(arginfo){
                            cout<<"#input:"<<o<<endl;
                        }
                        if(argt==true){
                            //cout<<"artg=========="<<endl;
                            ret=o->eval(o,env);
                        }else{
                            ret=o->eval2(o,env);
                        }
                        if(arginfo){
                            cout<<"#result:";
                        }
                        if(ret!=Object::none){
                            //cout<<ret<<endl;
                            std::cout << ret<<endl << ">";
                        }else{
                            std::cout << ">";
                        }
                        if(arginfo){
                            cout<<endl;
                        }

                        
                        
                        //delete ret;
                        
                        line="";
                    }
                }
                catch (std::exception &e)
                {
                    std::cout << e.what() << std::endl;
                }
            }
            
        }
        return 0;
    }else{
    }
    
    return 0;
}

