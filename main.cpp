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
  int m_lives{6};
  std::vector<bool> m_letterGuessed{std::vector<bool>(26)};
  bool m_valid{};

  std::size_t toIndex(char c) const { return static_cast<std::size_t>((c % 32) - 1); }

public:
  std::string_view getWord() const { return m_word; }
  int getLives() { return m_lives; }

  bool isLetterGuessed(char c) const { return m_letterGuessed[toIndex(c)]; }
  void setLetterGuessed(char c) { m_letterGuessed[toIndex(c)] = true; }
  void removeLife() { m_lives--; }

  bool isLetterInWord(char letter)
  {
    for (auto c : m_word)
    {
      if (c == letter)
        return true;
    }
    return false;
  }

  bool won()
  {
    for (auto c : m_word)
    {
      if (!isLetterGuessed(c))
        return false;
    }
    return true;
  }
};

void draw(Session &s)
{
  std::cout << '\n';
  std::cout << "The word:";
  for (auto letter : s.getWord())
  {
    if (s.isLetterGuessed(letter))
      std::cout << " " << letter;
    else
      std::cout << " _";
  }

  std::cout << "\nWrong guesses:";

  for (char c = 'a'; c <= 'z'; ++c)
    if (s.isLetterGuessed(c) && !s.isLetterInWord(c))
      std::cout << " [ " << c << " ]";

  std::cout << '\n';
}

bool hasBeenGuessed(Session &s, char &letter)
{
  if (!s.isLetterGuessed(letter))
  {
    s.setLetterGuessed(letter);
    return false;
  }
  return true;
}

char getLetterInput(Session &s)
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

    if (hasBeenGuessed(s, letter))
    {
      std::cout << "Letter has been guessed. Try again.\n";
      continue;
    }

    return letter;
  }
}

int main()
{
  startMessage();

  Session s{};

  while (s.getLives() && !s.won())
  {
    draw(s);
    std::cout << "You have " << s.getLives() << " lives left\n";
    char letter{getLetterInput(s)};
    if (!s.isLetterInWord(letter))
      s.removeLife();
  }

  draw(s);

  !s.getLives() ? std::cout << "You lost! The word was: " << s.getWord() << '\n' : std::cout << "you won!\n";
}