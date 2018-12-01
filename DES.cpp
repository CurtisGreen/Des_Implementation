#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include <vector>
#include "des_data.h"


// Xors each char in the string
std::string xor_strings(std::string s1, std::string s2){
	if (s1.size() == s2.size()){
		std::string out;
		for (int i = 0; i < s1.size(); i++){
			int temp = int(s1[i])^int(s2[i]);
			out.push_back('0' + temp);
		}
		return out;
	}
	else {
		std::cout << "Xor: Strings are not the same size" << std::endl;
		return s1;
	}
}

// Compute p-box expansion
std::string expansion_p_box(std::string input){
	if (input.size() == 32){
		std::string output;
		output.resize(48);

		for (int i = 0; i < 48; i++){
			output[i] = input[Expansion[i]-1];
		}

		return output;
	}
	else {
		std::cout << "Expansion: input not length 32" << std::endl;
		return "";
	}
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

std::string ip_permutation(std::string plaintext){
	if (plaintext.size() == 64){
		std::string output;
		output.resize(64);
		for (int i = 0; i < 64; i++){
			output[i] = plaintext[IP[i]-1];
		}
		return output;
	}
	else {
		std::cout << "IP: Plaintext not length 64" << std::endl;
		return "";
	}

}

std::string reverse_ip(std::string almost_encrypted_text){
	if (almost_encrypted_text.size() == 64){
		std::string output;
		output.resize(64);
		for (int i = 0; i < 64; i++){
			output[IP[i]-1] = almost_encrypted_text[i];
		}
		return output;
	}
	else {
		std::cout << "IP_inv: almost_encrypted_text not length 64" << std::endl;
		return "";
	}

}

std::string pc1_permutation(std::string key){
	if (key.size() == 64){
		std::string output;
		output.resize(56);
		for (int i = 0; i < 56; i++){
			output[i] = key[PC1[i]-1];
		}
		return output;
	}
	else {
		std::cout << "PC1: Key not length 64" << std::endl;
		return "";
	}
	
}

std::string pc2_permutation(std::string key){
	if (key.size() == 56){
		std::string output;
		output.resize(48);
		for (int i = 0; i < 48; i++){
			output[i] = key[PC2[i]-1];
		}
		return output;
	}
	else {
		std::cout << "PC2: Key not length 56" << std::endl;
		return "";
	}
	
}

std::string shrink_s_box(std::string input){
	if (input.size() == 48){
		std::string output;
		for (int i = 0; i < 8; i++){
			// Row is first and last bit
			std::string temp;
			temp += input[i*6];
			temp += input[i*6+5];
			std::cout << "row binary " << temp << std::endl;
			int row = std::stoi(temp, nullptr, 2);

			// Column is middle 4 bits
			temp = input.substr(i*6+1, 4);
			std::cout << "column binary " << temp << std::endl;
			int column = std::stoi(temp, nullptr, 2);

			// Get value conversion from sbox table
			int s_box_out = (int)(SBOX[i][row*16 + column]);
			std::cout << "row " << row << " column " << column << std::endl;
			std::cout << "sboxout " << s_box_out << std::endl;
			std::cout << "binary " << std::bitset<4>(s_box_out).to_string() << std::endl;
			std::cout << "\n";
			// Add to output
			output += std::bitset<4>(s_box_out).to_string();
		}
		return output;
	}
	else {
		std::cout << "Shrink: Input length not 48" << std::endl;
		return "";
	}
}

std::string straight_p_box(std::string input){
	if (input.size() == 32){
		std::string output;
		output.resize(32);
		for (int i = 0; i < input.size(); i++){
			int index = (int)PBOX[i]-1;
			output[i] = input[index];
		}
		return output;
	}
	else {
		std::cout << "Straight: Input length not 32" << std::endl;
		return "";
	}
}

std::vector<std::string> shift_key(std::string input){
	if (input.size() == 28){
		std::vector<std::string> output;
		output.push_back(input);
		std::string prev_round;
		// Shift each round based on the last
		for (int round = 0; round < 16; round++){
			std::string curr_round;
			curr_round.resize(input.size());

			// Shift once, first time
			if (round == 0){
				curr_round[27] = input[0];
				for (int i = 0; i < input.size()-1; i++){
					curr_round[i] = input[i+1];
				}
			}
			// Shift once
			else if (round == 1 || round == 8 || round == 15){
				curr_round[27] = prev_round[0];
				for (int i = 0; i < prev_round.size()-1; i++){
					curr_round[i] = prev_round[i+1];
				}
			}
			// Shift twice
			else {
				curr_round[26] = prev_round[0];
				curr_round[27] = prev_round[1];
				for (int i = 0; i < prev_round.size()-2; i++){
					curr_round[i] = prev_round[i+2];
				}
			}

			// Store round and prep next
			output.push_back(curr_round);
			prev_round = curr_round;
		}
		return output;
	}
	else {
		std::cout << "Shift: Input not size 28" << std::endl;
		return std::vector<std::string>();
	}
}

// 64-bit key -> 16 48-bit subkeys
std::vector<std::string> create_subkeys(std::string key){
	std::string key_permutation = pc1_permutation(key);
	//std::cout << key_permutation << std::endl;
	std::string left_perm_key = key_permutation.substr(0, 28);
	std::string right_perm_key = key_permutation.substr(28, 55);
	//std::cout << left_perm_key << std::endl;
	//std::cout << right_perm_key << std::endl;
	std::vector<std::string> shifted_left = shift_key(left_perm_key);
	std::vector<std::string> shifted_right = shift_key(right_perm_key);

	for (int i = 0; i < shifted_right.size(); i++){
		//std::cout << "C" << i << " = " << shifted_left[i] << std::endl;
		//std::cout << "D" << i << " = " << shifted_right[i] << std::endl;
		//std::cout << std::endl;
	}

	// Combine left/right and perm through PC2
	std::vector<std::string> pc2_perms;
	for (int i = 1; i < shifted_right.size(); i++){
		std::string pc2_perm = pc2_permutation(shifted_left[i] + shifted_right[i]);
		pc2_perms.push_back(pc2_perm);
	}

	for (int i = 0; i < pc2_perms.size(); i++){
		//std::cout << "K" << i+1 << " = " << pc2_perms[i] << std::endl;
	}
	
	return pc2_perms;
}

std::string mangler(std::string input, std::string key){

	// Convert to binary for p-box
	//std::string bin_string = string_to_bin(input);
	std::string expanded = expansion_p_box(input);
	//std::cout << "Expansion: " << expanded << std::endl;

	// Convert back to string for xor with key
	//std::string text_string = bin_to_string(expanded);
	std::string xor_out = xor_strings(expanded, key);
	//std::cout << "Xor: " << xor_out << std::endl;

	// Convert back to binary for s-box
	//bin_string = string_to_bin(xor_out);
	std::string s_box_out = shrink_s_box(xor_out);
	//std::cout << "Shrink: " << s_box_out << std::endl;

	std::string out = straight_p_box(s_box_out);
	//std::cout << "Straight: " << out << std::endl;

	// convert to string before return
	//return bin_to_string(out);
	return out;
}

int main(){

	// Input 
	// 64 bit (8 byte) plaintext and 48 bit (6 byte) key
	std::string plaintext = "0000000100100011010001010110011110001001101010111100110111101111";
	std::string key = "0001001100110100010101110111100110011011101111001101111111110001";

	// Create 16 subkeys of length 48bits
	std::vector<std::string> subkeys = create_subkeys(key);

	// Encode each 64 bit block
	std::string ip_permed = ip_permutation(plaintext);
	//std::cout << ip_permed << std::endl;

	// Split halves
	std::string initial_left = ip_permed.substr(0, 32);
	std::string initial_right = ip_permed.substr(32, 32);
	std::cout << "L0 = " << initial_left << std::endl;
	std::cout << "R0 = " << initial_right << std::endl;

	// Run 16 rounds of operations
	std::string left_half;
	std::string right_half;
	for (int round = 0; round < 16; round++){
		std::cout << "Round " << round << ":" << std::endl;
		if (round == 0){
			std::string mangled = mangler(initial_right, subkeys[0]);
			//std::cout << "Mangled: " << mangled << std::endl;
			std::string xor_out = xor_strings(initial_left, mangled);
			//std::cout << "Xor'd: " << xor_out << std::endl;
			left_half = initial_right;
			right_half = xor_out;
		}
		else {
			std::string mangled = mangler(right_half, subkeys[round]);
			//std::cout << "Mangled: " << mangled << std::endl;
			std::string xor_out = xor_strings(left_half, mangled);
			//std::cout << "Xor'd: " << xor_out << std::endl;
			left_half = right_half;
			right_half = xor_out;
		}
		std::cout << "K" << round+1 << " = " << subkeys[round] << std::endl;
		std::cout << "L" << round+1 << " = " << left_half << std::endl;
		std::cout << "R" << round+1 << " = " << right_half << std::endl;
		std::cout << "\n";
	}

	// Reverse the initial swap & perm
	std::string final_output = reverse_ip(right_half + left_half);
	std::cout << "\nDES output " << final_output << std::endl;
}