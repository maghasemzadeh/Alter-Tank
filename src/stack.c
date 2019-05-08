//
// Created by amin on 1/4/19.
//

#include <stdio.h>
#include "stack.h"


int MAXSIZE = 1000;
int stack[1000];
int top = -1;

int isempty() {
    if(top == -1)
        return 1;
    else
        return 0;
}

int isfull() {
    if(top == MAXSIZE)
        return 1;
    else
        return 0;
}

int peek() {
    return stack[top];
}

int pop() {
    int data;

    if(!isempty()) {
        data = stack[top];
        top = top - 1;
        return data;
    } else {
        printf("Could not retrieve data, Stack is empty.\n");
    }
}

int push(int data) {
    if(!isfull()) {
        top = top + 1;
        stack[top] = data;
    } else {
        printf("Could not insert data, Stack is full.\n");
    }
}


