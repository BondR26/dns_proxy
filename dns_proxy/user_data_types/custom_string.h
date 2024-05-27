#pragma once
#include <stdio.h>

#define DEFAULT_CAPACITY 512

typedef struct string
{
	size_t size;
	char data[DEFAULT_CAPACITY];
}string;
