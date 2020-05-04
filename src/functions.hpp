#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <vector>
#include <string>
#include <bitset>
#include <map>

/**
@file function.hpp
@brief All functions declarations 
@author Levon Muradyan
*/

/**
@brief This function helps in logging level 
*/
void init_logging();

/**
@brief initializing random matrices
@param matrices - vector of bitsets, contain all initialized matrices of length 100 
*/
void init_random_matrices(
	std::vector<std::bitset<100>>& matrices);

/**
@brief reading dictionary from file 
@param dictionary_filename - the filename that need to be read
@param dictionary - contain all words in the file  
*/
void read_dict_from_file(const std::string& dictionary_filename, 
	std::vector<std::string>& dictionary);

/**
@brief reading input text, stop words from file
@param text_filename - the filename that need to be read
@param text - contain all words in the file 
*/
void read_text_from_file(const std::string& text_filename, 
	std::vector<std::string>& text);

/**
@brief calculating hashes of words stored in words
@param hasher - object that hashes words
@param words - all words that need to be hashed
*/
void init_hashes(std::vector<size_t>& hashes, 
	const std::hash<std::string>& hasher, 
	const std::vector<std::string>& words);

/**
@brief finding duplicates and correcting dictionary hashes
@param duplicates - contain words old hashes, indexes to the words with that hashes, and new hashes
@param hashes - hash of dictionary, contain hashed strings of each word in dictionary
*/
void correct_dict_hashes_and_extract_duplicates(
	std::vector<std::vector<size_t>>& duplicates, 
	std::vector<size_t>& hashes);

/**
@brief correcting input text hashes, maybe there are duplicates
@param text_hashes - hash of text, contain hashed strings of each word in text
@param text - contain all words in input text(maximum 500terms)
@param duplicates - contain words old hashes, indexes to the words with that hashes, and new hashes 
@param dictionary - conatin all words in dictionary.txt
*/
void correct_text_hashes(std::vector<size_t>& text_hashes,
	const std::vector<std::string>& text,
	const std::vector<std::vector<size_t>>& duplicates,
	const std::vector<std::string>& dictionary);

/**
@brief hashing open adressing with linear probbing algorithm
@param hash_table - hash table contain hashes and indexes to the words in dictionary
@param hashes - hash of dictionary, contain hashed strings of each word in dictionary
*/
void hash_function(std::vector<std::pair<size_t,size_t>>& hash_table,
	const std::vector<size_t>& hashes);

/**
@brief correcting input text hashes, maybe there are duplicates
@param output - the result of the program, score of the sentence
@param text_single_term_hashes - hash of text, contain hashed strings of each word in text
@param text_double_term_hashes - hash of text, contain hashed strings of each double combination of words in text
@param stop_words_hashes - contain all hashes of stop words
@param hash_table - hash table contain hashes and indexes to the words in dictionary
@param matrices - vector of bitsets, contain all initialized matrices of length 100
@param dictionary - conatin all words in dictionary.txt
*/
void search_and_calculate_matrices(std::bitset<100>& output,
	std::map<std::string, size_t>& words_count,
	const std::vector<size_t>& text_single_term_hashes,
	const std::vector<size_t>& text_double_term_hashes,
	const std::vector<size_t>& text_triple_term_hashes,
	const std::vector<size_t>& stop_words_hashes,
	const std::vector<std::pair<size_t, size_t>>& hash_table,
	const std::vector<std::bitset<100>>& matrices,
	const std::vector<std::string>& dictionary);



void write_in_file();
void print_hash_table(const std::vector<std::pair<size_t, size_t>>& hash_table, size_t begin = 0, size_t end = 466548);
void print_duplicates(const std::vector<std::vector<size_t>>& duplicates,
	const std::vector<std::string>& dictionary);
void print_matrix(const std::vector<size_t>& matrix);
void print_words(const std::vector<std::string>& dictionary);
void print_vector(const std::vector<size_t>& vector);
void print_bitset(const std::bitset<100>& bitset);
void print_bitset_vector(const std::bitset<100>& bitset);
bool contains_duplicates(std::vector<size_t> a);
bool contains_duplicates_h(std::vector<size_t> a);
void init_random_matrices(
	std::vector<std::vector<size_t>>& matrices);
void extract_words_count(const std::map<std::string, size_t>& m);
#endif // FUNCTIONS_HPP


