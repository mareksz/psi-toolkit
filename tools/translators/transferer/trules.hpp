#pragma once

/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#include "zvalue.hpp"
#include "tscript_visitor.hpp"

/**
 * Visitor support.
 */
#ifdef VISITOR_SUPPORT
#error Macro VISITOR_SUPPORT was defined somewhere else.
#else
#define VISITOR_SUPPORT \
    virtual void acceptVisitor(TScriptVisitor* visitor) { \
        assert(NULL!=visitor); \
        visitor->visit(this); \
    }
#endif //VISITOR_SUPPORT

/* Common namespace for the whole TraMIL */
namespace tmil {

class Visitor;

/**
 * Parent class for all other classes, for type identification and
 * chaining facility.
 */
class Statement {
public:

        /**
         * Type identifier.
         *
         * For expressions:
         *   EXPRESSION       - <code>Expression</code> class
         *   ATOM             - <code>AtomicExpression</code> class
         *   SUBR_EXPRESSION  - <code>SubrExpression</code> class
         *   VARIABLE         - <code>VariableExpression</code> class
         *   PLREF            - <code>PlrefExpression</code> class
         *   SREF             - <code>SrefExpression</code> class
         *   ATTRIBUTE        - <code>AttributeExpression</code> class
         *
         * For instructions:
         *   INSTRUCTION        - <code>Instruction</code>
         *   SIMPLE_INSTRUCTION - <code>SimpleInstruction</code> class
         *   INSTRUCTION_IF     - <code>InstructionIf</code> class
         *   INSTRUCTION_BLOCK  - <code>InstructionBlock</code> class
         *   TRANSFER_SPEC      - <code>TransferSpec</code> class
         *   SUBROUTINE         - <code>Subroutine</code> class
         *   FOREACH            - <code>ForeachLoop</code> class
         *   SREF_SETTING       - <code>SrefSetting</code> class
         *   ORDER_SETTING      - <code>OrderSetting</code> class
         *   RETURN_INSTRUCTION - <code>ReturnInstruction</code> class
         *
         * @see type
         */
        enum EStatementType
        {
                //expressions
                EXPRESSION,
                ATOM,
                SUBR_EXPRESSION,
                VARIABLE,
                PLREF,
                SREF,
                ATTRIBUTE,
                //instructions
                INSTRUCTION,
                SIMPLE_INSTRUCTION,
                INSTRUCTION_IF,
                INSTRUCTION_BLOCK,
                TRANSFER_SPEC,
                SUBROUTINE,
                FOREACH,
                SREF_SETTING,
                ORDER_SETTING,
                RETURN_INSTRUCTION
        };

        /**
         * RTTI fuction. Returns the type of this object --- an identifier
         * of a subclass.
         *
         * @return        The type identifier of this object.
         */
        EStatementType getType() { return type; }

        /**
         * Sets the origin of this expression. Used for debug.
         *
         * @param        fileName        The name of the file that contains
         *                               this expression.
         * @param        lineNumber        The number of the line where this
         *                                 expression starts.
         */
        void setOrigin(zsymbol* fileName, int lineNumber) {
                origin_file_name = fileName;
                origin_file_line_number = lineNumber;
        }

        /**
         * The previous statement in chain.
         * Used to keep substatements together.
         *
         * @return        The previous <code>Statement</code> in chain.
         *                NULL if it is the first substatement.
         */
        Statement* getPreviousStatement() { return prev_statement; }

        /**
         * The next statement in chain.
         * Used to keep subexpresions together.
         *
         * @return        The next <code>Statement</code> in chain.
         *                NULL if it is the last substatements.
         */
        Statement* getNextStatement() { return next_statement; }

        /**
         * Adds the chain containing <code>aStatement</code> to this chain.
         * The chain containing <code>aStatement</code> is placed behind
         * this statement in this chain. If this statement is not the last
         * one in chain, statement that are next to it, will be connected
         * next to the end of the chain containing <code>aStatement</code>.
         *
         * @param        aStatement        An statement representing a chain
         *                                   to be inserted behind this statement.
         */
        void insertChainBehind(Statement* aStatement);

        /**
         * Adds the chain containing <code>aStatement</code> to this chain.
         * The chain containing <code>aStatement</code> is placed behind
         * the last statement in this chain.
         *
         * @param        aStatement        An statement representing a chain
         *                                   to be inserted at the end of
         *                                   this chain.
         */
        void insertChainAtTheEnd(Statement* aStatement) {
                getLastChainElement()->insertChainBehind(aStatement);
        }

        /**
         * Delete the whole chain including this statement. If this statement
         * is not a part of a chain, only this statement will be deleted.
         */
        void deleteChain();

        /**
         * Looks for the first <code>Statement</code> in a chain.
         *
         * @return        The first element in the chain. May be this.
         */
        Statement* getFirstChainElement();

        /**
         * Looks for the last <code>Statement</code> in a chain.
         *
         * @return        The last element in the chain. May be this.
         */
        Statement* getLastChainElement();

    /**
     * Visitor support.
     */
    virtual void acceptVisitor(TScriptVisitor* visitor) = 0;

        virtual ~Statement();

protected:
        Statement(EStatementType aType):
                type(aType),
                origin_file_name(NULL),
                origin_file_line_number(-1),
                next_statement(NULL),
                prev_statement(NULL)
        {}

        /**
         * Checks if <code>aStatement</code> is in the same chain.
         *
         * @param        aStatement        A chain element.
         *
         * @return        1        if <code>aStatement</code> is in
         *                         the same chain,
         *                0        if it is not.
         */
        int inThisChain(Statement* anStatement);

private:
        /**
         * Manual RTTI.
         */
        EStatementType type;

        /**
         * The name of the script file that contains this statement.
         */
        zsymbol* origin_file_name;

        /**
         * The line number in the script file that contains this statement.
         */
        int origin_file_line_number;

        /**
         * Next <code>Statement</code> in chain.
         */
        Statement* next_statement;

        /**
         * Previous <code>Statement</code> in chain.
         */
        Statement* prev_statement;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * Just a common base-class.
 */
class Expression : public Statement {
public:
    VISITOR_SUPPORT

        virtual ~Expression() {}

        static Expression* createEmptyExpression() {
                return new Expression(EXPRESSION);
        }

protected:
        Expression(EStatementType aType):Statement(aType) {}

};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * A representation of an atomic expression.
 * This can be: a number, an atom (string), an attribute, a variable.
 * The holder for the value is <code>zvalue</code> structure.
 */
class AtomicExpression: public Expression {
public:
    VISITOR_SUPPORT

        AtomicExpression(zvalue a_value):Expression(ATOM),
                value(a_value)
        {}

        virtual ~AtomicExpression();

        /**
         * Value holder.
         */
        zvalue value;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * A representation of a variable.
 */
class VariableExpression: public Expression {
public:
    VISITOR_SUPPORT

        VariableExpression(zsymbol* a_name):Expression(VARIABLE),
                name(a_name)
        {}

        virtual ~VariableExpression();

        /**
         * Variable name.
         */
        zsymbol* name;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * A representation of a plref.
 */
class PlrefExpression: public Expression {
public:
    VISITOR_SUPPORT

        PlrefExpression(zsymbol* a_plref):Expression(PLREF),
                plref(a_plref)
        {}

        virtual ~PlrefExpression();

        zsymbol* plref;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * A representation of a sref.
 */
class SrefExpression: public Expression {
public:
    VISITOR_SUPPORT

        SrefExpression(zsymbol* a_sref):Expression(SREF),
                sref(a_sref)
        {}

        virtual ~SrefExpression();

        zsymbol* sref;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * A representation of a attribute.
 */
class AttributeExpression: public Expression {
public:
    VISITOR_SUPPORT

        AttributeExpression(zsymbol* an_attribute):Expression(ATTRIBUTE),
                attribute(an_attribute)
        {}

        virtual ~AttributeExpression();

        zsymbol* attribute;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * Class for expression that can be evaluated.
 */
class SubrExpression: public Expression {
public:
    VISITOR_SUPPORT

        SubrExpression(zsymbol* a_subr_name, Expression* a_chain_start_expression):
                Expression(SUBR_EXPRESSION),
                chain_start_expression(a_chain_start_expression),
                subr_name(a_subr_name)
        {}

        virtual ~SubrExpression();

        /**
         * The first subexpression.
         */
        Expression* chain_start_expression;

        /**
         * Subroutine name (could be an operator, e.g. "+")
         */
        zsymbol* subr_name;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * Just a common base-class.
 */
class Instruction : public Statement {
public:
    VISITOR_SUPPORT

        virtual ~Instruction() {}

        static Instruction* createEmptyInstruction() {
                return new Instruction(INSTRUCTION);
        }

protected:
        Instruction(EStatementType a_type): Statement(a_type) {}
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

class SimpleInstruction: public Instruction {
public:
    VISITOR_SUPPORT

        SimpleInstruction(Expression* anExpression):
                Instruction(SIMPLE_INSTRUCTION),
                expression(anExpression)
        { }

        virtual ~SimpleInstruction();

        Expression* expression;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

class InstructionIf: public Instruction {
public:
    VISITOR_SUPPORT

        InstructionIf(Expression* aCondition,
                        Instruction* aOntrueInstruction,
                        Instruction* aOnfalseInstruction=NULL):
                Instruction(INSTRUCTION_IF),
                condition(aCondition),
                ontrue_instruction(aOntrueInstruction),
                onfalse_instruction(aOnfalseInstruction)
        {}

        virtual ~InstructionIf();

        Expression* condition;

        /**
         * A single instruction or a block. Nota a chain.
         */
        Instruction* ontrue_instruction;

        /**
         * A single instruction or a block. Nota a chain.
         */
        Instruction* onfalse_instruction;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

class InstructionBlock: public Instruction {
public:
    VISITOR_SUPPORT

        /**
         * An object of this class holds instructions grouped in a block.
         * Normaly you will have to pass a chain of instructions as
         * <code>anInstructionChain</code>. So you do have to pass a
         * pointer to any <code>Instruction</code> in the chain.
         *
         * @param   anInstructionChain  An element of a chain of
         *                              instructions in the block.
         */
        InstructionBlock(Instruction* aInstructionChain):
                Instruction(INSTRUCTION_BLOCK),
                instructionChain(aInstructionChain)
        {}

        virtual ~InstructionBlock();

        Instruction* instructionChain;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

class TransferSpecHash;

class TransferSpec: public Instruction {
public:
    VISITOR_SUPPORT

        TransferSpec(zobjects_holder* holder, zsymbol* a_src_cat, zsymbol* a_src_label,
                        zsymbol* a_target_cat, zsymbol* a_target_label,
                        Instruction* a_to_do);

        virtual ~TransferSpec();

        zsymbol* src_cat;
        zsymbol* src_label;
        zsymbol* target_cat;
        zsymbol* target_label;
        Instruction* to_do;
    TransferSpecHash* local_transfer_spec;
};

class TransferSpecHash {
    zhash* hash;

    zpair* genKey(zobjects_holder* holder, zsymbol* cat, zsymbol* label) {
        assert(NULL!=cat || NULL!=label);
        return zpair::generate(holder, cat, label);
    }

    zpair* genVal(zobjects_holder* holder, TransferSpec* transfer_spec) {
        assert(NULL!=transfer_spec);
        return zpair::generate(
            holder,
            genKey(holder, transfer_spec->src_cat, transfer_spec->src_label),
            transfer_spec);
    }

public:
    TransferSpecHash(zobjects_holder* holder) {
        hash=zhash::generate(
            holder,
            1024,
            pair_key_key_hash_function,
            pair_key_val_hash_function,
            pair_key_identity_function);
    }
    ~TransferSpecHash();

    char* add(zobjects_holder* holder, TransferSpec* transfer_spec) {
        char error[200]; error[0]=0;

        zpair* zp = genVal(holder, transfer_spec);
        zvalue zv = hash->get(zp->getFirst());
        if (!NULLP(zv)) {
#undef print_name
#define print_name(ts) \
    strcat(error, (NULL==ts->src_cat)?"":ts->src_cat->get_string() ); \
    strcat(error, ":" ); \
    strcat(error, (NULL==ts->src_label)?"":ts->src_label->get_string() ); \
    strcat(error, " => " ); \
    strcat(error, (NULL==ts->target_cat)?"":ts->target_cat->get_string() ); \
    strcat(error, ":" ); \
    strcat(error, (NULL==ts->target_label)?"":ts->target_label->get_string() )
            TransferSpec* ts = (TransferSpec*)(ZPAIRC(zv)->getSecond());
            print_name(ts);
            strcat(error, " has been allready hashed");
            if ( transfer_spec!=ts ) {
                //bad keys, hash error??? or just...
                strcat(error, " and that is why ");
                print_name(transfer_spec);
                strcat(error, " can not be hashed");
            }
#undef print_name
        } else {
            hash->put(zp);
        }

        char* result = NULL;
        if (0!=error[0]) {
            result = new char[strlen(error)+1];
            strcpy(result, error);
        }
        return result;
    }

    TransferSpec* find(zobjects_holder* holder, zsymbol* cat, zsymbol* label) {
        if (NULLP(cat) && NULLP(label))
            return NULL;
        TransferSpec* transfer_spec = NULL;
        zvalue zv = hash->get(genKey(holder, cat, label));
        if (!NULLP(zv)) transfer_spec=(TransferSpec*)(ZPAIRC(zv)->getSecond());
        return transfer_spec;
    }

    char* to_string()
          {
            return zvalue_to_string(hash);
          }

};


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

class Subroutine: public Instruction
{
public:
    VISITOR_SUPPORT

        Subroutine(zsymbol* a_cat, zsymbol* a_name,
                        zvector* a_args, Instruction* a_instruction):
                Instruction(SUBROUTINE),
                cat(a_cat),
                name(a_name),
                args(a_args),
                instruction(a_instruction)
        {}

        virtual ~Subroutine();

        /**
         * <code>cat</code> can be NULL
         */
        zsymbol* cat;

        zsymbol* name;

        /**
         * Vector of zsymbols.
         */
        zvector* args;

        Instruction* instruction;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

class ForeachLoop: public Instruction
{
public:
    VISITOR_SUPPORT

        ForeachLoop(zsymbol* a_var, zsymbol* a_sref,
                        Instruction* a_instr):
                Instruction(FOREACH),
                var(a_var),
                sref(a_sref),
                instruction(a_instr)
        {}

        virtual ~ForeachLoop();

        zsymbol* var;
        zsymbol* sref;

        Instruction* instruction;
};


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

class SrefSetting: public Instruction
{
public:
    VISITOR_SUPPORT

        SrefSetting(zsymbol* a_sref, Expression* a_expr):
                Instruction(SREF_SETTING),
                sref(a_sref),
                expr(a_expr)
        {}

        virtual ~SrefSetting();

        zsymbol*    sref;
        Expression* expr;
};


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * A "template" for a sentence. It describes the correct order of words.
 */
class OrderSetting: public Instruction
{
public:
    VISITOR_SUPPORT

        OrderSetting(zobjects_holder* holder, zvector* a_srefs_in_order);
        virtual ~OrderSetting();

        /**
         * Vector of zsymbols.
         */
        zvector* srefs_in_order;

        /**
         * The same information as in <code>srefs_in_order</code>.
         * Elements from <code>srefs_in_order</code> are keys (zsymbol).
         * The value for a pasticular string is its position in the
         * <code>srefs_in_order</code> vector (integer).
         */
        zhash* srefs_hashed;
};


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

class ReturnInstruction: public Instruction
{
public:
    VISITOR_SUPPORT

        ReturnInstruction(Expression* a_ret_expr):
                Instruction(RETURN_INSTRUCTION),
                ret_expr(a_ret_expr)
        { }

        virtual ~ReturnInstruction();

        Expression* ret_expr;
};

/**
 * The whole script. This is the start of the chain of subs.
 */
typedef Instruction* TScript;

}
