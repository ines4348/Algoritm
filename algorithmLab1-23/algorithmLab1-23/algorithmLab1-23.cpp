#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <set>

/*23. Имеется  выражение  в  постфиксной  форме,   включающее
обращение к функциям SIN, COS, EXP.  Переменные  заданы  одной
строчной латинской буквой.  Запросить  значения  переменных  и
вычислить выражение. Представить  его  в  инфиксной  форме  со
скобками. Лишние скобки присутствовать не должны (11).*/

using namespace std;

enum operationPriority { EXP_OP = 3, COS_OP = 3, SIN_OP = 3, DIV_OP = 2, MUL_OP = 2, SUB_OP = 1, ADD_OP = 1, null = 0 };

struct variables
{
    string variable;
    float value;
    struct variables* next;
} *headVariables;

struct stack
{
    string dataExp;
    operationPriority op;
    float valueExp;
    struct stack* next;
    size_t positionOp;
    size_t stackSize;
} *head;

//Добавить степень, приоритет больше операций, но меньше функций

operationPriority getOperation(string expressionElement)
{
    operationPriority op;
    if (expressionElement == "EXP" || expressionElement == "exp") { op = EXP_OP; }
    else if (expressionElement == "COS" || expressionElement == "cos") { op = COS_OP; }
    else if (expressionElement == "SIN" || expressionElement == "sin") { op = SIN_OP; }
    else if (expressionElement == "/") { op = DIV_OP; }
    else if (expressionElement == "*") { op = MUL_OP; }
    else if (expressionElement == "-") { op = SUB_OP; }
    else if (expressionElement == "+") { op = ADD_OP; }
    else { op = null; }
    return op;
}

float evaluateBinaryExpression(stack& operandFirst, stack& operandSecond, string expressionElement)
{
    float value = 0;
    if (expressionElement == "/") { value = operandFirst.valueExp / operandSecond.valueExp; }
    else if (expressionElement == "*") { value = operandFirst.valueExp * operandSecond.valueExp; }
    else if (expressionElement == "-") { value = operandFirst.valueExp - operandSecond.valueExp; }
    else if (expressionElement == "+") { value = operandFirst.valueExp + operandSecond.valueExp; }
    else { value = 0; }
    return value;
}

float evaluateUnaryExpression(stack& operandFirst, string expressionElement)
{
    float value = 0;
    if (expressionElement == "EXP" || expressionElement == "exp") { value = exp(operandFirst.valueExp); }
    else if (expressionElement == "COS" || expressionElement == "cos") { value = cos(operandFirst.valueExp); }
    else if (expressionElement == "SIN" || expressionElement == "sin") { value = sin(operandFirst.valueExp); }
    else { value = 0; }
    return value;
}

bool intSigned(string s)
{
    size_t offset = 0;
    if (s[offset] == '-')
        ++offset;
    return s.find_first_not_of("0123456789", offset) == std::string::npos;
}

float returnValue(string variableName)
{
    variables* variable;
    variable = headVariables;
    float requiredElementValue = 0;
    while (variable->next != NULL || variable->variable != variableName)
    {
        variable = variable->next;
    }
    if (variable->variable == variableName)
    {
        requiredElementValue = variable->value;
    }
    return requiredElementValue;
}

void push(string expressionElement, operationPriority op, float value, size_t positionOp)
{
    stack* element;
    element = new stack;
    element->dataExp = expressionElement;
    element->op = op;
    element->valueExp = value;
    element->positionOp = positionOp;
    element->next = head;
    head = element;
}

stack* pop()
{
    stack* element;
    if (head == NULL) return NULL;
    element = head;
    head = element->next;
    return element;
}

void deleteStackElement(stack* element)
{
    free(element);
}

void deleteAllStackElement()
{
    stack* temp = head;

    while (head != NULL)
    {
        head = temp->next;
        free(temp);
    }
    head = NULL;
}

void deleteAllVariableElement()
{
    variables* temp = headVariables;

    while (headVariables->next != NULL)
    {
        headVariables = temp->next;
        free(temp);
    }
    free(headVariables);
    headVariables = NULL;
}

bool isReply(char str)
{
    bool result = false;
    if (str == 'y')
    {
        result = true;
    }
    return result;
}



int toInfix(string expression)
{
    set <string> operationBinary;
    operationBinary.insert("/");
    operationBinary.insert("*");
    operationBinary.insert("-");
    operationBinary.insert("+");

    set <string> operationUnary;
    operationUnary.insert("COS");
    operationUnary.insert("SIN");
    operationUnary.insert("EXP");
    operationUnary.insert("cos");
    operationUnary.insert("sin");
    operationUnary.insert("exp");

    set <string> variablesList;

    stack* operandFirst, * operandSecond;
    variables* currentVariable;
    operationPriority currentOperation;

    size_t currentPosition = 0;

    string tempExpression;
    string tempOperand;
    string tempChar;
    float value = 0;

    for (int i = 0; i < expression.length(); i++)
    {
        tempChar = expression[i];
        if ((tempChar == " " && tempOperand.length() != 0) || i == expression.length() - 1)
        {
            if (tempChar != " ")
            {
                tempOperand += tempChar;
            }

            if (operationBinary.find(tempOperand) != operationBinary.end())
            {
                operandSecond = pop();
                operandFirst = pop();
                if (operandSecond == nullptr || operandFirst == nullptr)
                {
                    std::cout << "Wrong expression. Error after position = " + std::to_string(currentPosition) << endl;
                    return 0;
                }
                currentOperation = getOperation(tempOperand);

                if (operandFirst->op != null && operandFirst->op < currentOperation)
                {
                    tempExpression = "(" + operandFirst->dataExp + ")" + tempOperand + operandSecond->dataExp;
                }
                else
                {
                    tempExpression = operandFirst->dataExp + tempOperand + operandSecond->dataExp;
                }

                if (operandSecond->op != null && operandSecond->op < currentOperation)
                {
                    tempExpression = "(" + operandFirst->dataExp + tempOperand + operandSecond->dataExp + ")";
                }

                value = evaluateBinaryExpression(*operandFirst, *operandSecond, tempOperand);
                currentPosition++;
                push(tempExpression, currentOperation, value, currentPosition);

                deleteStackElement(operandFirst);
                deleteStackElement(operandSecond);
                tempOperand.clear();
            }
            else if (operationUnary.find(tempOperand) != operationUnary.end())
            {
                operandFirst = pop();
                if (operandFirst == nullptr)
                {
                    std::cout << "Wrong expression. Error after position = " + std::to_string(currentPosition) << endl;
                    return 0;
                }
                currentOperation = getOperation(tempOperand);
                tempExpression = tempOperand + "(" + operandFirst->dataExp + ")";
                value = evaluateUnaryExpression(*operandFirst, tempOperand);
                currentPosition++;
                push(tempExpression, currentOperation, value, currentPosition);
                deleteStackElement(operandFirst);
                tempOperand.clear();
            }
            else
            {
                if (!intSigned(tempOperand))
                {
                    if (variablesList.find(tempOperand) == variablesList.end())
                    {
                        std::cout << "Input value " + tempOperand + " = ";
                        cin >> value;
                        variablesList.insert(tempOperand);
                        currentVariable = new variables;
                        currentVariable->variable = tempOperand;
                        currentVariable->value = value;
                        currentVariable->next = headVariables;
                        headVariables = currentVariable;
                    }
                    else
                    {
                        value = returnValue(tempOperand);
                    }
                }
                else
                {
                    value = stoi(tempOperand);
                }
                currentPosition++;
                std::cout << tempOperand << " = " << value << endl;
                push(tempOperand, null, value, currentPosition);
                tempOperand.clear();
            }
        }
        else
        {
            tempOperand += tempChar;
        }
    }

    operandFirst = pop();
    operandSecond = pop();
    if (operandSecond != nullptr)
    {
        std::cout << "Wrong expression. Too many operands." << endl;
        return 0;
    }
    else if (operandFirst != nullptr)
    {
        std::cout << operandFirst->dataExp << " = " << operandFirst->valueExp << endl;
    }
    else
    {
        std::cout << "Wrong expression." << endl;
        return 0;
    }

    variablesList.clear();
    deleteAllStackElement();
    deleteAllVariableElement();
    return 0;
}

int main()
{
    string expression;
    bool isReplay = true;

    while (isReplay)
    {
        std::ifstream inFile("1.txt");

        if (inFile.is_open())
        {
            if (getline(inFile, expression))
            {
                std::cout << expression << std::endl;
                toInfix(expression);
            }
        }
        else {
            std::cout << "File error!" << std::endl;
        }

        inFile.close();

        char ch;
        cout << "Replay? y/n" << endl;
        cin.get();
        cin.get(ch);
        isReplay = isReply(ch);
    }

    return 0;
}

