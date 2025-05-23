#include <iostream>

#include <fstream>

#include <vector>

#include <sstream>

#include <map>

#include "lexanalyzer.cpp"

void RemoveDuplicates(std::vector < LexItem > & tokens);
void OrganizeTokens(std::vector < LexItem > & tokens);
void DetermineArgument(std::vector < std::string > & arguments, int index);
std::vector < std::string > CommandArguments(int argc, char ** argv);

int main(int argc, char ** argv) {
  std::vector < std::string > arguments = CommandArguments(argc, argv);

  LexItem token;
  std::ifstream file;
  file.open(arguments[0]);
  std::vector < LexItem > tokens;
  int lineNumber = 0;
  int tokenCount = 0;

  if (file.is_open()) {
    // Loop will break at error or end of file
    while (true) {
      token = getNextToken(file, lineNumber);

      if (token == DONE)
        break;
      tokenCount++;

      // Record tokens based on command line flag arguments
      if (arguments[1] == "true")
        std::cout << token << std::endl;
      else if (arguments[2] == "true" && token.GetToken() == ICONST)
        tokens.push_back(token);
      else if (arguments[3] == "true" && token.GetToken() == RCONST)
        tokens.push_back(token);
      else if (arguments[4] == "true" && token.GetToken() == SCONST)
        tokens.push_back(token);
      else if (arguments[5] == "true" && token.GetToken() == IDENT)
        tokens.push_back(token);

      if (token == ERR) {
        std::cout << "ERROR ON LINE " << token.GetLinenum() << " " << token.GetLexeme() << std::endl;
        std::exit(1);
      }
    }
    std::cout << std::endl;
    std::cout << "Lines: " << lineNumber << std::endl;
    if (tokenCount != 0)
      std::cout << "Tokens: " << tokenCount << std::endl;

    std::cout << std::endl;
    if (tokens.size() > 0) {
      if (arguments[4] == "true")
        std::cout << "STRINGS:" << std::endl;
      else if (arguments[2] == "true")
        std::cout << "INTEGERS:" << std::endl;
      else if (arguments[3] == "true")
        std::cout << "REALS:" << std::endl;
      else if (arguments[5] == "true")
        std::cout << "IDENTIFIERS: ";
    }

    // Size will be larger than 0 if command line flag arguments are given
    if (tokens.size() != 0) {
      OrganizeTokens(tokens);
      if (tokens[0].GetToken() != IDENT) {
        for (int i = 0; i < tokens.size(); i++) {
          std::cout << tokens.at(i).GetLexeme() << std::endl;
        }
      } else {
        for (int i = 0; i < tokens.size() - 1; i++) {
          std::cout << tokens.at(i).GetLexeme() << ", ";
        }
        std::cout << tokens.at(tokens.size() - 1).GetLexeme() << std::endl;
      }
    }
  } else {
    // Invalid file directory path
    std::cout << "CANNOT OPEN THE FILE " << arguments[0] << std::endl;
  }
}

/**
 * Record and organize command line arguments in a vector
 * Also responsible for catching errors when multiple files
 * or invalid argument is given.
 *
 * All arguments except file name need to start with a dash
 */
std::vector < std::string > CommandArguments(int argc, char ** argv) {
  std::vector < std::string > properties {
    "file",
    "false",
    "false",
    "false",
    "false",
    "false"
  };

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "-v")
      DetermineArgument(properties, 1);
    else if (arg == "-iconsts")
      DetermineArgument(properties, 2);
    else if (arg == "-rconsts")
      DetermineArgument(properties, 3);
    else if (arg == "-sconsts")
      DetermineArgument(properties, 4);
    else if (arg == "-ids")
      DetermineArgument(properties, 5);
    else if (arg.find(".txt") != -1) {
      if (properties[0] != "file") {
        std::cout << "ONLY ONE FILE NAME ALLOWED" << std::endl;
        std::exit(1);
      }
      properties[0] = arg;
    } else {
      std::cout << "UNRECOGNIZED FLAG " << arg << std::endl;
      std::exit(1);
    }
  }
  return properties;
}

/**
 * Remove duplicate values from a vector of Tokens to print to
 * the screen. All values that are printed have to be unique.
 */
void RemoveDuplicates(std::vector < LexItem > & tokens) {
  for (int i = 0; i < tokens.size() - 1; i++) {
    for (int j = i + 1; j < tokens.size(); j++) {
      if (tokens.at(i).GetLexeme() == tokens.at(j).GetLexeme()) {
        tokens.erase(tokens.begin() + j);
        i--;
        break;
      }
    }
  }
}

/**
 * Organize tokens within a vector in either alphabetical order
 * or numerical order depending on value.
 * Tokens must have duplicates removed and then organized to
 * be printed to the scrren.
 */
void OrganizeTokens(std::vector < LexItem > & tokens) {
  if (tokens.size() == 1)
    return;
  RemoveDuplicates(tokens);
  for (int i = 0; i < tokens.size(); i++) {
    for (int j = i; j < tokens.size(); j++) {
      LexItem remove = tokens.at(j);
      bool erase = false;
      if (tokens.at(i).GetToken() == ICONST) {
        int alpha = std::stoi(tokens.at(j).GetLexeme());
        int beta = std::stoi(tokens.at(i).GetLexeme());

        if (alpha < beta)
          erase = true;
      } else if (tokens.at(i).GetToken() == RCONST) {
        int alpha = std::stof(tokens.at(j).GetLexeme());
        int beta = std::stof(tokens.at(i).GetLexeme());

        if (alpha < beta)
          erase = true;
      } else if (tokens.at(j).GetLexeme() < tokens.at(i).GetLexeme())
        erase = true;

      if (erase) {
        tokens.erase(tokens.begin() + j);
        tokens.insert(tokens.begin() + i, remove);
      }
    }
  }
}

/**
 * Sets a single argument to true at a time. Because only a single
 * argument can be true at a time, whenever a new argument is given
 * all other elements have to be set to false.
 */
void DetermineArgument(std::vector < std::string > & arguments, int index) {
  for (int i = 1; i < arguments.size(); i++)
    arguments[i] = i == index ? "true" : "false";
}
