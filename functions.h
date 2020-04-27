#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <vector>
#include <string>
#include <bitset>

void init_random_matrices(
	std::vector<std::bitset<100>>& matrices);
void read_dict_from_file(const std::string& dictionary_filename, 
	std::vector<std::string>& dictionary);
void read_from_text_file(const std::string& text_filename, 
	std::vector<std::string>& text);
void init_hashes(std::vector<size_t>& hashes, 
	const std::hash<std::string>& hasher, 
	const std::vector<std::string>& words);
void correct_dict_hashes_and_extract_duplicates(
	std::vector<std::vector<size_t>>& duplicates, 
	std::vector<size_t>& hashes);
void correct_text_hashes(std::vector<size_t>& text_hashes,
	const std::vector<std::string>& text,
	const std::vector<std::vector<size_t>>& duplicates,
	const std::vector<std::string>& dictionary);
void hash_function(std::vector<std::pair<size_t,size_t>>& hash_table,
	const std::vector<size_t>& hashes);
void search_and_calculate_matrices(std::bitset<100>& output,
	const std::vector<size_t>& text_single_term_hashes,
	const std::vector<size_t>& text_double_term_hashes,
	const std::vector<size_t>& stop_words_hashes,
	const std::vector<std::pair<size_t, size_t>>& hash_table,
	const std::vector<std::bitset<100>>& matrices,
	const std::vector<std::string>& dictionary,
	std::vector<size_t>& words_count);

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
void extract_words_count(std::vector<size_t>& words_count, const std::vector<std::string>& dictionary);
#endif // FUNCTIONS_HPP


