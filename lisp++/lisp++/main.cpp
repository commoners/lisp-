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
        return -1;
    }else{
        cout<<"[PASS] test"<<number<<" input:"<<test<<" result:"<<result<<" ret:"<<os.str()<<endl;;
        return 0;
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
    char *test_result[]={"(A . (B . (C . NIL)))",
        "((A . NIL) . (B . (C . NIL)))",
        "((A . (B . NIL)) . (C . (D . NIL)))",
        "(A . ((B . NIL) . (C . NIL)))",
        "(A . (B . ((C . NIL) . NIL)))",
        "((A . NIL) . ((B . NIL) . ((C . NIL) . NIL)))",
        "(((A . (B . NIL)) . NIL) . NIL)",
        "(abc . (efg . NIL))",
        "(a . b)"
    };
    for(int i=0;i<sizeof(test_string)/sizeof(char*);i++){
        test_parse(i,test_string[i],test_result[i]);
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
        Object *ret=o[i]->eval(o[i],env);
        
        cout<<">>>>>>>===============################################# result=";ret->dprint();
        cout<<endl;
    }
    return 0;
}

int main(int argc, const char * argv[])
{
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
    char *test_setb="(set! x '(a b c d e f))";
    char *test_setcar="(cdr x)";
    char *test_begin="(begin (+ 1 2) (+ 3 4) (+ 9 8))";
    
    char *test_mix="(begin (define f1 (+ 1)) (define add3 (lambda (x) (+ x 3))) (f1) f1 add3 (add3 10) )";
    char *test_gt="(> 10 2)";
    char *test_lt="(< 10 2 )";
    
    
    //    Object *o1=p.parse(test_primop);
    //    cout<<"input=";o1->dprint();
    //    Object *ret=o1->eval(o1,env);
    //    cout<<"result=";ret->dprint();
    ////
    ////    Object::topEnv->dprint();
    //    Object *o2=p.parse(test_if);
    //    Object *ret1=o1->eval(o2,env);
    //    cout<<"result1=";ret1->dprint();
    
    //
    //    Object::topEnv->dprint();
    //    Object *o3=p.parse(test_mix);
    //    Object *ret2=o1->eval(o3,env);
    //    cout<<"result2=";ret2->dprint();
    //
    //    test1();
    //    test2();
    
    //    Object *o1=p.parse(test_primop);
    //    cout<<"input=";o1->dprint();
    //    Object *ret=o1->eval(o1,env);
    //    cout<<"result=";ret->dprint();
    //    delete o1;
    //    delete ret;
    //    return 0;
    
    if(argc>1){
        if(argc==2){
            //cout<<argv[1]<<endl;
            string arg1(argv[1]);
            if(arg1=="--help"||arg1=="-h"){
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
            string line;
            //            ofstream ostrm;
            //            ostrm.open("f1.dat");
            //            ostrm<<120<<endl;
            //            ostrm<<310.85<<endl;
            //            ostrm.close();
            ifstream istrm(argv[1]);
            
            
            while(true){
                Object *o;
                Object *ret;
                o=p.parse(istrm);
                if(istrm.peek()==EOF){
                    break;
                }
                //cout<<o<<endl;
                ret=o->eval(o,env);
                cout<<ret<<endl;
                
                delete ret;
            }
            istrm.close();
            
        }else{
            
        }
        return 0;
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
                    Object *ret=o->eval(o,env);
                    std::cout << ret<<endl << ">";
                    //delete o;
                    delete ret;
                    
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
}

