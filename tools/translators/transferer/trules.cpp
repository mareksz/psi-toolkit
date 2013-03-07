/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#include "trules.hpp"
#include <iostream>

using namespace tmil;

Statement::~Statement() {
    origin_file_name = NULL; //a GC object

    //just to be sure that you don't delete a single chain element
    //to delete the whole chain use <code>Statement::deleteChain</code>
    assert(NULL==next_statement);
    assert(NULL==prev_statement);
}

void Statement::deleteChain() {
    Statement *s = prev_statement, *d = NULL;
    while (NULL!=s) {
        d = s;
        s = s->prev_statement;
        d->prev_statement = d->next_statement = NULL;
        delete d;
    }
    prev_statement = NULL;
    s = next_statement;
    while (NULL!=s) {
        d = s;
        s = s->next_statement;
        d->prev_statement = d->next_statement = NULL;
        delete d;
    }
    next_statement = NULL;
    delete this;
}

void Statement::insertChainBehind(Statement* anStatement) {
    assert(NULL != anStatement);
    assert(0 == inThisChain(anStatement)); //I want this check to be done only in a debug version.
    Statement* tmpStatement = next_statement;
    Statement* anStatementChainStart = anStatement->getFirstChainElement();
    next_statement = anStatementChainStart;
    anStatementChainStart->prev_statement = this;
    if (NULL != tmpStatement) {
        Statement* anStatementChainEnd = getLastChainElement();
        anStatementChainEnd->next_statement = tmpStatement;
        tmpStatement->prev_statement = anStatementChainEnd;
    }
}

Statement* Statement::getFirstChainElement() {
    Statement* firstElement = this;
    while (NULL != firstElement->prev_statement ) {
        firstElement = firstElement->prev_statement;
    }
    return firstElement;
}

Statement* Statement::getLastChainElement() {
    Statement* lastElement = this;
    while (NULL != lastElement->next_statement)
        lastElement = lastElement->next_statement;
    return lastElement;
}

int Statement::inThisChain(Statement* anStatement) {
    assert(NULL != anStatement);
    Statement* anElement = this;
    if (anElement == anStatement)
        return 1;
    while (NULL != anElement->prev_statement ) {
        anElement = anElement->prev_statement;
        if (anElement == anStatement)
            return 1;
    }
    anElement = this;
    while (NULL != anElement->next_statement) {
        anElement = anElement->next_statement;
        if (anElement == anStatement)
            return 1;
    }
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

AtomicExpression::~AtomicExpression() {
    value = NULL; //a GC object or instant-integer
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

VariableExpression::~VariableExpression() {
    name = NULL; //a GC object
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

PlrefExpression::~PlrefExpression() {
    plref = NULL; //a GC object
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

SrefExpression::~SrefExpression() {
    sref = NULL; //a GC object
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

AttributeExpression::~AttributeExpression() {
    attribute = NULL; //a GC object
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

SubrExpression::~SubrExpression() {
    assert(NULL!=chain_start_expression);
    chain_start_expression->deleteChain();
    subr_name = NULL;  //a GC object
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

SimpleInstruction::~SimpleInstruction() {
    assert(NULL!=expression);
    delete expression;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

InstructionIf::~InstructionIf() {
    assert(NULL!=condition);
    delete condition;
    assert(NULL!=ontrue_instruction);
    ontrue_instruction->deleteChain();
    // IF instruction can be without ELSE
    if (NULL!=onfalse_instruction) onfalse_instruction->deleteChain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

InstructionBlock::~InstructionBlock() {
    //no empty blocks allowed
    assert(NULL!=instructionChain);
    instructionChain->deleteChain();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

TransferSpec::TransferSpec(zobjects_holder* holder, zsymbol* a_src_cat, zsymbol* a_src_label,
        zsymbol* a_target_cat, zsymbol* a_target_label,
        Instruction* a_to_do):
    Instruction(TRANSFER_SPEC),
    src_cat(a_src_cat),
    src_label(a_src_label),
    target_cat(a_target_cat),
    target_label(a_target_label),
    to_do(a_to_do)
{
    assert(NULL!=to_do);

    local_transfer_spec = new TransferSpecHash(holder);

    Statement* s = to_do;
    if (Statement::INSTRUCTION_BLOCK==s->getType())
        s=(dynamic_cast<InstructionBlock*>(s))->instructionChain;

    while (s != NULL) {
        if (Statement::TRANSFER_SPEC==s->getType()) {
            char* error = local_transfer_spec->add(holder, dynamic_cast<TransferSpec*>(s));
            if (NULL!=error) {
                std::cerr << "[tmil::TransferSpec] in ";
                if (NULL!=src_cat) std::cerr << src_cat->get_string();
                std::cerr << ":";
                if (NULL!=src_label) std::cerr << src_label->get_string();
                std::cerr << " => ";
                if (NULL!=target_cat) std::cerr << target_cat->get_string();
                std::cerr << ":";
                if (NULL!=target_label) std::cerr << target_label->get_string();
                std::cerr << "\n" << error;
                delete error;
            }
        }
        s=s->getNextStatement();
    }
}

TransferSpec::~TransferSpec() {
    src_cat=NULL; //a GC object
    src_label=NULL; //a GC object
    target_cat=NULL; //a GC object
    target_label=NULL; //a GC object
    delete local_transfer_spec;
    assert(NULL!=to_do);
    delete to_do;
}

TransferSpecHash::~TransferSpecHash() {
    hash = NULL; //a GC object
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Subroutine::~Subroutine() {
    cat=NULL; //a GC object
    name=NULL; //a GC object

    //there must be a vector even if there are no arguments
    assert(NULL!=args);
    args=NULL; //a GC object
    assert(NULL!=instruction);
    delete instruction;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ForeachLoop::~ForeachLoop() {
    var=NULL; //a GC object
    sref=NULL; //a GC object
    assert(NULL!=instruction);
    delete instruction;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

SrefSetting::~SrefSetting() {
    sref=NULL; //a GC object
    assert(NULL!=expr);
    delete expr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

OrderSetting::OrderSetting(zobjects_holder* holder, zvector* a_srefs_in_order) :
    Instruction(ORDER_SETTING),
    srefs_in_order(a_srefs_in_order)
{
    int w = 0;
    assert(NULL!=srefs_in_order);
    srefs_hashed = zhash::generate(
    holder,
    16, standard_zvalue_hash_function,
    env_val_hash_function, env_identity_function);

    for (int i=0; i<srefs_in_order->getSize(); ++i)
    {
        if (ZSYMBOLP(srefs_in_order->elementAt(i)) &&
           !strcmp(ZSYMBOLC(srefs_in_order->elementAt(i))->get_string(), "<"))
            ++w;
        else if (ZSYMBOLP(srefs_in_order->elementAt(i)) &&
                !strcmp(ZSYMBOLC(srefs_in_order->elementAt(i))->get_string(), "="))
            ;
        else
        {
            zpair* zp = zpair::generate(
                holder,
                srefs_in_order->elementAt(i),
                INTEGER_TO_ZVALUE(w));
            srefs_hashed->put(zp);
        }
    }
}

OrderSetting::~OrderSetting() {
    assert(NULL!=srefs_in_order);
    srefs_in_order=NULL; //a GC object
    assert(NULL!=srefs_hashed);
    srefs_hashed=NULL; //a GC object
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ReturnInstruction::~ReturnInstruction() {
    assert(NULL!=ret_expr);
    delete ret_expr;
}
