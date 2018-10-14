#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include "des_data.h"


// Xors each char in the string
std::string xor_strings(std::string s1, std::string s2){
	if (s1.size() == s2.size()){
		std::string out;
		for (int i = 0; i < s1.size(); i++){
			out.push_back(s1[i]^s2[i]);
		}
		return out;
	}
	else {
		std::cout << "Strings are not the same size" << std::endl;
		return s1;
	}
}

// Compute p-box expansion
std::string expansion_p_box(std::string input){
	int output_size = 48;
	int out_box_size = 6;
	int in_box_size = 4;
	int num_out_boxes = 8;

	std::string output;
	output.resize(output_size);

	// Set first/last
	output[0] = input[32];
	output[47] = input[0];

	// Expansion perumutation
	for (int i = 0; i < num_out_boxes; i++){

		// First bit switch
		if (i != 0){
			output[i*out_box_size] = input[(i-1)*in_box_size];
		}

		// 4 middle no switch
		output[i*out_box_size+1] = input[i*in_box_size];
		output[i*out_box_size+2] = input[i*in_box_size + 1];
		output[i*out_box_size+3] = input[i*in_box_size + 2];
		output[i*out_box_size+4] = input[i*in_box_size + 3];

		// Last bit switch
		if (i != num_out_boxes-1){
			output[i*out_box_size+5] = input[(i+1)*in_box_size];
		}
	}

	return output;
}

// Convert string to binary string
std::string string_to_bin(std::string input){
	std::string output = "";

	// Convert each char to binary and put each binary part into output
	for (int i = 0; i < input.size(); i++){
		output += (std::bitset<8>(input.c_str()[i])).to_string<char, std::string::traits_type,std::string::allocator_type>();
	}

	return output;
}

// Convert a string of binary numbers to their ascii equivalent
std::string bin_to_string(std::string input){
	std::string output;
	std::bitset<8> bits;

	// Convert in 8 bit blocks
	for (int i = 0; i < input.size()/8; i++){
		std::string substring = input.substr(i*8, 8);
		bits = std::bitset<8>(substring.c_str());
		char c = char(bits.to_ulong());
		output += c;
	}

	return output;
}

std::string shrink_s_box(std::string input){
	std::string output;
	for (int i = 0; i < 8; i++){
		// Row is first and last bit
		std::string temp;
		temp += input[i*6];
		temp += input[i*6+5];
		int row = std::stoi(temp, nullptr, 2);

		// Column is middle 4 bits
		temp = input.substr(i*6+1, 4);
		int column = std::stoi(temp, nullptr, 2);

		// Get value conversion from sbox table
		int s_box_out = (int)(SBOX[i][row*16 + column]);

		// Add to output
		output += std::bitset<4>(s_box_out).to_string();
	}
	return output;
}

std::string straight_p_box(std::string input){
	std::string output;
	output.resize(32);
	for (int i = 0; i < input.size(); i++){
		int index = (int)PBOX[i]-1;
		output[i] = input[index];
	}
	return output;
}

std::string mangler(std::string input, std::string key){

	// Convert to binary for p-box
	std::string bin_string = string_to_bin(input);
	std::string expanded = expansion_p_box(bin_string);

	// Convert back to string for xor with key
	std::string text_string = bin_to_string(expanded);
	std::string xor_out = xor_strings(text_string, key);

	// Convert back to binary for s-box
	bin_string = string_to_bin(xor_out);
	std::string s_box_out = shrink_s_box(bin_string);

	std::string out = straight_p_box(s_box_out);

	// convert to string before return
	return bin_to_string(out);
}

int main(){
	/*
	
	// Pseudocode

	Input:
	64 bit plaintext
	48 bit key

	left_half = plaintext[0-31]
	right half = plaintext[32-63]
	
	mangled_right = mangler(right half, key)

	xor_out = left_half ^ mangled_right

	Output:
	right_out = xor_out
	left_out = right_half
	*/

	// Mock input 
	// 64 bit (8 byte) plaintext and 48 bit (6 byte) key
	std::string plaintext = "abcdefgh";
	std::string key = "123456";

	// Seperate left and right halves
	std::string left_half = plaintext.substr(0, 4);
	std::string right_half = plaintext.substr(4, 4);
	std::cout << "Left: " << left_half << ", Right: " << right_half << std::endl;
	std::cout << "Left bin:  " << string_to_bin(left_half) << "\nRight bin: " << string_to_bin(right_half) << std::endl;

	// Send through mangler function
	std::string mangled = mangler(right_half, key);
	std::cout << "Mangled: " << mangled << std::endl;

	std::string xor_out = xor_strings(left_half, mangled);
	std::cout << "Xor'd: " << xor_out << std::endl;

	std::string des_output = right_half + xor_out;
	std::cout << "Output: " << des_output << std::endl;
	std::cout << "Binary left output:  " << string_to_bin(right_half) << std::endl;
	std::cout << "Binary right output: " << string_to_bin(xor_out) << std::endl;
}