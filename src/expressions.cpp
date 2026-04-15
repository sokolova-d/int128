#include "expressions.hpp"

std::ostream& operator<<(std::ostream& os, const Expression& e) {
    e.print(os);
    return os;
}

Const::Const(Int128 v) : value(v) {
}

Int128 Const::eval(const std::map<std::string, Int128>&) const {
    return value;
}

Expression* Const::clone() const {
    return new Const(value);
}

void Const::print(std::ostream& os) const {
    os << value;
}

Variable::Variable(std::string n) : name(std::move(n)) {
}

Int128 Variable::eval(const std::map<std::string, Int128>& vars) const {
    return vars.at(name);
}

Expression* Variable::clone() const {
    return new Variable(name);
}

void Variable::print(std::ostream& os) const {
    os << name;
}

BinaryOp::BinaryOp(const Expression& l, const Expression& r)
    : left(l.clone()), right(r.clone()) {
}

Int128 Add::eval(const std::map<std::string, Int128>& vars) const {
    return left->eval(vars) + right->eval(vars);
}

Expression* Add::clone() const {
    return new Add(*left, *right);
}

void Add::print(std::ostream& os) const {
    os << "(" << *left << " + " << *right << ")";
}

Int128 Subtract::eval(const std::map<std::string, Int128>& vars) const {
    return left->eval(vars) - right->eval(vars);
}

Expression* Subtract::clone() const {
    return new Subtract(*left, *right);
}

void Subtract::print(std::ostream& os) const {
    os << "(" << *left << " - " << *right << ")";
}

Int128 Multiply::eval(const std::map<std::string, Int128>& vars) const {
    return left->eval(vars) * right->eval(vars);
}

Expression* Multiply::clone() const {
    return new Multiply(*left, *right);
}

void Multiply::print(std::ostream& os) const {
    os << "(" << *left << " * " << *right << ")";
}

Int128 Divide::eval(const std::map<std::string, Int128>& vars) const {
    return left->eval(vars) / right->eval(vars);
}

Expression* Divide::clone() const {
    return new Divide(*left, *right);
}

void Divide::print(std::ostream& os) const {
    os << "(" << *left << " / " << *right << ")";
}

Negate::Negate(const Expression& e) : expr(e.clone()) {
}

Int128 Negate::eval(const std::map<std::string, Int128>& vars) const {
    return -expr->eval(vars);
}

Expression* Negate::clone() const {
    return new Negate(*expr);
}

void Negate::print(std::ostream& os) const {
    os << "(-" << *expr << ")";
}

Negate operator-(const Expression& e) {
    return Negate(e);
}

Add operator+(const Expression& l, const Expression& r) {
    return Add(l, r);
}

Subtract operator-(const Expression& l, const Expression& r) {
    return Subtract(l, r);
}

Multiply operator*(const Expression& l, const Expression& r) {
    return Multiply(l, r);
}

Divide operator/(const Expression& l, const Expression& r) {
    return Divide(l, r);
}