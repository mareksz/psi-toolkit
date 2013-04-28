#pragma once

/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

#include "zvalue.hpp"
#include "trules.hpp"
#include "tscript_factory.hpp"

#include "test_results_recorder.hpp"

#define TRANSFERER_NO_SEMANTICS
#ifndef TRANSFERER_NO_SEMANTICS
#include "semantics-hierarchy.h"
#include "dhierarchy.h"
#endif

#include "regexp.hpp"

#ifdef USE_LOG4CPLUS
#include <log4cplus/logger.h>
#endif

#include <vector>
#include <boost/shared_ptr.hpp>

#include "general_case_converter.hpp"
#include "string_case_converter_manager.hpp"
#include "simple_convert.hpp"


namespace tmil {

class Transferer
{
    private:
#ifdef USE_LOG4CPLUS
        static log4cplus::Logger logger;
#endif

    public:
        /**
         * Constructs a new Transferer object.
         *
         * @param  script_factory   A producer for TScript structures.
         */
        Transferer(ScriptFactory* script_factory
                   , zobjects_holder* main_holder
#ifndef TRANSFERER_NO_SEMANTICS
                   , SemanticsHierarchy* a_sem_hierarchy
                   , DomainHierarchy* a_dsem_hierarchy
#endif
                   , zsymbolfactory* a_zsymfac=NULL
                   , bool structure_mode=false);
        ~Transferer();

        /**
         * Reads a script using current script factory.
         *
         * @param  name  The name of the script. Usually a name of a text file
         *               with a script.
         */
        void include(const char* name);

        void setTestResultsRecorder(boost::shared_ptr<TestResultsRecorder> recorder);

        /**
         * Executes the subroutine <code>sub_name</code> as
         * <code>node</code>'s method with arguments <code>args</code>
         * (given in the reverse order!)
         *
         * @return Value returned by the subroutine
         */
        zvalue run(zsyntree* node,
                zsymbol* sub_name,
                zvector* args);

        /**
         * Executes the subroutine sub_name for all nodes of the tree.
         * It starts with the leaves.
         *
         * @return Value returned for the root node
         */
        zvalue runRecursivelyBottomUp(zsyntree* node, zsymbol* sub_name, zvector* args);

        /**
         * Gets a zsymbol factory. Using this factory object you can create
         * zsymbol that can be passed to the #run(zobject*, zsymbol*) method.
         * All zsymbol created this way are owned by this Transferer object.
         *
         * @return  A zsymbol factory.
         */
        zsymbolfactory* get_zsymbolfactory();

        /**
         * Sets options.
         * An option is a parameter that may be used by transfer scripts.
         *
         * @param  an_option_name   The name of the option to be set.
         * @param  an_option_value  The new value for this option.
         */
        void setOption(zsymbol* an_option_name, zvalue an_option_value);

        void unsetOption(zsymbol* an_option_name);

        /**
         * Creates the eqv-tree using TransferSpec procedures.
         *
         * @param  a_source_tree  A tree to be transfered into an eqv-tree.
         * @param  a_parent_node
         *
         * @return The eqv-tree.
         */
        zsyntree* doTransfer(zsyntree* a_source_tree, TransferSpecHash* a_local_transfer_spec_hash, zsyntree* a_parent_node);

        /**
         * Runs modification stage (interpretes m-rules)
         *
         * @param a_tree A tree to be modified
         */
        void doModification(zsyntree* a_tree);

        /**
         * Runs special stage for generating a variant
         *
         * @param a_tree a tree to be modifed
         * @param a_model_tree a tree from which values of some attributes
         * should be taken
         */
        void doVariant(zsyntree* a_tree, zsyntree* a_model_tree);

        /**
         * Runs inflection (surface-generating) stage (interpretes i-rules)
         *
         * @param a_tree A tree to be analyzed
         */
        void doSurface(zsyntree* a_tree);

        zsymbol* getSurfSymbol();
        zsymbol* getNrSymbol();
        zsymbol* getNilSymbol();
        zsymbol* getEidSymbol();

        /**
         * Translates a syntactic tree - runs all the stages
         * (transfer, modification, inflection) of translation process
         *
         * @param a_source_tree a tree to be translated
         * @param ptt @todo ask TK
         * @param a_model_tree a tree from which values of some attributes
         * should be taken (used only when generating a variant
         * @return a translated tree (in the target language)
         */
        zsyntree* doTranslate(zsyntree* a_source_tree,
                              char** ptt = NULL,
                              zsyntree* a_model_tree = NULL);

        void startHolderRegistration();
        void stopHolderRegistration();
        zobjects_holder* getZObjectsHolder();

    private:

        // - - - - - - - - - - - - FIELDS - - - - - - - - - - - -

        zobjects_holder* my_holder_;
        zobjects_holder* current_holder_;

        // obiekt nie jest wlascicielem tego holdera
        zobjects_holder* main_holder_;

        /**
         * Faster access to transferspecs.
         */
        TransferSpecHash* global_transfer_spec;

        /**
         * @see setOption(zsymbol*, zvalue)
         */
        zenvironment* options;

        /**
         * Script management field.
         *
         * Key:   name (zsymbol)
         * Value: a pair of name and script ( zpair(zsymbol, TScript) )
         *
         * @see #include(const char*)
         */
        zhash* name_to_script;

        /**
         * Faster access to subroutines.
         *
         * Key: category and name of subroutine
         *      category can be NULL_ZVALUE
         *      ( zpair(zsymbol, zsymbol) )
         *
         * Value: a pair of key and subroutine
         *        ( zpair( zpair(zsymbol, zsymbol), SubroutineSlot) )
         */
        zhash* cat_and_name_pair_to_subroutine;

        /**
         * A script factory used to obtain TScript instances.
         */
        ScriptFactory* script_factory;

        /**
         * Factory for zsymbols. Just like #z_sym_tab but without exposing
         * the whole zsymboltable interafce.
         */
        zsymbolfactory* z_sym_fac;

        /**
         * Various symbols used explicitly
         */
        zsymbol* sm_token_assign;
        zsymbol* sm_token_arrow;
        zsymbol* sm_token_bind;
        zsymbol* sm_token_dot;
        zsymbol* sm_token_and;
        zsymbol* sm_token_or;
        zsymbol* sm_any;
        zsymbol* sm_default;
        zsymbol* sm_variant;
        zsymbol* sm_this;
        zsymbol* sm_out;
        zsymbol* sm_E_returned;
        zsymbol* sm_surface;
        zsymbol* sm_gensurface;
        zsymbol* sm_Surf;
        zsymbol* sm_Equiv;
        zsymbol* sm_TEquiv;
        zsymbol* sm_Eid;

        zsymbol* sm_zero_string;
        zsymbol* sm_LexId;

        zsymbol* sm_Nr;
        zsymbol* sm_nil;

        zsymbol* sm_main_subtree_label;

#ifndef TRANSFERER_NO_SEMANTICS
        SemanticsHierarchy* sem_hierarchy;
        DomainHierarchy* dsem_hierarchy;
#endif

        typedef zvalue (Transferer::*fun_build_in)(zvector* args,
                zsyntree* node,
                Statement* S,
                int nb_arguments);

        struct SubroutineSlot
        {
            bool is_build_in;
            union
            {
                fun_build_in internal_subr;
                Subroutine* defined_subr;
            };

            int nb_arguments; // Used only for build-ins

            /**
             * Gets the number of arguments expected for the subroutine
             */
            int getNbArguments() const
            {
                if(is_build_in) return nb_arguments;
                else return defined_subr->args->getSize();
            }
        };

        // - - - - - - - - - - - - FUNCTIONS - - - - - - - - - - - -

        /** helper for #include() */
        void hashTransferSpec(TransferSpec* ts);

        /** helper for #include() */
        void hashSubroutine(Subroutine* s);

        void registerBuildIn(fun_build_in cfun, const char* name, int nb_args);

        /**
         * Gets the definition of a subroutine of a given
         * <code>name</code> for category <code>cat</code>
         *
         * @param  cat   Category name. NULL permitted (fe. build-in subroutines)
         * @param  name  Subroutine name.
         *
         * @return The subroutine definition.
         *         NULL if a subroutine does not exist.
         */
        SubroutineSlot* findSubroutine_(zsymbol* cat, zsymbol* name);


        /**
         *
         */
        zenvironment* args2Env_(zvector* args, zsyntree* node,
                Subroutine* subr);

        /**
         *
         */
        void compute_(Statement* S, zvector* DSS,
                zvector* OSS, zenvironment* SE);

        void warning_(const char* message, Statement* SP, zvalue V);



        /**
         * Fake expression for invoking subroutines
         */
        SubrExpression* fake_expression;

        /**
         * Helper for all string operations.
         *
         * subsymbol_("0123456789",  2, 3)   ---> "234"   [ OK ]
         * subsymbol_("0123456789", -8, 3)   ---> "234"   [ OK ]
         * subsymbol_("0123456789",-15, 3)   ---> "012"   [ beg to low ]
         * subsymbol_("0123456789", 15, 3)   ---> ""      [ beg to high ]
         * subsymbol_("0123456789",  8, 3)   ---> "89"    [ len to high ]
         * subsymbol_("0123456789",  8,-1)   ---> ""      [ len to low ]
         *
         * @return  A symbol that is the sunstring of <code>s</code>.
         */
        zsymbol* subsymbol_(const char* s, int beg, int len);

        zvalue concat_(zvalue a, zvalue b);
        zvalue concat_with_space_(zvalue a, zvalue b);
        zvalue nounderscores_(zvalue sm);
        zvalue strip_quotes_(zvalue sm);
        zvalue symbol_wanted_(zvalue z);
        zvalue integer_wanted_(zvalue z);

        zvalue match_regexp_(zvalue a,
                             PerlRegExp& PREG,
                             zenvironment* E);

#define BUILDINDECL(S) zvalue S##I(zvector*, zsyntree*, Statement*, int)

        BUILDINDECL(atoi);
        BUILDINDECL(cat);
        BUILDINDECL(change_category);
        BUILDINDECL(change_label);

        // @todo chyba nieużywana, do usunięcia?
        BUILDINDECL(clear);

        BUILDINDECL(cut_suffix);
        BUILDINDECL(default);
        BUILDINDECL(delete);
        BUILDINDECL(do_surface);

        // operator ==
        BUILDINDECL(eq);
        BUILDINDECL(eqv);
        BUILDINDECL(et);
        BUILDINDECL(find);
        BUILDINDECL(find_left);
        BUILDINDECL(find_right);
        BUILDINDECL(first);
        BUILDINDECL(first_subtree);
        // @todo ??
        //BUILDINDECL(gensurface);
        // @todo używane?
        BUILDINDECL(get);
        // @todo opisać?
        BUILDINDECL(get_dvalue);
        BUILDINDECL(get_morpho);
        BUILDINDECL(get_morpho_ru);
        BUILDINDECL(get_morpho_de);
        BUILDINDECL(get_morpho_es);
        BUILDINDECL(get_option);
        BUILDINDECL(inherit);
        BUILDINDECL(insert_after);
        BUILDINDECL(insert_before);
        BUILDINDECL(insert_left);
        BUILDINDECL(insert_right);
        /**
         * ^ operator
         */
        BUILDINDECL(intersection);

        BUILDINDECL(is);
        BUILDINDECL(is_upper);
        BUILDINDECL(is_variant);
        BUILDINDECL(label);
        BUILDINDECL(lc);

        /**
         * < operator
         */
        BUILDINDECL(less);

        /**
         * Replaces underscores with spaces.
         *
         * @param   SYMBOL  A symbol.
         *
         * @return    NULL  /1/ bad argument (not a symbol)
         *                  /2/ symbol is a single underscore
         *          SYMBOL  A symbol with spaces instead of underscores.
         */
        BUILDINDECL(nounderscores);

        /**
         * ! operator
         */
        BUILDINDECL(not);

        /**
         * != operator
         */
        BUILDINDECL(noteq);
        BUILDINDECL(null);
        BUILDINDECL(ok);
        BUILDINDECL(ordnum2pltext);
        BUILDINDECL(pair);
        BUILDINDECL(parent);
        BUILDINDECL(plus);
        BUILDINDECL(precedes);
        BUILDINDECL(replace_with);
        BUILDINDECL(second);
        BUILDINDECL(semmatch);
        BUILDINDECL(set);
        BUILDINDECL(show_tree);
        BUILDINDECL(strip_quotes);
        BUILDINDECL(substring);
        BUILDINDECL(suffix);
        BUILDINDECL(surface);
        BUILDINDECL(surface_sm);
        BUILDINDECL(this);
        BUILDINDECL(tree_representation);
        BUILDINDECL(ucfirst);
        BUILDINDECL(variant);
        BUILDINDECL(undefined);
        BUILDINDECL(warning);

        BUILDINDECL(do_transfer);

#undef BUILDINDECL

        std::string tree_representation_(zsyntree* t);
        std::string tree_representation_attr_(zsyntree* t, std::string attr, bool& separator);

        std::vector<TScript> included_scripts_;
        std::vector<SubroutineSlot*> allocated_sub_slots_;

        boost::shared_ptr<TestResultsRecorder> test_results_recorder_;

    boost::shared_ptr<GeneralCaseConverter<std::string::const_iterator,
                                           std::back_insert_iterator<std::string> > >
       lowerCaseConverter_;


    boost::shared_ptr<GeneralCaseConverter<std::string::const_iterator,
                                           std::back_insert_iterator<std::string> > >
       upperCaseConverter_;


};

}; //namespace tmil
