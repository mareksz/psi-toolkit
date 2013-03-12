/*
  Copyright (C) 2007-2011 Poleng Sp. z o.o.

  This file can be redistributed and/or modified under the terms of the
  GNU Lesser General Public Licence as published by the Free Software
  Foundation.
*/

// program do uruchamiania reguł transferu do celów testowych

#include <boost/program_options.hpp>
#include <boost/scoped_ptr.hpp>
namespace po = boost::program_options;

#include <stdexcept>
#include <string>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <fstream>

#include "transferer.hpp"
#include "zvalue.hpp"
#include "tscript_from_file.hpp"

#include <boost/shared_ptr.hpp>

std::vector<std::string> rule_files;
//std::string semhier_file("PE-semantics.xml");

zobjects_holder* main_holder = NULL;
zsymbolfactory* z_sym_fac = NULL;
//SemanticsHierarchy sem_hierarchy;
//DomainHierarchy* dsem_hierarchy = NULL;

boost::shared_ptr<TestResultsRecorder> test_results_recorder;

void go();
void run_file(const std::string& rule_filename);
void run_pfile(const std::string& rule_filename);

int main(int argc, char* argv[])
{
    po::options_description desc("Dostępne opcje");

    try
    {
        desc.add_options()
            ("help", "tekst pomocy")
            ("rule-files", po::value<std::vector<std::string> >(&rule_files)->composing(), "pliki z regułami transferu")
//            ("semhier-file", po::value<std::string>(&semhier_file), "plik z listą semantyk")
            ;

        po::positional_options_description p;
        p.add("rule-files", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
                  options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return 1;
        }

        if (rule_files.empty())
        {
            std::cerr << "nie podano pliku z regułami\n";
            return 1;
        }

        go();
    }
    catch(std::exception& e)
    {
        std::cerr << "błąd w parametrach programu: " << e.what() << "\n";
        return 1;
    }

    return test_results_recorder->wereTestsPassed() ? 0 : 1;
}

void go()
{
    test_results_recorder.reset(new TestResultsRecorder);

    main_holder = zvector::generate(EMPTY_ZOBJECTS_HOLDER, 32000);
    z_sym_fac = new zsymbolfactory(zsymboltable::generate(main_holder));
//    dsem_hierarchy = new DomainHierarchy(z_sym_fac, main_holder);

//    sem_hierarchy.init(semhier_file);
//    dsem_hierarchy->init(semhier_file.c_str());

    for (int i = 0; i < rule_files.size(); ++i)
        run_file(rule_files[i]);

    zobject::freeZObjects(main_holder);
    main_holder->annihilate();

//    delete dsem_hierarchy;
}

void run_file(const std::string& rule_filename)
{
    std::string contents;
    std::string line;
    std::ifstream s(rule_filename.c_str());
    while (getline(s, line))
    {
        contents += line;
        contents += "\n";
    }

    std::string tempfile(tempnam(NULL, "trunner"));
    std::ofstream tf(tempfile.c_str());
    tf << contents;
    tf.close();

    run_pfile(tempfile);

    unlink(tempfile.c_str());
}

void run_pfile(const std::string& rule_filename)
{
    tmil::ScriptFactory* sf = new tmil::FileParsingScriptFactory();

    boost::scoped_ptr<tmil::Transferer> ap_transferer(
        new tmil::Transferer(
            sf,
            main_holder,
//            &sem_hierarchy,
//            &(*dsem_hierarchy),
            z_sym_fac,
            false));

    ap_transferer->include(rule_filename.c_str());
    ap_transferer->setTestResultsRecorder(test_results_recorder);

    zsyntree* syntree = zsyntree::generate(main_holder);
    zvalue xcat = z_sym_fac->get_symbol("X");
    syntree->setCategory(ZSYMBOLC(xcat));

    ap_transferer->run(syntree, z_sym_fac->get_symbol("build"), zvector::generate(main_holder,0));

    ap_transferer->doTranslate(syntree);

// @todo?    delete sf;
}
