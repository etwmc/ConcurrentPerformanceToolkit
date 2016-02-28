//
//  main.cpp
//  ConcurrentEvaluation
//
//  Created by Wai Man Chan on 2/24/16.
//  Copyright © 2016 Wai Man Chan. All rights reserved.
//

#include <iostream>
#include "ConcurrentInstance.hpp"

#include <boost/lockfree/queue.hpp>

struct entry {
    unsigned int tid;
    unsigned int lid;
};

void push(void*ptr, unsigned int threadID, unsigned int loopIndex) {
    
    boost::lockfree::queue<entry> *queue = (boost::lockfree::queue<entry> *)ptr;
    entry e;
    
    e.tid = threadID;
    e.lid = loopIndex;
    queue->push(e);

}

void pop(void*ptr, unsigned int threadID, unsigned int loopIndex) {
    boost::lockfree::queue<entry> *queue = (boost::lockfree::queue<entry> *)ptr;
    entry e;
    queue->pop(e);
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    //Test
    
    boost::lockfree::queue<entry> queue(128);
    
    testFieldContext context;
    context.objPtr = &queue;
    context.process = &push;
    context.loop = 100000;
    
    ConcurrentTestField *field = new ConcurrentTestField(100, context);
    double pushTime = field->start();
    printf("Push Time: %f\n", pushTime);
    delete field;
    
    context.process = &pop;
    
    field = new ConcurrentTestField(100, context);
    double popTime = field->start();
    printf("Pop Time: %f\n", popTime);
    delete field;
    
    return 0;
}
