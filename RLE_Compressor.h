#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h> // ��� itoa
#include "CImg.h"
using namespace std;
using namespace cimg_library;

// ����� �������� � ���������� ������� � � ����������.
class RLE_Compressor
{
private:
    string filename; // ��� �����, ������� ����� �����/�������
    bool compare_pixels(CImg<int> pix_1, CImg<int> pix_2);
public:
    RLE_Compressor();
    RLE_Compressor(string filename);
    void Encode(); // �������� � ������ ����
    void Decode(); // ���������� ������������ ����
    void print_sizes(); // ���������� ������ �������� �����
    void print_sizes(string filename_1, string filename_2); // ���������� ������ filename_1 � filename_2
};
