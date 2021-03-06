//
//  parser.cpp
//  lisp++
//
//  Created by evil on 9/9/14.
//  Copyright (c) 2014 evilbinary.org. All rights reserved.
//  rootntsd@gmail.com

#include "parser.h"
#include <string>
#include <sstream>
#include  <istream>

#include<stdio.h>
#include<string.h>


using namespace std;



Object* Parser::parse(char * inputs){
    istringstream iss(inputs);
    istream & isp=iss;
    //cout<<"inputs:"<<inputs<<endl;
    return parse(isp);
}
Object* Parser::parse(string inputs){
    istringstream iss(inputs);
    istream & isp=iss;
    //cout<<"inputs:"<<inputs<<endl;
    return parse(isp);
}



Object* Parser::parse(istream & in){
    Object *o=Object::el;
    int c;
    short sign = 1;
    int num = 0;
//    cout<<"#=====eof:"<<in.eof()<<" bad:"<<in.bad()<<" failbit:"<<in.fail()<<" good:"<<in.good()<<endl;

    eatws(in);
    c=in.get();
    if(c==EOF)
        return Object::eob;
//    cout<<(char)c<<endl;
    
    if(c=='('){
       // cout<<"(:"<<endl;
        return pair(in);
    }else if(c==EOF){
        return NULL;
    }if (c == '#') {
        c = in.get();
        switch (c) {
            case 't':
//                cout<<"#t---"<<Object::tee<<endl;
                return Object::tee;;
            case 'f':
//                cout<<"#f---"<<Object::fee<<endl;

                return Object::fee;
            case '\\':
//                cout<<"===================="<<endl;
                //return read_character(in);
            default:
                error("unknown boolean or character literal\n");
                
        }
    }else if (isdigit(c) || ((c=='.'||c == '-') && (isdigit(in.peek())))) {
//        cout<<"c:"<<(char)c<<endl;
        if (c == '-') {
            sign = -1;
        }else {
            in.unget();
        }
        while (isdigit(c = in.get())) {
            num = (num * 10) + (c - '0');
        }
        num *= sign;
        if (is_delimiter(c)) {
            in.unget();
            //cout<<"mkint:"<<num<<endl;
            return Object::mkint(num);
        }else if(c=='.'){
            double fnum=num*1.0;
            double fdp=0.0;
            double dp=0.1;
            while (isdigit(c = in.get())) {
                fdp = fdp  + (c - '0')*dp;
//                cout<<"##:"<<(c - '0')*dp<<endl;
                dp=dp*0.1;
            }
            fdp*=sign;
            if(is_delimiter(c)){
                in.unget();
//                cout<<"fnum="<<fnum<<" fdp="<<fdp<<" ret:"<<(fnum+fdp)<<endl;
                return Object::mkfloat(fdp+fnum);
            }else{
               error("float number not followed by delimiter\n");
            }
        }else {
            error("number not followed by delimiter\n");
        }
    }
        else if(c=='\''){
        //cout<<"':"<<endl;
        Object *str=parse(in);
//        cout<<"str:"<<str<<endl;
        return Object::cons(Object::symQuote,str);
    }
//   else if (c == '\'') { /* read quoted expression */
//        Object *ret= Object::cons(Object::symQuote, Object::cons(parse(in),Object::el));
////        cout<<"ret:"<<ret<<endl;
//        return ret;
//    }
    else if (c == '"') { /* read a string */
        int i = 0;
        char buf[BUFFER_SIZE]={0};
        while ((c = in.get()) != '"') {
            if (c == '\\') {
                c = in.get();
                if (c == 'n') {
                    c = '\n';
                }
            }
            if (c == EOF) {
                fprintf(stderr, "non-terminated string literal\n");
                exit(1);
            }
            /* subtract 1 to save space for '\0' terminator */
            if (i < BUFFER_SIZE - 1) {
                buf[i++] = c;
            }
            else {
                fprintf(stderr,
                        "string too long. Maximum length is %d\n",
                        BUFFER_SIZE);
                exit(1);
            }
        }
        Object *obj;
        buf[i] = '\0';
//        cout<<"buf:"<<buf<<endl;
        obj=Object::mkstring(buf);
//        cout<<"obj:"<<obj<<endl;
        return obj;
    }
    else if(is_initial(c) ||((c=='+'||c=='-')&&is_delimiter(in.peek()) ) ){
        Object *obj;
        int i=0;
        char buf[BUFFER_SIZE]={0};
        while(is_initial(c)||isdigit(c)||c=='+'||c=='-'){
            if(i<BUFFER_SIZE-1){
                buf[i++]=c;
            }else{
                cout<<"maximum length of buff "<<BUFFER_SIZE<<"length."<<endl;
            }
            c=in.get();
//            cout<<"ccc:"<<(char)c<<endl;
//            cout<<buf<<endl;
        }
        if(is_delimiter(c)){
            buf[i]='\0';
            in.unget();
            obj=Object::inter(buf);
//            cout<<"obj:"<<obj<<endl;
            //obj->dprint();
            return obj;
        }else{
//            cout<<c<<" buf:"<<buf<<endl;
            cout<<"symbol '"<< (char)c<<"' not followed by delimiter."<<endl;
        }
        //cout<<len<<endl;
//        cout<<"=======:"<<buf[0]<<buf[len]<<endl;
        
    }else{
        cout<<"syntax error on '"<<(char)c<<"'."<<endl;
    }
    //cout<<"ret1"<<endl;
    return o;
}
Object *Parser::pair(istream &in){
    Object *car=NULL;
    Object *cdr=NULL;
    int c;
//    cout<<"#eof:"<<in.eof()<<" bad:"<<in.bad()<<" failbit:"<<in.fail()<<" good:"<<in.good()<<endl;
    
    eatws(in);
    
    if(in.eof())
        return Object::nil;
    
    c=in.get();
    
    if(c==')'){
        return Object::el;
    }
    in.unget();
    //cout<<"car===begin"<<endl;;
    car=parse(in);
//    cout<<"car===";
//    car->dprint();
    eatws(in);
    c=in.get();
    if (c == '.'&&!isdigit(in.peek())) {
        c = in.peek();
//        cout<<"pair c===:"<<(char)c<<endl;
        if (!is_delimiter(c)) {
                error("dot not followed by delimiter\n");
        }
        cdr = parse(in);
        eatws(in);
        c = in.get();
//        cout<<"cc:"<<(char)c<<endl;
//        cout<<"cdr:"<<cdr<<endl;
        if (c != ')') {
//            error("where was the trailing right paren?\n");
            
        }
        return Object::cons(car,cdr);
    }else {
//        cout<<"pair else "<<endl;
        
//        if(in.eof())
//            return Object::nil;
        
        in.unget();
        
        cdr = pair(in);
//        cout<<" =car:"<<car<<endl;;
//        car->dprint();
        
//        cout<<" =cdr:"<<cdr<<endl;
//        cdr->dprint();
        Object *obj=Object::cons(car, cdr);
//        cout<<"     =cons:"<<obj<<endl;
        //obj->dprint();
        //cout<<endl;
        return obj;
    }
    
}

void Parser::eatws(istream &in){
    int c;
    while ((c = in.get())!=EOF) {
        if (isspace(c)) {
            //cout<<"isspace"<<endl;
            continue;
        }else if (c == ';') {//;注释忽略
//            cout<<"conmmon:"<<(char)c<<endl;
            while (((c = in.get())) != EOF &&(c !='\n')){
//                cout<<(char)c<<endl;
            }
            continue;
        }
        in.unget();
        //in.putback(c);
        break;;
    }
    
    return ;
}



bool Parser::is_delimiter(int c){
    return isspace(c) || c == EOF ||
    c == '('   || c == ')' ||
    c == '"'   || c == ';';
}


//if((ch1>=0x81)&&(ch1<=0xfe))    //GB码
//if((ch1>=0xa1)&&(ch1<=0xfe))    //BIG5码
int is_gbk(int ch){
    if(ch<0) return 0;
    unsigned char *c;
    c=(unsigned char*)&ch;
    if(c[0]>=0x81&&c[0]<=0xef){
        return 1;
    }
    return 0;
//    if (c[0]>=0xa1){
//        if (c[0]==0xa3)
//            return 1;//全角字符
//        else
//            return 2;//汉字
//    }else{
//        return 0;//英文、数字、英文标点
//    }
}

bool Parser::is_initial(int c) {
//    cout<<"c:"<<c<<" is gbk:"<<is_gbk(c)<<" is alpha:"<<isalpha(c)<<" ret:"<<(is_gbk(c)==2&&!isalpha(c))<<endl;
    
    return isalpha(c) || c == '*' || c == '/' || c == '>' ||
    c == '<' || c == '=' || c == '?' || c == '!'||is_gbk(c)||c=='.'||c==':';
}



