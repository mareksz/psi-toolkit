/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#include "transferer.hpp"

#include "numconversion.hpp"

#include <ctype.h>

//#include "plstring.h"

#include "utf8/utf8.h"

#include <iostream>
#define ERROR_MELDER(x)  std::cerr << x ;

#ifdef USE_LOG4CPLUS
log4cplus::Logger tmil::Transferer::logger = log4cplus::Logger::getInstance("Transferer");
#else
#define LOG4CPLUS_INFO(a1, a2)
#define LOG4CPLUS_WARN(a1, a2)
#define LOG4CPLUS_ERROR
#define LOG4CPLUS_FATAL
#define LOG_TO_STREAM(a1, a2, a3, a4)
#endif

using namespace tmil;

Transferer::Transferer(ScriptFactory* sf
                       , zobjects_holder* main_holder
#ifndef TRANSFERER_NO_SEMANTICS
                       , SemanticsHierarchy* r_sem_hierarchy
                       , DomainHierarchy* r_dsem_hierarchy
#endif
                       , zsymbolfactory* a_zsymfac
                       , bool structure_mode)
    :main_holder_(main_holder)
#ifndef TRANSFERER_NO_SEMANTICS
     , sem_hierarchy(r_sem_hierarchy)
     , dsem_hierarchy(r_dsem_hierarchy)
#endif
    , lowerCaseConverter_(
        StringCaseConverterManager::getInstance().
        getLowerCaseConverter("pl"))
    , upperCaseConverter_(
        StringCaseConverterManager::getInstance().
        getUpperCaseConverter("pl"))
 {
     my_holder_ = zvector::generate(EMPTY_ZOBJECTS_HOLDER, 32000);
     current_holder_ = 0;

    assert(NULL!=sf);
    script_factory = sf;

    if (a_zsymfac == NULL)
      z_sym_fac = new zsymbolfactory(zsymboltable::generate(main_holder_));
    else
      z_sym_fac = a_zsymfac;

    // for scripts
    name_to_script=zhash::generate(
        main_holder_, 10, standard_zvalue_hash_function, env_val_hash_function, env_identity_function);

    // for subroutines
    cat_and_name_pair_to_subroutine=zhash::generate(
        main_holder_, 1024, pair_key_key_hash_function,
        pair_key_val_hash_function, pair_key_identity_function);

    // for transferspecs
    global_transfer_spec=new TransferSpecHash(main_holder_);

    // for options
    options = zenvironment::generate(main_holder_, 8);

    sm_token_assign = z_sym_fac->get_symbol(":=");
    sm_token_arrow = z_sym_fac->get_symbol("->");
    sm_token_dot = z_sym_fac->get_symbol(".");
    sm_token_and = z_sym_fac->get_symbol("&&");
    sm_token_or = z_sym_fac->get_symbol("||");
    sm_token_bind = z_sym_fac->get_symbol("=~");
    sm_any = z_sym_fac->get_symbol("any");
    sm_default = z_sym_fac->get_symbol("default");
    sm_variant = z_sym_fac->get_symbol("variant");
    sm_surface = z_sym_fac->get_symbol("surface");
    sm_gensurface = z_sym_fac->get_symbol("gensurface");
    sm_Surf = z_sym_fac->get_symbol("Surf");
    sm_Equiv = z_sym_fac->get_symbol("Equiv");
    sm_TEquiv = z_sym_fac->get_symbol("TEquiv");
    sm_this = z_sym_fac->get_symbol("this");
    sm_out = z_sym_fac->get_symbol("out");
    sm_Eid = z_sym_fac->get_symbol("Eid");
    sm_E_returned = z_sym_fac->get_symbol("!returned");

    sm_zero_string = z_sym_fac->get_symbol("0");
    sm_LexId = z_sym_fac->get_symbol("LexId");

    sm_Nr = z_sym_fac->get_symbol("Nr");
    sm_nil = z_sym_fac->get_symbol("nil");

    sm_main_subtree_label = z_sym_fac->get_symbol("#");

    if (structure_mode)
        registerBuildIn(&Transferer::surface_smI, "surface", 0);
    else
        registerBuildIn(&Transferer::surfaceI, "surface", 0);

    registerBuildIn(&Transferer::surfaceI, "gensurface", 0);

    registerBuildIn(&Transferer::atoiI, "atoi", 1);
    registerBuildIn(&Transferer::catI, "cat", 0);
    registerBuildIn(&Transferer::clearI, "clear", 1);
    registerBuildIn(&Transferer::cut_suffixI, "cut_suffix", 2);
    registerBuildIn(&Transferer::defaultI, "default", 0);
    registerBuildIn(&Transferer::deleteI, "delete", 2);
    registerBuildIn(&Transferer::do_surfaceI, "do_surface" ,0);
    registerBuildIn(&Transferer::eqI, "=" , 2);
    registerBuildIn(&Transferer::eqI, "==", 2);
    registerBuildIn(&Transferer::eqvI, "eqv", 0);
    registerBuildIn(&Transferer::etI, "&", 2);
    registerBuildIn(&Transferer::findI, "find", 2);
    registerBuildIn(&Transferer::find_leftI, "find_left", 2);
    registerBuildIn(&Transferer::find_rightI, "find_right", 2);
    registerBuildIn(&Transferer::firstI, "first", 1);
    registerBuildIn(&Transferer::first_subtreeI, "first_subtree", 0);
    registerBuildIn(&Transferer::change_categoryI, "change_category", 1);
    registerBuildIn(&Transferer::change_labelI, "change_label", 1);
    registerBuildIn(&Transferer::getI, "get", 1);
    registerBuildIn(&Transferer::get_dvalueI, "get_dvalue", 1);
    registerBuildIn(&Transferer::get_morphoI, "get_morpho", 5);
    registerBuildIn(&Transferer::get_morpho_deI, "get_morpho_de", 5);
    registerBuildIn(&Transferer::get_morpho_ruI, "get_morpho_ru", 5);
    registerBuildIn(&Transferer::get_optionI, "get_option", 1);
    registerBuildIn(&Transferer::inheritI, "inherit", 2);
    registerBuildIn(&Transferer::insert_beforeI, "insert_before", 2);
    registerBuildIn(&Transferer::insert_afterI, "insert_after", 2);
    registerBuildIn(&Transferer::insert_leftI, "insert_left", 2);
    registerBuildIn(&Transferer::insert_rightI, "insert_right", 2);
    registerBuildIn(&Transferer::isI, "is", 2);
    registerBuildIn(&Transferer::is_upperI, "is_upper", 1);
    registerBuildIn(&Transferer::is_variantI, "is_variant", 0);
    registerBuildIn(&Transferer::intersectionI, "^", 2);
    registerBuildIn(&Transferer::labelI, "label", 0);
    registerBuildIn(&Transferer::lcI, "lc", 1);
    registerBuildIn(&Transferer::lessI, "<", 2);
    registerBuildIn(&Transferer::nounderscoresI, "nounderscores", 1);
    registerBuildIn(&Transferer::notI, "!", 1);
    registerBuildIn(&Transferer::noteqI, "!=", 2);
    registerBuildIn(&Transferer::nullI, "null", 0);
    registerBuildIn(&Transferer::okI, "ok", 1);
    registerBuildIn(&Transferer::ordnum2pltextI, "ordnum2pltext", 1);
    registerBuildIn(&Transferer::pairI, "pair", 2);
    registerBuildIn(&Transferer::parentI, "parent", 0);
    registerBuildIn(&Transferer::plusI, "+", 2);
    registerBuildIn(&Transferer::precedesI, "precedes", 1);
    registerBuildIn(&Transferer::replace_withI, "replace_with", 1);
    registerBuildIn(&Transferer::secondI, "second", 1);
    registerBuildIn(&Transferer::semmatchI, "semmatch", 2);
    registerBuildIn(&Transferer::setI, "set", 2);
    registerBuildIn(&Transferer::show_treeI, "show_tree", 0);
    registerBuildIn(&Transferer::strip_quotesI, "strip_quotes", 1);
    registerBuildIn(&Transferer::substringI, "substring", 3);
    registerBuildIn(&Transferer::suffixI, "suffix", 2);
    registerBuildIn(&Transferer::thisI, "this", 0);
    registerBuildIn(&Transferer::tree_representationI, "tree_representation", 0);
    registerBuildIn(&Transferer::ucfirstI, "ucfirst", 1);
    registerBuildIn(&Transferer::variantI, "variant", 1);
    registerBuildIn(&Transferer::undefinedI, "undefined", 1);
    registerBuildIn(&Transferer::warningI, "warning", 1);

    registerBuildIn(&Transferer::do_transferI, "do_transfer", 1);

    fake_expression = new SubrExpression( NULL, Expression::createEmptyExpression());

    setTestResultsRecorder(boost::shared_ptr<TestResultsRecorder>(new TestResultsRecorder()));
}

Transferer::~Transferer()
{
    delete fake_expression;
    delete global_transfer_spec;

    sm_token_assign = NULL; // a GC object
    sm_token_arrow = NULL; // a GC object
    sm_token_dot = NULL; // a GC object
    sm_token_and = NULL; // a GC object
    sm_token_or = NULL; // a GC object
    sm_this = NULL; // a GC object
    sm_out = NULL; // a GC object
    sm_E_returned = NULL; // a GC object
    sm_surface = NULL; // a GC object
    sm_gensurface = NULL; // a GC object
    sm_Surf = NULL; // a GC object
    sm_Equiv = NULL; // a GC object
    sm_TEquiv = NULL; // a GC object
    sm_Eid = NULL; // a GC object

    sm_main_subtree_label = NULL; // a GC object

    my_holder_->annihilate();

    //trzeba jakos poniszczyc te hashe i skrypty
    for (std::vector<TScript>::iterator sit = included_scripts_.begin();
        sit != included_scripts_.end();
        ++sit)
        (*sit)->deleteChain();

    for (std::vector<SubroutineSlot*>::iterator ssit = allocated_sub_slots_.begin();
        ssit != allocated_sub_slots_.end();
        ++ssit)
        delete (*ssit);

        //ML
        delete this->script_factory;
}

void Transferer::include(const char* name)
{
    assert(NULL!=name);

    // loading script (from the factory)
    TScript script = script_factory->getScriptForName(name, z_sym_fac, main_holder_);
    if (NULL==script) {
        ERROR_MELDER("[tmil:Transferer] No script to include.\n");
        return;
    }

    // checking if script is allready loaded
    zsymbol* zs_name = z_sym_fac->get_symbol(name);
    zvalue zv = name_to_script->get(zs_name);
    if (NULLP(zv)) {
        name_to_script->put(zpair::generate(main_holder_, zs_name, script));
    } else {
        ERROR_MELDER("[tmil:Transferer] Script " \
                << name << " allready loaded. \n");
    }

    // hashing script
    Statement* s = script;
    while (s != NULL) {
        switch (s->getType()) {
        case Statement::SUBROUTINE: hashSubroutine((Subroutine*)s);
                                        break;
        case Statement::TRANSFER_SPEC: hashTransferSpec((TransferSpec*)s);
        default: ;
        }
        s = s->getNextStatement();
    }

    included_scripts_.push_back(script);
}

void Transferer::setTestResultsRecorder(boost::shared_ptr<TestResultsRecorder> recorder)
{
    test_results_recorder_ = recorder;
}

void Transferer::hashSubroutine(Subroutine* s)
{
    assert(NULL!=s);
    assert(NULL!=s->name);

    // making a slot
    SubroutineSlot* sub_slot = new SubroutineSlot;
    allocated_sub_slots_.push_back(sub_slot);
    sub_slot->is_build_in = false;
    sub_slot->defined_subr = (Subroutine*)s;

    // making a hash key for the sub
    zpair* zp = NULL;
    if (NULL!=s->cat)
        zp = zpair::generate(main_holder_, s->cat, s->name);
    else
        zp = zpair::generate(main_holder_, NULL_ZVALUE, s->name);

    // if there is allready such a sub...
    zvalue zv = NULL;
    if ( !NULLP(zv = cat_and_name_pair_to_subroutine->get(zp)) )
    {
        //we have got a function redefinition or... something else
        const char *nothing = "";
        ERROR_MELDER("[tmil:Transferer] Redefining subroutine " \
                << ((NULL==s->cat)?nothing:s->cat->get_string()) << "::" \
                << s->name->get_string() << " detected.\n");

        // removing the old version from the hash
        // @todo to nie dziala!!!!!!!! zastosuj remove
        zv = DELETED_ZVALUE;
    }

    cat_and_name_pair_to_subroutine->put(zpair::generate(main_holder_, zp, sub_slot));
}

void Transferer::hashTransferSpec(TransferSpec* ts)
{
    assert(NULL!=ts);

    char *error=global_transfer_spec->add(main_holder_, ts);
    if (NULL!=error) {
        ERROR_MELDER("[tmil:Transferer] " << error << "\n");
        delete error;
    }
}

zvalue Transferer::run(zsyntree* node, zsymbol* sub_name, zvector* args)
{
    zsymbol* cat = node->getCategory();
    SubroutineSlot* s = findSubroutine_(cat, sub_name);
    if (NULL!=s) {
        if (args->getSize() != s->getNbArguments())
        {
            ERROR_MELDER("z³a liczba argumentów\n");
        }
        else
        {
            zvector* dsstack = zvector::generate(current_holder_, 2048);
            zvector* osstack = zvector::generate(current_holder_, 2048);

            dsstack->push(INTEGER_TO_ZVALUE(100));
            osstack->push(NULL_ZVALUE);

            if (!s->is_build_in)
            {
                zenvironment* senv = args2Env_(args, node, s->defined_subr);
                compute_(s->defined_subr, dsstack, osstack, senv);
            } else {
                zenvironment* senv = zenvironment::generate(current_holder_, 8);
                senv->change(sm_this, (zvalue)node);

                Expression* chain = NULL;
                if (NULL!=args) {
                    if (args->getSize()>0) chain = new AtomicExpression(args->pop());
                    while (args->getSize()>0) {
                        AtomicExpression* ae = new AtomicExpression(args->pop());
                        ae->insertChainBehind(chain);
                        chain = ae;
                    }
                }
                if (NULL==chain) chain = Expression::createEmptyExpression();

                SubrExpression* subr = new SubrExpression(sub_name, chain);
                compute_(subr, dsstack, osstack, senv);
                delete subr;
            }
            return dsstack->pop();
        }
    } else {
        const char* c_s = "";
        if (NULL!=cat) c_s = (char*)(cat->get_string());
        ERROR_MELDER("[tmil:Transferer] Running subroutine " \
                << c_s << "::" << sub_name->get_string() \
                << "\n                  Subroutine not found.\n");
    }

    return NULL_ZVALUE;
}

zvalue Transferer::runRecursivelyBottomUp(zsyntree* node, zsymbol* sub_name, zvector* args)
{
    if (node == NULL)
        return NULL_ZVALUE;

    zsyntree* n = NULL;

    int i;
    int stop_idx;

    zvalue ret = NULL_ZVALUE;

    zvector* stack = zvector::generate(current_holder_, 512);

    stack->push(node);
    stack->push(NULL_ZVALUE);     // NULL_ZVALUE means that the node wasn't expanded

    while (stack->getSize() != 0)
    {
        if (!NULLP(stack->peek(0))) // was expanded
        {
            stack->pop();
            n = ZSYNTREEC(stack->pop());
            ret = run(n, sub_name, args);
        }
        else
        {
            stack->pop();
            n = ZSYNTREEC(stack->peek(0));
            stack->push(INTEGER_TO_ZVALUE(1)); // marking the node as expanded

            stop_idx = n->last_subtree;
            for (i = 0; i <= stop_idx; ++i)
            {
                stack->push(n->fetch(INTEGER_TO_ZVALUE(i)));
                stack->push(NULL_ZVALUE);
            }
        }
    }

    return ret;
}

void Transferer::registerBuildIn(fun_build_in cfun, const char* name,
                                 int nb_args)
{
    SubroutineSlot* sub_slot = new SubroutineSlot;
    allocated_sub_slots_.push_back(sub_slot);
    sub_slot->is_build_in = true;
    sub_slot->internal_subr = cfun;
    sub_slot->nb_arguments = nb_args;

    zpair* zp = zpair::generate(main_holder_, NULL_ZVALUE, z_sym_fac->get_symbol(name));
    cat_and_name_pair_to_subroutine->put(zpair::generate(main_holder_, zp, sub_slot));
}

Transferer::SubroutineSlot* Transferer::findSubroutine_(zsymbol* cat,
        zsymbol* name)
{
    assert(NULL!=name);
    SubroutineSlot* result = NULL;
    if (NULL!=cat) { //if category given then look for cat::name
        zpair* zp = zpair::generate(current_holder_, cat, name);
        zvalue zv = cat_and_name_pair_to_subroutine->get(zp);
        if (!NULLP(zv)) result = (SubroutineSlot*)((zpair*)zv)->getSecond();
    }
    if (NULL==result) { //if cat::name not found or there was no category then look for ::name
        zpair* zp = zpair::generate(current_holder_, NULL_ZVALUE, name);
        zvalue zv = cat_and_name_pair_to_subroutine->get(zp);
        if (!NULLP(zv)) result = (SubroutineSlot*)((zpair*)zv)->getSecond();
    }
    return result;
}

zenvironment* Transferer::args2Env_(zvector* args, zsyntree* node,
                                    Subroutine* subr)
{
    zenvironment* renv = zenvironment::generate(current_holder_, 8);
    renv->change(sm_this, (zvalue)node);

    int i;
    for (i = subr->args->getSize() - 1; i >= 0; --i)
        renv->change((zvalue)(subr->args->elementAt(i)), args->pop());

    return renv;
}

void Transferer::setOption(zsymbol* an_option_name, zvalue an_option_value) {
    assert(NULL!=an_option_name);
    options->change(an_option_name, an_option_value);
}

void Transferer::unsetOption(zsymbol* an_option_name) {
    assert(NULL!=an_option_name);
    options->remove(an_option_name);
}


#define SIMPLE_RECURSIVE_CALL

#ifdef SIMPLE_RECURSIVE_CALL
#define GOSUB(S) compute_((S), DSS, OSS, SE)
#else
#error
#endif

#define Sc dynamic_cast<STATEMENT_CLASS*>(S)

#define THIS_TREE ( ZSYNTREEC(SE->fetch(sm_this)) )

void Transferer::compute_(Statement* S, zvector* DSS,
                          zvector* OSS, zenvironment* SE)
{
    Statement* tts;
    int tti;
    SubroutineSlot* ttsslot;
    zvalue ttz;
    zsyntree* ttt;

    assert(S != NULL);

    switch (S->getType())
    {
#undef STATEMENT_CLASS
#define STATEMENT_CLASS Expression
    case Statement::EXPRESSION:
        DSS->push(DEFAULT_ZVALUE);
        break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS AtomicExpression
    case Statement::ATOM:
        DSS->push(Sc->value);
        break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS SubrExpression
    case Statement::SUBR_EXPRESSION:
    {
        // przypisanie trzeba potraktowaæ osobno
        // lewej strony nie mo¿emy po prostu ewaluowaæ
        if (Sc->subr_name == sm_token_assign)
        {
            assert(Sc->chain_start_expression != NULL);
            assert(Sc->chain_start_expression->getNextStatement() != NULL);

            GOSUB(Sc->chain_start_expression->getNextStatement());

            if (Sc->chain_start_expression->getType() ==
               Statement::VARIABLE)
            {
                SE->change(dynamic_cast<VariableExpression*>(
                               Sc->chain_start_expression)->name,
                           DSS->peek(0));
            }
            else if (Sc->chain_start_expression->getType() ==
                    Statement::ATTRIBUTE)
            {
                THIS_TREE->setAttr(dynamic_cast<AttributeExpression*>(
                                       Sc->chain_start_expression)->attribute,
                                   DSS->peek(0));
            }
            // poni¿sze barokowe wyra¿enie dotyczy sytuacji, gdy po lewej
            // stronie jest atrybut kwalifikowany odwo³aniem do wêz³a,
            // np. daj_drzewo()->X := 10
            else if (Sc->chain_start_expression->getType() ==
                    Statement::SUBR_EXPRESSION &&
                    ( dynamic_cast<SubrExpression*>(
                        Sc->chain_start_expression)->subr_name == sm_token_dot
                      ||
                      dynamic_cast<SubrExpression*>(
                          Sc->chain_start_expression)->subr_name ==
                      sm_token_arrow)
                    &&
                    dynamic_cast<SubrExpression*>(Sc->chain_start_expression)
                    ->chain_start_expression
                    != NULL &&
                    dynamic_cast<SubrExpression*>(Sc->chain_start_expression)
                    ->chain_start_expression->getNextStatement() != NULL &&
                    dynamic_cast<SubrExpression*>(Sc->chain_start_expression)
                      ->chain_start_expression->getNextStatement()->getType()==
                    Statement::ATTRIBUTE)
            {
                GOSUB(dynamic_cast<SubrExpression*>(
                          Sc->chain_start_expression)->chain_start_expression);

                if (ZSYNTREEP(DSS->peek(0)))
                   ZSYNTREEC(DSS->peek(0))
                   ->setAttr(dynamic_cast<AttributeExpression*>
                                   (
                                       dynamic_cast<SubrExpression*>(
                                           Sc->chain_start_expression)->
                                       chain_start_expression->
                                       getNextStatement())->attribute,
                                   DSS->peek(-1));
                else
                    warning_("syntree expected", S, NULL_ZVALUE);

                DSS->pop();
            }
            else
            {
                // a teraz ciekawostka: je¶li nie mo¿na zastosowaæ normalnego
                // przypisania to ewaluujemy po prostu lew± stronê,
                // a nastêpnie przypisujemy warto¶æ do wyliczonego symbolu...
                // to rozszerzenie jêzyka, powinno byæ nieszkodzliwe (?),
                // a mo¿e siê przydaæ

                warning_("suspicious assignment", S, NULL_ZVALUE);

                GOSUB(Sc->chain_start_expression);

                if (ZSYMBOLP(DSS->peek(0)))
                {
                    assert(ZSYMBOLC(DSS->peek(0))->get_string() != NULL);

                    if (isupper(*(ZSYMBOLC(DSS->peek(0)))->get_string()))
                        THIS_TREE->setAttr(ZSYMBOLC(DSS->peek(0)),
                                           DSS->peek(-1L));
                    else
                        SE->change(ZSYMBOLC(DSS->peek(0)),
                                   DSS->peek(-1L));
                }
                else
                {
                    warning_("REALLY suspicious assignment", S, NULL_ZVALUE);
                }

                DSS->pop();
            }
        }
        // "and" is a short-circuit operator
        else if (Sc->subr_name == sm_token_and)
        {
            GOSUB(Sc->chain_start_expression);
            if (TRUEP(DSS->peek(0)))
            {
                DSS->pop();
                GOSUB(Sc->chain_start_expression->getNextStatement());
            }
        }
        // "or" is a short-circuit operator
        else if (Sc->subr_name == sm_token_or)
        {
            GOSUB(Sc->chain_start_expression);
            if (!TRUEP(DSS->peek(0)))
            {
                DSS->pop();
                GOSUB(Sc->chain_start_expression->getNextStatement());
            }
        }
        // =~ has side-effects
        else if (Sc->subr_name == sm_token_bind)
        {
            GOSUB(Sc->chain_start_expression);
            GOSUB(Sc->chain_start_expression->getNextStatement());
            ttz = symbol_wanted_(DSS->pop());
            if (NULLP(ttz))
            {
                DSS->pop();
                DSS->push(NULL_ZVALUE);
            }
            else
            {
                PerlRegExp preg(ZSYMBOLC(ttz)->get_string());

                if (preg.ok())
                {
                    DSS->push(match_regexp_(DSS->pop(),
                                            preg, SE));

                }
                else
                {
                    DSS->pop();
                    DSS->push(NULL_ZVALUE);

                    std::cerr << "ERROR: [" << ZSYMBOLC(ttz)->get_string() << "] " << preg.error() << std::endl;
                }
            }
        }
        else
        {
            // wywo³anie na rzecz innego wêz³a
            if (Sc->subr_name == sm_token_arrow ||
               Sc->subr_name == sm_token_dot)
            {
                GOSUB(Sc->chain_start_expression);
                ttz = DSS->pop();
                S = Sc->chain_start_expression->getNextStatement();
            }
            else
                ttz = THIS_TREE;

            if (ZSYNTREEP(ttz) &&
               S->getType() == Statement::SUBR_EXPRESSION)
                ttsslot = findSubroutine_(ZSYNTREEC(ttz)->getCategory(),
                                          Sc->subr_name);

            if (!ZSYNTREEP(ttz))
            {
                if (!NULLP(ttz))
                    warning_("tree expected", S, NULL_ZVALUE);
                DSS->push(NULL_ZVALUE);
            }
            else if (S->getType() != Statement::SUBR_EXPRESSION)
            {
                if (S->getType() == Statement::ATTRIBUTE)
                {
                    DSS->push(ZSYNTREEC(ttz)->getAttr(
                                  dynamic_cast<AttributeExpression*>(S)
                                  ->attribute));
                }
                else
                {
                    warning_("attribute expected", S, NULL_ZVALUE);
                    DSS->push(NULL_ZVALUE);
                }
            }
            else if (ttsslot == NULL)
            {
                warning_("unknown subroutine", S, Sc->subr_name);
                DSS->push(NULL_ZVALUE);
            }
            else
            {
                // zliczamy, ile jest argumentów w wywo³aniu
                tts = Sc->chain_start_expression;
                tti = 0;

                while (tts != NULL)
                {
                    ++tti;
                    tts = tts->getNextStatement();
                }

                // @todo trzeba poprawiæ w trparser.y definicjê args
                // w tej chwili nie ma wywo³ania z zerow± liczb± argumentów!
                if (tti == 1 &&
                   Sc->chain_start_expression->getType() == Statement::EXPRESSION)
                    tti = 0;

                if (tti != ttsslot->getNbArguments())
                {
                    warning_("bad number of arguments", S, Sc->subr_name);
                    DSS->push(NULL_ZVALUE);
                }
                else
                {
                    if (!ttsslot->is_build_in)
                    {
                        // to musi byæ w tym miejscu ze wzglêdu na ttz
                        zenvironment* nenv = zenvironment::generate(current_holder_, 8);
                        nenv->change(sm_this, ttz);

                        tts = Sc->chain_start_expression;
                        tti = 0;
                        while (tts != NULL && tti < ttsslot->getNbArguments())
                        {
                            OSS->push(ZWRAP(tts));
                            OSS->push(INTEGER_TO_ZVALUE(tti));
                            OSS->push(ZWRAP(ttsslot));
                            GOSUB(tts);
                            ttsslot = ZUNWRAP(SubroutineSlot*, OSS->pop());
                            tti = ZVALUE_TO_INTEGER(OSS->pop());
                            nenv->change(ttsslot->defined_subr->args->elementAt(tti),
                                       DSS->pop());
                            ++tti;
                            tts = ZUNWRAP(Statement*,OSS->pop());
                            tts = tts->getNextStatement();
                        }

                        OSS->push(SE);
                        SE = nenv;
                        GOSUB(ttsslot->defined_subr);
                        SE = ZENVIRONMENTC(OSS->pop());
                    }
                    else
                    {
                        tts = Sc->chain_start_expression;
                        OSS->push(ttz);
                        while (tts != NULL && tti)
                        {
                            OSS->push(ZWRAP(tts));
                            GOSUB(tts);
                            tts = ZUNWRAP(Statement*,OSS->pop());
                            tts = tts->getNextStatement();
                        }
                        ttz = OSS->pop();

                        // do tego momentu ttz przechowywa³o drzewo, na
                        // rzecz którego bêdzie wywo³ywana procedura
                        // teraz u¿yjemy ttz do czego¶ innego
                        ttz =
                            (this->*(ttsslot->internal_subr))(
                                DSS,
                                ZSYNTREEC(ttz),
                                S,
                                ttsslot->nb_arguments);

                        for (tti = 0; tti < ttsslot->nb_arguments; ++tti)
                            DSS->pop();

                        DSS->push(ttz);
                    }
                }
            }
        }
    }
    break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS VariableExpression
    case Statement::VARIABLE:
        DSS->push(SE->fetch(Sc->name));
        break;

        // @todo PLREF zamieniæ na EQVREF (PL to pozosta³o¶æ po "polski")

    case Statement::PLREF:
        DSS->push(THIS_TREE->getEquivTree());
        break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS SrefExpression
    case Statement::SREF:
        {
            int tti = 0;
            zvalue ret_value = NULL_ZVALUE;
            while (tti <= THIS_TREE->last_subtree)
            {
                zvalue t = THIS_TREE->fetch(INTEGER_TO_ZVALUE(tti));

                if (ZSYNTREEP(t) && THIS_TREE->fetch(t) == Sc->sref)
                {
                    ret_value = t;
                    break;
                }

                ++tti;
            }
            DSS->push(ret_value);
        }
        break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS AttributeExpression
    case Statement::ATTRIBUTE:
        DSS->push(ZSYNTREEC(SE->fetch(sm_this))->getAttr(Sc->attribute));
        break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS ForeachLoop
    case Statement::FOREACH:
        if (Sc->sref == NULL_ZVALUE)
        {
            tti = 0;
            while (tti <= THIS_TREE->last_subtree)
            {
                OSS->push(INTEGER_TO_ZVALUE(tti));

                SE->change(Sc->var,
                           THIS_TREE->fetch(INTEGER_TO_ZVALUE(tti)));

                GOSUB(Sc->instruction);

                tti = ZVALUE_TO_INTEGER(OSS->pop());
                ++tti;
            }
        }
        else
        {
            // @todo
        }
        break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS SrefSetting
    case Statement::SREF_SETTING:
        tti = 0;
        while (tti <= THIS_TREE->last_subtree)
        {
            OSS->push(INTEGER_TO_ZVALUE(tti));

            OSS->push(THIS_TREE);

            SE->change(sm_this, THIS_TREE->fetch(INTEGER_TO_ZVALUE(tti)));

            GOSUB(Sc->expr);

            SE->change(sm_this, OSS->pop());

            if (TRUEP(DSS->peek(0)))
            {
                ttt = ZSYNTREEC(THIS_TREE->fetch(INTEGER_TO_ZVALUE(tti)));

                // @todo u¿yæ getNext zamiast fetch
                ttz = THIS_TREE->fetch(ttt);
                THIS_TREE->deleteAllKeyValue(ttz, ttt);
                THIS_TREE->deleteAllKeyValue(ttt, ttz);

                THIS_TREE->insert(Sc->sref,ttt);
                THIS_TREE->insert(ttt, Sc->sref);
            }

            DSS->pop();

            tti = ZVALUE_TO_INTEGER(OSS->pop());
            ++tti;
        }
        break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS InstructionBlock
    case Statement::INSTRUCTION_BLOCK:
        tts = Sc->instructionChain->getFirstChainElement();
        while (tts != NULL &&
              ZVALUE_TO_INTEGER(SE->fetch(sm_E_returned)) != 1)
        {
            OSS->push(ZWRAP(tts));
            GOSUB(tts);
            tts = ZUNWRAP(Statement*,OSS->pop());
            tts = tts->getNextStatement();
        }
        break;

    case Statement::INSTRUCTION:
        break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS SimpleInstruction
    case Statement::SIMPLE_INSTRUCTION:
        GOSUB(Sc->expression);
        DSS->pop();
        break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS InstructionIf
    case Statement::INSTRUCTION_IF:
        GOSUB(Sc->condition);
        if (TRUEP(DSS->peek(0)))
            GOSUB(Sc->ontrue_instruction);
        else if (Sc->onfalse_instruction != NULL)
            GOSUB(Sc->onfalse_instruction);
        DSS->pop();
        break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS Subroutine
    case Statement::SUBROUTINE:
        GOSUB(Sc->instruction);
        DSS->push(SE->fetch(sm_out));
        break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS OrderSetting
    case Statement::ORDER_SETTING:
        THIS_TREE->sortByOrder(Sc->srefs_hashed);
        break;

#undef STATEMENT_CLASS
#define STATEMENT_CLASS ReturnInstruction
    case Statement::RETURN_INSTRUCTION:
        GOSUB(Sc->ret_expr);
        SE->change(sm_out, DSS->pop());
        SE->change(sm_E_returned, INTEGER_TO_ZVALUE(1));
        break;
    }

}

#undef STATEMENT_CLASS

zsyntree* Transferer::doTransfer(zsyntree* a_source_tree
        ,TransferSpecHash* a_local_transfer_spec
        ,zsyntree* a_parent_node)
{
    // @todo czy musi byæ replace_with_could... tak jak w starym transferze?

    // We don't transfer subtrees twice, i.e. if label transfer rules
    // subtrees are not transferred.
    bool subs_constructed = false;

    int num_in_parent = -1L;

    if (NULL==a_source_tree) {
        ERROR_MELDER("[tmil::Transferer::doTransfer] NULL tree to transfer.");
        return NULL;
    }

    zsyntree* a_target_tree = NULL;

    if ( NULL==(a_target_tree=a_source_tree->equiv_tree) ) {
        a_target_tree = a_source_tree->equiv_tree = zsyntree::generate(current_holder_);
        a_target_tree->equiv_tree = a_source_tree;
        a_target_tree->setOrigin(a_source_tree->getOrigin());
    }

    a_target_tree->is_variant = a_source_tree->is_variant;


    if (a_parent_node != a_target_tree->parent_tree) {
        a_target_tree->detachFromParent();
        if (NULL!=a_parent_node) a_parent_node->addSubtree(a_target_tree);
    }

    if (a_target_tree->parent_tree != NULL)
        num_in_parent = a_target_tree->number_in_parent_tree;

    TransferSpec* transfer_spec = NULL;
    if (   NULL!=a_source_tree->label
        && NULL!=a_local_transfer_spec
        && NULL!=(transfer_spec=a_local_transfer_spec->find(
                      current_holder_,
                      a_source_tree->category,
                      a_source_tree->label) )
       )
    {
#ifdef RUN_TO_DO
#error Name conflict RUN_TO_DO.
#endif
#define RUN_TO_DO \
    { \
        if (!subs_constructed) {\
             subs_constructed = true;\
            for (int i=0; i<=a_source_tree->last_subtree; ++i) { \
            doTransfer(ZSYNTREEC(a_source_tree->fetch(INTEGER_TO_ZVALUE(i))) \
                    ,transfer_spec->local_transfer_spec \
                    ,a_target_tree); \
        }} \
        zenvironment* renv = zenvironment::generate(current_holder_, 8); \
        renv->change(sm_this, a_target_tree); \
        zvector* dsstack = zvector::generate(current_holder_, 2048); \
        zvector* osstack = zvector::generate(current_holder_, 2048); \
        dsstack->push(INTEGER_TO_ZVALUE(100)); \
        osstack->push(NULL_ZVALUE); \
        compute_(transfer_spec->to_do, dsstack, osstack, renv); \
        if (num_in_parent != -1L) \
           a_target_tree = ZSYNTREEC(a_parent_node->fetch(INTEGER_TO_ZVALUE(num_in_parent))); \
    }

#ifdef WORK1
#error Name conflict WORK1.
#endif
#define WORK1 \
    if (NULL!=transfer_spec->target_cat)  \
        a_target_tree->category=transfer_spec->target_cat; \
    else  \
        ERROR_MELDER("There is no target in for transfer spec "); \
    if (NULL!=transfer_spec->target_label)  \
        a_target_tree->label=transfer_spec->target_label; \
    else  \
        if (sm_main_subtree_label==a_source_tree->label)  \
            a_target_tree->label=a_source_tree->label; \
    RUN_TO_DO

        WORK1;
    } else if ( NULL!=a_source_tree->label
                && NULL!=(transfer_spec=global_transfer_spec->find(
                              current_holder_,
                              a_source_tree->category,
                              a_source_tree->label) )
            )
    {
        WORK1;
    } else {
        if (   NULL!=a_local_transfer_spec
            && NULL!=(transfer_spec=a_local_transfer_spec->find(
                          current_holder_,
                          a_source_tree->category,
                          NULL) )
           )
        {
#ifdef WORK2
#error Name conflict WORK2.
#endif
#define WORK2 \
    if (NULL!=transfer_spec->target_cat)  \
        a_target_tree->category=transfer_spec->target_cat; \
    else  \
        warning_("There is no target in for transfer spec ",transfer_spec,NULL); \
    if (NULL!=transfer_spec->target_label)  \
        warning_("There is a target label in transfer spec ",transfer_spec,NULL); \
    RUN_TO_DO
            WORK2;
        } else if (NULL!=(transfer_spec=global_transfer_spec->find(
                              current_holder_,
                              a_source_tree->category,
                              NULL) )
                )
        {
            WORK2;
        } else {
            bool error = true;
            const char* s = "";
            if (NULL!=a_source_tree->category) {
                s = (char*)a_source_tree->category->get_string();
                int len = strlen(s);
                if (len>1)
                {
                    if ('\''==s[0] && '\''==s[len-1])
                    {
                        a_target_tree->category = a_source_tree->category;
                        error = false;
                    }
                }
            }
            if (error)
                ERROR_MELDER("Do not know how to transfer " << s << " category.\n");

            if (!subs_constructed)
            {
                subs_constructed = true;
                for (int i=0; i<=a_source_tree->last_subtree; ++i) {
                    doTransfer(ZSYNTREEC(a_source_tree->fetch(INTEGER_TO_ZVALUE(i)))
                               ,NULL
                               ,a_target_tree);
                }
            }

        }

        if (   NULL!=a_local_transfer_spec
            && NULL!=(transfer_spec=a_local_transfer_spec->find(
                          current_holder_,
                          NULL,
                          a_source_tree->label) )
           )
        {
#ifdef WORK3
#error Name conflict WORK3.
#endif
#define WORK3 \
    if (NULL!=transfer_spec->target_cat)  \
        warning_("There is a target category in transfer spec ",transfer_spec,NULL); \
    if (NULL==transfer_spec->target_label) \
        warning_("There is no target label in transfer spec ",transfer_spec,NULL); \
    a_target_tree->label=transfer_spec->target_label; \
    RUN_TO_DO;

            WORK3;
        } else if (NULL!=(transfer_spec=global_transfer_spec->find(
                              current_holder_,
                              NULL,
                              a_source_tree->label) )
                )
        {
            WORK3;
        } else if (sm_main_subtree_label==a_source_tree->label) {
            a_target_tree->label=sm_main_subtree_label;
        } else if (NULL == a_source_tree->label) {
            a_target_tree->label = NULL;
        }
        else {
            char* s=(char*)a_source_tree->label->get_string();
            ERROR_MELDER("Do not know how to transfer " << s << " label.\n");
            a_target_tree->label=a_source_tree->label;
        }
    }

    return a_source_tree->equiv_tree;
}

void Transferer::doModification(zsyntree* a_tree)
{
    // @todo should be NULL instead of empty zvector
    zvector* args = zvector::generate(current_holder_, 0);
    runRecursivelyBottomUp(a_tree, sm_default, args);
}

void Transferer::doVariant(zsyntree* a_tree, zsyntree* a_model_tree)
{
    zvector* args = zvector::generate(current_holder_, 1);
    args->push(a_model_tree);
    run(a_tree, sm_variant, args);
}

void Transferer::doSurface(zsyntree* a_tree)
{
    if (a_tree == NULL)
        return;

    // odwiedzamy wêz³y drzewa od korzenia do li¶ci
    // na ka¿dym odwiedzanym wê¼le wywo³ujemy procedurê surface
    // je¶li dla danego wêz³a surface da warto¶æ ró¿na od NULL_ZVALUE
    // nie odwiedzamy potomków wêz³a
    // funkcja ustawia tak¿e atrybut Surf na warto¶æ zwrócon± przez surface

    // u¿yjemy kolejki, z tym ¿e nie bêdziemy usuwali wêz³ów z kolejki
    // tylko u¿yjemy indeksu nqi
    zvector* nqueue = zvector::generate(current_holder_, 256);
    int nqi = 0;
    nqueue->push(a_tree);

    zsyntree* n = NULL;
    int i;

    zvalue ret = NULL_ZVALUE;

    // @todo should be NULL instead of empty zvector
    zvector* args = zvector::generate(current_holder_, 0);

    while (nqi < nqueue->getSize())
    {
        // zabezpieczamy siê przed zapêtleniem
        if (nqi > 8192) return;

        // n bêdzie zawiera³o odwiedzany wêze³
        n = ZSYNTREEC(nqueue->elementAt(nqi));

        ret = run(n, sm_surface, args);

        if (NULLP(ret))
        {
            // wrzucamy do kolejki potomków n
            for (i = 0; i <= n->last_subtree; ++i)
                nqueue->push(n->fetch(INTEGER_TO_ZVALUE(i)));
        }
        else
            n->change(sm_Surf, ret);

        ++nqi;
    }

    return;
}

zsymbol* Transferer::getSurfSymbol()
{
    return sm_Surf;
}

zsymbol* Transferer::getNrSymbol()
{
    return sm_Nr;
}

zsymbol* Transferer::getNilSymbol()
{
    return sm_nil;
}

zsymbol* Transferer::getEidSymbol()
{
    return sm_Eid;
}


zsyntree* Transferer::doTranslate(zsyntree* a_source_tree,
                                  char** printed_trg_tree,
                                  zsyntree* a_model_tree)
{
    zsyntree* r = NULL;

    if (a_source_tree != NULL)
    {
        zsyntree* target_tree = doTransfer(a_source_tree, NULL, NULL);

        if (target_tree != NULL)
        {
#ifdef USE_LOG4CPLUS
        {
            log4cplus::Logger l = log4cplus::Logger::getInstance("TargetTree");
            char* t = target_tree->zsyntree_to_string();
            LOG4CPLUS_INFO(l, t);
            delete t;
        }
#endif

            doModification(target_tree);
#ifdef USE_LOG4CPLUS
        {
            log4cplus::Logger l = log4cplus::Logger::getInstance("ModifiedTargetTree");
            char* t = target_tree->zsyntree_to_string();
            LOG4CPLUS_INFO(l, t);
            delete t;
        }
#endif

        if (a_model_tree != NULL &&
           target_tree != NULL &&
           target_tree->getCategory() == a_model_tree->getCategory())
            doVariant(target_tree, a_model_tree);

        doSurface(target_tree);
#ifdef USE_LOG4CPLUS
        {
            log4cplus::Logger l = log4cplus::Logger::getInstance("InflectedTree");
            char* t = target_tree->zsyntree_to_string();
            LOG4CPLUS_INFO(l, t);
            delete t;
        }
#endif

        if (printed_trg_tree != NULL)
            *printed_trg_tree = target_tree->zsyntree_to_itf();

        r = target_tree;
        }
    }

    return r;
}

void Transferer::warning_(const char* message, Statement* /*SP*/, zvalue V)
{
    char* s = zvalue_to_string(V);
    std::cerr <<  message << ':' << s << std::endl;
    delete s;
}


zsymbolfactory* Transferer::get_zsymbolfactory() {
    return z_sym_fac;
}

// - - - - - -| BUILDIN procedures support |- - - - - -

zsymbol* Transferer::subsymbol_(const char* s, int beg, int len) {
    const int s_len_in_bytes = strlen(s);
    const int s_len = utf8::distance(s, s+s_len_in_bytes);
    if (beg<0) {
        beg+=s_len;
        if (beg<0) beg=0;
    }
    if (s_len<beg) beg=s_len;
    if (len>(s_len-beg)) len=s_len-beg;
    if (0>len) len=0;

    const char *beg_ptr = s;
    utf8::unchecked::advance(beg_ptr, beg);
    const char *end_ptr = beg_ptr;
    utf8::unchecked::advance(end_ptr, len);

    size_t len_in_bytes = end_ptr - beg_ptr;

    char *t = new char[len_in_bytes+1];
    strncpy(t, beg_ptr, len_in_bytes);
    t[len_in_bytes]=0;
    zsymbol* zs = z_sym_fac->get_symbol(current_holder_, t, true);

    return zs;
}

zvalue Transferer::concat_(zvalue a, zvalue b)
{
    a = symbol_wanted_(a);
    b = symbol_wanted_(b);

    if (NULLP(a) && ZSYMBOLP(b)) return b;
    if (NULLP(b) && ZSYMBOLP(a)) return a;

    if (ZSYMBOLP(a) && ZSYMBOLP(b))
        return ZOBJECT_TO_ZVALUE(
            zf_concat(get_zsymbolfactory(), current_holder_,
                      ZSYMBOLC(a), ZSYMBOLC(b)));

    warning_("arguments of concat should be strings",NULL,NULL_ZVALUE);
    return NULL_ZVALUE;
}


zvalue Transferer::concat_with_space_(zvalue a, zvalue b)
{
    a = symbol_wanted_(a);
    b = symbol_wanted_(b);

    if (NULLP(a) && ZSYMBOLP(b)) return b;
    if (NULLP(b) && ZSYMBOLP(a))  return a;

    if (ZSYMBOLP(a) && ZSYMBOLP(b))
        return ZOBJECT_TO_ZVALUE(
            zf_concat_with_space(get_zsymbolfactory(), current_holder_,
                                 ZSYMBOLC(a), ZSYMBOLC(b)));

    warning_("arguments of concat should be strings",NULL,NULL_ZVALUE);
    return NULL_ZVALUE;
}

zvalue Transferer::nounderscores_(zvalue sm)
{
    if (ZSYMBOLP(sm))
    {
        const char *s = ZSYMBOLC(sm)->get_string();
        int len = strlen(s);

        int i;

        for (i=0; i<len && s[i] != '_' ; ++i) ;

        if (i == len) return sm;

        char* t = new char[len+1];
        strcpy(t, s);

        for (i=0; i<len; ++i) if ('_'==t[i]) t[i]=' ';

        zsymbol* zs = z_sym_fac->get_symbol(current_holder_, t, true);

        return ZOBJECT_TO_ZVALUE(zs);
    }
    else if (ZVECTORP(sm))
    {
        int i;
        zvector* v = ZVECTORC(sm);
        zvector* w = zvector::generate(current_holder_, v->getSize());
        for (i = 0; i < v->getSize(); ++i)
            w->push(nounderscores_(v->elementAt(i)));
        return ZOBJECT_TO_ZVALUE(w);
    }

    warning_("argument of nounderscores should be a string",NULL,NULL_ZVALUE);
    return sm;
}

zvalue Transferer::strip_quotes_(zvalue sm)
{
    if (ZSYMBOLP(sm))
    {
        const char *s = ZSYMBOLC(sm)->get_string();
        const size_t len = strlen(s);
        if (len>2 && '\''==s[0] && '\''==s[len-1]) {
            const size_t len_in_chars = utf8::distance(s, s+len);
            return subsymbol_(s, 1, len_in_chars-2);
        }
        return sm;
    }

    warning_("argument of strip_quotes should be a string",NULL,NULL_ZVALUE);
    return NULL_ZVALUE;
}

zvalue Transferer::symbol_wanted_(zvalue z)
{
    if (NULLP(z))
        return NULL_ZVALUE;

    if (ZSYMBOLP(z))
        return z;

    if (INTEGERP(z))
        return zf_itos(get_zsymbolfactory(), current_holder_, z);

    return NULL_ZVALUE;
}

zvalue Transferer::integer_wanted_(zvalue z)
{
    if (NULLP(z))
        return INTEGER_TO_ZVALUE(0);

    if (INTEGERP(z))
        return z;

    if (ZSYMBOLP(z))
    {
        char* pc;
        int i;
        i = strtol(ZSYMBOLC(z)->get_string(), &pc, 10);
        if (*pc == '\0')
            return INTEGER_TO_ZVALUE(i);
    }

    return NULL_ZVALUE;
}

zvalue Transferer::match_regexp_(zvalue a,
                                 PerlRegExp& preg,
                                 zenvironment* E)
{
    a = symbol_wanted_(a);

    const char* s = (NULLP(a) ? "" : ZSYMBOLC(a)->get_string());

    PerlStringPiece input(s);

    int groupSize = preg.NumberOfCapturingGroups();
    if (groupSize > 10)
        groupSize = 10;

    std::vector<Arg> argv(groupSize);
    std::vector<Arg*> args(groupSize);
    std::vector<PerlStringPiece> ws(groupSize);
    for (int i = 0; i < groupSize; ++i) {
        args[i] = &argv[i];
        argv[i] = &ws[i];
    }

    if (PerlRegExp::PartialMatchN(input, preg, &(args[0]), groupSize)) {
        int i;
        char vn[2];
        vn[1] = '\0';
        char* t = new char [ strlen(s) + 1 ];

        assert(E != NULL);

        for (i = 0; i < groupSize; ++i)
        {
            vn[0] = char(i + 1 + '0');
            E->change(z_sym_fac->get_symbol(current_holder_, vn, false),
                      z_sym_fac->get_symbol(current_holder_, ws[i].as_string().c_str(), false));

//            else
//                E->change(z_sym_fac->get_symbol(current_holder_, vn, false),
//                          NULL_ZVALUE);
        }

        delete [] t;

        return INTEGER_TO_ZVALUE(1);
    }

    return INTEGER_TO_ZVALUE(0);
}

// - - - - - -| BUILDIN procedures |- - - - - -

#define BUILDINDEF(S) zvalue Transferer::S##I(zvector* DSS,   \
        zsyntree* node, \
        Statement* stat, \
        int nb_arguments \
        )

#define FIRST_ARG ( DSS->peek(1L-nb_arguments) )
#define SECOND_ARG ( DSS->peek(2L-nb_arguments) )
#define THIRD_ARG ( DSS->peek(3L-nb_arguments) )
#define FOURTH_ARG ( DSS->peek(4L-nb_arguments) )
#define FIFTH_ARG ( DSS->peek(5L-nb_arguments) )

BUILDINDEF(atoi)
{
    if (NULLP(FIRST_ARG))
        return INTEGER_TO_ZVALUE(0);
    else if (INTEGERP(FIRST_ARG))
        return FIRST_ARG;
    else if (ZSYMBOLP(FIRST_ARG))
        return INTEGER_TO_ZVALUE(atoi(ZSYMBOLC(FIRST_ARG)->get_string()));
    else
        return INTEGER_TO_ZVALUE(0);
}

BUILDINDEF(cat)
{
    return ZOBJECT_TO_ZVALUE(node->getCategory());
}

BUILDINDEF(change_category)
{
    if (NULLP(FIRST_ARG))
    {
        node->setCategory(NULL);
        return ZOBJECT_TO_ZVALUE(node);
    }

    if (!ZSYMBOLP(FIRST_ARG)) {
        warning_("first argument of change_category should be a category name", stat, NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    node->setCategory(ZSYMBOLC(FIRST_ARG));
    return ZOBJECT_TO_ZVALUE(node);
}

BUILDINDEF(change_label)
{
    if (NULLP(FIRST_ARG))
    {
        node->setLabel(NULL);
        return ZOBJECT_TO_ZVALUE(node);
    }

    if (!ZSYMBOLP(FIRST_ARG)) {
        warning_("first argument of change_label should be a label name", stat, NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    node->setLabel(ZSYMBOLC(FIRST_ARG));
    return ZOBJECT_TO_ZVALUE(node);
}

BUILDINDEF(clear)
{
    if (!ZSYMBOLP(FIRST_ARG)) {
        warning_("argument of clear should be a string", stat, NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    node->deleteAllKey(ZSYMBOLC(FIRST_ARG));
    return NULL_ZVALUE;
}

BUILDINDEF(cut_suffix)
{
    if (!ZSYMBOLP(FIRST_ARG))
    {
        warning_("first argument of cut_suffix should be a string", stat, NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    else
    {
        zvalue cv = integer_wanted_(SECOND_ARG);
        if (NULLP(cv) || ZVALUE_TO_INTEGER(cv)<0)
        {
            warning_("second argument of cut_suffix should be a nonnegative integer", stat, NULL_ZVALUE);
            return NULL_ZVALUE;
        }
        else
        {
            const char* s = ZSYMBOLC(FIRST_ARG)->get_string();
            const int s_len_in_bytes = strlen(s);
            const int s_len = utf8::distance(s, s+s_len_in_bytes);
            const int pos = s_len-ZVALUE_TO_INTEGER(cv);

            return subsymbol_(s, 0, pos);
        }
    }
}

BUILDINDEF(default)
{
    return INTEGER_TO_ZVALUE(1);
}

BUILDINDEF(delete)
{
    zsyntree* to_delete = NULL;
    zsymbol *cat = NULL, *label = NULL;
    if (!ZSYNTREEP(FIRST_ARG)) {
        if (!ZSYMBOLP(FIRST_ARG)) {
            warning_("first argument of delete should be a string or a node", stat, NULL_ZVALUE);
            return NULL_ZVALUE;
        } else {
            cat = ZSYMBOLC(FIRST_ARG);
            if ( !DEFAULTP(SECOND_ARG) ) {
                if ( !ZSYMBOLP(SECOND_ARG) ) {
                    warning_("second argument of delete should be a string", stat, NULL_ZVALUE);
                    return NULL_ZVALUE;
                } else {
                    label = ZSYMBOLC(SECOND_ARG);
                }
            }
        }
    } else {
        to_delete=ZSYNTREEC(FIRST_ARG);
    }
    if (NULL==to_delete) to_delete = node->find(cat, label);
    if (NULL!=to_delete) node->deleteSubtree(to_delete);
    return to_delete;
}

BUILDINDEF(do_surface)
{
    doSurface(node);
    return node;
}

BUILDINDEF(eq)
{
    if (FIRST_ARG==SECOND_ARG) return INTEGER_TO_ZVALUE(1);
    return INTEGER_TO_ZVALUE(0);
}

BUILDINDEF(eqv)
{
    return ZOBJECT_TO_ZVALUE(node->equiv_tree);
}

BUILDINDEF(et)
{
    if (NULLP(FIRST_ARG))
        return SECOND_ARG;

    if (NULLP(SECOND_ARG))
        return FIRST_ARG;

    return zf_put_to_vector(current_holder_, FIRST_ARG, SECOND_ARG);
}

#define find_param_test(proc_name) \
zsymbol *cat = NULL, *label = NULL; \
    if ( !DEFAULTP(FIRST_ARG) ) { \
        if ( !ZSYMBOLP(FIRST_ARG) ) { \
            warning_("first argument of " #proc_name " should be a string", stat, NULL_ZVALUE); \
                return NULL_ZVALUE; \
        } else cat = ZSYMBOLC(FIRST_ARG); \
    } \
if ( !DEFAULTP(SECOND_ARG) ) { \
    if ( !ZSYMBOLP(SECOND_ARG) ) { \
        warning_("second argument of " #proc_name " should be a string", stat, NULL_ZVALUE); \
            return NULL_ZVALUE; \
    } else label = ZSYMBOLC(SECOND_ARG); \
}

BUILDINDEF(find)
{
    find_param_test(find);
    return ZOBJECT_TO_ZVALUE(node->find(cat, label));
}

BUILDINDEF(find_left)
{
    find_param_test(find_left);
    return ZOBJECT_TO_ZVALUE(node->find(cat, label));
}

BUILDINDEF(find_right)
{
    find_param_test(find_right);
    return ZOBJECT_TO_ZVALUE(node->find(cat, label,false));
}

BUILDINDEF(first)
{
    if (!ZPAIRP(FIRST_ARG)) {
        warning_("argument of first should be a pair", stat, NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    return ZPAIRC(FIRST_ARG)->getFirst();
}

BUILDINDEF(first_subtree)
{
    if (!ZSYNTREEP(node))
        return NULL_ZVALUE;

    if (node->last_subtree < 0)
        return NULL_ZVALUE;

    return node->fetch(INTEGER_TO_ZVALUE(0));
}

BUILDINDEF(get)
{
    if (!ZSYMBOLP(FIRST_ARG)) {
        warning_("argument of set should be a string", stat, NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    return node->getAttr(ZSYMBOLC(FIRST_ARG));
}

BUILDINDEF(get_dvalue)
{
    if (NULLP(FIRST_ARG))
    {
        return NULL_ZVALUE;
    }
    else if (!ZSYMBOLP(FIRST_ARG))
    {
        warning_("argument of get_dvalue should be a string",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    }

#ifdef TRANSFERER_NO_SEMANTICS
    return NULL_ZVALUE;
#else
    return dsem_hierarchy->getDValue(ZSYMBOLC(FIRST_ARG)->get_string());
#endif
}

BUILDINDEF(get_morpho)
{
#include "i2morpho.i"
  ;

  int P = ZVALUE_TO_INTEGER(FIRST_ARG);
  int r = ZVALUE_TO_INTEGER(SECOND_ARG);
  int l = ZVALUE_TO_INTEGER(THIRD_ARG);
  int o = ZVALUE_TO_INTEGER(FOURTH_ARG);
  int p = ZVALUE_TO_INTEGER(FIFTH_ARG);

  // specjalna wartosc 14 oznacza opcjonalna forme meska/zenska
  if (r == 14)
  {
      if (P < 0 || P > 16 || r < 0 || l < 0 || l > 2 || o < 0 || o > 3 || p < 0 || p > 7)
         return z_sym_fac->get_symbol(current_holder_, i2morpho[0][0][0][0][0], false);

      if (strcmp(i2morpho[P][1][l][o][p], i2morpho[P][4][l][o][p]) == 0)
         return z_sym_fac->get_symbol(current_holder_, i2morpho[P][1][l][o][p], false);

      std::string combined_gender =
         std::string(i2morpho[P][1][l][o][p])
         + "/"
         + i2morpho[P][4][l][o][p];

      return z_sym_fac->get_symbol(current_holder_, combined_gender.c_str(), false);
  }

  if (P < 0 || P > 16 || r < 0 || r > 5 || l < 0 || l > 2 ||
     o < 0 || o > 3 || p < 0 || p > 7)
      return z_sym_fac->get_symbol(current_holder_, i2morpho[0][0][0][0][0], false);


  return z_sym_fac->get_symbol(current_holder_, i2morpho[P][r][l][o][p], false);
}

BUILDINDEF(get_morpho_ru)
{
#include "i2morpho_ru.i"
  ;

  int P = ZVALUE_TO_INTEGER(FIRST_ARG);
  int r = ZVALUE_TO_INTEGER(SECOND_ARG);
  int l = ZVALUE_TO_INTEGER(THIRD_ARG);
  int o = ZVALUE_TO_INTEGER(FOURTH_ARG);
  int p = ZVALUE_TO_INTEGER(FIFTH_ARG);

  if (P < 0 || P > 18 || r < 0 || r > 5 || l < 0 || l > 2 ||
     o < 0 || o > 3 || p < 0 || p > 7)
      return z_sym_fac->get_symbol(current_holder_, i2morpho_ru[0][0][0][0][0], false);


  return z_sym_fac->get_symbol(current_holder_, i2morpho_ru[P][r][l][o][p], false);
}

BUILDINDEF(get_morpho_de)
{
#include "i2morpho_de.i"
  ;

  int P = ZVALUE_TO_INTEGER(FIRST_ARG);
  int r = ZVALUE_TO_INTEGER(SECOND_ARG);
  int l = ZVALUE_TO_INTEGER(THIRD_ARG);
  int o = ZVALUE_TO_INTEGER(FOURTH_ARG);
  int p = ZVALUE_TO_INTEGER(FIFTH_ARG);

  if (P < 0 || P > I2MORPHO_DE_MAX || r < 0 || r > 5 || l < 0 || l > 2 ||
     o < 0 || o > 3 || p < 0 || p > 7)
      return z_sym_fac->get_symbol(current_holder_, i2morpho_de[0][0][0][0][0], false);


  return z_sym_fac->get_symbol(current_holder_, i2morpho_de[P][r][l][o][p], false);
}

BUILDINDEF(get_morpho_es)
{
#include "i2morpho_es.i"
  ;

  int P = ZVALUE_TO_INTEGER(FIRST_ARG);
  int r = ZVALUE_TO_INTEGER(SECOND_ARG);
  int l = ZVALUE_TO_INTEGER(THIRD_ARG);
  int o = ZVALUE_TO_INTEGER(FOURTH_ARG);
  int p = ZVALUE_TO_INTEGER(FIFTH_ARG);

  if (P < 0 || P > I2MORPHO_ES_MAX || r < 0 || r > 5 || l < 0 || l > 2 ||
     o < 0 || o > 3 || p < 0 || p > 7)
      return z_sym_fac->get_symbol(current_holder_, i2morpho_es[0][0][0][0][0], false);

  return z_sym_fac->get_symbol(current_holder_, i2morpho_es[P][r][l][o][p], false);
}

BUILDINDEF(get_option)
{
    if (!ZSYMBOLP(FIRST_ARG)) {
        warning_("argument of strip_quotes should be a string", stat, NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    return options->fetch(ZSYMBOLC(FIRST_ARG));
}

#define insert_params(proc_name) \
zsyntree* nt = NULL; zsymbol *cat = NULL, *label = NULL; \
    if (!ZSYNTREEP(FIRST_ARG)) { \
        if ( !ZSYMBOLP(FIRST_ARG) ) {  \
            warning_("first argument of " #proc_name " should be a string or a node", stat, NULL_ZVALUE); \
                return NULL_ZVALUE; \
        } else { \
            cat = ZSYMBOLC(FIRST_ARG); \
                if ( !DEFAULTP(SECOND_ARG) ) { \
                    if ( !ZSYMBOLP(SECOND_ARG) ) { \
                        warning_("second argument of " #proc_name " should be a string", stat, NULL_ZVALUE);  \
                            return NULL_ZVALUE; \
                    } else label = ZSYMBOLC(SECOND_ARG); \
                } \
        }  \
    } else { \
        nt=ZSYNTREEC(FIRST_ARG); \
    }

BUILDINDEF(inherit)
{
    zsymbol* A = ZSYMBOLC(symbol_wanted_(FIRST_ARG));
    if (!NULLP(A))
        node->inherit(A, SECOND_ARG);
    return INTEGER_TO_ZVALUE(0);
}

BUILDINDEF(insert_after)
{
    if (node->parent_tree == NULL)
        return NULL_ZVALUE;

    insert_params(insert_after);

    if (NULL==nt) {
        nt = zsyntree::generate(current_holder_);
        nt->setCategory(cat);
    }

    node->insertBeside(nt,true);
    if (NULL!=label) nt->setLabel(label);

    return ZOBJECT_TO_ZVALUE(nt);
}

BUILDINDEF(insert_before)
{
    if (node->parent_tree == NULL)
        return NULL_ZVALUE;

    insert_params(insert_before);

    if (NULL==nt) {
        nt = zsyntree::generate(current_holder_);
        nt->setCategory(cat);
    }

    node->insertBeside(nt,false);
    if (NULL!=label) nt->setLabel(label);

    return ZOBJECT_TO_ZVALUE(nt);
}

BUILDINDEF(insert_left)
{
    insert_params(insert_left);

    if (NULL==nt) {
        nt = zsyntree::generate(current_holder_);
        nt->setCategory(cat);
    }

    node->insertLeft(nt);
    if (NULL!=label) nt->setLabel(label);

    return ZOBJECT_TO_ZVALUE(nt);
}


BUILDINDEF(insert_right)
{
    insert_params(insert_right);

    if (NULL==nt) {
        nt = zsyntree::generate(current_holder_);
        nt->setCategory(cat);
    }

    node->insertRight(nt);
    if (NULL!=label) nt->setLabel(label);

    return ZOBJECT_TO_ZVALUE(nt);
}

BUILDINDEF(intersection)
{
    if (NULLP(FIRST_ARG)) return FIRST_ARG;
    if (NULLP(SECOND_ARG)) return SECOND_ARG;

    if (FIRST_ARG == sm_any) return SECOND_ARG;
    if (SECOND_ARG == sm_any) return FIRST_ARG;

    if (ZSYMBOLP(FIRST_ARG) && ZSYMBOLP(SECOND_ARG))
    {
#ifdef TRANSFERER_NO_SEMANTICS
        return NULL_ZVALUE;
#else
        if (sem_hierarchy == NULL)
            return NULL_ZVALUE;
        else
        {
            const char* s = sem_hierarchy->intersection(
                    ZSYMBOLC(FIRST_ARG)->get_string(),
                    ZSYMBOLC(SECOND_ARG)->get_string()).c_str();
            if (s == NULL || s[0] == '\0')
                return NULL_ZVALUE;
            return z_sym_fac->get_symbol(current_holder_, s, false);
        }
#endif
    }

    warning_("bad argument for ^", stat, NULL_ZVALUE);
    return NULL_ZVALUE;
}

BUILDINDEF(is)
{
    if (FIRST_ARG==SECOND_ARG)
    {
        test_results_recorder_->markAsSuccess();
        return INTEGER_TO_ZVALUE(1);
    }
    else
    {
        test_results_recorder_->markAsFailure(
            std::string("got: [")
            + zvalue_to_string(FIRST_ARG)
            + std::string("] expected: [")
            + zvalue_to_string(SECOND_ARG)
            + std::string("]"));
        return INTEGER_TO_ZVALUE(0);
    }
}

BUILDINDEF(is_upper)
{
    if (ZSYMBOLP(FIRST_ARG))
    {
        const char *s = ZSYMBOLC(FIRST_ARG)->get_string();
	if (simpleWillBeTouchedWhenHeadConverted(*lowerCaseConverter_, std::string(s)))
            return INTEGER_TO_ZVALUE(1);
    }

    return NULL_ZVALUE;
}

BUILDINDEF(is_variant)
{
    if (node->is_variant) return INTEGER_TO_ZVALUE(1);
    else return NULL_ZVALUE;
}

BUILDINDEF(label)
{
    return ZOBJECT_TO_ZVALUE(node->getLabel());
}

BUILDINDEF(lc)
{
    if (!ZSYMBOLP(FIRST_ARG))
    {
        warning_("first argument of lc should be a string", stat, NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    else
    {
        const char* s = ZSYMBOLC(FIRST_ARG)->get_string();

        std::string outputString = simpleConvert(*lowerCaseConverter_, std::string(s));

        char *t = new char[strlen(s)+1];
        strncpy(t, outputString.c_str(), strlen(s)+1);

        zsymbol* zs = z_sym_fac->get_symbol(current_holder_, t, true);
        return zs;
    }
}


BUILDINDEF(less)
{
    if (INTEGERP(FIRST_ARG) && INTEGERP(SECOND_ARG))
    {
        return INTEGER_TO_ZVALUE(ZVALUE_TO_INTEGER(FIRST_ARG) < ZVALUE_TO_INTEGER(SECOND_ARG));
    }
    else if (NULLP(FIRST_ARG))
    {
        // @todo for compatibility false is returned when
        // the first argument is NULL, consider changing that
        return INTEGER_TO_ZVALUE(0);
    }
    else if (NULLP(SECOND_ARG))
    {
        return INTEGER_TO_ZVALUE(0);
    }
    else if (SECOND_ARG == sm_any)
    {
        return INTEGER_TO_ZVALUE(1);
    }
    else if (ZSYMBOLP(FIRST_ARG) && ZSYMBOLP(SECOND_ARG))
    {
#ifdef TRANSFERER_NO_SEMANTICS
        return NULL_ZVALUE;
#else
        return INTEGER_TO_ZVALUE(
            sem_hierarchy->isSubset(
                sem_hierarchy->getValue(ZSYMBOLC(FIRST_ARG)->get_string()),
                sem_hierarchy->getValue(ZSYMBOLC(SECOND_ARG)->get_string())));
#endif
    }

    warning_("bad argument for <", stat, NULL_ZVALUE);
    return NULL_ZVALUE;
}


BUILDINDEF(not)
{
    if (TRUEP(FIRST_ARG)) return INTEGER_TO_ZVALUE(0);
    return INTEGER_TO_ZVALUE(1);
}

BUILDINDEF(noteq)
{
    if (FIRST_ARG==SECOND_ARG) return INTEGER_TO_ZVALUE(0);
    return INTEGER_TO_ZVALUE(1);
}

BUILDINDEF(null)
{
    return NULL_ZVALUE;
}

BUILDINDEF(nounderscores)
{
// @todo raczej regu³y poprawiæ (?)
    if (ZVECTORP(FIRST_ARG))
    {
        zvector* v = ZVECTORC(FIRST_ARG);
        zvector* r = zvector::generate(current_holder_, v->getSize());

        for (int c=0; c<v->getSize(); ++c)
            r->add(nounderscores_(v->elementAt(c)));

        return r;
    }

    if (!ZSYMBOLP(FIRST_ARG)) {
        warning_("argument of nounderscores should be a string", stat, NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    return nounderscores_(FIRST_ARG);
}

BUILDINDEF(ok)
{
    if (TRUEP(FIRST_ARG))
        test_results_recorder_->markAsSuccess();
    else
        test_results_recorder_->markAsFailure();

    return FIRST_ARG;
}

BUILDINDEF(ordnum2pltext)
{
    if (!ZSYMBOLP(FIRST_ARG))
    {
        warning_("first argument of ordnum2pltext should be a string", stat, NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    else
    {
        std::string n = ZSYMBOLC(FIRST_ARG)->get_string();
        std::string tequiv;
        std::string tinflection;
        std::string tsyntax;
        ncnv::num2pltext(ncnv::ORD, n, tequiv, tinflection, tsyntax);

        zsymbol* ztequiv = z_sym_fac->get_symbol(current_holder_, tequiv.c_str(), false);
        zsymbol* ztinflection = z_sym_fac->get_symbol(current_holder_, tinflection.c_str(), false);
        return zpair::generate(current_holder_, ztequiv, ztinflection);
    }
}

BUILDINDEF(pair)
{
    return zpair::generate(current_holder_, FIRST_ARG, SECOND_ARG);
}

BUILDINDEF(parent)
{
    zsyntree* zst = node->parent_tree;
    if (NULL==zst) return NULL_ZVALUE;
    return ZOBJECT_TO_ZVALUE(zst);
}

BUILDINDEF(precedes)
{
    if (!ZSYNTREEP(node) || !ZSYNTREEP(FIRST_ARG))
    {
        warning_("argument of precedes should be a syntree", stat, NULL_ZVALUE);
        return NULL_ZVALUE;
    }

    if (ZSYNTREEC(node)->parent_tree ==
        ZSYNTREEC(FIRST_ARG)->parent_tree &&
        ZSYNTREEC(node)->number_in_parent_tree <
        ZSYNTREEC(FIRST_ARG)->number_in_parent_tree)
        return INTEGER_TO_ZVALUE(1);

    return INTEGER_TO_ZVALUE(0);
}

BUILDINDEF(plus)
{
    if (NULLP(FIRST_ARG)) return SECOND_ARG;
    if (NULLP(SECOND_ARG)) return FIRST_ARG;

    if (ZSYMBOLP(FIRST_ARG) || ZSYMBOLP(SECOND_ARG))
    {
        return concat_(FIRST_ARG, SECOND_ARG);
    }
    else if (INTEGERP(FIRST_ARG) && INTEGERP(SECOND_ARG))
    {
        return INTEGER_TO_ZVALUE(ZVALUE_TO_INTEGER(FIRST_ARG) +
                ZVALUE_TO_INTEGER(SECOND_ARG));
    }

    warning_("bad argument for +", stat, NULL_ZVALUE);
    return NULL_ZVALUE;
}

BUILDINDEF(replace_with)
{
    if (NULL==node->parent_tree) {
        warning_("'replace_with' can not replace current node because it is the root node",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    if (!ZSYNTREEP(FIRST_ARG)) {
        warning_("first argument of 'replace_with' should be a string or a node",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    zsyntree* zst = ZSYNTREEC(FIRST_ARG);
    if (node!=zst->parent_tree) {
        warning_("'replace_with' can not replace current node with a tree that already has a parent",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    node->replaceWith(zst);
    return ZOBJECT_TO_ZVALUE(node);
}

BUILDINDEF(second)
{
    if (!ZPAIRP(FIRST_ARG)) {
        warning_("argument of first should be a pair",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    return ZPAIRC(FIRST_ARG)->getSecond();
}

BUILDINDEF(semmatch)
{
    if (NULLP(FIRST_ARG) || NULLP(SECOND_ARG))
    {
        return NULL_ZVALUE;
    }
    else
    {
        int s = -9999L;

#ifndef TRANSFERER_NO_SEMANTICS
        dsem_hierarchy->match(FIRST_ARG,NULL_ZVALUE,SECOND_ARG, s);
#endif

        if (s < 0)
            return NULL_ZVALUE;

        return INTEGER_TO_ZVALUE(s);
    }
}

BUILDINDEF(set)
{
    if (!ZSYMBOLP(FIRST_ARG)) {
        warning_("argument of set should be a string",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    node->setAttr(ZSYMBOLC(FIRST_ARG),SECOND_ARG);
    return SECOND_ARG;
}

BUILDINDEF(show_tree)
{
    char* r = node->zsyntree_to_string();
    char* s = new char[strlen(r)+3];
    strcpy(s, "\n");
    strcat(s,r);
    delete r;
    strcat(s, "\n");
    warning_(s,stat,NULL_ZVALUE);
    delete s;
    return ZOBJECT_TO_ZVALUE(node);
}

BUILDINDEF(strip_quotes)
{
    if (!ZSYMBOLP(FIRST_ARG)) {
        warning_("argument of strip_quotes should be a string",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    return strip_quotes_(FIRST_ARG);
}

BUILDINDEF(substring)
{
    if (!ZSYMBOLP(FIRST_ARG)) {
        warning_("first argument of suffix should be a string",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    } else if (!INTEGERP(SECOND_ARG)) {
        warning_("second argument of suffix should be an integer",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    } else if (!INTEGERP(THIRD_ARG) && !DEFAULTP(THIRD_ARG)) {
        warning_("third argument of suffix should be an positive integer",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    const char *s = ZSYMBOLC(FIRST_ARG)->get_string();
    int beg = ZVALUE_TO_INTEGER(SECOND_ARG);
    int len = DEFAULTP(THIRD_ARG) ? ( strlen(s) - beg ) : ZVALUE_TO_INTEGER(THIRD_ARG);

    return subsymbol_(s,beg,len);
}

BUILDINDEF(suffix)
{
    if (!ZSYMBOLP(FIRST_ARG)) {
        warning_("first argument of suffix should be a string",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    } else if (!INTEGERP(SECOND_ARG) && ZVALUE_TO_INTEGER(SECOND_ARG)<=0) {
        warning_("second argument of suffix should be a positive integer",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    const char* s = ZSYMBOLC(FIRST_ARG)->get_string();
    int suffix_len = ZVALUE_TO_INTEGER(SECOND_ARG);

    return subsymbol_(s, -suffix_len, suffix_len);
}

BUILDINDEF(surface)
{
    if (node->defined(sm_Equiv))
        return nounderscores_(node->fetch(sm_Equiv));
    else if (node->defined(sm_TEquiv))
        return nounderscores_(node->fetch(sm_TEquiv));
    else if (node->last_subtree == -1L)
        return strip_quotes_(node->getCategory());

    return NULL_ZVALUE;
}

BUILDINDEF(surface_sm)
{
    if (node->defined(sm_LexId))
        return node->fetch(sm_LexId);
    else if (node->defined(sm_Equiv) ||
            node->defined(sm_TEquiv) ||
            node->last_subtree == -1L)
        return sm_zero_string;

    return NULL_ZVALUE;
}

BUILDINDEF(this)
{
    return ZOBJECT_TO_ZVALUE(node);
}

BUILDINDEF(tree_representation)
{
    return ZOBJECT_TO_ZVALUE(z_sym_fac->get_symbol(current_holder_, tree_representation_(node).c_str(), false));
}


BUILDINDEF(variant)
{
    return INTEGER_TO_ZVALUE(1);
}

BUILDINDEF(ucfirst)
{
    if (!ZSYMBOLP(FIRST_ARG))
    {
        warning_("first argument of ucfirst should be a string",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    else
    {
        const char* s = ZSYMBOLC(FIRST_ARG)->get_string();

        std::string outputString = simpleHeadConvert(*upperCaseConverter_, std::string(s));
        char *t = new char[outputString.length()+1];
        strncpy(t, outputString.c_str(), outputString.length()+1);
        zsymbol* zs = z_sym_fac->get_symbol(current_holder_, t, true);
        return zs;
    }
}

BUILDINDEF(undefined)
{
    if (NULLP(FIRST_ARG)) return INTEGER_TO_ZVALUE(1);
    return INTEGER_TO_ZVALUE(0);
}

BUILDINDEF(warning)
{
    if (ZSYNTREEP(FIRST_ARG)) std::cerr << "\n";

    char* s = zvalue_to_string(FIRST_ARG);
    std::cerr << s;
    delete s;
    std::cerr << "\n";

    return NULL_ZVALUE;
}

BUILDINDEF(do_transfer)
{
    if (!ZSYNTREEP(FIRST_ARG)) {
        warning_("first argument of cut_suffix should be a string",stat,NULL_ZVALUE);
        return NULL_ZVALUE;
    }
    return doTransfer(ZSYNTREEC(FIRST_ARG),NULL,NULL);
}

#undef THIRD_ARG
#undef SECOND_ARG
#undef FIRST_ARG

#undef BUILDINDEF

std::string Transferer::tree_representation_(zsyntree* t)
{
    if (t == NULL) return std::string("???");

    if (t->last_subtree >= 0 && t->is_variant)
    {
            return tree_representation_(ZSYNTREEC(t->fetch(INTEGER_TO_ZVALUE(0))));
    }

    std::string r;

    std::string category;
    if (t->category)
        category = zvalue_to_string(t->category);
    else
        category = std::string("?");

    r = category;

    if (category.length() >= 4 &&
       category[0] == '\'' &&
       category[1] == '$' &&
       category[category.length()-1] == '\'')
    {
        bool separator = false;
        r += "(";
        r += tree_representation_attr_(t, "A", separator);
        r += tree_representation_attr_(t, "C", separator);
        r += tree_representation_attr_(t, "L", separator);
        r += tree_representation_attr_(t, "O", separator);
        r += tree_representation_attr_(t, "P", separator);
        r += tree_representation_attr_(t, "R", separator);
        r += ")";
    }

    if (t->last_subtree >= 0)
    {
        r += "[";
        int i;
        for (i = 0; i <= t->last_subtree; ++i)
        {
            zsyntree* zst = ZSYNTREEC(t->fetch(INTEGER_TO_ZVALUE(i)));
            if (i > 0) r += ",";
            r += tree_representation_(zst);
        }
        r += "]";
    }
    else
    {
        bool separator = false;
        r += "(";
        r += tree_representation_attr_(t, "Beg", separator);
        r += tree_representation_attr_(t, "Length", separator);
        r += ")";
    }

    return r;
}

std::string Transferer::tree_representation_attr_(zsyntree* t, std::string attr,bool& separator)
{
    std::string r;

    zvalue z = t->getAttr(z_sym_fac->get_symbol(current_holder_, attr.c_str(), false));

    if (NULLP(z))
    {
        return "";
    }

    std::string v = zvalue_to_string(z);

    if (v.length() > 0)
    {
        if (separator) r = ",";

        r += attr;
        r += "=";
        r += v;

        separator = true;
    }

    return r;
}

void Transferer::startHolderRegistration()
{
    current_holder_ = my_holder_;
}

void Transferer::stopHolderRegistration()
{
    zobject::freeZObjects(current_holder_);
    current_holder_ = 0;
}

zobjects_holder* Transferer::getZObjectsHolder()
{
    return current_holder_;
}
