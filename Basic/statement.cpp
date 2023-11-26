/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include <cstddef>

/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

void REM::execute(EvalState &state, Program &program) { return; }

void LET::execute(EvalState &state, Program &program) {
  state.setValue(var, exp->eval(state));
}

void PRINT::execute(EvalState &state, Program &program) {
  std::cout << exp->eval(state) << std::endl;
}

void INPUT::execute(EvalState &state, Program &program) {
  std::string input;
  std::cout << " ? ";
  getline(std::cin, input);
  while (!IsNumLegal(input)) {

    std::cout << "INVALID NUMBER\n";
    std::cout << " ? ";
    getline(std::cin, input);
  }
  const int n = stringToInteger(input);
  state.setValue(var, n);
}

void END::execute(EvalState &state, Program &program) {
  program.is_ended = true;
}

void GOTO::execute(EvalState &state, Program &program) {
  if (program.line_nums.find(line_num) == program.line_nums.end()) {
    error("LINE NUMBER ERROR");
    program.is_ended = true;
    return;
  }
  program.branch = line_num;
}

void IF::execute(EvalState &state, Program &program) {
  bool val = false;
  const int lval = lhs->eval(state), rval = rhs->eval(state);

  if (op == '>') {
    val = lval > rval;
  } else if (op == '<') {
    val = lval < rval;
  } else {
    val = lval == rval;
  }

  if (val) {
    if (program.line_nums.find(line_num) == program.line_nums.end()) {
      error("LINE NUMBER ERROR");
      program.is_ended = true;
      return;
    }
    program.branch = line_num;
  }
}

bool IsVarLegal(const std::string &var) {
  // if (var[0] >= '0' && var[0] <= '9') {
  //   return false;
  // }
  for (int i = 0; i != var.size(); ++i) {
    if (var[i] == '+' || var[i] == '-' || var[i] == '*' || var[i] == '/' ||
        var[i] == '=') {
      return false;
    }
  }
  for (int i = 0; i != KEY_WORDS.size(); ++i) {
    if (var == KEY_WORDS[i]) {
      return false;
    }
  }

  return true;
}

bool IsNumLegal(const std::string &num) {
  if (num == "-" || num == "") {
    return false;
  }

  const char &ch = num[0];

  if (ch != '-') {
    if (ch < '0' || ch > '9') {
      return false;
    }
  }

  for (int i = 1; i != num.size(); ++i) {
    if (num[i] < '0' || num[i] > '9') {
      return false;
    }
  }

  return true;
}

LET::~LET() {
  delete exp;
  exp = nullptr;
}

PRINT::~PRINT() {
  delete exp;
  exp = nullptr;
}

IF::~IF() {
  delete lhs;
  delete rhs;
  lhs = nullptr;
  rhs = nullptr;
}