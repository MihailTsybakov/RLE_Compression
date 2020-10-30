#pragma once
#include "RLE_Compressor.h"
#include "cmath"

void gen_binary_txt(string filename, int* bytes, int byte_count);
void read_binary_txt(string filename);
template <class Type>
void gen_binary(string filename, Type* bytes, int byte_count);
template <class Type>
void read_binary(string filename);
void gen_char_file(string filename, uint8_t* bytes, int byte_count);

bool get_num(string line, int *curr_pos, int* number);
string to_bits(int byte);
int to_byte(string bit);
int Autotest();
