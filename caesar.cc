#include <iostream>
#include <string>

std::string translate(std::string line, char* argument)
{ std::string result = "";
  int shift = atoi(argument);
  for (char c : line) {
    if (c >= 65 && c <= 122) {
      //hoofdletters
      if (c <= 90) {
        c += shift;
        if (c > 90)
          c -= 26;
        else if (c < 65)
          c += 26;
      }
      //kleine letters
      else if (c >= 97) {
        c += shift;
        if (c > 122)
          c -= 26;
        else if (c < 97)
          c += 26;
      }
    }
    result += c;
  }
  return result; }

int main(int argc, char *argv[])
{ std::string line;

  if(argc != 2)
  { std::cerr << "Deze functie heeft exact 1 argument nodig" << std::endl;
    return -1; }

  while(std::getline(std::cin, line))
  { std::cout << translate(line, argv[1]) << std::endl; }

  return 0; }
