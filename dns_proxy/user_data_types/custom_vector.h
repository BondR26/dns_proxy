#pragma once
#include "custom_string.h"
#include <stdlib.h>
#include <stdbool.h>


typedef struct StringVector
{
	string** data;
	size_t size;
	bool is_sortedAscending;
}StringVector;

// size - number of elemnts(strings), not bytes
void allocStringVector(size_t size, StringVector* vec_);

// add str_ to vec
void appendStr(StringVector* vec, string str_);

//remove str_ (if exists) from vec
void eraseStr(StringVector* vec, string str_);

//uninit vec
void uninitStringVector(StringVector* vec_);

void sortAscending(StringVector* const vector);

bool isInSortedAscVector(const StringVector* const blacklist, const string* const name_);