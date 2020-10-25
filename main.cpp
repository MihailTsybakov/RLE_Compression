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

    RLE_Compressor test_binary_txt_enc("Test_binary_1.txt");
    RLE_Compressor test_binary_txt_dec("Test_binary_1_encoded_bin.txt");

    RLE_Compressor test_binary_bit_txt_enc("Test_binary_1_bit.txt");
    RLE_Compressor test_binary_bit_txt_dec("Test_binary_1_bit_encoded_bit.txt");

    RLE_Compressor test_binary_enc("Test_BIN.bin");
    RLE_Compressor test_binary_dec("Test_BIN_encoded.bin");

    /// Создаём массив байт чтобы записать в файл и его закодировать

    /*
    int bytes_count = 150000; //1.5e5
    double *bytes = new double[bytes_count];
    for (int i = 0; i < bytes_count; i++)
    {
        double byte = (i % 7)/3.2;
        for (int k = 0; k < 10; k++)
        {
            bytes[i + k] = byte;
        }
        i += 10;
    }
    */

    //gen_binary<double>("Autotest_binary.bin", bytes, bytes_count);
    //read_binary<double>("Test_BIN_BIG_Double.bin");
    //read_binary<double>("Test_BIN_BIG_Double_encoded.bin");

    test_txt_enc.Encode_Txt();
    test_bmp_enc.Encode_Bmp();
    test_binary_txt_enc.Encode_Bin_Txt();
    test_binary_bit_txt_enc.Encode_Bin_Bit_Txt();
    test_binary_enc.Encode_Bin<int>();

    cout << endl;

    test_txt_dec.Decode_Txt();
    test_bmp_dec.Decode_Bmp();
    test_binary_txt_dec.Decode_Bin_Txt();
    test_binary_bit_txt_dec.Decode_Bin_Bit_Txt();
    test_binary_dec.Decode_Bin<int>();

    cout << endl;

    test_txt_enc.print_sizes("Test_input.txt", "Test_input_encoded.txt");
    test_bmp_enc.print_sizes("Test_BMP_2.bmp", "Test_BMP_2_encoded_bmp.txt");
    test_binary_txt_enc.print_sizes("Test_binary_1.txt", "Test_binary_1_encoded_bin.txt");
    test_binary_bit_txt_enc.print_sizes("Test_binary_1_bit.txt", "Test_binary_1_bit_encoded_bit.txt");
    test_binary_enc.print_sizes("Test_BIN.bin", "Test_BIN_encoded.bin");

    cout << endl;

    return 0;
}
