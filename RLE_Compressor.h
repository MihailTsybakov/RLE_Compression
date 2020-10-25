#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> // Для itoa
#include "CImg.h"
using namespace std;
using namespace cimg_library;

// Умеет работать с текстовыми файлами и с картинками.
class RLE_Compressor
{
private:
    string filename; // Имя файла, который нужно сжать/расжать
    bool compare_pixels(CImg<int> pix_1, CImg<int> pix_2);
public:
    RLE_Compressor();
    RLE_Compressor(string filename);

    void Encode_Txt();
    void Decode_Txt();
    void Encode_Bmp();
    void Decode_Bmp();
    void Encode_Bin_Txt();
    void Decode_Bin_Txt();
    void Encode_Bin_Bit_Txt();
    void Decode_Bin_Bit_Txt();
    template <class T>
    void Encode_Bin();
    template <class T>
    void Decode_Bin();

    void print_sizes();
    void print_sizes(string filename_1, string filename_2);
};
