#pragma once

/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

namespace tmil {

class Statement;
class Expression;
class AtomicExpression;
class VariableExpression;
class PlrefExpression;
class SrefExpression;
class AttributeExpression;
class SubrExpression;
class Instruction;
class SimpleInstruction;
class InstructionIf;
class InstructionBlock;
class TransferSpec;
class Subroutine;
class ForeachLoop;
class SrefSetting;
class OrderSetting;
class ReturnInstruction;

typedef Instruction* TScript;

/**
 * Visitor interface ("Visitor" pattern) for trules objects.
 */
class TScriptVisitor {
public:
                  TScriptVisitor() {}
    virtual      ~TScriptVisitor() {}
    virtual void visit(Statement* e) = 0;
    virtual void visit(Expression* e) = 0;
    virtual void visit(AtomicExpression* e) = 0;
    virtual void visit(VariableExpression* e) = 0;
    virtual void visit(PlrefExpression* e) = 0;
    virtual void visit(SrefExpression* e) = 0;
    virtual void visit(AttributeExpression* e) = 0;
    virtual void visit(SubrExpression* e) = 0;
    virtual void visit(Instruction* i) = 0;
    virtual void visit(SimpleInstruction* i) = 0;
    virtual void visit(InstructionIf* i) = 0;
    virtual void visit(InstructionBlock* i) = 0;
    virtual void visit(TransferSpec* i) = 0;
    virtual void visit(Subroutine* i) = 0;
    virtual void visit(ForeachLoop* i) = 0;
    virtual void visit(SrefSetting* i) = 0;
    virtual void visit(OrderSetting* i) = 0;
    virtual void visit(ReturnInstruction* i) = 0;
};

}; //namespace tmil

#define TSCRIPT_VISITOR_DELEGATION(this_class_name, target_class, target_method) \
    class this_class_name: public TScriptVisitor { \
    public: \
                      _VisitorsDelegation(target_class& t): _target(t) {} \
        virtual      ~_VisitorsDelegation() {} \
\
        virtual void visit(Statement* e)           { _target.target_method(e); } \
        virtual void visit(Expression* e)          { _target.target_method(e); } \
        virtual void visit(AtomicExpression* e)    { _target.target_method(e); } \
        virtual void visit(VariableExpression* e)  { _target.target_method(e); } \
        virtual void visit(PlrefExpression* e)     { _target.target_method(e); } \
        virtual void visit(SrefExpression* e)      { _target.target_method(e); } \
        virtual void visit(AttributeExpression* e) { _target.target_method(e); } \
        virtual void visit(SubrExpression* e)      { _target.target_method(e); } \
        virtual void visit(Instruction* i)         { _target.target_method(i); } \
        virtual void visit(SimpleInstruction* i)   { _target.target_method(i); } \
        virtual void visit(InstructionIf* i)       { _target.target_method(i); } \
        virtual void visit(InstructionBlock* i)    { _target.target_method(i); } \
        virtual void visit(TransferSpec* i)        { _target.target_method(i); } \
        virtual void visit(Subroutine* i)          { _target.target_method(i); } \
        virtual void visit(ForeachLoop* i)         { _target.target_method(i); } \
        virtual void visit(SrefSetting* i)         { _target.target_method(i); } \
        virtual void visit(OrderSetting* i)        { _target.target_method(i); } \
        virtual void visit(ReturnInstruction* i)   { _target.target_method(i); } \
    private: \
        target_class& _target; \
    };
