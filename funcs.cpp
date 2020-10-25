#include "funcs.h"

void gen_binary_txt(string filename, int* bytes, int byte_count)
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

void read_binary_txt(string filename)
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

template <class Type>
void gen_binary(string filename, Type* bytes, int byte_count)
{
    ofstream byte_file;
    byte_file.open(filename, ios::binary);
    if (!byte_file.is_open())
    {
        cout << "Error occured while tried to create bin file. " << endl;
        exit(-1);
    }
    for (int i = 0; i < byte_count; i++)
    {
        byte_file.write((char*)&bytes[i], sizeof(bytes[i]));
    }
    byte_file.close();
}
template  <class Type>
void read_binary(string filename)
{
    ifstream byte_file;
    byte_file.open(filename, ios::binary);
    if (!byte_file.is_open())
    {
        cout << "Error occured while tried to read bin file. " << endl;
        exit(-1);
    }
    cout << "File " << filename << ": " << endl;
    int i = 0;
    Type byte;
    while (!byte_file.eof())
    {
        byte_file.read((char*)&byte, sizeof(byte));
        cout << i << ": " << byte << endl;
        i++;
    }
    byte_file.close();
}

template void gen_binary<int>(string filename, int* bytes, int byte_count);
template void gen_binary<double>(string filename, double* bytes, int byte_count);
template void read_binary<int>(string filename);
template void read_binary<double>(string filename);

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
    RLE_Compressor test_bin_txt_enc("Autotest_bin.txt");
    RLE_Compressor test_bin_txt_dec("Autotest_bin_encoded_bin.txt");
    RLE_Compressor test_bin_bit_txt_enc("Autotest_bin_bit.txt");
    RLE_Compressor test_bin_bit_txt_dec("Autotest_bin_bit_encoded_bit.txt");
    RLE_Compressor test_bin_enc("Autotest_binary.bin");
    RLE_Compressor test_bin_dec("Autotest_binary_encoded.bin");

    cout << ">------------ Autotest actions ------------<" << endl;
    test_text_enc.Encode_Txt();
    test_bmp_enc.Encode_Bmp();
    test_bin_txt_enc.Encode_Bin_Txt();
    test_bin_bit_txt_enc.Encode_Bin_Bit_Txt();
    test_bin_enc.Encode_Bin<double>();
    cout << endl;
    test_text_dec.Decode_Txt();
    test_bmp_dec.Decode_Bmp();
    test_bin_txt_dec.Decode_Bin_Txt();
    test_bin_bit_txt_dec.Decode_Bin_Bit_Txt();
    test_bin_dec.Decode_Bin<double>();
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
    // 5. Сравнение бинарных файлов с double-ми:
    ifstream source_file;
    ifstream result_file;
    source_file.open("Autotest_binary.bin", ios::binary);
    result_file.open("Autotest_binary_encoded_decoded.bin", ios::binary);
    if (!source_file.is_open() || !result_file.is_open())
    {
        cout << "Error occured while tried to open binary files in autotest " << endl;
        exit(-1);
    }
    double tmp_byte_1, tmp_byte_2;
    for (int i = 0; i < 150000; i++)
    {
        source_file.read((char*)&tmp_byte_1, sizeof(tmp_byte_1));
        result_file.read((char*)&tmp_byte_2, sizeof(tmp_byte_2));
        if (tmp_byte_1 != tmp_byte_2)
        {
            cout << "Autotest failed: there is a difference between source binary and result binary. " << endl;
            return 0;
        }
    }
    source_file.close();
    result_file.close();
    cout << "<Autotest> Binary.bin files compared, ok" << endl;
    cout << ">-------------- Autotest ended --------------<" << endl << endl;
    return 1;
}
