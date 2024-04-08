#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>

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

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::string target_word = "the";
    std::string text;
    std::ifstream file("D:\\bible.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    text = buffer.str();
    // Broadcast text to all processes
    int text_length = text.length();
    MPI_Bcast(&text_length, 1, MPI_INT, 0, MPI_COMM_WORLD);
    char *text_buffer = new char[text_length + 1];
    double startTime;
    if (rank == 0)
    {
        startTime = MPI_Wtime();
        strcpy(text_buffer, text.c_str());
    }
    MPI_Bcast(text_buffer, text_length + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    int chunk_size = text_length / size;
    int start = rank * chunk_size;
    int end = (rank == size - 1) ? text_length : start + chunk_size;
    if (rank != size - 1)
    {
        end += target_word.length();
    }
    std::string local_text = std::string(text_buffer + start, text_buffer + end);
    int split_pos = local_text.find_last_of(" ");
    if (split_pos != std::string::npos)
    {
        end = start + split_pos;
        local_text = std::string(text_buffer + start, text_buffer + end);
    }
    std::istringstream iss(local_text);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word)
    {
        words.push_back(word);
    }
    int local_count = count_word_occurrences(words, target_word);
    std::cout << "Occurrences of '" << target_word << "' in rank " << rank << ": " << local_count << std::endl;

    int global_count;
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        std::cout << "Occurrences of '" << target_word << "': " << global_count << std::endl;
        double endTime = MPI_Wtime();
        double elapsedTime = endTime - startTime;
        std::cout << "Time: " << elapsedTime << " seconds" << std::endl;
    }

    delete[] text_buffer;
    MPI_Finalize();
    return 0;
}
