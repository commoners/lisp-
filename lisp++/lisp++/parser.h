//
//  parser.h
//  lisp++
//
//  Created by evil on 9/9/14.
//  Copyright (c) 2014 evilbinary.org. All rights reserved.
//  rootntsd@gmail.com
//

#ifndef __lisp____parser__
#define __lisp____parser__

#include <iostream>
#include <string>
#include <vector>
#include "object.h"
#include  <istream>
#include <fstream>




class Parser{
public:
    
    Object* parse(istream & inputs);
    Object* parse(char * inputs);
    Object *pair(istream &in);
    
private:
    void eatws(istream &in);
    bool is_delimiter(int c);
 
};

#endif /* defined(__lisp____parser__) */
