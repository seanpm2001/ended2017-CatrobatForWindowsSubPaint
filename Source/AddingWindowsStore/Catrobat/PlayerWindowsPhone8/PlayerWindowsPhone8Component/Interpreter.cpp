#include "pch.h"
#include "Interpreter.h"
#include "FormulaTree.h"
#include "ProjectDaemon.h"
#include "CompassProvider.h"
#include "string"
#include <sstream>
//#include <cmath>
#include <ctime>
#include <math.h>

using namespace std;
using namespace Windows::Devices::Sensors;

Interpreter *Interpreter::__instance = NULL;

Interpreter *Interpreter::Instance()
{
    if (!__instance)
        __instance = new Interpreter();
    return __instance;
}

Interpreter::Interpreter()
{
    m_accelerometer = Windows::Devices::Sensors::Accelerometer::GetDefault();
}

double Interpreter::EvaluateFormula(FormulaTree *tree, Object *object)
{
    Type type = tree->GetType();
    switch (type)
    {
    case OPERATOR:
        return InterpretOperator(tree, object);
    case NUMBER:
        return atof(tree->Value().c_str());
    case USER_VARIABLE:
        {
            string varName = tree->Value();
            UserVariable *var = object->GetVariable(varName);
            if (var)
                return atof(var->GetValue().c_str());
            var = ProjectDaemon::Instance()->GetProject()->GetVariable(varName);
            if (var)
            {

                return atof(var->GetValue().c_str());
            }

            // TODO: Check logic here (What should we do when variable is not found)
            return 0;
         }
    case BRACKET:
        return this->EvaluateFormula(tree->GetRightChild(), object);
    case FUNCTION:
        return InterpretFunction(tree, object);
    case SENSOR:
        return ReadSensor();
    default:
        break;
    }
    // TODO: What should we do when we get a invalid tree here?
    throw "Exception in Interpreter.cpp: No such type available";
}

int Interpreter::EvaluateFormulaToInt(FormulaTree *tree, Object *object)
{
    return (int)(this->EvaluateFormula(tree, object));
}

float Interpreter::EvaluateFormulaToFloat(FormulaTree *tree, Object *object)
{
    return (float)(this->EvaluateFormula(tree, object));
}

RotationProvider* Interpreter::GetRotationProvider(FormulaTree *tree, Object *object)
{
    float rotationValue = 0.0f;
    RotationProvider* rotationProvider;

    if (tree->GetType() != SENSOR)
    {
        rotationValue = (float)(this->EvaluateFormula(tree, object));
        rotationProvider = new RotationProvider(STATIC, rotationValue);
    }
    else
    {
        rotationProvider = new RotationProvider(COMPASS, rotationValue);
    }
    
    return rotationProvider;
}

bool Interpreter::EvaluateFormulaToBool(FormulaTree *tree, Object *object)
{
    double result = this->EvaluateFormula(tree, object);

    if (result != 0)
        return true;
    else
        return false;
}

void Interpreter::ReadAcceleration()
{
    // Reading Accelerometer Data
    if (m_accelerometer != nullptr)
    {
        try
        {
            m_accReading = m_accelerometer->GetCurrentReading();
            Platform::String ^acceleration = L"Acceleration: " + "X: " + m_accReading->AccelerationX + " Y: " + m_accReading->AccelerationY + " Z: " + m_accReading->AccelerationZ;
        }
        catch (Platform::Exception^ e)
        {
            // there is a bug tracking this issue already
            // we need to remove this try\catch once the bug # 158858 hits our branch
            // For now, to make this App work, catching the exception
            // The reverting is tracked by WP8 # 159660
        }
    }
}

double Interpreter::ReadSensor()
{
    return 0.0;
}

double Interpreter::InterpretOperator(FormulaTree *tree, Object *object)
{
    FormulaTree *leftChild = tree->GetLeftChild();
    double leftValue = 0.0;
    if (tree->GetLeftChild() != NULL)
        leftValue = this->EvaluateFormula(leftChild, object);
    FormulaTree *rightChild = tree->GetRightChild();
    double rightValue = this->EvaluateFormula(rightChild, object);

    double returnValue = 0.0;

    switch (tree->GetOperator())
    {
    case Operator::PLUS:
        if (this->TestChilds(tree, Childs::LeftAndRightChild))
            returnValue = leftValue + rightValue;
        break;
    case Operator::MINUS:
        if (this->TestChilds(tree, Childs::LeftAndRightChild) ||
            this->TestChilds(tree, Childs::RightChild))
            returnValue = leftValue - rightValue;
        break;
    case Operator::MULT:
        if (this->TestChilds(tree, Childs::LeftAndRightChild))
            returnValue = leftValue * rightValue;
        break;
    case Operator::DIVIDE:
        if (this->TestChilds(tree, Childs::LeftAndRightChild))
        {
            if (rightValue == 0)
                return 0.0f;
            returnValue = leftValue / rightValue;
        }
        break;
    case Operator::POW:
        if (this->TestChilds(tree, Childs::LeftAndRightChild))
            returnValue = pow(leftValue, rightValue);
        break;
    case Operator::LOGICAL_AND:
        if (this->TestChilds(tree, Childs::LeftAndRightChild))
            returnValue = leftValue && rightValue;
        break;
    case Operator::LOGICAL_OR:
        if (this->TestChilds(tree, Childs::LeftAndRightChild))
            returnValue = leftValue || rightValue;
        break;
    case Operator::EQUAL:
        if (this->TestChilds(tree, Childs::LeftAndRightChild))
            returnValue = leftValue == rightValue;
        break;
    case Operator::NOT_EQUAL:
        if (this->TestChilds(tree, Childs::LeftAndRightChild))
            returnValue = leftValue != rightValue;
        break;
    default:
        break;
    }

    return returnValue;
}

double Interpreter::InterpretFunction(FormulaTree *tree, Object *object)
{
    double returnValue = 0.0;
    FormulaTree *leftChild = tree->GetLeftChild();
    FormulaTree *rightChild = tree->GetRightChild();
    double pi = 4.0 * std::atan(1.0);

    double leftValue = 0.0;
    if (leftChild != NULL)
        leftValue = this->EvaluateFormula(leftChild, object);
    double rightValue = 0.0;
    if (rightChild != NULL)
        rightValue = this->EvaluateFormula(rightChild, object);

    switch (tree->GetFunction())
    {
    case Function::L_TRUE:
        if (this->TestChilds(tree, Childs::NoChild))
            returnValue = 1.0;
        break;
    case Function::L_FALSE:
        if (this->TestChilds(tree, Childs::NoChild))
            returnValue = 0.0;
        break;
    case Function::SIN:
        if (this->TestChilds(tree, Childs::LeftChild))
            returnValue = sin(leftValue * pi / 180.0);
        break;
    case Function::COS:
        if (this->TestChilds(tree, Childs::LeftChild))
            returnValue = this->CalculateCosinus(leftValue);
        break;
    case Function::TAN:
        if (this->TestChilds(tree, Childs::LeftChild))
            returnValue = tan(leftValue * pi / 180.0);
        break;
    case Function::LN:
        if (this->TestChilds(tree, Childs::LeftChild))
        {
            if (leftValue <= 0)
                returnValue = -1.0; //TODO: exception!
            else
                returnValue = log(leftValue);
        }
        break;
    case Function::LOG:
        if (this->TestChilds(tree, Childs::LeftChild))
        {
            if (leftValue <= 0)
                returnValue = -1.0; //TODO: exception!
            else
                returnValue = log10(leftValue);
        }
        break;
    case Function::SQRT:
        if (this->TestChilds(tree, Childs::LeftChild))
            returnValue = sqrt(leftValue);
        break;
    case Function::RAND:
        if (this->TestChilds(tree, Childs::LeftAndRightChild))
            returnValue = this->CalculateRand(leftValue, rightValue);
        break;
    case Function::ABS:
        if (this->TestChilds(tree, Childs::LeftChild))
            returnValue = abs(leftValue);
        break;
    case Function::ROUND:
        if (this->TestChilds(tree, Childs::LeftChild))
            returnValue = this->RoundDoubleToInt(leftValue);
        break;
    case Function::PI:
        if (this->TestChilds(tree, Childs::NoChild))
            returnValue = 4.0 * std::atan(1.0);
        break;
    case Function::MOD:
        if (this->TestChilds(tree, Childs::LeftAndRightChild))
            returnValue = this->CalculateModulo(leftValue, rightValue);
        break;
    case Function::ARCSIN:
        if (this->TestChilds(tree, Childs::LeftChild))
            returnValue = asin(leftValue) * 180 / pi;
        break;
    case Function::ARCCOS:
        if (this->TestChilds(tree, Childs::LeftChild))
            returnValue = acos(leftValue) * 180 / pi;
        break;
    case Function::ARCTAN:
        if (this->TestChilds(tree, Childs::LeftChild))
            returnValue = atan(leftValue) * 180 / pi;
        break;
    case Function::EXP:
        if (this->TestChilds(tree, Childs::LeftChild))
            returnValue = exp(leftValue);
        break;
    case Function::MAX:
        if (this->TestChilds(tree, Childs::LeftAndRightChild))
            returnValue = this->CalculateMax(leftValue, rightValue);
        break;
    case Function::MIN:
        if (this->TestChilds(tree, Childs::LeftAndRightChild))
            returnValue = this->CalculateMin(leftValue, rightValue);
        break;
    default:
        break;
    }

    return returnValue;
}

bool Interpreter::TestChilds(FormulaTree *tree, Childs childs)
{
    bool returnValue = false;

    switch (childs)
    {
    case LeftChild:
        if (tree->GetLeftChild() != NULL && tree->GetRightChild() == NULL)
            returnValue = true;
        break;
    case RightChild:
        if (tree->GetLeftChild() == NULL && tree->GetRightChild() != NULL)
            returnValue = true;
        break;
    case LeftAndRightChild:
        if (tree->GetLeftChild() != NULL && tree->GetRightChild() != NULL)
            returnValue = true;
        break;
    case NoChild:
        if (tree->GetLeftChild() == NULL && tree->GetRightChild() == NULL)
            returnValue = true;
        break;
    default:
        returnValue = false;
        break;
    }

    return returnValue;
}

double Interpreter::CalculateMax(double value1, double value2)
{
    if (value1 < value2)
        return value2;
    else
        return value1;
}

double Interpreter::CalculateMin(double value1, double value2)
{
    if (value1 < value2)
        return value1;
    else
        return value2;
}

double Interpreter::CalculateRand(double value1, double value2)
{
    double min = this->CalculateMin(value1, value2);
    double max = this->CalculateMax(value1, value2);

    double diff = max - min;

    srand(time(NULL));
    double percentOfMaxValue = (double)rand() / RAND_MAX;
    double random_num = min + percentOfMaxValue * diff;
    return random_num;
}

double Interpreter::CalculateModulo(double dividend, double divisor)
{
    int integerQuotient = (int)(dividend / divisor);
    if ((dividend < 0 || divisor < 0) && !(dividend < 0 && divisor < 0) && fmod(dividend, divisor) != 0)
        integerQuotient -= 1;
    double returnValue = dividend - (double)(divisor * integerQuotient);

    return returnValue;
}

double Interpreter::CalculateCosinus(double degree)
{
    double pi = 4.0 * std::atan(1.0);

    if (this->CalculateModulo(degree + 90.0, 180) == 0.0)
        return 0.0;
    else
        return cos(degree * pi / 180.0);
}

double Interpreter::RoundDoubleToInt(double value)
{
    double roundedNumber;

    if (value >= 0)
        roundedNumber = (int)(value + 0.5);
    else
        roundedNumber = (int)(value - 0.5);
    return roundedNumber;
}