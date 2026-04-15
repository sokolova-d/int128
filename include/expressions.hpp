#pragma once

#include "int128.hpp"

#include <map>
#include <memory>
#include <string>

class Expression {
public:
    virtual ~Expression() = default;
    virtual Int128 eval(const std::map<std::string, Int128>& vars) const = 0;
    virtual Expression* clone() const = 0;
    virtual void print(std::ostream& os) const = 0;
};

std::ostream& operator<<(std::ostream& os, const Expression& e);

class Const : public Expression {
    Int128 value;

public:
    Const(Int128 v);
    Int128 eval(const std::map<std::string, Int128>&) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Variable : public Expression {
    std::string name;

public:
    Variable(std::string n);
    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class BinaryOp : public Expression {
protected:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

public:
    BinaryOp(const Expression& l, const Expression& r);
};

class Add : public BinaryOp {
public:
    using BinaryOp::BinaryOp;
    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Subtract : public BinaryOp {
public:
    using BinaryOp::BinaryOp;
    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Multiply : public BinaryOp {
public:
    using BinaryOp::BinaryOp;
    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Divide : public BinaryOp {
public:
    using BinaryOp::BinaryOp;
    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

class Negate : public Expression {
    std::unique_ptr<Expression> expr;

public:
    Negate(const Expression& e);
    Int128 eval(const std::map<std::string, Int128>& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;
};

Negate operator-(const Expression& e);
Add operator+(const Expression& l, const Expression& r);
Subtract operator-(const Expression& l, const Expression& r);
Multiply operator*(const Expression& l, const Expression& r);
Divide operator/(const Expression& l, const Expression& r);