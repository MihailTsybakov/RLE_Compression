#include "RLE_Compressor.h"
#include "funcs.h"

int main(void)
{
    int autotest_result = Autotest();
    if (autotest_result == 0)
    {
        cout << "Autotest failed. Exiting" << endl;
        exit(-1);
    }

    RLE_Compressor test_bmp_enc("Test_BMP_2.bmp");
    RLE_Compressor test_bmp_dec("Test_BMP_2_encoded_bmp.txt");

    RLE_Compressor test_txt_enc("Test_Input.txt");
    RLE_Compressor test_txt_dec("Test_Input_encoded.txt");

    RLE_Compressor test_binary_enc("Test_binary_1.txt");
    RLE_Compressor test_binary_dec("Test_binary_1_encoded_bin.txt");

    RLE_Compressor test_binary_bit_enc("Test_binary_1_bit.txt");
    RLE_Compressor test_binary_bit_dec("Test_binary_1_bit_encoded_bit.txt");

    // Создаём массив байт чтобы записать в файл и его закодировать
    /*
    int bytes_count = 300;
    int *bytes = new int[bytes_count];
    for (int i = 0; i < bytes_count; i++)
    {
        if (i < 150)
        {
            bytes[i] = 47;
        }
        else
        {
            bytes[i] = 223;
        }
    }
    */


    test_txt_enc.Encode();
    test_bmp_enc.Encode();
    test_binary_enc.Encode();
    test_binary_bit_enc.Encode();
    cout << endl;
    test_txt_dec.Decode();
    test_bmp_dec.Decode();
    test_binary_dec.Decode();
    test_binary_bit_dec.Decode();
    cout << endl;
    test_txt_enc.print_sizes("Test_input.txt", "Test_input_encoded.txt");
    test_bmp_enc.print_sizes("Test_BMP_2.bmp", "Test_BMP_2_encoded_bmp.txt");
    test_binary_enc.print_sizes("Test_binary_1.txt", "Test_binary_1_encoded_bin.txt");
    test_binary_bit_enc.print_sizes("Test_binary_1_bit.txt", "Test_binary_1_bit_encoded_bit.txt");
    cout << endl;

    return 0;
}
