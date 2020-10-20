#pragma once
#include "RLE_Compressor.h"
#include "cmath"

void gen_binary(string filename, int* bytes, int byte_count);
void read_binary(string filename);
bool get_num(string line, int *curr_pos, int* number);
string to_bits(int byte);
int to_byte(string bit);
int Autotest();
