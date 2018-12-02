# Des_Implementation
Implementation of DES encryption

Compile:

`g++ -std=c++11 DES.cpp -o DES`

If gitbash:

`g++ -std=c++11 DES.cpp -o DES -static`

Run:

Flags:
-e: encrypt
-d: decrypt
-p: plaintext (input)
-k: key (input)
-a: ascii
-b: binary

Example: encrypt binary
`./DES -e -p 0000000100100011010001010110011110001001101010111100110111101111 -k 0001001100110100010101110111100110011011101111001101111111110001 -b
`
Example: encrypt ascii
`./DES -e -p abcdefgh -k ijklmnop -a`