/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"
#include "Utils/strlib.hpp"
#include "Utils/tokenScanner.hpp"
#include "statement.hpp"
#include <string>

Program::Program() = default;

Program::~Program() { clear(); }

void Program::clear() {

  for (auto i = line_nums.begin(); i != line_nums.end(); ++i) {
    const auto &line_num = *i;
    Statement *&ptr = line_parses[line_num];
    delete ptr;
    ptr = nullptr;
  }
  line_nums.clear();
  line_parses.clear();
  line_texts.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
  Statement *tmp = nullptr;
  TokenScanner scanner;
  scanner.ignoreWhitespace();
  scanner.scanNumbers();
  scanner.setInput(line);
  scanner.nextToken();
  const std::string type = scanner.nextToken();
  bool err = false;

  if (type == "REM") {

    tmp = new REM;
  } else if (type == "LET") {

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
      p->exp = readE(scanner);
    }
    if (!IsVarLegal(p->var) || scanner.hasMoreTokens()) {
      err = true;
    }
  } else if (type == "PRINT") {

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
  } else if (type == "INPUT") {

    tmp = new INPUT;
    INPUT *const p = dynamic_cast<INPUT *>(tmp);
    p->var = scanner.nextToken();
    if (!IsVarLegal(p->var) || scanner.hasMoreTokens()) {
      err = true;
    }
  } else if (type == "END") {

    tmp = new END;
    if (scanner.hasMoreTokens()) {
      err = true;
    }
  } else if (type == "GOTO") {

    tmp = new GOTO;
    GOTO *const p = dynamic_cast<GOTO *>(tmp);
    const std::string str = scanner.nextToken();
    if (!IsNumLegal(str) || str[0] == '-') {
      err = true;
    } else {
      p->line_num = stringToInteger(str);
    }
  } else if (type == "IF") {

    tmp = new IF;
    IF *const p = dynamic_cast<IF *>(tmp);
    p->lhs = readE(scanner, 1);
    const std::string op = scanner.nextToken();
    p->rhs = readE(scanner, 1);
    if (!(op == "=" || op == "<" || op == ">")) {

      err = true;
    }
    p->op = op[0];
    const std::string then = scanner.nextToken();
    if (then != "THEN") {
      err = true;
    }
    const std::string n = scanner.nextToken();
    // std::cout << "n = " << n << std::endl;
    if (!IsNumLegal(n) || n[0] == '-') {
      err = true;
    } else {
      p->line_num = stringToInteger(n);
    }

    if (scanner.hasMoreTokens()) {
      err = true;
    }
  } else {

    err = true;
  }

  if (err) {
    delete tmp;
    tmp = nullptr;
    error("SYNTAX ERROR");
    return;
  }

  if (line_nums.find(lineNumber) != line_nums.end()) {
    line_texts[lineNumber] = line;
    delete line_parses[lineNumber];
    line_parses[lineNumber] = tmp;
  } else {
    line_nums.emplace(lineNumber);
    line_texts.emplace(lineNumber, line);
    line_parses.emplace(lineNumber, tmp);
  }
}

void Program::removeSourceLine(int lineNumber) {
  if (line_nums.find(lineNumber) != line_nums.end()) {
    line_texts.erase(lineNumber);
    line_nums.erase(lineNumber);
    delete line_parses[lineNumber];
    line_parses.erase(lineNumber);
  }
}

std::string Program::getSourceLine(int lineNumber) {
  if (line_nums.find(lineNumber) == line_nums.end()) {
    return "";
  } else {
    return line_texts[lineNumber];
  }
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
  if (line_nums.find(lineNumber) == line_nums.end()) {
    error("SYNTAX ERROR");
    return;
  }
}

// void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
  if (line_nums.find(lineNumber) == line_nums.end()) {
    return nullptr;
  } else {
    return line_parses[lineNumber];
  }
}

int Program::getFirstLineNumber() {
  const auto i = line_nums.begin();
  if (i == line_nums.end()) {
    return -1;
  } else {
    return *i;
  }
}

int Program::getNextLineNumber(int lineNumber) {
  if (branch == -1) { // judging whether IF or GOTO works
    const auto i = ++line_nums.find(lineNumber);
    if (i == line_nums.end()) {
      return -1;
    } else {
      return *i;
    }
  } else {
    const int tmp = branch;
    branch = -1;
    return tmp;
  }
}
