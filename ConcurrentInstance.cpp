//
//  ConcurrentInstance.cpp
//  ConcurrentEvaluation
//
//  Created by Wai Man Chan on 2/24/16.
//  Copyright © 2016 Wai Man Chan. All rights reserved.
//

#include "ConcurrentInstance.hpp"
#include <strings.h>
#include <ctime>

void* threadHandle(void*ptr) {
    testFieldObject *obj = (testFieldObject*)ptr;
    testFieldContext *context = obj->context;
    unsigned int threadID = obj->index;
    while (context->threadActivate == false);
    for (unsigned int i = context->loop; i > 0; i--) {
        context->process(context->objPtr, threadID, i);
        (context->counterStack[obj->index])++;
    }
    return NULL;
}

ConcurrentTestField::ConcurrentTestField(unsigned int numberOfThread, testFieldContext context) {
    threadCount = numberOfThread;
    
    threads = new pthread_t[numberOfThread];
    
    //Init context
    testFieldContext *_context = new testFieldContext;
    bcopy(&context, _context, sizeof(context));
    _context->threadActivate = false;
    _context->counterStack = new unsigned int[numberOfThread];
    
    objs = new testFieldObject[numberOfThread];
    
    for (unsigned int i = 0; i < numberOfThread; i++) {
        //Create objs
        objs[i].context = _context;
        objs[i].index = i;
    }
}

double ConcurrentTestField::start() {
    clock_t startTime = clock();
    for (unsigned int i = 0; i < threadCount; i++) {
        //Create thread
        pthread_create(&threads[i], NULL, &threadHandle, (void*)&(objs[i]));
    }
    //Actual start
    objs[0].context->threadActivate = true;
    
    void *tmp;
    for (unsigned int i = 0; i < threadCount; i++) {
        //waiting for the end
        pthread_join(threads[i], &tmp);
    }
    clock_t stopTime = clock();
    return (threadCount*objs->context->loop)
    /((stopTime-startTime)/(double)CLOCKS_PER_SEC);
}

ConcurrentTestField::~ConcurrentTestField() {
    delete objs[0].context;
    delete [] objs;
    delete [] threads;
}