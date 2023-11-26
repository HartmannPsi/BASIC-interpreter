/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include "Utils/error.hpp"
#include "Utils/strlib.hpp"
#include "Utils/tokenScanner.hpp"
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "statement.hpp"
#include <cctype>
#include <iostream>
#include <string>

/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
  EvalState state;
  Program program;
  // cout << "Stub implementation of BASIC" << endl;
  while (true) {
    try {
      std::string input;
      getline(std::cin, input);
      if (input.empty())
        return 0;
      processLine(input, program, state);
    } catch (ErrorException &ex) {
      std::cout << ex.getMessage() << std::endl;
    }
  }
  return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
  TokenScanner scanner;
  scanner.ignoreWhitespace();
  scanner.scanNumbers();
  scanner.setInput(line);
  const auto tok = scanner.nextToken();
  if (tok[0] >= '0' && tok[0] <= '9') { // Line Numbers
    if (IsNumLegal(tok)) {
      const auto line_num = stringToInteger(tok);
      if (scanner.hasMoreTokens()) {

        program.addSourceLine(line_num, line);
      } else {

        program.removeSourceLine(line_num);
      }
    } else {
      error("SYNTAX ERROR");
    }
  } else {
    Statement *tmp = nullptr;
    bool err = false;

    try {
      if (tok == "LET") {

        tmp = new LET;
        LET *const p = dynamic_cast<LET *>(tmp);
        p->var = scanner.nextToken();
        const std::string op = scanner.nextToken();
        if (op != "=") {
          err = true;
        }
        if (!scanner.hasMoreTokens()) {
          err = true;
        } else {
          p->exp = readE(scanner); // Memory Leak!!!
        }
        if (!IsVarLegal(p->var) || scanner.hasMoreTokens()) {
          err = true;
        }

        if (err) {
          delete tmp;
          tmp = nullptr;
          error("SYNTAX ERROR");
        } else {
          tmp->execute(state, program);
          delete tmp;
          tmp = nullptr;
        }

      } else if (tok == "PRINT") {

        tmp = new PRINT;
        PRINT *const p = dynamic_cast<PRINT *>(tmp);
        if (!scanner.hasMoreTokens()) {
          err = true;
        } else {
          p->exp = readE(scanner);
        }
        if (scanner.hasMoreTokens()) {
          err = true;
        }

        if (err) {
          delete tmp;
          tmp = nullptr;
          error("SYNTAX ERROR");
        } else {
          tmp->execute(state, program);
          delete tmp;
          tmp = nullptr;
        }

      } else if (tok == "INPUT") {

        tmp = new INPUT;
        INPUT *const p = dynamic_cast<INPUT *>(tmp);
        p->var = scanner.nextToken();
        if (!IsVarLegal(p->var) || scanner.hasMoreTokens()) {
          err = true;
        }

        if (err) {
          delete tmp;
          tmp = nullptr;
          error("SYNTAX ERROR");
        } else {
          tmp->execute(state, program);
          delete tmp;
          tmp = nullptr;
        }

      } else if (!scanner.hasMoreTokens()) {
        if (tok == "RUN") {

          int line_num = program.getFirstLineNumber();
          while (line_num != -1 && !program.is_ended) {
            program.line_parses[line_num]->execute(state, program);
            line_num = program.getNextLineNumber(line_num);
          }
          program.is_ended = false;
        } else if (tok == "LIST") {

          for (auto i = program.line_nums.begin(); i != program.line_nums.end();
               ++i) {
            std::cout << program.line_texts[*i] << std::endl;
          }

        } else if (tok == "CLEAR") {

          program.clear();
          state.Clear();
        } else if (tok == "QUIT") {

          exit(0);
        } else if (tok == "HELP") {

          std::cout << "For More Instructions:" << std::endl;
          std::cout << "https://youtu.be/dQw4w9WgXcQ?si=jz7pfxvb4iynCjIq/"
                    << std::endl
                    << "https://www.bilibili.com/video/BV1GJ411x7h7/"
                    << std::endl;
        } else {

          error("SYNTAX ERROR");
        }
      } else {

        error("SYNTAX ERROR");
      }
    } catch (ErrorException &ex) {
      err = true;
      delete tmp;
      std::cout << ex.getMessage() << std::endl;
    }
  }

  // todo
}
