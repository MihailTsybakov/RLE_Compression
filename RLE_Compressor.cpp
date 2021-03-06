#include "RLE_Compressor.h"
#include "funcs.h"
RLE_Compressor::RLE_Compressor()
{
    filename = "";
}
RLE_Compressor::RLE_Compressor(string filename)
{
    this->filename = filename;
}

void RLE_Compressor::Encode_Txt()
{
    fstream file;
        fstream encoded_file;
        string line;
        string encoded_name = filename.substr(0, static_cast<int>(filename.size()) - 4);;
        encoded_name += "_encoded.txt";
        encoded_file.open(encoded_name, ios::out);
        file.open(filename, ios::in);
        if (!file.is_open() || !encoded_file.is_open())
        {
            cout << "Error occured while tried to open files. " << endl;
            exit(-1);
        }

        while(getline(file, line))
        {
            string writeline = "";
            string line_fragment = "";
            int fragm_len = 0;
            int i = 0;
            while(i < static_cast<int>(line.size()) - 1)
            {
                if (line[i] != line[i + 1])
                {
                    int j = 1;
                    while(line[i + j - 1] != line[i + j] && (i + j) < static_cast<int>(line.size()))
                    {
                        j++;
                    }
                    fragm_len = (-1)*(j);
                    line_fragment += to_string(fragm_len) + " ";
                    for (int k = i; k <i + j; k++)
                    {
                        line_fragment += line[k];
                    }
                    i += j - 1;
                    writeline += line_fragment + " ";
                    line_fragment = "";
                    fragm_len = 0;
                }
                else
                {
                    int j = 0;
                    while (line[i + j] == line[i + j + 1] && (i + j + 1) < static_cast<int>(line.size()))
                    {
                        j++;
                    }
                    fragm_len = j + 1;
                    line_fragment = to_string(fragm_len) + " " + line[i];
                    i += j;
                    writeline += line_fragment + " ";
                    line_fragment = "";
                    fragm_len = 0;
                }
                i++;
            }
            writeline = writeline.substr(0, static_cast<int>(writeline.size()) - 1);
            encoded_file << writeline << '\n';
        }
        file.close();
        encoded_file.close();
        cout << "Done encoding in " << encoded_name << endl;
}

void RLE_Compressor::Decode_Txt()
{
    if (filename.find("encoded") == string::npos)
    {
        cout << "Error: this file may be not encoded. Do you want to proceed? Y/N: " << endl;
        string ans;
        cin >> ans;
        if (ans == "N")
        {
            cout << "Ending execution." << endl;
            exit(0);
        }
        if (ans != "N" && ans != "Y")
        {
            cout << "Error: cannot recognize answer." << endl;
            exit(-1);
        }
    }
    fstream source_file;
    fstream decode_file;
    string decode_name = filename.substr(0, static_cast<int>(filename.size()) - 4);
    string line;
    decode_name += "_decoded.txt";
    decode_file.open(decode_name, ios::out);
    source_file.open(filename, ios::in);
    if (!decode_file.is_open() || !source_file.is_open())
    {
        cout << "Error occured while tried to open files." << endl;
        exit(-1);
    }
    while(getline(source_file, line))
    {
        string writeline = "";
        int i = 0;
        while (i < static_cast<int>(line.size()))
        {
            if (line[i] != '-' && (line[i] > 57 || line[i] < 49))
            {
                cout << "<Decoding txt> Error: source file is not correctly compressed." << endl;
                exit(-1);
            }
            if (line[i] == '-')
            {
                string fragm_len = "";
                int j = 1;
                while (line[i+j] < 58 && line[i+j] > 47)
                {
                    fragm_len += line[i+j];
                    j++;
                }
                int fragm_len_int = atoi(fragm_len.c_str()); // Получили длину разнородного фрагмента
                for (int k = j + 1; k < fragm_len_int + j + 1; k++)
                {
                    writeline += line[i+k];
                }
                i += j + fragm_len_int + 1;
            }
            else
            {
                string fragm_len = "";
                int j = 0;
                while (line[i + j] < 58 && line[i + j] > 48)
                {
                    fragm_len += line[i+j];
                    j++;
                }
                int fragm_len_int = atoi(fragm_len.c_str());
                i += j + 1;
                for (int k = 0; k < fragm_len_int; k++)
                {
                    writeline += line[i];
                }
                i++;
            }
            i++;
        }
        decode_file << writeline << '\n';
    }
    cout << "Done decoding in " << decode_name << endl;
    decode_file.close();
    source_file.close();
}

void RLE_Compressor::Encode_Bmp()
{
    CImg<int> image(filename.c_str());
    string encoded_image = filename.substr(0, static_cast<int>(filename.size()) - 4);
    encoded_image += "_encoded_bmp.txt";
    fstream encode_file;
    encode_file.open(encoded_image, ios::out);
    if (!encode_file.is_open())
    {
        cout << "Error occured while tried to open file." << endl;
        exit(-1);
    }
    encode_file << "x" <<to_string(image.width()) << " " << to_string(image.height()) << " "<< '\n';
    for (int y = 0; y < image.height(); y ++)
    {
        string writeline = "";
        int x = 0;
        while (x < image.width())
        {
            if (!compare_pixels(image.get_crop(x,y,x,y), image.get_crop(x+1,y,x+1,y)))
            {
                int j = 0;
                while (!compare_pixels(image.get_crop(x+j,y,x+j,y), image.crop(x+j+1,y,x+j+1,y)))
                {
                    j++;
                }
                writeline += to_string((-1)*j) + " ";
                for (int k = x; k < x + j; k++)
                {
                    writeline += to_string(image.get_crop(x + k,y,x + k,y)[0]) + " ";
                    writeline += to_string(image.get_crop(x + k,y,x + k,y)[1]) + " ";
                    writeline += to_string(image.get_crop(x + k,y,x + k,y)[2]) + " ";
                }
                x += j - 1;
            }
            else
            {
                int j = 0;
                while (compare_pixels(image.get_crop(x+j,y,x+j,y), image.get_crop(x+j+1,y,x+j+1,y)) &&
                    x + j + 1 < image.width() + 1) /// + 1 !!!
                    {
                        j++;
                    }
                writeline += to_string(j + 1) + " ";
                writeline += to_string(image.get_crop(x,y,x,y)[0]) + " ";
                writeline += to_string(image.get_crop(x,y,x,y)[1]) + " ";
                writeline += to_string(image.get_crop(x,y,x,y)[2]) + " ";
                x += j;
            }
            x++;
        }
        encode_file << writeline << '\n';
    }
    encode_file.close();
    cout << "Done encoding in " << encoded_image << endl;
}

void RLE_Compressor::Decode_Bmp()
{
    if (filename.find("encoded") == string::npos)
    {
        cout << "Warning: this file may be not encoded, what will cause incorrect results. Do you want to proceed? Y/N: " << endl;
        string ans;
        cin >> ans;
        if (ans == "N")
        {
            cout << "Ending execution." << endl;
            exit(0);
        }
        if (ans != "N" && ans != "Y")
        {
            cout << "Error: cannot recognize answer." << endl;
            exit(-1);
        }
    }
    fstream source_file;
    string line;
    source_file.open(filename, ios::in);
    if (!source_file.is_open())
    {
        cout << "Error occured while tried to open packed bmp image." << endl;
        exit(-1);
    }
    getline(source_file, line);
    int width, height;
    int f = 1;
    get_num(line, &f, &width);
    get_num(line, &f, &height);
    CImg<int> image(width, height, 1, 3);
    int row_number = 0, col_number = 0;
    while(getline(source_file, line))
    {
        if (line[0] == 'x')
        {
            continue;
        }
        int i = 0;
        col_number = 0;
        while (i < static_cast<int>(line.size()))
        {
            int flag;
            get_num(line, &i, &flag);
            if (flag > 0)
            {
                int R, G, B;
                get_num(line, &i, &R); get_num(line, &i, &G); get_num(line, &i, &B);
                int color[3] = {R, G, B};
                for (int x = col_number; x < col_number + flag; x++)
                {
                    image.draw_point(x, row_number, color);
                }
                col_number += flag;
            }
            else
            {
                for (int x = col_number; x < col_number + (-1)*flag; x++)
                {
                    int R, G, B;
                    get_num(line, &i, &R); get_num(line, &i, &G); get_num(line, &i, &B);
                    int color[3] = {R, G, B};
                    image.draw_point(x, row_number, color);
                }
                col_number += (-1)*flag;
            }
            i++;
        }
        row_number++;
    }
    string encoded_name = filename.substr(0, static_cast<int>(filename.size()) - 4);
    encoded_name += "_decoded.bmp";
    cout << "Done decoding in " << encoded_name << endl;
    source_file.close();
    image.save(encoded_name.c_str());
}


void  RLE_Compressor::Encode_Bin_Txt()
{
    fstream encoded_bin;
    fstream current_bin;
    string encoded_name = filename.substr(0, static_cast<int>(filename.size()) - 4);
    string line;
    int tmp_byte = 0;
    encoded_name += "_encoded_bin.txt";
    encoded_bin.open(encoded_name, ios::out);
    current_bin.open(filename, ios::in);
    int byte_count = 0, position = 0;
    if (!encoded_bin.is_open() || !current_bin.is_open())
    {
        cout << "Error occured while tried to open binary file. " << endl;
        exit(-1);
    }
    getline(current_bin, line);
    while (line[position] != '*')
    {
        get_num(line, &position, &tmp_byte);
        byte_count++;
    }
    int *bytes = new int[byte_count];
    position = 0;
    int i = 0;
    while (i < byte_count)
    {
            get_num(line, &position, &bytes[i]);
        i++;
    }
    string res_line = "";
    i = 0;
    while (i < byte_count - 1)
    {
        if (bytes[i] != bytes[i+1])
        {
            int j = 0;
            while (bytes[i+j] != bytes[i+j+1] && i+j+1 < byte_count + 1 && j+1 < 256)
            {
                j++;
            }
            res_line += to_string((-1)*j) + " ";
            for (int k = i; k < i + j; k++)
            {
                res_line += to_string(bytes[k]) + " ";
            }
            i += j - 1;
        }
        else
        {
            int j = 0;
            while (bytes[i+j] == bytes[i+j+1] && i+j+1 < byte_count && j + 1 < 256)
            {
                j++;
            }
            res_line += to_string(j + 1) + " ";
            res_line += to_string(bytes[i]) + " ";
            i += j;
        }
        i++;
    }
    delete[] bytes;
    res_line += "*";
    encoded_bin << res_line;
    encoded_bin.close();
    current_bin.close();
    cout << "Done encoding in " << encoded_name << endl;
}

void RLE_Compressor::Decode_Bin_Txt()
{
    if (filename.find("encoded") == string::npos)
    {
        cout << "Warning: this file may be not encoded, what will cause incorrect results. Do you want to proceed? Y/N: " << endl;
        string ans;
        cin >> ans;
        if (ans == "N")
        {
            cout << "Ending execution." << endl;
            exit(0);
        }
        if (ans != "N" && ans != "Y")
        {
            cout << "Error: cannot recognize answer." << endl;
            exit(-1);
        }
    }
    fstream source_file;
    fstream decode_file;
    string decode_name = filename.substr(0, static_cast<int>(filename.size()) - 4);
    string line;
    decode_name += "_decoded.txt";
    source_file.open(filename, ios::in);
    decode_file.open(decode_name, ios::out);
    if (!source_file.is_open() || !decode_file.is_open())
    {
        cout << "Error occured while tried to open binary files in decoder. " << endl;
        exit(-1);
    }
    int byte_count = 0, position = 0, tmp_byte = 0;
    getline(source_file, line);
    while (line[position] != '*')
    {
        get_num(line, &position, &tmp_byte);
        byte_count++;
    }
    int *bytes = new int[byte_count];
    position = 0;
    int i = 0;
    while (i < byte_count)
    {
        get_num(line, &position, &bytes[i]);
        i++;
    }
    string res_line = "";
    i = 0;
    while (i < byte_count)
    {
        if (bytes[i] < 0)
        {
            for (int k = i + 1; k < i + (-1)*bytes[i] + 1; k++)
            {
                res_line += to_string(bytes[k]) + " ";
            }
            i += (-1)*bytes[i];
        }
        else
        {
            for (int k = 0; k < bytes[i]; k++)
            {
                res_line += to_string(bytes[i + 1]) + " ";
            }
            i += 1;
        }
        i++;
    }
    res_line += "*";
    decode_file << res_line;
    source_file.close();
    decode_file.close();
    cout << "Done decoding in " << decode_name << endl;
    delete[] bytes;
}

void RLE_Compressor::Encode_Bin_Bit_Txt()
{
    fstream source_file;
    fstream encoded_file;
    string encoded_name = filename.substr(0, static_cast<int>(filename.size()) - 4);
    encoded_name += "_encoded_bit.txt";
    source_file.open(filename, ios::in);
    encoded_file.open(encoded_name, ios::out);
    if (!source_file.is_open() || !encoded_file.is_open())
    {
        cout << "Error occured while tried to open files." << endl;
        exit(-1);
    }
    string line;
    int position = 0, i = 0, tmp_byte = 0, byte_count = 0;
    getline(source_file, line);
    while (line[position] != '*')
    {
        get_num(line, &position, &tmp_byte);
        byte_count++;
    }
    int *bytes = new int[byte_count];
    position = 0;
    while (i < byte_count)
    {
        get_num(line, &position, &bytes[i]);
        i++;
    }
    string bitstring = "";
    for (int j = 0; j < byte_count; j++)
    {
        bitstring += to_bits(bytes[j]);
    }
    // Получили строку битов
    string bits_line = "";
    i = 0;
    while (i < static_cast<int>(bitstring.size()) - 1)
    {
        if (bitstring[i] != bitstring[i+1])
        {
            int j = 0;
            while (bitstring[i+j] != bitstring[i+j+1] && i+j+1 < static_cast<int>(bitstring.size()) + 1 && j + 1 < 256)
            {
                j++;
            }
            bits_line += to_string(-j) + " ";
            for (int k = i; k < i + j; k++)
            {
                bits_line += bitstring[k];
            }
            bits_line += " ";
            i += j - 1;
        }
        else
        {
            int j = 0;
            while (bitstring[i+j] == bitstring[i+j+1] && i+j+1 < static_cast<int>(bitstring.size()) + 1 && j + 1 < 256)
            {
                j++;
            }
            j++;
            bits_line += to_string(j) + " ";
            if (bitstring[i] == '0')
            {
                bits_line += "0 ";
            }
            else
            {
                bits_line += "1 ";
            }
            i += j - 1;
        }
        i++;
    }
    bits_line += "*";
    encoded_file << bits_line;
    encoded_file.close();
    source_file.close();
    cout << "Done encoding in " << encoded_name << endl;
    delete[] bytes;
}

void RLE_Compressor::Decode_Bin_Bit_Txt()
{
    if (filename.find("encoded") == string::npos)
    {
        cout << "Warning: this file may be not encoded, what will cause incorrect results. Do you want to proceed? Y/N: " << endl;
        string ans;
        cin >> ans;
        if (ans == "N")
        {
            cout << "Ending execution." << endl;
            exit(0);
        }
        if (ans != "N" && ans != "Y")
        {
            cout << "Error: cannot recognize answer." << endl;
            exit(-1);
        }
    }
    fstream source_file;
    fstream decode_file;
    string decode_name = filename.substr(0, static_cast<int>(filename.size()) - 4);
    decode_name += "_decoded_bit.txt";
    source_file.open(filename, ios::in);
    decode_file.open(decode_name, ios::out);
    if (!source_file.is_open() || !decode_file.is_open())
    {
        cout << "Error occured while tried to open files. " << endl;
        exit(-1);
    }
    string line;
    getline(source_file, line);
    int i = 0;
    string res_line = "";
    while (line[i] != '*')
    {
        int num = 0;
        get_num(line, &i, &num);
        if (num > 0)
        {
            for (int k = 0; k < num; k++)
            {
                res_line += line[i];
            }
            i++;
        }
        else
        {
            for (int k = i; k < (-1)*num + i; k++)
            {
                res_line += line[k];
            }
            i += -num;
        }
        i++;
    }
    string byte_line = "";
    for (int k = 0; k < static_cast<int>(res_line.size()); )
    {
        string byte = "";
        for (int j = 0; j < 8; j++)
        {
            byte += res_line[k + j];
        }
        byte_line += to_string(to_byte(byte)) + " ";
        k += 8;
    }
    byte_line += "*";
    decode_file << byte_line;
    decode_file.close();
    source_file.close();
    cout << "Done decoding in " << decode_name << endl;
}
template <class Type>
void RLE_Compressor::Encode_Bin()
{
    ifstream source_bin;
    ofstream encode_bin;
    string encode_name = filename.substr(0, static_cast<int>(filename.size()) - 4) + "_encoded.bin";
    source_bin.open(filename, ios::binary);
    encode_bin.open(encode_name, ios::binary);
    if (!source_bin.is_open() || !encode_bin.is_open())
    {
        cout << "Error occured while tried to open files and encode bin. " << endl;
        exit(-1);
    }
    int byte_count = 0;
    Type tmp_byte;
    while(!source_bin.read((char*)&tmp_byte, sizeof(tmp_byte)), !source_bin.eof())
    {
        byte_count++;
    }
    source_bin.clear();
    source_bin.seekg(0);
    int estim_bytes = byte_count;
    for (int p = 0; p < (byte_count/100000) + 1; p++)
    {
        Type *bytes = new Type[(estim_bytes/100000 > 0)? 100000:estim_bytes];
        int i = 0, counter = 0;
        while(!source_bin.eof() && i < estim_bytes  && i < 100000)
        {
            source_bin.read((char*)&tmp_byte, sizeof(tmp_byte));
            bytes[i] = tmp_byte;
            i++;
            counter ++;
        }
        i = 0;

        while (i < counter)
        {


            if (bytes[i] != bytes[i+1])
            {
                Type j = 0;
                while (bytes[i+static_cast<int>(j)] != bytes[i+static_cast<int>(j)+1] && i + static_cast<int>(j) + 1 < counter && static_cast<int>(j) < 255)
                {
                    j += 1;
                }
                j = -j;
                encode_bin.write((char*)&j, sizeof(j));
                for (int k = i; k < i - static_cast<int>(j); k++)
                {

                    encode_bin.write((char*)&bytes[k], sizeof(bytes[k]));
                }
                i += -j - 1;
            }
            if (bytes[i] == bytes[i+1])
            {
                Type j = 0;

                while (bytes[i+static_cast<int>(j)] == bytes[i+static_cast<int>(j)+1] && i + static_cast<int>(j) + 1 < counter && static_cast<int>(j) < 255)
                {
                    j += 1;
                }
                j++;
                encode_bin.write((char*)&j, sizeof(j));
                encode_bin.write((char*)&bytes[i], sizeof(bytes[i]));
                i += j - 1;
            }
            i++;
        }
        estim_bytes -= 100000;
        delete[] bytes;
    }
    source_bin.close();
    encode_bin.close();
    cout << "Done encoding in " << encode_name << endl;
}

template <class Type>
void RLE_Compressor::Decode_Bin()
{
    ifstream decode_bin;
    ofstream result_bin;
    string result_name = filename.substr(0, static_cast<int>(filename.size())-  4) + "_decoded.bin";
    decode_bin.open(filename, ios::binary);
    result_bin.open(result_name, ios::binary);
    if (!decode_bin.is_open() || !result_bin.is_open())
    {
        cout << "Error occured while tried to open bin files and decode. " << endl;
        exit(-1);
    }
    int byte_count = 0;
    Type tmp_byte;
    while(!decode_bin.read((char*)&tmp_byte, sizeof(tmp_byte)), !decode_bin.eof())
    {
        byte_count++;
    }
    decode_bin.clear();
    decode_bin.seekg(0);
    int estim_bytes = byte_count;
    for (int p = 0; p < (byte_count/100000) + 1; p++)
    {
        Type *bytes = new Type[(estim_bytes/100000 > 0)? 100000:estim_bytes];
        int i = 0, counter = 0;
        while((!decode_bin.read((char*)&tmp_byte, sizeof(tmp_byte)), !decode_bin.eof()) && i < estim_bytes && i < 100000)
        {
            bytes[i] = tmp_byte;
            i++;
            counter ++;
        }
        // Есть массив байт, нужно раскодировать
        i = 0;
        while (i < counter)
        {
            if (bytes[i] < 0)
            {
                int j = 1;
                for (int k = i + j; k < i + j - bytes[i]; k++)
                {
                    result_bin.write((char*)&bytes[k], sizeof(bytes[k]));
                }
                i += -bytes[i];
            }
            if (bytes[i] > 0)
            {
                for (int k = 0; k < bytes[i]; k++)
                {
                    result_bin.write((char*)&bytes[i + 1], sizeof(bytes[i + 1]));
                }
                i += 1;
            }
            i++;
        }
        delete[] bytes;
    }
    decode_bin.close();
    result_bin.close();
    cout << "Done decoding in " << result_name << endl;
}


bool RLE_Compressor::compare_pixels(CImg<int> pix_1, CImg<int> pix_2)
{
    if (pix_1[0] == pix_2[0] && pix_1[1] == pix_2[1] && pix_1[2] == pix_2[2])
    {
        return true;
    }
    return false;
}
void RLE_Compressor::print_sizes() // Автоматическое определение измеряемых файлов
{
    fstream curr_encoded;
    fstream curr_decoded;
    if (filename.find("encoded") != string::npos)
    {
        string decoded_name = "";
        if (filename.find("bmp") != string::npos)
        {
            for (int i = 0; i < static_cast<int>(filename.size()) - 16; i++)
            {
                decoded_name += filename[i];
            }
            decoded_name += ".bmp";
        }
        else
        {
            for (int i = 0; i < static_cast<int>(filename.size()) - 12; i++)
            {
                decoded_name += filename[i];
            }
            decoded_name += ".txt";
        }
        curr_encoded.open(filename, ios::in);
        curr_decoded.open(decoded_name, ios::in);
        if (!curr_encoded.is_open() || !curr_decoded.is_open())
        {
            cout << "Error occured while tried to open files and calculate sizes. " << endl;
            exit(-1);
        }
    }
    else
    {
        string encoded_name = filename.substr(0, static_cast<int>(filename.size()) - 4);
        if (filename.find("bmp") != string::npos)
        {
            encoded_name += "_encoded_bmp.bmp";
        }
        if (filename.find("txt") != string::npos)
        {
            encoded_name += "_encoded.txt";
        }
        curr_encoded.open(encoded_name, ios::in);
        curr_decoded.open(filename, ios::in);
        if (!curr_encoded.is_open() || !curr_decoded.is_open())
        {
            cout << "Error occured while tried to open files and calculate sizes. " << endl;
            exit(-1);
        }
    }
    int encoded_begin = curr_encoded.tellg();
    int decoded_begin = curr_decoded.tellg();
    curr_decoded.seekg(0, ios::end);
    curr_encoded.seekg(0, ios::end);
    int encoded_end = curr_encoded.tellg();
    int decoded_end = curr_decoded.tellg();
    double compress_ratio = static_cast<double>(decoded_end - decoded_begin)/static_cast<double>(encoded_end - encoded_begin);
    cout << "Default size is " << decoded_end - decoded_begin << " bytes. ";
    cout << "Encoded size is " << encoded_end - encoded_begin << " bytes. " << endl;
    cout << "Compression ratio is " << compress_ratio << endl;
    curr_decoded.close();
    curr_encoded.close();
}
void RLE_Compressor::print_sizes(string filename_1, string filename_2)
{
    fstream file_1;
    fstream file_2;
    file_1.open(filename_1, ios::in);
    file_2.open(filename_2, ios::in);
    if (!file_1.is_open() || !file_2.is_open())
    {
        cout << "Error occured while tried to open files and compare sizes. " << endl;
        exit(-1);
    }
    int begin_1 = file_1.tellg();
    int begin_2 = file_2.tellg();
    file_1.seekg(0, ios::end);
    file_2.seekg(0, ios::end);
    int end_1 = file_1.tellg();
    int end_2 = file_2.tellg();
    double compress_ratio = static_cast<double>(end_1 - begin_1)/static_cast<double>(end_2 - begin_2);
    cout << "Size of " << filename_1 << " is " << end_1 - begin_1 << " bytes. " << endl;
    cout << "Size of " << filename_2 << " is " << end_2 - begin_2 << " bytes. " << endl;
    cout << "Compression ratio (1st size / 2nd size) is " << compress_ratio << endl;
    cout << endl;
    file_1.close();
    file_2.close();
 }
void RLE_Compressor::Encode_Char()
{
    if (filename.find(".bin") == string::npos)
    {
        cout << "Error: unsupported file format. " << endl;
        exit(-1);
    }
    ifstream source_file;
    ofstream output_file;
    string encoded_name = filename.substr(0, static_cast<int>(filename.size()) - 4) + "_encoded.bin";
    source_file.open(filename, ios::binary);
    output_file.open(encoded_name, ios::binary);
    if (!source_file.is_open() || !output_file.is_open())
    {
        cout << "Error occured while tried to open binary files. " << endl;
        exit(-1);
    }
    int byte_count = 0;
    uint8_t tmp_byte;
    while (!source_file.eof())
    {
        source_file.read((char*)&tmp_byte, sizeof(tmp_byte));
        byte_count++;
    }
    byte_count--;
    source_file.clear();
    source_file.seekg(0);
    int estim_bytes = byte_count;
    int buffer_size = 1000000;
    for (int p = 0; p < (byte_count/buffer_size) + 1; p++)
    {
        uint8_t *bytes = new uint8_t[(estim_bytes/buffer_size > 0)? buffer_size:estim_bytes];
        int counter = 0;
        for (int i = 0; (i < buffer_size && i < estim_bytes); i++)
        {
            source_file.read((char*)&tmp_byte, sizeof(tmp_byte));
            bytes[i] = tmp_byte;
            counter++;
        }
        int i = 0;
        while (i < counter)
        {
            if (bytes[i] != bytes[i+1])
            {
                uint8_t tmp = 1;
                output_file.write((char*)&tmp, sizeof(tmp));
                output_file.write((char*)&bytes[i], sizeof(bytes[i]));
            }
            if (bytes[i] == bytes[i + 1])
            {
                int j = 1;
                while (bytes[i+j - 1] == bytes[i+j] && i + j < estim_bytes && j < 255)
                {
                    j++;
                }
                uint8_t j_ = static_cast<uint8_t>(j);
                output_file.write((char*)&j_, sizeof(j_));
                output_file.write((char*)&bytes[i], sizeof(bytes[i]));

                i += j - 1;
            }
            i++;
        }
        estim_bytes -= buffer_size;
        delete[] bytes;
    }
    cout << "Ended encoding " << filename << " in " << encoded_name << endl;
    source_file.close();
    output_file.close();
}

void RLE_Compressor::Decode_Char()
{
    ifstream source_file;
    ofstream decoded_file;
    string decoded_name = filename.substr(0, static_cast<int>(filename.size()) - 4) + "_decoded.bin";
    source_file.open(filename, ios::binary);
    decoded_file.open(decoded_name, ios::binary);
    if (!source_file.is_open() || !decoded_file.is_open())
    {
        cout << "Error ocured while tried to open binary files during decoding. " << endl;
        exit(-1);
    }

    int byte_count = 0;
    uint8_t tmp_byte;
    while (!source_file.eof())
    {
        source_file.read((char*)&tmp_byte, sizeof(uint8_t));
        byte_count++;
    }
    byte_count--;
    source_file.clear();
    source_file.seekg(0);
    int estim_bytes = byte_count;
    int buffer_size = 1000000;
    for (int p = 0; p < (byte_count/buffer_size) + 1; p++)
    {
        uint8_t *bytes = new uint8_t[(estim_bytes/buffer_size > 0)? buffer_size:estim_bytes];
        int counter = 0;
        for (int i = 0; (i < buffer_size && i < estim_bytes); i++)
        {
            source_file.read((char*)&tmp_byte, sizeof(tmp_byte));
            bytes[i] = tmp_byte;
            counter++;
        }
        int i = 0;
        while (i < counter)
        {
            if (bytes[i] == 1)
            {
                decoded_file << bytes[i + 1];
                i++;
            }
            else
            {
                for (int k = 0; k < bytes[i]; k++)
                {
                    decoded_file << bytes[i + 1];
                }
                i++;
            }
            i++;
        }
        estim_bytes -= buffer_size;
        delete[] bytes;
    }
    source_file.close();
    decoded_file.close();
    cout << "Ended decoding in " << decoded_name << endl;
}

void RLE_Compressor::Encode_Voxels()
{
    if (filename.find(".bin") == string::npos)
    {
        cout << "Error: unsupported file format. " << endl;
        exit(-1);
    }
    ifstream source_file;
    ofstream output_file;
    string encoded_name = filename.substr(0, static_cast<int>(filename.size()) - 4) + "_encoded.bin";
    source_file.open(filename, ios::binary);
    output_file.open(encoded_name, ios::binary);
    if (!source_file.is_open() || !output_file.is_open())
    {
        cout << "Error occured while tried to open binary files. " << endl;
        exit(-1);
    }

    int byte_count = 0;
    uint8_t tmp_byte;
    while (!source_file.eof())
    {
        source_file.read((char*)&tmp_byte, sizeof(tmp_byte));
        byte_count++;
    }

    byte_count--;
    source_file.clear();
    source_file.seekg(0);
    int estim_bytes = byte_count;
    int buffer_size = 1000000;
    for (int p = 0; p < (byte_count/buffer_size) + 1; p++)
    {
        uint8_t *bytes = new uint8_t[(estim_bytes/buffer_size > 0)? buffer_size:estim_bytes];
        int counter = 0;
        for (int i = 0; (i < buffer_size && i < estim_bytes); i++)
        {
            source_file.read((char*)&tmp_byte, sizeof(tmp_byte));
            (tmp_byte == 0) ? tmp_byte = 0 : tmp_byte = 1;
            bytes[i] = tmp_byte;
            counter++;
        }
        int i = 0;
        while (i < counter - 1)
        {
            if (bytes[i] != bytes[i+1])
            {
                uint8_t tmp = 1;
                output_file.write((char*)&tmp, sizeof(tmp));
                output_file.write((char*)&bytes[i], sizeof(bytes[i]));
            }
            else
            {
                uint8_t j = 1;
                while (bytes[i+j - 1] == bytes[i+j] && i + j < counter && j < 255)
                {
                    j++;
                }
                output_file.write((char*)&j, sizeof(j));
                output_file.write((char*)&bytes[i], sizeof(bytes[i]));

                i += j - 1;
            }
            i++;
        }
        estim_bytes -= buffer_size;
        delete[] bytes;
    }
    cout << "Ended encoding " << filename << " in " << encoded_name << endl;
    source_file.close();
    output_file.close();
}

void RLE_Compressor::Decode_Voxels()
{
    ifstream source_file;
    ofstream decoded_file;
    string decoded_name = filename.substr(0, static_cast<int>(filename.size()) - 4) + "_decoded.bin";
    source_file.open(filename, ios::binary);
    decoded_file.open(decoded_name, ios::binary);
    if (!source_file.is_open() || !decoded_file.is_open())
    {
        cout << "Error ocured while tried to open binary files during decoding. " << endl;
        exit(-1);
    }

    int byte_count = 0;
    uint8_t tmp_byte;
    while (!source_file.eof())
    {
        source_file.read((char*)&tmp_byte, sizeof(uint8_t));
        byte_count++;
    }

    byte_count--;
    source_file.clear();
    source_file.seekg(0);
    int estim_bytes = byte_count;
    int buffer_size = 1000000;
    for (int p = 0; p < (byte_count/buffer_size) + 1; p++)
    {
        uint8_t *bytes = new uint8_t[(estim_bytes/buffer_size > 0)? buffer_size:estim_bytes];
        int counter = 0;
        for (int i = 0; (i < buffer_size && i < estim_bytes); i++)
        {
            source_file.read((char*)&tmp_byte, sizeof(tmp_byte));
            bytes[i] = tmp_byte;
            counter++;
        }
        int i = 0;
        while (i < counter - 1)
        {
            if (bytes[i] == 1)
            {
                decoded_file << bytes[i + 1];
                i++;
            }
            else
            {
                for (int k = 0; k < bytes[i]; k++)
                {
                    decoded_file << bytes[i + 1];
                }
                i++;
            }
            i++;
        }
        estim_bytes -= buffer_size;
        delete[] bytes;
    }
    source_file.close();
    decoded_file.close();
    cout << "Ended decoding in " << decoded_name << endl;
}

template void RLE_Compressor::Decode_Bin<int>();
template void RLE_Compressor::Encode_Bin<int>();
template void RLE_Compressor::Encode_Bin<char>();
template void RLE_Compressor::Encode_Bin<uint8_t>();
template void RLE_Compressor::Decode_Bin<double>();
template void RLE_Compressor::Encode_Bin<double>();
