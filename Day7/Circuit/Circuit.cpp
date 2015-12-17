// Circuit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <cassert>

using namespace std;

// ( name | number ) -> name
// ( name | number ) ( AND | OR | LSHIFT | RSHIFT ) ( name | number ) -> name
// NOT ( name | number ) -> name

class Expression
{
public:
    virtual unsigned short GetValue() = 0;
    virtual string ToString() = 0;
    virtual void DumpPreorder() = 0;
};

//class Wire
//{
//public:
//    Wire(string name, Expression *source) : m_name(name), m_source(source) {}
//    unsigned short GetValue() { return m_source->GetValue(); }
//    void SetSource(Expression *source) { m_source = source; }
//    string ToString() { return m_source ? m_source->ToString() : "?"; }
//    Expression *GetExpression() { return m_source; }
//
//private:
//    string m_name;
//    Expression *m_source;
//};

class NotExpression : public Expression
{
public:
    NotExpression(Expression *exp) : m_exp(exp) {}
    unsigned short GetValue() override { return ~m_exp->GetValue(); }
    string ToString() override { return "(NOT " + m_exp->ToString() + ")"; }
    void DumpPreorder() override { cout << "NotExpression" << endl; }

private:
    Expression *m_exp;
};

class AndExpression : public Expression
{
public:
    AndExpression(Expression *exp1, Expression *exp2) : m_exp1(exp1), m_exp2(exp2) {}
    unsigned short GetValue() override { return m_exp1->GetValue() & m_exp2->GetValue(); }
    string ToString() override { return "(" + m_exp1->ToString() + " AND " + m_exp2->ToString() + ")"; }
    void DumpPreorder() override { cout << "AndExpression" << endl; }

private:
    Expression *m_exp1, *m_exp2;
};

class OrExpression : public Expression
{
public:
    OrExpression(Expression *exp1, Expression *exp2) : m_exp1(exp1), m_exp2(exp2) {}
    unsigned short GetValue() override { return m_exp1->GetValue() | m_exp2->GetValue(); }
    string ToString() override { return "(" + m_exp1->ToString() + " OR " + m_exp2->ToString() + ")"; }
    void DumpPreorder() override { cout << "OrExpression" << endl; }

private:
    Expression *m_exp1, *m_exp2;
};

class LShiftExpression : public Expression
{
public:
    LShiftExpression(Expression *exp1, Expression *exp2) : m_exp1(exp1), m_exp2(exp2) {}
    unsigned short GetValue() override { return m_exp1->GetValue() << m_exp2->GetValue(); }
    string ToString() override { return "(" + m_exp1->ToString() + " LSHIFT " + m_exp2->ToString() + ")"; }
    void DumpPreorder() override { cout << "LShiftExpression" << endl; }

private:
    Expression *m_exp1, *m_exp2;
};

class RShiftExpression : public Expression
{
public:
    RShiftExpression(Expression *exp1, Expression *exp2) : m_exp1(exp1), m_exp2(exp2) {}
    unsigned short GetValue() override { return m_exp1->GetValue() >> m_exp2->GetValue(); }
    string ToString() override { return "(" + m_exp1->ToString() + " RSHIFT " + m_exp2->ToString() + ")"; }
    void DumpPreorder() override { cout << "RShiftExpression" << endl; }

private:
    Expression *m_exp1, *m_exp2;
};

class ConstantExpression : public Expression
{
public:
    ConstantExpression(unsigned short value = 0) : m_value(value) {}
    unsigned short GetValue() override { return m_value; }
    string ToString() override {
        ostringstream retval;
        retval << m_value;
        return retval.str();
    }
    void DumpPreorder() override { cout << "ConstantExpression" << endl; }

private:
    unsigned short m_value;
};

//class WireExpression : public Expression
//{
//public:
//    WireExpression(Wire *op) : m_op(op) {}
//    unsigned short GetValue() override { return m_op->GetValue(); }
//    string ToString() override { return m_op->ToString(); }
//    void DumpPreorder() override { cout << "WireExpression" << endl; }
//
//private:
//    Wire *m_op;
//};

//class WireFactory
//{
//public:
//    Expression *GetWire(string name)
//    {
//        return m_wires[name];
//    }
//
//    Expression *GetOrCreateWire(string name)
//    {
//        Expression *exp = m_wires[name];
//        if (!exp)
//        {
//            exp = new Expression(name, new ConstantExpression());
//            m_wires[name] = nullptr;
//        }
//        return exp;
//    }
//
//private:
//    map<string, Expression *> m_wires;
//};

map<string, string> wires;

unsigned short Evaluate(string wire)
{
    cout << wire << endl;
    assert(wire.length());
    assert(!isdigit(wire[0]));

    string source = wires[wire];
    istringstream iss(source);

    string token, token2;
    iss >> token;

    if (token == "NOT")
    {
        iss >> token;
        return ~(isdigit(token[0]) ? atoi(token.c_str()) : Evaluate(token));
    }

    unsigned short op1 = isdigit(token[0]) ? atoi(token.c_str()) : Evaluate(token);

    if (iss >> token >> token2)
    {
        unsigned short op2 = isdigit(token2[0]) ? atoi(token2.c_str()) : Evaluate(token2);

        if (token == "AND") return op1 & op2;
        if (token == "OR") return op1 | op2;
        if (token == "LSHIFT") return op1 << op2;
        if (token == "RSHIFT") return op1 >> op2;
        assert(false);
    }
    else
    {
        return isdigit(token[0]) ? atoi(token.c_str()) : Evaluate(token);
    }

    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    ifstream f("Input.txt");
    for (string line; getline(f, line);)
    {
        auto arrow = line.find(" -> ");
        wires[line.substr(arrow + 4)] = line.substr(0, arrow);
    }

    unsigned short a = Evaluate("a");

    //{
    //    unsigned short x = Evaluate("x");
    //    unsigned short y = Evaluate("y");
    //    unsigned short d = Evaluate("d");
    //    unsigned short e = Evaluate("e");
    //    unsigned short f = Evaluate("f");
    //    unsigned short g = Evaluate("g");
    //    unsigned short h = Evaluate("h");
    //    unsigned short i = Evaluate("i");
    //}
    /*
    WireFactory wf;
    unsigned line = 0;

    ifstream f("Input.txt");

    string token1, token2, token3, token4, token5;
    while (f >> token1 >> token2 >> token3)
    {
        cout << "line " << ++line << endl;
        //{
        //    Wire *a = wf.GetWire("a");
        //    if (a)
        //        cout << "debug a = " << a->GetValue() << endl;
        //}

        if (token1 == "NOT")
        {
            cout << "NOT" << endl;

            Expression *op;
            if (isdigit(token2[0]))
            {
                op = new ConstantExpression(atoi(token2.c_str()));
            }
            else
            {
                op = new WireExpression(wf.GetOrCreateWire(token2));
            }

            f >> token4;
            wf.GetOrCreateWire(token4)->SetSource(new NotExpression(op));

            //cout << "  debug " << token4 << " = " << wf.GetOrCreateWire(token4)->ToString() << endl;
        }
        else
        {
            Expression *op1;
            if (isdigit(token1[0]))
            {
                op1 = new ConstantExpression(atoi(token1.c_str()));
            }
            else
            {
                op1 = new WireExpression(wf.GetOrCreateWire(token1));
            }

            if (token2 == "->")
            {
                cout << "STORE" << endl;
                wf.GetOrCreateWire(token3)->SetSource(op1);

                //cout << "  debug " << token3 << " = " << wf.GetOrCreateWire(token3)->ToString() << endl;
            }
            else
            {
                f >> token4 >> token5;

                Expression *op2;
                if (isdigit(token3[0]))
                {
                    op2 = new ConstantExpression(atoi(token3.c_str()));
                }
                else
                {
                    op2 = new WireExpression(wf.GetOrCreateWire(token3));
                }

                Expression *overallExpression = nullptr;
                if (token2 == "AND")
                {
                    cout << "AND" << endl;
                    overallExpression = new AndExpression(op1, op2);
                }
                else if (token2 == "OR")
                {
                    cout << "OR" << endl;
                    overallExpression = new OrExpression(op1, op2);
                }
                else if (token2 == "LSHIFT")
                {
                    cout << "LSHIFT" << endl;
                    overallExpression = new LShiftExpression(op1, op2);
                }
                else if (token2 == "RSHIFT")
                {
                    cout << "RSHIFT" << endl;
                    overallExpression = new RShiftExpression(op1, op2);
                }
                else
                {
                    assert(false);
                }

                wf.GetOrCreateWire(token5)->SetSource(overallExpression);
                //cout << "  debug " << token5 << " = " << wf.GetOrCreateWire(token5)->ToString() << endl;
            }
        }
    }

    f.close();

    //unsigned xw = wf.GetOrCreateWire("x")->GetValue();
    //unsigned yw = wf.GetOrCreateWire("y")->GetValue();
    //unsigned dw = wf.GetOrCreateWire("d")->GetValue();
    //unsigned ew = wf.GetOrCreateWire("e")->GetValue();
    //unsigned fw = wf.GetOrCreateWire("f")->GetValue();
    //unsigned gw = wf.GetOrCreateWire("g")->GetValue();
    //unsigned hw = wf.GetOrCreateWire("h")->GetValue();
    //unsigned iw = wf.GetOrCreateWire("i")->GetValue();

    //cout << "x = " << wf.GetOrCreateWire("x")->ToString() << endl;
    //cout << "y = " << wf.GetOrCreateWire("y")->ToString() << endl;
    //cout << "d = " << wf.GetOrCreateWire("d")->ToString() << endl;
    //cout << "e = " << wf.GetOrCreateWire("e")->ToString() << endl;
    //cout << "f = " << wf.GetOrCreateWire("f")->ToString() << endl;
    //cout << "g = " << wf.GetOrCreateWire("g")->ToString() << endl;
    //cout << "h = " << wf.GetOrCreateWire("h")->ToString() << endl;
    //cout << "i = " << wf.GetOrCreateWire("i")->ToString() << endl;

    wf.GetWire("a")->GetExpression()->DumpPreorder();
    unsigned a = wf.GetWire("dr")->GetValue();

    cout << a;
    return a; // 8192 is too high
*/
}

