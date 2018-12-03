# Des_Implementation
Implementation of DES encryption

### Compile:

`g++ -std=c++11 main.cpp des.cpp -o des`

If gitbash:

`g++ -std=c++11 main.cpp des.cpp -o des -static`

### Run:

Flags:
```
-e: encrypt
-d: decrypt
-m: message (input)
-k: key (input)
-a: ascii
-b: binary
-h: hex
-v: verbose
```

### Examples:

Example: encrypt binary

`./des -e -m 0000000100100011010001010110011110001001101010111100110111101111 -k 0001001100110100010101110111100110011011101111001101111111110001 -b`

Example: decrypt binary verbose

`./des -d -m 1000010111101000000100110101010000001111000010101011010000000101 -k 0001001100110100010101110111100110011011101111001101111111110001 -b -v`

Example: encrypt hex

`./des -e -m 0123456789ABCDEF -k 133457799BBCDFF1 -h`

Example: encrypt ascii

`./des -e -m abcdefgh -k ijklmnop -a`

### Notes to grader:
 * Message and key need to be the same type (binary/ASCII)
 * Most encryptions of ASCII characters will not properly render in the terminal as the values do not fall within ASCII
	* This means that the ASCII output might not be able to be decrypted if you copy and paste it
	* The binary output for the ASCII encryption can still be decrypted
