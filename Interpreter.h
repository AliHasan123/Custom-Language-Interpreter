#ifndef UNTITLED_INTERPRETER_H
#define UNTITLED_INTERPRETER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <stack>

#include "UserFunction.h"
#include "Parser.h"

using namespace std;

class Interpreter {
    
private:
    // suggestions:
    map<string, double> variableMap; // a map of variable names to Variable values
    map<string,vector<string>> functionDefinition; // a map of name of function to string of function
    map<string,vector<string>> returnFunctionDefinition; // a map of name of a function that returns a value to the definition of the function
    map<string,vector<string>> parameterMap; // a map of name of function to a vector of parameter strings
    map<int,bool> ifMap;
    map<bool,int> elseIfMap;
    vector<bool> ifConditionVector;
    vector<bool> elseIfConditionVector;
    stack<int> depth;
    stack<bool> globalConditionStack;
    
    
public:
    
    void interpretScript(ifstream&, ofstream&);
    
    // suggestions:
    double computeInfix(string); // separate function for computing infix notation
    // Other helper functions
    bool isOperator(char c);
    bool isOperand(char c);
    double evaluate(double i,double j,char op);
    int precedenceValue(char c);
    bool precedence(char c,char d);
    string removeBlankSpaces(string s);
    string onlyCharacters(string s);
    bool isInfix(string s);
    double evaluateReturnFunction(string s, ofstream& file);
    void evaluateFunction(string s, ofstream& file);
    bool evaluateCondition(string s);
    void evaluate(ofstream& outputFile, string& lineFromFile);
    void evaluateConditionalBlock(ifstream& inputFile, ofstream& outputFile, string& s,bool b,int a);
    
};


#endif //UNTITLED_INTERPRETER_H

