/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "Utils/error.hpp"
#include "Utils/strlib.hpp"
#include "Utils/tokenScanner.hpp"
#include "evalstate.hpp"
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include <sstream>
#include <string>
#include <vector>

class Program;

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:
  /*
   * Constructor: Statement
   * ----------------------
   * The base class constructor is empty.  Each subclass must provide
   * its own constructor.
   */

  Statement();

  /*
   * Destructor: ~Statement
   * Usage: delete stmt;
   * -------------------
   * The destructor deallocates the storage for this expression.
   * It must be declared virtual to ensure that the correct subclass
   * destructor is called when deleting a statement.
   */

  virtual ~Statement();

  /*
   * Method: execute
   * Usage: stmt->execute(state);
   * ----------------------------
   * This method executes a BASIC statement.  Each of the subclasses
   * defines its own execute method that implements the necessary
   * operations.  As was true for the expression evaluator, this
   * method takes an EvalState object for looking up variables or
   * controlling the operation of the interpreter.
   */

  virtual void execute(EvalState &state, Program &program) = 0;
};

class REM : public Statement {
public:
  virtual void execute(EvalState &state, Program &program);
};

class LET : public Statement {
public:
  std::string var;
  Expression *exp;
  virtual void execute(EvalState &state, Program &program);
  virtual ~LET();
};

class PRINT : public Statement {
public:
  Expression *exp;
  virtual void execute(EvalState &state, Program &program);
  virtual ~PRINT();
};

class INPUT : public Statement {
public:
  std::string var;
  virtual void execute(EvalState &state, Program &program);
};

class END : public Statement {
public:
  virtual void execute(EvalState &state, Program &program);
};

class GOTO : public Statement {
public:
  int line_num;
  virtual void execute(EvalState &state, Program &program);
};

class IF : public Statement {
public:
  int line_num;
  Expression *lhs;
  char op;
  Expression *rhs;
  virtual void execute(EvalState &state, Program &program);
  virtual ~IF();
};

bool IsVarLegal(const std::string &var);

bool IsNumLegal(const std::string &num);

const std::vector<std::string> KEY_WORDS = {
    "REM",  "LET", "PRINT", "INPUT", "END",  "GOTO", "IF",
    "THEN", "RUN", "LIST",  "CLEAR", "QUIT", "HELP"};

#endif
