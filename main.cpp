#include <iostream>
#include <unistd.h>

#include "des.hpp"
#include "des_data.hpp"

int main(int argc, char** argv){

	bool encrypt = false, decrypt = false, ascii = false, binary = false, hex = false, verbose = false;
	std::string plaintext, key;
	int opt = 0;
	while((opt = getopt(argc, argv, "edabhvm:k:")) != -1){
		switch(opt) {
	        case 'e':
	            encrypt = true;break;
	        case 'd':
	            decrypt = true;break;
	        case 'm':
	            plaintext = optarg;break;
	        case 'k':
	            key = optarg;break;
	        case 'a':
	            ascii = true;break;
	        case 'b':
	            binary = true;break;
	        case 'h':
	        	hex = true;break;
	        case 'v':
	        	verbose = true;break;
	        default:
	            std::cerr << "Invalid Command Line Argument\n";
        }
	}
	Des(encrypt, decrypt, plaintext, key, ascii, binary, hex, verbose);

}