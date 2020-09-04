#include <iostream>
#include <cstdlib>
#include<ctime>
#include <fstream>
#include <vector>
#include<math.h>


using namespace std;

int main() {

    /**ENCRYPTION**/
 /**Error handling, check if there is a plain text and key.**/
    ifstream ifs2("plain.txt", ios::ate);
    if (ifs2.tellg() == 0)
    {
        cout << "The plain.txt file is empty, please input the plain text.";
        return 0;
    }

    ifstream ifs("key-encrypt.txt", ios::ate);
    if (ifs.tellg() == 0)
    {
        cout << "The key-encrypt.txt file is empty, please input the key.";
        return 0;
    }

    /**In this part we grab the plain text from the "plain.txt" file and store as integers in plain_vector**/
    ifstream plain_input;
    plain_input.open("plain.txt");
    char p;
    vector<int> plain_vector;
    while (plain_input >> p)
    {
        int plain_int = static_cast<int>(p);
        if (plain_int < 97 || plain_int>122) {
            cout << "Error:Plaintext should be in small letters";
            return 0;
        }
        plain_int -= 97;
        plain_vector.push_back(plain_int);

    }
    plain_input.close();
    /**Next step is to store the plain text in an array**/
    int plain_arr[50];
    for (int r = 0;r < plain_vector.size();r++)
        plain_arr[r] = plain_vector[r];

    /**Next step is to grab the encryption key from "key-encrypt.txt" and store in key_vector**/
    ifstream key_input;
    key_input.open("key-encrypt.txt");
    char d;
    vector<int> key_vector;
    while (key_input >> d)
    {
        int key_int = static_cast<int>(d);
        key_int -= 97;
        key_vector.push_back(key_int);
    }
    key_input.close();
    /**Next step is to make a 3 by 3 matrix using the key above**/
    int key[3][3], determinant;

    key[0][0] = key_vector[0];
    key[0][1] = key_vector[1];
    key[0][2] = key_vector[2];
    key[1][0] = key_vector[3];
    key[1][1] = key_vector[4];
    key[1][2] = key_vector[5];
    key[2][0] = key_vector[6];
    key[2][1] = key_vector[7];
    key[2][2] = key_vector[8];

    /**Next step is to find the determinant of the key matrix, check if invertible**/
    /**I used the co-factor expansion method**/

    determinant = key[0][0] * (key[1][1] * key[2][2] - key[1][2] * key[2][1]) -
        key[0][1] * (key[1][0] * key[2][2] - key[1][2] * key[2][0]) +
        key[0][2] * (key[1][0] * key[2][1] - key[2][0] * key[1][1]);
    if (determinant == 0)
    {
        cout << "Error:The key is not invertible.Please use a different key." << endl;
        return 0;
    }
    if (determinant > 25)determinant = determinant % 26;
    while (determinant < 0)determinant += 26;

    /**Next step is to encrypt using the key we have stored**/
    /**The input vector shall be divided into 1 by 3 vectors **
    /**(in line 81, length is increased by 3 each iteration),then**/
    /**by multiplying the 1 by 3 input vector and the key matrix**/

    int datasize = plain_vector.size();
    int w = 0, index;
    int encr_array[50];

    for (int length = 0; length < datasize;length += 3)
    {
        for (index = 0;index < 3;index++)
        {
            encr_array[w] = (key[0][index] * plain_arr[length] + key[1][index] * plain_arr[length + 1] + key[2][index] * plain_arr[length + 2]) % 26;
            w++;

        }
    }

    /**Now the array above is encrypted, the next step is to typecast it back to char and store as ASCII in cipher.txt**/
    ofstream write_ciph;
    write_ciph.open("cipher.txt");
    for (int arr1_siz = 0; arr1_siz < datasize; arr1_siz++)
    {
        encr_array[arr1_siz] += 97;
        char ciph_char = static_cast<char>(encr_array[arr1_siz]);
        write_ciph << ciph_char;
    }
    write_ciph.close();

    /**End of Encryption**/

    /**DECRYPTION**/
    /**First step is to find the inverse of the key matrix, it shall be our decryption key**/
    int inverse[3][3], inv = 0;

    for (int f = 1; f < 26;f++)
    {
        if ((f * determinant) % 26 == 1)
            inv = f;
    }
    if (inv == 0) {
        cout << "The determinant of key has no inverse mod 26. Try another key";
        return 0;
    }
    /**Here I used matrix of minors cofactor and adjugate method then multiplied with inverse of determinant**/
    inverse[0][0] = ((key_vector[4]) * (key_vector[8]) - (key_vector[5]) * (key_vector[7])) * inv;
    inverse[0][1] = ((key_vector[2]) * (key_vector[7]) - (key_vector[1]) * (key_vector[8])) * inv;
    inverse[0][2] = ((key_vector[1]) * (key_vector[5]) - (key_vector[2]) * (key_vector[4])) * inv;
    inverse[1][0] = ((key_vector[5]) * (key_vector[6]) - (key_vector[3]) * (key_vector[8])) * inv;
    inverse[1][1] = ((key_vector[0]) * (key_vector[8]) - (key_vector[2]) * (key_vector[6])) * inv;
    inverse[1][2] = ((key_vector[2]) * (key_vector[3]) - (key_vector[0]) * (key_vector[5])) * inv;
    inverse[2][0] = ((key_vector[3]) * (key_vector[7]) - (key_vector[4]) * (key_vector[6])) * inv;
    inverse[2][1] = ((key_vector[1]) * (key_vector[6]) - (key_vector[0]) * (key_vector[7])) * inv;
    inverse[2][2] = ((key_vector[0]) * (key_vector[4]) - (key_vector[1]) * (key_vector[3])) * inv;

    /**Next store this inverse matrix as an array**/
    int dkey_arr[9];
    dkey_arr[0] = inverse[0][0];
    dkey_arr[1] = inverse[0][1];
    dkey_arr[2] = inverse[0][2];
    dkey_arr[3] = inverse[1][0];
    dkey_arr[4] = inverse[1][1];
    dkey_arr[5] = inverse[1][2];
    dkey_arr[6] = inverse[2][0];
    dkey_arr[7] = inverse[2][1];
    dkey_arr[8] = inverse[2][2];

    /**Next, reduce the inverse mod 26**/
    for (int num = 0;num < 9;num++) {
        if (dkey_arr[num] > 25)dkey_arr[num] = dkey_arr[num] % 26;
        while (dkey_arr[num] < 0)dkey_arr[num] += 26;

    }

    /**Next step is to store the decryption key in key-decrypt.txt**/
    ofstream write_dkey;
    write_dkey.open("key-decrypt.txt");
    for (int arr2_siz = 0; arr2_siz < 9; arr2_siz++)
    {
        dkey_arr[arr2_siz] += 97;
        char dkey_char = static_cast<char>(dkey_arr[arr2_siz]);
        write_dkey << dkey_char;
    }
    write_dkey.close();

    /**Next step is to grab the cipher text and store as integers in cipher_vector **/
    ifstream cipher_input;
    cipher_input.open("cipher.txt");
    char r;
    vector<int> cipher_vector;
    while (cipher_input >> r)
    {
        int cipher_int = static_cast<int>(r);
        cipher_int -= 97;
        cipher_vector.push_back(cipher_int);
    }
    cipher_input.close();

    /**Next step is to multiply with the decryption key we obtained and cipher text to find the plain text **/
    int w2 = 0, decr_arr[50];
    for (int length2 = 0; length2 < datasize;length2 += 3)
    {
        for (int index2 = 0;index2 < 3;index2++)
        {
            decr_arr[w2] = (inverse[0][index2] * cipher_vector[length2] + inverse[1][index2] * cipher_vector[length2 + 1] + inverse[2][index2] * cipher_vector[length2 + 2]) % 26;
            w2++;

        }
    }
    /**Reduce the above array mod 26**/
    for (int num2 = 0;num2 < datasize;num2++) {
        if (decr_arr[num2] > 25)decr_arr[num2] = decr_arr[num2] % 26;
        while (decr_arr[num2] < 0)decr_arr[num2] += 26;

    }
    /**Now that the array above is decrypted, the next step is to typecast it back to char and store as ASCII in plaintext.txt**/
    ofstream write_dec;
    write_dec.open("plaintext.txt");
    for (int arr2_siz = 0; arr2_siz < datasize; arr2_siz++)
    {
        decr_arr[arr2_siz] += 97;
        char dec_char = static_cast<char>(decr_arr[arr2_siz]);
        write_dec << dec_char;
    }
    write_dec.close();

    /**End of decryption**/
    cout << endl;
    cout << "ENCRYPTION AND DECRYPTION COMPLETE!";
    return 0;
}
