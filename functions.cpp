#include "functions.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <functional>
#include <map>
#include <random>
#include <chrono>

void init_random_matrices(std::vector<std::bitset<100>>& matrices)
{
	auto start = std::chrono::steady_clock::now();
	size_t dict_size = matrices.size();
	std::default_random_engine dre(std::random_device{}());
	std::uniform_int_distribution<long long> dist(0, (1ll << 50));
	for (size_t i = 0; i < dict_size; ++i)
	{	
		matrices[i] = dist(dre);
		matrices[i] <<= 50;
		matrices[i] |= dist(dre);
	}
	auto end = std::chrono::steady_clock::now();
	std::cout << "Elapsed time in milliseconds : "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << "  init_random_matrices()" << std::endl;

}

void read_dict_from_file(const std::string& dictionary_filename, std::vector<std::string>& dictionary)
{
	auto start = std::chrono::steady_clock::now();
	std::ifstream file(dictionary_filename);
	size_t counter = 0;
	if (file.is_open())
	{
		std::string line;
		int i = 0;
		while (std::getline(file, line))
		{
			dictionary[i++] = line;
			counter++;
		}
		dictionary.resize(counter);
		file.close();
	}
	else
	{
		std::cerr << "Couldn't open " << "dictionary.txt" << " for reading\n";
	}
	auto end = std::chrono::steady_clock::now();
	std::cout << "Elapsed time in milliseconds : "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << "  read_dict_from_file()" << std::endl;
}

void read_from_text_file(const std::string& text_filename, std::vector<std::string>& text)
{
	auto start = std::chrono::steady_clock::now();
	std::ifstream file(text_filename);
	if (file.is_open())
	{
		std::string line;
		size_t i = 0, counter = 0;
		while (std::getline(file, line))
		{
			char* str = &line[0];
			char* pch;
			pch = strtok(str, " ,.-?");
			
			while (pch != NULL)
			{
				for (int j = 0; j <= strlen(pch); j++)
				{
					if (pch[j] >= 65 && pch[j] <= 92)
					{
						pch[j] = pch[j] + 32;
					}
				}
				text[i++] = pch;
				pch = strtok(NULL, " ,.-?");
				counter++;
			}			
		}
		text.resize(counter);
		file.close();
	}
	else
	{
		std::cerr << "Couldn't open " << "text.txt" << " for reading\n";
	}

	auto end = std::chrono::steady_clock::now();
	std::cout << "Elapsed time in milliseconds : "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << "  read_from_text_file()" << std::endl;

}

void init_hashes(std::vector<size_t>& hashes, const std::hash<std::string>& hasher, 
	const std::vector<std::string>& words)
{
	auto start = std::chrono::steady_clock::now();
	size_t size = hashes.size();
	for (size_t i = 0; i < size; ++i)
	{
		hashes[i] = hasher(words[i]);
	}
	auto end = std::chrono::steady_clock::now();
	std::cout << "Elapsed time in milliseconds : "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << "  init_hashes()" << std::endl;
}

void correct_dict_hashes_and_extract_duplicates(
	std::vector<std::vector<size_t>>& duplicates, 
	std::vector<size_t>& hashes)
{
	auto start = std::chrono::steady_clock::now();
	size_t size = hashes.size();
	std::map<size_t, size_t> m;

	if (size < 2)
	{
		return;
	}

	for (size_t i = 0; i < size; ++i)
	{
		m[hashes[i]] = 0;
	}
	for (size_t i = 0; i < size; ++i)
	{
		if (m[hashes[i]])
		{
			size_t temp = hashes[i];
			while (m[temp])
			{
				temp++;
			}
			duplicates.push_back({ hashes[i], i, temp });
			hashes[i] = temp;
		}
		else
		{
			m[hashes[i]] = 1;
		}
	}
	auto end = std::chrono::steady_clock::now();
	std::cout << "Elapsed time in milliseconds : "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << "  correct_dict_hashes_and_extract_duplicates()" << std::endl;
}

void correct_text_hashes(std::vector<size_t>& text_hashes,
	const std::vector<std::string>& text,
	const std::vector<std::vector<size_t>>& duplicates,
	const std::vector<std::string>& dictionary)
{
	auto start = std::chrono::steady_clock::now();
	size_t duplicate_size = duplicates.size();
	size_t text_size = text.size();
	for (size_t i = 0; i < duplicates.size(); ++i)
	{
		for (size_t j = 0; j < text_size; ++j)
		{
			if (text_hashes[j] == duplicates[i][0])
			{
				if (text[j] == dictionary[duplicates[i][1]])
				{
					text_hashes[j] = duplicates[i][2];
				}
			}
		}
	}
	auto end = std::chrono::steady_clock::now();
	std::cout << "Elapsed time in milliseconds : "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << "  correct_text_hashes()" << std::endl;
}

void hash_function(std::vector<std::pair<size_t, size_t>>& hash_table,
	const std::vector<size_t>& hashes)
{
	auto start = std::chrono::steady_clock::now();
	size_t words_count = hashes.size();
	for (size_t i = 0; i < words_count; ++i)
	{
		size_t index = hashes[i] % words_count;
		while (hash_table[index].first)
		{
			index = (index + 1) % words_count;
		}
		hash_table[index] = std::make_pair(hashes[i], i);
	}
	auto end = std::chrono::steady_clock::now();
	std::cout << "Elapsed time in milliseconds : "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << "  hash_function()" << std::endl;
}

void search_and_calculate_matrices(std::bitset<100>& output,
	const std::vector<size_t>& text_hashes,
	const std::vector<std::pair<size_t, size_t>>& hash_table,
	const std::vector<std::bitset<100>>& matrices,
	const std::vector<std::string>& dictionary)
{
	auto start = std::chrono::steady_clock::now();
	size_t text_size = text_hashes.size();
	size_t dict_size = dictionary.size();
	size_t matrix_size = output.size();
	for (size_t i = 0; i < text_size; ++i)
	{
		size_t index = text_hashes[i] % dict_size;
		for (size_t j = 0; j < dict_size; ++j)
		{
			if (hash_table[index].first == text_hashes[i])
			{				
				output |= matrices[hash_table[index].second];			
				//std::cout << "The word found = " << dictionary[hash_table[index].second] << std::endl;
				//print_bitset(matrices[hash_table[index].second]);
				//std::cout << std::endl;
				break;
			}
			else
			{
				index = (index + 1) % dict_size;
			}
		}
	}
	auto end = std::chrono::steady_clock::now();
	std::cout << "Elapsed time in milliseconds : "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms" << "  search_and_calculate_matrices()" << std::endl;
}















void write_in_file()
{
	std::string str;
	std::ifstream in("words.txt");
	while (std::getline(in, str))
	{
		std::cout << str << std::endl;
	}
}

void print_hash_table(const std::vector<std::pair<size_t, size_t>>& hash_table, size_t size = 466547)
{
	for (size_t i = 0; i < size; ++i)
	{
		std::cout << "hash = " << hash_table[i].first << "  index = " << hash_table[i].second << std::endl;
	}
}

void print_duplicates(const std::vector<std::vector<size_t>>& duplicates,
	const std::vector<std::string>& dictionary)
{
	for (int i = 0; i < duplicates.size(); ++i)
	{
		for (int j = 0; j < duplicates[0].size(); ++j)
		{
			if (j == 1)
			{
				std::cout << dictionary[duplicates[i][j]] << " ";
			}
			else
			{
				std::cout << duplicates[i][j] << " ";
			}

		}
		std::cout << std::endl;
	}
}

void print_matrix(const std::vector<size_t>& matrix)
{
	for (size_t j = 0; j < 10; ++j)
	{
		for (size_t i = j * 10; i < j * 10 + 10; ++i)
		{
			std::cout << matrix[i] << " ";
		}
		std::cout << std::endl;
	}
}

void print_words(const std::vector<std::string>& dictionary)
{
	for (size_t i = 0; i < dictionary.size(); ++i)
	{
		std::cout << dictionary[i] << std::endl;
	}
	std::cout << std::endl;
}

void print_vector(const std::vector<size_t>& vector)
{
	for (size_t i = 0; i < vector.size(); ++i)
	{
		std::cout << vector[i];
	}
	std::cout << std::endl;
}

void print_bitset(const std::bitset<100>& bitset)
{
	for (size_t j = 0; j < 10; ++j)
	{
		for (size_t i = j * 10; i < j * 10 + 10; ++i)
		{
			std::cout << bitset[i] << " ";
		}
		std::cout << std::endl;
	}
}

bool contains_duplicates(std::vector<size_t> a)
{
	if (a.size() < 2)
	{
		return false;
	}
	sort(a.begin(), a.end());
	//std::cout << a[a.size() - 1] << std::endl;
	//std::cout << a[0] << std::endl;
	for (int i = 0; i < a.size() - 1; i++)
	{
		if (a[i] == a[i + 1])
		{
			std::cout << a[i] << std::endl;
		}
	}
	return false;
}

void init_random_matrices(std::vector<std::vector<size_t>>& matrices)
{
	size_t dict_size = matrices.size();
	size_t matrix_size = matrices[0].size();

	for (size_t i = 0; i < dict_size; ++i)
	{
		for (size_t j = 0; j < matrix_size; ++j)
		{
			matrices[i][j] = rand() % 2;
		}
	}
}

void read_from_file(const std::string& text_filename, std::string& text)
{
	std::ifstream file(text_filename);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			text += line;
			text += " ";
		}
		file.close();
	}
	else
	{
		std::cerr << "Couldn't open " << "text.txt" << " for reading\n";
	}
}