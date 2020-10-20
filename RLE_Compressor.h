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
    void Encode(); // Кодирует в сжатый файл
    void Decode(); // Декодирует запакованный файл
    void print_sizes(); // Возвращает размер текущего файла
    void print_sizes(string filename_1, string filename_2); // Возвращает размер filename_1 и filename_2
};
