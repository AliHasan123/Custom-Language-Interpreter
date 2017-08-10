#include "Interpreter.h"
#include "Parser.h"
#include<iostream>
#include<string>
#include<map>
#include<stack>
#include<vector>

using namespace std;

bool first = false;
bool elseif;
int ifCount = 0;
int elseIfCount = 0;
int elseCount = 0;
bool globalBool;
int globalCounter = 0;

double Interpreter::computeInfix(string s) {
    double finalAnswer;
    stack<double> operands;
    stack<char> operators;
    string number;
    double numbervalue;
    double variableValue;
    string variable;
    for(int i = 0; i < s.length(); i = i + 1) {
        char c = s.at(i);
        if(isOperand(c) == true || c == '.') {
            number = number + c;
            if(i != s.length() -1 ) {
                if(s.at(i+1) == '.') {}
                else if(isdigit(s.at(i+1)) == 0 && i != s.length() - 1) {
                    operands.push(std::stod(number));
                    number = "";
                }
            }
            
            else if(i == s.length() - 1) {
                operands.push(std::stod(number));
                number = "";
            }
            
        }
        else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            variable = variable + c;
            if(i != s.length() -1) {
                if(isalpha(s.at(i+1)) == 0 && i != s.length() - 1) {
                    variableValue = variableMap.find(variable)->second;
                    operands.push(variableValue);
                    variable = "";
                }
            }
            
            else if(i == s.length() - 1) {
                variableValue = variableMap.find(variable)->second;
                operands.push(variableValue);
                variable = "";
            }
            
        }
        
        else if(isOperator(c) == true) {
            if(operands.size() >= 2) {
                while(precedence(c,operators.top()) == true && operators.size() >= 1) {
                    double op2 = operands.top();
                    operands.pop();
                    double op1 = operands.top();
                    operands.pop();
                    double answer = evaluate(op1,op2,operators.top());
                    operands.push(answer);
                    operators.pop();
                    if(operators.size() == 0) {
                        break;
                    }
                }
            }
            operators.push(c);
        }
        else if(c == '(') {
            operators.push(c);
            
        }
        else if(c == ')') {
            while(operators.top() != '(') {
                double op2 = operands.top();
                operands.pop();
                double op1 = operands.top();
                operands.pop();
                double answer = evaluate(op1,op2,operators.top());
                operands.push(answer);
                operators.pop();
            }
            operators.pop();
        }
    }
    while(operands.size() != 1) {
        double op2 = operands.top();
        operands.pop();
        double op1 = operands.top();
        operands.pop();
        double answer = evaluate(op1,op2,operators.top());
        operands.push(answer);
        operators.pop();
    }
    //cout<<operands.top()<<endl;
    return operands.top();
}

bool Interpreter::isOperator(char c) {
    if(c == '+' || c == '-' || c == '*' || c == '/') {
        return true;
    }
    return false;
}

bool Interpreter::isOperand(char c) {
    if(c >= '0' && c <= '9') {
        return true;
    }
    return false;
}

double Interpreter::evaluate(double i, double j, char op) {
    double answer;
    if(op == '+') {
        answer = i + j;
    }
    else if(op == '-') {
        answer = i - j;
    }
    else if(op == '*') {
        answer = i * j;
    }
    else if(op == '/') {
        answer = i/j;
    }
    return answer;
}
bool Interpreter::precedence(char operator1,char operator2) {
    int op1 = precedenceValue(operator1);
    int op2 = precedenceValue(operator2);
    if(op1 <= op2) {
        
        return true;
    }
    else {
        return false;
    }
}

string Interpreter::removeBlankSpaces(string s) {
    string str;
    for(int i = 0; i < s.length(); i = i + 1) {
        char c = s.at(i);
        if(c != ' ') {
            str = str + c;
        }
    }
    return str;
}

string Interpreter::onlyCharacters(string s) {
    string str;
    for(int i = 0; i < s.length(); i = i + 1) {
        char c = s.at(i);
        if(isalpha(c)) {
            str = str + c;
        }
    }
    return str;
    
}

bool Interpreter::isInfix(string s) {
    for(int i = 0; i < s.length(); i = i + 1) {
        char c = s.at(i);
        if(isOperator(c) == true) {
            return true;
        }
    }
    return false;
}

bool Interpreter::evaluateCondition(string s) {
    string one = getNextSymbol(s);
    string comparator = getNextSymbol(s);
    string two = getNextSymbol(s);
    double var1 = variableMap.find(one)->second;
    double var2 = variableMap.find(two)->second;
    if(comparator == "<") {
        return (var1 < var2);
    }
    else if(comparator == ">") {
        return (var1 > var2);
    }
}

void Interpreter::evaluate(ofstream& outputFile, string& lineFromFile) {
    LINE_TYPE lineType = getLineType(lineFromFile);
    if(lineType == DEFINE_VAR) {
        string value;
        double numberValue;
        string variableName;
        int equalSign = lineFromFile.find_first_of("=");
        int firstR = lineFromFile.find_first_of("r");
        vector<string> rhslhs = tokenize(lineFromFile,"=");
        string rhs = removeBlankSpaces(rhslhs.at(1));
        string lhs = removeBlankSpaces(rhslhs.at(0));
        if(isNumber(rhs) == 1) {
            numberValue = std::stof(rhs);
            
            for(int j = 3; j < lhs.length(); j = j + 1) {
                char c = lhs.at(j);
                variableName = variableName + c;
            }
            variableMap.insert(std::pair<string,double>(variableName,numberValue));
        }
        
        else if(isInfix(rhs) == true) {
            numberValue = computeInfix(rhs);
            for(int i = 3; i < lhs.length(); i = i + 1) {
                char c = lhs.at(i);
                variableName = variableName + c;
            }
            variableMap.insert(std::pair<string,double>(variableName,numberValue));
        }
        
        else {
            for(int j = 3; j < lhs.length(); j = j + 1) {
                char c = lhs.at(j);
                variableName = variableName + c;
            }
            numberValue = evaluateReturnFunction(rhs,outputFile);
            variableMap.insert(std::pair<string,double>(variableName,numberValue));
        }
    }
    
    if(lineType == DOC_WRITE) {
        string toPrint;
        string variable;
        vector<string> quotation = tokenize(lineFromFile,"\"");
        int firstParenthesis = lineFromFile.find_first_of("(");
        vector<string> parenthesis = tokenize(lineFromFile,"(");
        if(lineFromFile.at(firstParenthesis + 1) == '\"') {
            toPrint = quotation.at(1);
            outputFile << toPrint;
        }
        else {
            variable = onlyCharacters(parenthesis.at(1));
            outputFile << (variableMap.find(variable)->second);
        }
    }
    
    if(lineType == USER_DEFINED) {
        string copy = lineFromFile;
        string value;
        double numberValue;
        string variableName;
        vector<string> rhslhs = tokenize(lineFromFile,"=");
        string line = removeBlankSpaces(lineFromFile);
        string lhs = removeBlankSpaces(rhslhs.at(0));
        if(lineFromFile.find_first_of("=") != -1) {
            string rhs = removeBlankSpaces(rhslhs.at(1));
            
            if(isNumber(rhs) == 1) {
                variableMap.erase(lhs);
                numberValue = std::stof(rhs);
                variableName = lhs;
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
            else if(isInfix(rhs) == true) {
                numberValue = computeInfix(rhs);
                variableName = lhs;
                variableMap.erase(lhs);
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
        }
        else {
            evaluateFunction(line,outputFile);
        }
    }
    
}
void Interpreter::evaluateConditionalBlock(ifstream& inputFile, ofstream& outputFile, string& lineFromFile, bool solve, int parenthesisCounter) {
    cout<<"inside "<<solve<<endl;
    while(getline(inputFile,lineFromFile)) {
        if(parenthesisCounter != 0) {
            cout<<"parenthis "<<parenthesisCounter<<endl;
            LINE_TYPE lineType = getLineType(lineFromFile);
            if(lineType == END_BLOCK) {
                parenthesisCounter = parenthesisCounter - 1;
            }
            
            if(lineType == IF) {
                parenthesisCounter = parenthesisCounter + 1;
                string condition;
                lineFromFile = removeBlankSpaces(lineFromFile);
                int openParenthesis = lineFromFile.find_first_of("(");
                int closeParenthesis = lineFromFile.find_first_of(")");
                condition = lineFromFile.substr(openParenthesis+1,closeParenthesis-openParenthesis-1);
                bool b = evaluateCondition(condition);
                int tempParenthesisCounter = parenthesisCounter - 1;
                bool add = false;
                if(b == false) {
                    ifMap.insert(std::pair<int,bool>(parenthesisCounter,false));
                    while(getline(inputFile,lineFromFile)) {
                        if(getLineType(lineFromFile) == END_BLOCK) {parenthesisCounter = parenthesisCounter - 1; add = false;}
                        if(getLineType(lineFromFile) == IF) {parenthesisCounter = parenthesisCounter + 1; add = false;}
                        if(getLineType(lineFromFile) == ELSE_IF || getLineType(lineFromFile) == ELSE) {
                            parenthesisCounter = parenthesisCounter - 1;
                            add = true;
                            if(parenthesisCounter == tempParenthesisCounter) {
                                parenthesisCounter = parenthesisCounter + 1;
                                break;
                            }
                        }
                        if(parenthesisCounter == tempParenthesisCounter) {break;}
                        if(add = true) {parenthesisCounter = parenthesisCounter + 1;}
                    }
                }
                else if(b == true) {
                    solve = true;
                    ifMap.insert(std::pair<int,bool>(parenthesisCounter,true));
                    evaluateConditionalBlock(inputFile,outputFile,lineFromFile,solve,parenthesisCounter);
                }
            }
            
            if(lineType == ELSE_IF) {
                string condition;
                lineFromFile = removeBlankSpaces(lineFromFile);
                int openParenthesis = lineFromFile.find_first_of("(");
                int closeParenthesis = lineFromFile.find_first_of(")");
                condition = lineFromFile.substr(openParenthesis+1,closeParenthesis-openParenthesis-1);
                bool b = evaluateCondition(condition);
                if(ifMap.find(parenthesisCounter)->second != true && b == true) {
                    solve = true;
                    ifMap.erase(parenthesisCounter);
                    elseIfMap.insert(std::pair<int,bool>(parenthesisCounter,true));
                    evaluateConditionalBlock(inputFile,outputFile,lineFromFile,solve,parenthesisCounter);
                }
                else {
                    solve = false;
                    elseIfMap.insert(std::pair<int,bool>(parenthesisCounter,false));
                }
                
            }
            
            if(lineType == ELSE) {
                if(elseIfMap.size() != 0) {
                    if(ifMap.find(parenthesisCounter)->second != true && elseIfMap.find(parenthesisCounter)->second != true) {
                        solve = true;
                        ifMap.erase(parenthesisCounter);
                        elseIfMap.erase(parenthesisCounter);
                        evaluateConditionalBlock(inputFile,outputFile,lineFromFile,solve,parenthesisCounter);
                    }
                    else {
                        solve = false;
                    }
                }
                else if(elseIfMap.size() == 0) {
                    if(ifMap.find(parenthesisCounter)->second != true) {
                        solve = true;
                        ifMap.erase(parenthesisCounter);
                        
                    }
                    else {
                        solve = false;
                    }
                    
                }
            }
            
            else {
                cout<<solve<<endl;
                if(solve == true) {
                    evaluate(outputFile, lineFromFile);
                }
            }
        }
        else {
            break;
        }
    }
}
double Interpreter::evaluateReturnFunction(string s, ofstream& outputFile) {
    double returnValue;
    cout<<s<<endl;
    vector<string> toDelete;
    int firstParenthesis = s.find_first_of("(");
    int closingParenthesis = s.find_first_of(")");
    cout<<closingParenthesis<<endl;
    string name = s.substr(0,firstParenthesis);
    string parameters = s.substr(firstParenthesis+1,closingParenthesis-firstParenthesis-1);
    vector<string> parametersPassed;
    if(s.find_first_of(",") != -1) {
        parametersPassed = tokenize(parameters,",");
    }
    if(s.find_first_of(",") == -1) {
        parametersPassed.push_back(parameters);
    }
    vector<string> definition = returnFunctionDefinition.find(name)->second;
    vector<string> originalParameters = parameterMap.find(name)->second;
    
    for(int i = 0; i < definition.size(); i = i + 1) {
        string line = definition.at(i);
        LINE_TYPE lineType = getLineType(line);
        
        if(lineType == DEFINE_VAR) {
            string value;
            double numberValue;
            string variableName;
            int equalSign = line.find_first_of("=");
            int firstR = line.find_first_of("r");
            vector<string> rhslhs = tokenize(line,"=");
            string rhs = removeBlankSpaces(rhslhs.at(1));
            string lhs = removeBlankSpaces(rhslhs.at(0));
            
            for(int m = 0; m < parametersPassed.size(); m = m + 1) {
                string s = originalParameters.at(m);
                double value = variableMap.find(parametersPassed.at(m))->second;
                variableMap.insert(std::pair<string,double>(s,value));
                toDelete.push_back(s);
            }
            
            if(isNumber(rhs) == 1) {
                numberValue = std::stof(rhs);
                
                for(int j = 3; j < lhs.length(); j = j + 1) {
                    char c = lhs.at(j);
                    variableName = variableName + c;
                }
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
            
            else if(isInfix(rhs) == true) {
                numberValue = computeInfix(rhs);
                for(int i = 3; i < lhs.length(); i = i + 1) {
                    char c = lhs.at(i);
                    variableName = variableName + c;
                }
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
            
            else {
                for(int j = 3; j < lhs.length(); j = j + 1) {
                    char c = lhs.at(j);
                    variableName = variableName + c;
                }
                numberValue = evaluateReturnFunction(rhs, outputFile);
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
        }
        
        if(lineType == DOC_WRITE) {
            for(int m = 0; m < parametersPassed.size(); m = m + 1) {
                string s = originalParameters.at(m);
                double value = variableMap.find(parametersPassed.at(m))->second;
                variableMap.insert(std::pair<string,double>(s,value));
            }
            
            string toPrint;
            string variable;
            vector<string> quotation = tokenize(line,"\"");
            int firstParenthesis = line.find_first_of("(");
            vector<string> parenthesis = tokenize(line,"(");
            if(line.at(firstParenthesis + 1) == '\"') {
                toPrint = quotation.at(1);
                outputFile << toPrint;
            }
            else {
                variable = onlyCharacters(parenthesis.at(1));
                outputFile << (variableMap.find(variable)->second);
            }
        }
        
        if(lineType == USER_DEFINED) {
            string copy = line;
            string value;
            double numberValue;
            string variableName;
            vector<string> rhslhs = tokenize(line,"=");
            string rhs = removeBlankSpaces(rhslhs.at(1));
            string lhs = removeBlankSpaces(rhslhs.at(0));
            
            for(int m = 0; m < parametersPassed.size(); m = m + 1) {
                string s = originalParameters.at(m);
                double value = variableMap.find(parametersPassed.at(m))->second;
                variableMap.insert(std::pair<string,double>(s,value));
                toDelete.push_back(s);
            }
            
            if(isNumber(rhs) == 1) {
                variableMap.erase(lhs);
                numberValue = std::stof(rhs);
                variableName = lhs;
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
            else if(isInfix(rhs) == true) {
                numberValue = computeInfix(rhs);
                variableName = lhs;
                variableMap.erase(lhs);
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
            else if(isInfix(rhs) == false){
                numberValue = evaluateReturnFunction(rhs, outputFile);
                variableName = lhs;
                variableMap.erase(lhs);
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
            else {
                evaluateFunction(line,outputFile);
            }
        }
        
        if (lineType == RETURN) {
            string copy = line;
            string one = getNextSymbol(copy);
            string two = getNextSymbol(copy);
            returnValue = variableMap.find(two)->second;
        }
        
    }
    return returnValue;
}

void Interpreter::evaluateFunction(string s, ofstream& outputFile) {
    double returnValue;
    vector<string> toDelete;
    int firstParenthesis = s.find_first_of("(");
    int closingParenthesis = s.find_first_of(")");
    string name = s.substr(0,firstParenthesis);
    string parameters = s.substr(firstParenthesis+1,closingParenthesis-firstParenthesis-1);
    vector<string> parametersPassed;
    if(s.find_first_of(",") != -1) {
        parametersPassed = tokenize(parameters,",");
    }
    if(s.find_first_of(",") == -1 && parameters != "") {
        parametersPassed.push_back(parameters);
    }
    vector<string> definition = functionDefinition.find(name)->second;
    vector<string> originalParameters = parameterMap.find(name)->second;
    
    for(int i = 0; i < definition.size(); i = i + 1) { //we will check for each line of the definition
        string line = definition.at(i);
        LINE_TYPE lineType = getLineType(line);
        
        if(lineType == DEFINE_VAR) {
            string value;
            double numberValue;
            string variableName;
            int equalSign = line.find_first_of("=");
            int firstR = line.find_first_of("r");
            vector<string> rhslhs = tokenize(line,"=");
            string rhs = removeBlankSpaces(rhslhs.at(1));
            string lhs = removeBlankSpaces(rhslhs.at(0));
            
            for(int m = 0; m < parametersPassed.size(); m = m + 1) {
                string s = originalParameters.at(m);
                double value = variableMap.find(parametersPassed.at(m))->second;
                variableMap.insert(std::pair<string,double>(s,value));
                toDelete.push_back(s);
            }
            
            if(isNumber(rhs) == 1) {
                numberValue = std::stof(rhs);
                
                for(int j = 3; j < lhs.length(); j = j + 1) {
                    char c = lhs.at(j);
                    variableName = variableName + c;
                }
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
            
            else if(isInfix(rhs) == true) {
                numberValue = computeInfix(rhs);
                for(int i = 3; i < lhs.length(); i = i + 1) {
                    char c = lhs.at(i);
                    variableName = variableName + c;
                }
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
            
            else { //when a function call is assigned which returns a double value to be assigned to a var
                for(int j = 3; j < lhs.length(); j = j + 1) {
                    char c = lhs.at(j);
                    variableName = variableName + c;
                }
                numberValue = evaluateReturnFunction(rhs, outputFile);
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
        }
        
        if(lineType == DOC_WRITE) {
            for(int m = 0; m < parametersPassed.size(); m = m + 1) {
                string s = originalParameters.at(m);
                double value = variableMap.find(parametersPassed.at(m))->second;
                variableMap.insert(std::pair<string,double>(s,value));
                toDelete.push_back(s);
            }
            
            string toPrint;
            string variable;
            vector<string> quotation = tokenize(line,"\"");
            int firstParenthesis = line.find_first_of("(");
            vector<string> parenthesis = tokenize(line,"(");
            if(line.at(firstParenthesis + 1) == '\"') {
                toPrint = quotation.at(1);
                outputFile << toPrint;
            }
            else {
                variable = onlyCharacters(parenthesis.at(1));
                outputFile << (variableMap.find(variable)->second);
            }
        }
        
        if(lineType == USER_DEFINED) {
            string copy = line;
            string value;
            double numberValue;
            string variableName;
            if(line.find_first_of("=") != -1) {
                vector<string> rhslhs = tokenize(line,"=");
                string rhs = removeBlankSpaces(rhslhs.at(1));
                string lhs = removeBlankSpaces(rhslhs.at(0));
                
                for(int m = 0; m < parametersPassed.size(); m = m + 1) {
                    string s = originalParameters.at(m);
                    double value = variableMap.find(parametersPassed.at(m))->second;
                    variableMap.insert(std::pair<string,double>(s,value));
                    toDelete.push_back(s);
                }
                
                if(isNumber(rhs) == 1) {
                    variableMap.erase(lhs);
                    numberValue = std::stof(rhs);
                    variableName = lhs;
                    variableMap.insert(std::pair<string,double>(variableName,numberValue));
                }
                else if(isInfix(rhs) == true) {
                    numberValue = computeInfix(rhs);
                    variableName = lhs;
                    variableMap.erase(lhs);
                    variableMap.insert(std::pair<string,double>(variableName,numberValue));
                }
                else if(isInfix(rhs) == false){
                    numberValue = evaluateReturnFunction(rhs, outputFile);
                    variableName = lhs;
                    variableMap.erase(lhs);
                    variableMap.insert(std::pair<string,double>(variableName,numberValue));
                }
            }
            else {
                evaluateFunction(removeBlankSpaces(line),outputFile);
            }
        }
        
    }

}

void Interpreter::interpretScript(ifstream& inputFile, ofstream& outputFile){
    
    string lineFromFile;
    int whichType;
    int solve = 1;
    int lineNumber = 0;
    
    while (getline(inputFile, lineFromFile))
    {
        lineNumber++;
        LINE_TYPE lineType = getLineType(lineFromFile); // Check Parser.h for the different line types
        cout << "line " << lineNumber << " is type: " << lineType << endl;
        
        if(lineType == DEFINE_VAR) {
            string value;
            double numberValue;
            string variableName;
            int equalSign = lineFromFile.find_first_of("=");
            int firstR = lineFromFile.find_first_of("r");
            vector<string> rhslhs = tokenize(lineFromFile,"=");
            string rhs = removeBlankSpaces(rhslhs.at(1));
            string lhs = removeBlankSpaces(rhslhs.at(0));
            if(isNumber(rhs) == 1) {
                numberValue = std::stof(rhs);
                
                for(int j = 3; j < lhs.length(); j = j + 1) {
                    char c = lhs.at(j);
                    variableName = variableName + c;
                }
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
            
            else if(isInfix(rhs) == true) {
                numberValue = computeInfix(rhs);
                for(int i = 3; i < lhs.length(); i = i + 1) {
                    char c = lhs.at(i);
                    variableName = variableName + c;
                }
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
            
            else {
                for(int j = 3; j < lhs.length(); j = j + 1) {
                    char c = lhs.at(j);
                    variableName = variableName + c;
                }
                numberValue = evaluateReturnFunction(rhs,outputFile);
                variableMap.insert(std::pair<string,double>(variableName,numberValue));
            }
        }
        if(lineType == DOC_WRITE) {
            string toPrint;
            string variable;
            vector<string> quotation = tokenize(lineFromFile,"\"");
            int firstParenthesis = lineFromFile.find_first_of("(");
            vector<string> parenthesis = tokenize(lineFromFile,"(");
            if(lineFromFile.at(firstParenthesis + 1) == '\"') {
                toPrint = quotation.at(1);
                outputFile << toPrint;
            }
            else {
                variable = onlyCharacters(parenthesis.at(1));
                outputFile << (variableMap.find(variable)->second);
            }
        }
        
        if(lineType == USER_DEFINED) {
            string copy = lineFromFile;
            string value;
            double numberValue;
            string variableName;
            vector<string> rhslhs = tokenize(lineFromFile,"=");
            string line = removeBlankSpaces(lineFromFile);
            string lhs = removeBlankSpaces(rhslhs.at(0));
            if(lineFromFile.find_first_of("=") != -1) {
                string rhs = removeBlankSpaces(rhslhs.at(1));
                
                if(isNumber(rhs) == 1) {
                    variableMap.erase(lhs);
                    numberValue = std::stof(rhs);
                    variableName = lhs;
                    variableMap.insert(std::pair<string,double>(variableName,numberValue));
                }
                else if(isInfix(rhs) == true) {
                    numberValue = computeInfix(rhs);
                    variableName = lhs;
                    variableMap.erase(lhs);
                    variableMap.insert(std::pair<string,double>(variableName,numberValue));
                }
                else if(isInfix(rhs) == false) {
                    numberValue = evaluateReturnFunction(rhs, outputFile);
                    variableName = lhs;
                    variableMap.erase(lhs);
                    variableMap.insert(std::pair<string,double>(variableName,numberValue));
                }
            }
            else {
                evaluateFunction(line,outputFile);
            }
        }
        if(lineType == FUNCTION_DEF) {
            string functionName;
            string parameterString;
            string singleParameter;
            vector<string> parameters;
            vector<string> definitionLines;
            int firstSpace = lineFromFile.find_first_of(" ");
            int openParenthesis = lineFromFile.find_first_of("(");
            int closeParenthesis = lineFromFile.find_first_of(")");
            string str = lineFromFile.substr(openParenthesis + 1, closeParenthesis-openParenthesis-1);
            parameterString = removeBlankSpaces(str);
            
            for(int i = firstSpace + 1; i < openParenthesis; i = i + 1) {
                functionName = functionName + lineFromFile.at(i);
            }
            if(str != "") {
                if(lineFromFile.find_first_of(",") != -1) {
                    parameters = tokenize(parameterString,",");
                    parameterMap.insert(std::pair<string,vector<string>>(functionName,parameters));
                }
                if(lineFromFile.find_first_of(",") == -1) {
                    parameters.push_back(parameterString);
                    parameterMap.insert(std::pair<string,vector<string>>(functionName,parameters));
                }
            }
            
            while(getline(inputFile,lineFromFile)) {
                lineNumber++;
                LINE_TYPE lineType = getLineType(lineFromFile);
                cout << "line " << lineNumber << " is type: " << lineType << endl;
                if(lineType != END_BLOCK) {
                    definitionLines.push_back(lineFromFile);
                    if(lineType == RETURN) {
                        returnFunctionDefinition.insert(std::pair<string,vector<string>>(functionName,definitionLines));
                    }
                }
                else {
                    break;
                }
                
            }
            if(returnFunctionDefinition.find(functionName) == returnFunctionDefinition.end()) {
                functionDefinition.insert(std::pair<string,vector<string>>(functionName,definitionLines));
            }
            
        }
        
        if(lineType == IF) {
            string condition;
            string s = removeBlankSpaces(lineFromFile);
            int openParenthesis = s.find_first_of("(");
            int closeParenthesis = s.find_first_of(")");
            condition = s.substr(openParenthesis+1,closeParenthesis-openParenthesis-1);
            bool b = evaluateCondition(condition);
            if(b == true) {
                int parenthesisCounter = 1;
                bool solve = true;
                cout<<"value of solve is "<<solve<<endl;
                ifMap.insert(std::pair<int,bool>(parenthesisCounter,true));
                evaluateConditionalBlock(inputFile,outputFile,lineFromFile,solve,parenthesisCounter);
                parenthesisCounter = 1;
            }
            else {
                bool solve = false;
                int parenthesisCounter = 1;
                ifMap.insert(std::pair<int,bool>(parenthesisCounter,false));
                evaluateConditionalBlock(inputFile,outputFile,lineFromFile,solve,parenthesisCounter);
                
            }
        }
        
        if(lineType == ELSE_IF) {
            elseIfCount = elseIfCount + 1;
            string condition;
            string s = removeBlankSpaces(lineFromFile);
            int openParenthesis = s.find_first_of("(");
            int closeParenthesis = s.find_first_of(")");
            condition = s.substr(openParenthesis+1,closeParenthesis-openParenthesis-1);
            bool b = evaluateCondition(condition);
            globalConditionStack.push(b);
            if(globalConditionStack.top() == false) {
                //evaluateConditionalBlock(inputFile,outputFile,lineFromFile);
            }
        }
        
        if(lineType == ELSE) {
            //evaluateConditionalBlock(inputFile,outputFile,lineFromFile);
        }
        
    }
    
    
}
