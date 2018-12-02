#include <iostream>
#include <unistd.h>

#include "des.hpp"
#include "des_data.hpp"

int main(int argc, char** argv){

	bool encrypt, decrypt, ascii, binary, verbose = false;
	std::string plaintext, key;
	int opt = 0;
	while((opt = getopt(argc, argv, "edp:k:abv")) != -1){
		switch(opt) {
	        case 'e':
	            encrypt = true;break;
	        case 'd':
	            decrypt = true;break;
	        case 'p':
	            plaintext = optarg;break;
	        case 'k':
	            key = optarg;break;
	        case 'a':
	            ascii = true;break;
	        case 'b':
	            binary = true;break;
	        case 'v':
	        	verbose = true;break;
	        default:
	            std::cerr << "Invalid Command Line Argument\n";
        }
	}
	Des(encrypt, decrypt, plaintext, key, ascii, binary, verbose);

}