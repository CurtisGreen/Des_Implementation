# Des_Implementation
Implementation of DES encryption

Compile:

`g++ -std=c++11 main.cpp des.cpp -o des`

If gitbash:

`g++ -std=c++11 main.cpp des.cpp -o des -static`

Run:

Flags:
-e: encrypt

-d: decrypt

-m: message (input)

-k: key (input)

-a: ascii

-b: binary

-v: verbose

Example: encrypt binary

`./des -e -m 0000000100100011010001010110011110001001101010111100110111101111 -k 0001001100110100010101110111100110011011101111001101111111110001 -b`

Example: encrypt ascii

`./des -e -m abcdefgh -k ijklmnop -a`

Example: decrypt binary verbose

`./des -d -m 1000010111101000000100110101010000001111000010101011010000000101 -k 0001001100110100010101110111100110011011101111001101111111110001 -b -v`

Note:
 * Message and key need to be the same type (binary/ASCII)
 * Most encryptions of ASCII characters will not properly render as the values do not fall within the ASCII possible values
	* This means that the ASCII output might not be able to be decrypted if you copy and paste it
	* The binary output for the ASCII encryption can still be decrypted