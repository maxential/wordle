#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <limits>
#include <conio.h>

void clear_console() {
#ifdef _WIN32
    system("cls"); // Windows
#else
    system("clear"); // Unix
#endif
}

class Wordle {
public:
    Wordle(const std::string& secret_word) : word(secret_word) {}

    bool guess(const std::string& player_guess) {
        if (player_guess.length() != word.length()) {
            std::cerr << "Invalid guess length. The word should be " << word.length() << " characters long." << std::endl;
            return false;
        }

        std::vector<bool> word_matched(word.length(), false);
        std::vector<bool> guess_matched(player_guess.length(), false);

        bool all_correct = true;

        for (size_t i = 0; i < word.length(); ++i) {
            if (player_guess[i] == word[i]) {
                std::cout << player_guess[i] << " Is Correct" << std::endl;
                guess_matched[i] = true;
                word_matched[i] = true;
            }
            else {
                all_correct = false;
            }
        }

        for (size_t i = 0; i < player_guess.length(); ++i) {
            if (!guess_matched[i]) {
                bool is_yellow = false;
                for (size_t a = 0; a < word.length(); ++a) {
                    if (!word_matched[a] && player_guess[i] == word[a]) {
                        is_yellow = true;
                        break;
                    }
                }
                if (is_yellow) {
                    std::cout << player_guess[i] << " Is Yellow" << std::endl;
                }
            }
        }

        return all_correct;
    }

private:
    std::string word;
};

std::string get_random_word(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open wordlist file." << std::endl;
        exit(1);
    }

    std::vector<std::string> words;
    std::string line;
    while (std::getline(file, line)) {
        words.push_back(line);
    }

    file.close();

    if (words.empty()) {
        std::cerr << "Wordlist is empty." << std::endl;
        exit(1);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, words.size() - 1);

    return words[dis(gen)];
}

void play_game(int max_attempts) {
    std::string filename = "wordlist.txt";
    std::string secret_word = get_random_word(filename);

    Wordle game(secret_word);
    std::string guess;
    int attempts = 0;

    while (attempts < max_attempts) {
        std::cout << "Guess the word: ";
        std::cin >> guess;
        std::cout << std::endl;

        if (game.guess(guess)) {
            std::cout << "Congratulations! You guessed the word correctly!" << std::endl;
            return;
        }
        else {
            ++attempts;
            if (attempts < max_attempts) {
                std::cout << "You have " << (max_attempts - attempts) << " attempts left." << std::endl;
            }
            else {
                std::cout << "Sorry, you've used all your attempts. The word was: " << secret_word << std::endl;
                std::cout << "Press any key to continue" << std::endl;
                _getch();
            }
        }
    }
}

int main() {
    int max_attempts = 6;
    int choice;

    while (true) {
        clear_console();
        std::cout << "!! Wordle Game !!\n";
        std::cout << "1. Start New Game\n";
        std::cout << "2. Set Maximum Attempts\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";

        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clear_console();
            continue;
        }

        std::cout << std::endl;

        switch (choice) {
        case 1:
            clear_console();
            play_game(max_attempts);
            break;
        case 2:
            std::cout << "Enter new maximum attempts: ";
            std::cin >> max_attempts;

            if (std::cin.fail() || max_attempts <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                clear_console();
                continue;
            }

            std::cout << "Maximum attempts updated to " << max_attempts << "." << std::endl;
            break;
        case 3:
            std::cout << "Exiting the game. Goodbye!" << std::endl;
            return 0;
        default:
            clear_console();
            continue;
        }
    }

    return 0;
}
