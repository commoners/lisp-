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
    Object *o=Object::nil;
    int c;
    short sign = 1;
    size_t num = 0;
    eatws(in);
    c=in.get();
    if(c==EOF)
        return o;
    
    //cout<<"char:"<<(char)c<<endl;
    
    if(c=='('){
       // cout<<"(:"<<endl;
        return pair(in);
    }else if(c=='\''){
        //cout<<"':"<<endl;
        return Object::cons(Object::symQuote,parse(in));
    }else if(c==EOF){
        return NULL;
    }if (c == '#') {
        c = in.get();
        switch (c) {
            case 't':
                return Object::tee;;
            case 'f':
                return Object::fee;
            case '\\':
                //return read_character(in);
            default:
                error("unknown boolean or character literal\n");
                
        }
    }else if (isdigit(c) || (c == '-' && (isdigit(in.peek())))) {
        /* read a fixnum */
        if (c == '-') {
            sign = -1;
        }
        else {
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
        }
        else {
            error("number not followed by delimiter\n");
           
        }
    }else if (c == '\'') { /* read quoted expression */
        return Object::cons(Object::symQuote, Object::cons(parse(in),Object::nil));
    }else{
        Object *obj;
        int i=0;
        char buf[256]={0};
        buf[i++]=c;
        while((c=in.get())!=' '&&c!=EOF&&c!=')'){
            buf[i++]=c;
        }
        in.unget();
        //cout<<"buf:"<<buf<<endl;
        //obj=new Object(buf);
        obj=Object::inter(buf);
        //obj->dprint();
        return obj;
    }
    //cout<<"ret1"<<endl;
    return o;
}
Object *Parser::pair(istream &in){
    Object *car=NULL;
    Object *cdr=NULL;
    int c;
    eatws(in);
    c=in.get();
    if(c==')'){
        return Object::nil;
    }
    in.unget();
    //cout<<"car===begin"<<endl;;
    car=parse(in);
    //cout<<"car===";
    //car->dprint();
    eatws(in);
    c=in.get();
    if (c == '.') {
        c = in.peek();
        if (!is_delimiter(c)) {
            error("dot not followed by delimiter\n");
            
        }
        cdr = parse(in);
        eatws(in);
        c = in.get();
        if (c != ')') {
            error("where was the trailing right paren?\n");
            
        }
        return Object::cons(car,cdr);
    }else {
        //cout<<"pair else "<<endl;
        in.unget();
        cdr = pair(in);
        //cout<<"=car:";
//        car->dprint();
        
        //cout<<"=cdr:";
//        cdr->dprint();
        Object *obj=Object::cons(car, cdr);
        //cout<<" cons:";
        //obj->dprint();
        //cout<<endl;
        return obj;
    }
    
}

void Parser::eatws(istream &in){
    int c;
    while ((c = in.get())!=EOF) {
        if (isspace(c)||c=='\n') {
            //cout<<"isspace"<<endl;
            continue;
        }else if (c == ';') {//;注释忽略
            //cout<<"conmmon"<<endl;
            while (((c = in.get())) != EOF &&(c !='\n'));
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



