#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <omp.h>
#include <numeric>

// Function to count occurrences of a word in a vector of strings
int count_word_occurrences(const std::vector<std::string> &words, const std::string &target_word)
{
    int count = 0;
    for (const auto &word : words)
    {
        if (word == target_word)
        {
            count++;
        }
    }
    return count;
}

void get_local_text(std::string &res, int rank, int ranks_count, int text_length, int word_length, const std::string &text)
{
    int start = rank * (text_length / ranks_count);
    int end;
    if (rank == ranks_count - 1)
    {
        end = text_length;
    }
    else
    {
        end = start + (text_length / ranks_count) + word_length;
    }
    int size = end - start;
    char *text_buffer = new char[size];
    strncpy(text_buffer, text.c_str() + start, size);
    res = std::string(text_buffer);
    if (rank != ranks_count - 1)
    {
        int split_pos = res.find_last_of(" ");
        if (split_pos != std::string::npos)
        {
            end = split_pos;
            res = std::string(text_buffer, text_buffer + end);
        }
    }
}

int main()
{
    double startTime = omp_get_wtime();
    int size = 8;
    omp_set_num_threads(size);
    std::string target_word = "the";
    std::string text;
    std::ifstream file("D:\\bible.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    text = buffer.str();
    int text_length = text.length();
    // Vector to store counts from each thread
    std::vector<int> thread_counts(omp_get_max_threads(), 0);
    int word_length = target_word.length();
// Count occurrences
#pragma omp parallel
    {
        int rank = omp_get_thread_num();
        std::string local_text;
        get_local_text(local_text, rank, size, text_length, word_length, text);

        std::istringstream iss(local_text);
        std::vector<std::string> words;
        std::string word;
        while (iss >> word)
        {
            words.push_back(word);
        }
        int count = count_word_occurrences(words, target_word);
        thread_counts[rank] = count;
    }
    // Print count from each thread
    for (int i = 0; i < size; ++i)
    {
        std::cout << "Thread " << i << ": " << thread_counts[i] << " occurrences" << std::endl;
    }
    //   Sum up counts from all threads to get global count
    int global_count = std::accumulate(thread_counts.begin(), thread_counts.end(), 0);
    std::cout << "Total occurrences of '" << target_word << "': " << global_count << std::endl;
    // Record the end time and calculate elapsed time
    double endTime = omp_get_wtime();
    double elapsedTime = endTime - startTime;
    std::cout << "Time: " << elapsedTime << " seconds" << std::endl;
}
