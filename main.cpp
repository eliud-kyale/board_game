/**
 *  Written By Eliud Kyale 
 *  September 15th 2021
 * 
 *          * * * * * * * * * * * * * 
 *          *  S  *  E  *  F  *  I  *
 *          * * * * * * * * * * * * *
 *          *  O  *  C  *  A  *  T  *
 *          * * * * * * * * * * * * *
 *          *  R  *  E  *  B  *  O  *
 *          * * * * * * * * * * * * *
 *          *  C  *  T  *  H  *  J  *
 *          * * * * * * * * * * * * *
 * 
 *   WORDS:
 *     SO, SEC, SOCA, SORE
 *     ERO ...
 */ 
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

const unsigned int MAX_WORD_LENGTH = 16;

/**
 *  Load the english dictionary from file 
 *  Limit the words in the dictionary to max words
 */
vector<string> getFilteredEnglishDictionary(set<char> letters, unsigned int max_word_size)
{
  vector<string> dict;
  ifstream file("words_alpha.txt"); // from https://github.com/dwyl/english-words
  string str; 

  while (std::getline(file, str))
  {
    // Filter out some words 
    if(str.empty()) continue;
    if(str.length() > max_word_size) continue;          // Skip words that are too big 
    if(letters.find(str[0]) == letters.end()) continue; // Starting letter needs to be present
    
    // Add the word to the dictionary
    dict.push_back(str);
  }

  return dict;
}

set<char> getUniqueCharacters(const char board[4][4])
{
    set<char> unique_chars;
    for(int i = 0; i < 4; i++)
      for(int j = 0; j < 4; j++)
      {
        unique_chars.insert(board[i][j]);
      }

    return unique_chars;
}

enum Direction { left, right, up, down };
typedef pair<int,int> Point;

bool isPointInvalid(const Point &point, int min, int max)
{
  int x = point.first;
  int y = point.second;

  return ( x > max || x < min || y > max || y < min );
}

Point getNextPoint(Point point, Direction direction)
{
  Point new_point;
  switch(direction)
  {
    case Direction::left:
    {
      new_point = make_pair(point.first - 1, point.second);
      break;
    }
    case Direction::right:
    {
      new_point = make_pair(point.first + 1, point.second);
      break;
    }
    case Direction::up:
    {
      new_point = make_pair(point.first, point.second + 1);
      break;
    }
    case Direction::down:
    default:
    {
      new_point = make_pair(point.first, point.second - 1);
      break;
    }
  }

  return new_point;
}

bool searchForWord(const string& word, 
                   const char board[4][4], 
                   Direction direction, 
                   Point last_point, 
                   set<Point> visited_points, 
                   string str)
{
  Point point = getNextPoint(last_point, direction);
  int x = point.first;
  int y = point.second;
  char c;

  // Out of bounds
  if(true == isPointInvalid(point, 0, 3))
  {
    return false;
  }

  // Already visited - Ouroboros
  if(visited_points.find(point) != visited_points.end())
  {
    return false;
  }

  // Add new point to visited list 
  visited_points.insert(point);

  // Safety check - longest word
  if(visited_points.size() > MAX_WORD_LENGTH)
  {
    return false;
  }
  
  c = board[x][y];
  str.push_back(c);

  if(str.length() > word.length())
  {
    // something went wrong 
    return false;
  }

  // check for matches
  if(str.length() == word.length())
  {
    return (str.compare(word) == 0);
  }

  string subst = word.substr(0, str.length());
  if(str.compare(subst) != 0)
  {
    return false;
  }

  // Keep searching substring matched

  if(true == searchForWord(word, board, Direction::left, point, visited_points, str))
  {
    return true;
  }

  if(true == searchForWord(word, board, Direction::right, point, visited_points, str))
  {
    return true;
  }

  if(true == searchForWord(word, board, Direction::up, point, visited_points, str))
  {
    return true;
  }

  if(true == searchForWord(word, board, Direction::down, point, visited_points, str))
  {
    return true;
  }

  return false;
}

int main(void)
{
  // Create a 4x4 array of the board
  const char board[4][4] = 
  { 
    's', 'e', 'f', 'i', 
    'o', 'c', 'a', 't', 
    'r', 'e', 'b', 'o',
    'c', 't', 'h', 'j' 
  };

  set<char>      letters       = getUniqueCharacters(board);
  vector<string> english_words = getFilteredEnglishDictionary(letters, MAX_WORD_LENGTH); // all words in small caps

  vector<string> words;

  Point point;
  set<Point> visited_points;
  string str;

  char c;

  // loop through the squares on the board while looping through the words
  for(string word : english_words)
    for(int i = 0; i < 4; i++)
      for(int j = 0; j < 4; j++)
      {
        str.clear();
        visited_points.clear();

        c = board[i][j];

        // single character word
        if( (word.length() == 1) && (word[0] == c) )
        {
          // Word found
          words.push_back(word);
          continue;
        }

        point = make_pair(i,j);

        str.push_back(c);
        visited_points.insert(point);

        if(true == searchForWord(word, board, Direction::left, point, visited_points, str))
        {
          // Word found
          words.push_back(word);
          continue;
        }
        
        if(true == searchForWord(word, board, Direction::right, point, visited_points, str))
        {
          // Word found
          words.push_back(word);
          continue;
        }

        if(true == searchForWord(word, board, Direction::up, point, visited_points, str))
        {
          // Word found
          words.push_back(word);
          continue;
        }

        if(true == searchForWord(word, board, Direction::down, point, visited_points, str))
        {
          // Word found
          words.push_back(word);
          continue;
        }
      }

  // Print the words found
  for(string word : words)
  {
    cout << word << ",";
  }
  cout << endl;


  return 0;
}

