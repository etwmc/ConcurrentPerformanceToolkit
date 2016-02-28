//
//  ConcurrentInstance.hpp
//  ConcurrentEvaluation
//
//  Created by Wai Man Chan on 2/24/16.
//  Copyright © 2016 Wai Man Chan. All rights reserved.
//

#ifndef ConcurrentInstance_hpp
#define ConcurrentInstance_hpp

#include <stdio.h>
#include <pthread.h>

struct testFieldContext {
    void (*process)(void*ptr, unsigned int threadID, unsigned int loopIndex);
    void *objPtr;
    
    unsigned int *counterStack;
    bool threadActivate;
    unsigned int loop;
};

struct testFieldObject {
    testFieldContext *context;
    unsigned int index;
};

class ConcurrentTestField {
    pthread_t *threads;
    testFieldObject *objs;
    unsigned int threadCount;
public:
    ConcurrentTestField(unsigned int numberOfThread, testFieldContext context);
    ~ConcurrentTestField();
    double start();
};

#endif /* ConcurrentInstance_hpp */
