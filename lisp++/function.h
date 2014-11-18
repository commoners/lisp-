//
//  function.h
//  lisp++
//
//  Created by evil on 10/17/14.
//  Copyright (c) 2014 evilbinary.org. All rights reserved.
//

#ifndef __lisp____function__
#define __lisp____function__

#include <iostream>
#include "object.h"

void resist_functions();

Object *display(Object *args);
Object *newline(Object *args);
Object *trace(Object *trace);
Object *notf(Object *args);

Object *is_pair(Object *args);
Object *is_number(Object *args);
Object *is_interger(Object *args);
Object *is_boolean(Object *args);
Object *is_real(Object *args);
Object *is_eq(Object *args);
Object *is_zero(Object *args);


Object *is_symbol(Object *args);
Object *is_string(Object *args);
Object *is_char(Object *args);
Object *is_atom(Object *args);
Object *is_procedure(Object *args);
Object *is_null(Object *args);


Object *get_top_env(Object*args);
Object *lookup(Object *args);
Object *update(Object *args);
Object *load(Object *args);



#endif /* defined(__lisp____function__) */
