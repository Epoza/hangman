#include <iostream>
#include <vector>
#include "Random.h"

// output that displays at the start of the game
void startMessage()
{
  std::cout << "Welcome to hangman\n";
  std::cout << "To win: guess the word.  To lose: run out of pluses.\n";
}

namespace WordList
{
  std::vector<std::string_view> words{"mystery", "broccoli", "account", "almost", "spaghetti", "opinion", "beautiful", "distance", "luggage"};

  std::string_view getRandomWord()
  {
    return words[Random::get<std::size_t>(0, words.size() - 1)];
  }
};

class Session
{
private:
  std::string m_word{WordList::getRandomWord()};
  // char m_guess{};
  // bool m_valid{};

public:
  std::string_view getWord() const { return m_word; }
};

void hideWord(Session &s)
{
  std::cout << '\n';
  std::cout << "The word:";
  for (auto letter : s.getWord())
  {
    std::cout << " _";
  }
  std::cout << '\n';
}

char getLetterInput()
{
  // Keep asking user until they enter valid input
  while (true)
  {
    std::cout << "Enter your next letter: ";
    char letter{};
    std::cin >> letter;

    // if user entered bad input, try again
    if (!std::cin)
    {
      std::cin.clear();
      std::cout << "Invalid input. Try again.\n";
      // clear out any extraneous input
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }

    // input not a letter, try again
    if (letter < 'a' || letter > 'z')
    {
      std::cout << "Invalid input. Try again.\n";
      continue;
    }
    return letter;
  }
}

int main()
{
  startMessage();

  Session s{};
  hideWord(s);
  char letter{getLetterInput()};

  std::cout << "You guessed: " << letter << '\n';
}