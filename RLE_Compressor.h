#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> // Для itoa
#include "CImg.h"
using namespace std;
using namespace cimg_library;


class RLE_Compressor
{
private:
    string filename;
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
    void Encode_Bin(); // Only for int & double!
    template <class T>
    void Decode_Bin(); // Only for int & double!

    void Encode_Char(); // uint8t
    void Decode_Char(); // uint8t

    void Encode_Voxels(); // for voxel files
    void Decode_Voxels(); // for voxel files

    void print_sizes();
    void print_sizes(string filename_1, string filename_2);
};
