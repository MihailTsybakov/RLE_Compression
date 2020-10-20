#include "funcs.h"

void gen_binary(string filename, int* bytes, int byte_count)
{
    fstream file;
    file.open(filename + ".txt", ios::out);
    if (!file.is_open())
    {
        cout << "Error occured while tried to create binaru file." << endl;
        exit(-1);
    }
    string line;
    for (int i = 0; i < byte_count; i++)
    {
        if (bytes[i] > 255 || bytes[i] < -255)
        {
            cout << "Error: wrong byte." << endl;
            exit(-1);
        }
        line += to_string(bytes[i]) + " ";
    }
    line += "*";
    file << line;
    file.close();
}

void read_binary(string filename)
{
    fstream file;
    string line;
    file.open(filename, ios::in);
    if (!file.is_open())
    {
        cout << "Error occured while tried to open binary file." << endl;
        exit(-1);
    }
    getline(file, line);
    int i = 0, counter = 0;
    cout << filename << ": " << endl;
    while (line[i] != '*')
    {
        int byte;
        get_num(line, &i, &byte);
        cout << counter + 1 << ": " << byte << endl;
        counter++;
    }
    file.close();
}
bool get_num(string line, int *curr_pos, int* number)
{
    string tmp = "";
    if (line[*curr_pos] == '*')
    {
        return false;
    }
    while ((line[*curr_pos] < 58 && line[*curr_pos] > 47) || line[*curr_pos] == '-')
    {
        tmp += line[*curr_pos];
        (*curr_pos)++;
    }
    (*curr_pos)++;
    *number = atoi(tmp.c_str());
    return true;
}
string to_bits(int byte)
{
    if (byte > 255 || byte < 0)
    {
        cout << "Error: invalid byte." << endl;
        exit(-1);
    }
    string res = "";
    for (int i = 0; i < 8; i++)
    {
        if (byte >= pow(2, 7 - i))
        {
            res += "1";
            byte -= pow(2,7-i);
        }
        else
        {
            res += "0";
        }
    }
    return res;
}
int to_byte(string bit)
{
    int res = 0;
    for (int i = 0; i < 8; i++)
    {
        if (bit[i] != '0')
        {
            res += pow(2, 7-i);
        }
    }
    return res;
}
int Autotest()
{
    RLE_Compressor test_text_enc("Autotest_text.txt");
    RLE_Compressor test_text_dec("Autotest_text_encoded.txt");
    RLE_Compressor test_bmp_enc("Autotest_bmp.bmp");
    RLE_Compressor test_bmp_dec("Autotest_bmp_encoded_bmp.txt");
    RLE_Compressor test_bin_enc("Autotest_bin.txt");
    RLE_Compressor test_bin_dec("Autotest_bin_encoded_bin.txt");
    RLE_Compressor test_bin_bit_enc("Autotest_bin_bit.txt");
    RLE_Compressor test_bin_bit_dec("Autotest_bin_bit_encoded_bit.txt");
    cout << ">------------ Autotest actions ------------<" << endl;
    test_text_enc.Encode();
    test_bmp_enc.Encode();
    test_bin_enc.Encode();
    test_bin_bit_enc.Encode();
    cout << endl;
    test_text_dec.Decode();
    test_bmp_dec.Decode();
    test_bin_dec.Decode();
    test_bin_bit_dec.Decode();
    // 1. Сравнение текстовых файлов:
    fstream source_txt;
    fstream result_txt;
    source_txt.open("Autotest_text.txt", ios::in);
    result_txt.open("Autotest_text_encoded_decoded.txt", ios::in);
    if (!source_txt.is_open() || !result_txt.is_open())
    {
        cout << "Autotest failed: cannot open test files." << endl;
        return 0;
    }
    int source_lines_count = 0, result_lines_count = 0;
    string tmp_line;
    while (getline(source_txt, tmp_line))
    {
        source_lines_count++;
    }
    while (getline(result_txt, tmp_line))
    {
        result_lines_count++;
    }
    if (source_lines_count != result_lines_count)
    {
        cout << "Autotest failed: different lines count encountered while comparing text files in test. "<< endl;
        return 0;
    }
    source_txt.clear();
    result_txt.clear();
    source_txt.seekg(0);
    result_txt.seekg(0);
    int i = 0;
    while (i < source_lines_count)
    {
        string src_line;
        string res_line;
        getline(source_txt, src_line);
        getline(result_txt, res_line);
        if (src_line != res_line)
        {
            cout << "Autotest failed: different lines at position " << i << endl;
            return 0;
        }
        i++;
    }
    source_txt.close();
    result_txt.close();
    cout << endl << "<Autotest> Text files compared, ok" << endl;
    // 2. Сравнение bmp картинок:
    string src_bmp_name = "Autotest_bmp.bmp";
    string res_bmp_name = "Autotest_bmp_encoded_bmp_decoded.bmp";
    CImg<int> source_img(src_bmp_name.c_str());
    CImg<int> result_img(res_bmp_name.c_str());

    int src_width = source_img.width();
    int src_height = source_img.height();
    int res_width = result_img.width();
    int res_height = result_img.height();

    if ((src_width != res_width) || (src_height != res_height))
    {
        cout << "Autotest failed: bmp`s have different sizes. " << endl;
        return 0;
    }

    CImg<int> src_pixel;
    CImg<int> res_pixel;
    for (int x = 0; x < src_width; x++)
    {
        for (int y = 0; y < src_height; y++)
        {
            src_pixel = source_img.get_crop(x,y,x,y);
            res_pixel = result_img.get_crop(x,y,x,y);
            if ((src_pixel[0] != res_pixel[0]) ||
                (src_pixel[1] != res_pixel[1]) ||
                (src_pixel[2] != res_pixel[2]))
            {
                cout << "Autotest failed: different pixels at " << x << ", " << y << endl;
                return 0;
            }

        }
    }
    cout << "<Autotest> Bmp images compared, ok" << endl;
    // 3. Сравнение бинарных файлов:
    fstream source_binary;
    fstream result_binary;
    source_binary.open("Autotest_bin.txt", ios::in);
    result_binary.open("Autotest_bin_encoded_bin_decoded.txt", ios::in);
    if (!source_binary.is_open() || !result_binary.is_open())
    {
        cout << "Autotest failed: cannot open binary test files." << endl;
        return 0;
    }
    string src_line;
    string res_line;
    getline(source_binary, src_line);
    getline(result_binary, res_line);
    if (src_line != res_line)
    {
        cout << "Autotest failed: there is a difference between source binary and result binary." << endl;
        return 0;
    }
    source_binary.close();
    result_binary.close();
    cout << "<Autotest> Binary files compared, ok" << endl;
    // 4. Сравнение бинарных файлов, кодировавшихся по битам:
    fstream source_bit;
    fstream result_bit;
    source_bit.open("Autotest_bin_bit.txt", ios::in);
    result_bit.open("Autotest_bin_bit_encoded_bit_decoded_bit.txt", ios::in);
    if (!source_bit.is_open() || !result_bit.is_open())
    {
        cout << "Autotest failed: cannot open test bit files." << endl;
        return 0;
    }
    string src_bit_line;
    string res_bit_line;
    getline(source_bit, src_bit_line);
    getline(result_bit, res_bit_line);
    if (src_bit_line != res_bit_line)
    {
        cout << "Autotest failed: there is a difference between source bit file and result bit file." << endl;
        return 0;
    }
    source_bit.close();
    result_bit.close();
    cout << "<Autotest> Binary Bit compared, ok" << endl;
    cout << ">------------ Autotest ended ------------<" << endl << endl;
    return 1;
}
