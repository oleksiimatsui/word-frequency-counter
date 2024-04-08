#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <omp.h>

// Function to count occurrences of a word in a vector of strings
int count_word_occurrences(const std::vector<std::string> &words, const std::string &target_word)
{
    int count = 0;
#pragma omp parallel for reduction(+ : count)
    for (int i = 0; i < words.size(); ++i)
    {
        if (words[i] == target_word)
        {
            count++;
        }
    }
    return count;
}

int main()
{
    // The word to search for in the text
    std::string target_word = "the";
    std::string text;

    // Open the file containing the text
    std::ifstream file("D:\\bible.txt");

    // Read the entire file into a stringstream
    std::stringstream buffer;
    buffer << file.rdbuf();
    text = buffer.str();

    // Record the start time for performance measurement
    double startTime = omp_get_wtime();

    // Tokenize the text into individual words
    std::istringstream iss(text);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word)
    {
        words.push_back(word);
    }

    // Count the occurrences of the target word in the text
    int global_count = count_word_occurrences(words, target_word);
    std::cout << "Occurrences of '" << target_word << "': " << global_count << std::endl;

    // Record the end time and calculate elapsed time
    double endTime = omp_get_wtime();
    double elapsedTime = endTime - startTime;
    std::cout << "Time: " << elapsedTime << " seconds" << std::endl;

    return 0;
}
