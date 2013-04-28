#ifndef ME_TAGGER_HDR
#define ME_TAGGER_HDR

#include "config.hpp"
#include "annotator.hpp"
#include "language_dependent_annotator_factory.hpp"
#include "lang_specific_processor_file_fetcher.hpp"
#include <boost/program_options.hpp>
#include "regexp.hpp"
#include <fstream>

#include <string>
#include <vector>
#include <map>
#include "maxentmodel.hpp"

class MeTagger : public Annotator {
    public:
        class Factory : public LanguageDependentAnnotatorFactory {
            virtual Annotator* doCreateAnnotator(
                    const boost::program_options::variables_map& options);

            virtual void doAddLanguageIndependentOptionsHandled(
                boost::program_options::options_description& optionsDescription);

            virtual std::string doGetName() const;
            virtual boost::filesystem::path doGetFile() const;

            virtual std::list<std::list<std::string> > doRequiredLayerTags();
            virtual std::list<std::list<std::string> > doOptionalLayerTags();
            virtual std::list<std::string> doProvidedLayerTags();

            static const std::string DEFAULT_MODEL_FILE;
            static const int DEFAULT_ITERATIONS;
            static const std::string DEFAULT_UNKNOWN_POS_LABEL;
        };

        MeTagger(const std::string&, const std::string&, bool,
                 const std::string&);
        
        ~MeTagger();
        
        void tag(Lattice &lattice);
        
        void loadModel(std::string path);
        void train(Lattice &lattice);
        void prepareTraining();
        void finalizeTraining();
        
        void saveModel(std::string path);
        bool training();

        std::string getModelFile();

    private:
        class Worker : public LatticeWorker {
            public:
                Worker(MeTagger& processor, Lattice& lattice);
                
            private:
                virtual void doRun();
                MeTagger& processor_;
        };
        virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice);
        virtual std::string doInfo();

        std::string langCode_;

        typedef std::string Outcome;
        typedef std::string Feature;
        typedef std::vector<Feature> Context;
        
        typedef std::map<int, Lattice::EdgeDescriptor> TokenEdgesMap;

        std::string modelFile_;
        
        std::string unknownPosLabel_;
        
        RegExp rxUpperCaseFirst_;
        RegExp rxUpperCaseAll_;
        RegExp rxContainsNumber_;
        RegExp rxIsNumber_;
        RegExp rxContainsLetter_;
        RegExp rxContainsPunct_;
        RegExp rxHasFix_;        
        
        maxent::MaxentModel model_;
        
        std::vector<std::string> openClasses_;
        
        bool trainMode_;
        const std::string trainTagsFile_;
        std::vector<Outcome> outcomes_;
        
        bool openForEvents_;
        bool posModelLoaded_;
        size_t tokenNo_;
        size_t trainIterations_;

        
        void readOutcomes();
        
        TokenEdgesMap createTokenEdgesMap(
                Lattice &lattice, Lattice::VertexDescriptor start,
                Lattice::VertexDescriptor end);
        void tagSegment(Lattice &lattice, TokenEdgesMap tokenEdgesMap);
        
        std::vector<Outcome> getTokenTags(Lattice &lattice,
                TokenEdgesMap tokenEdgesMap);
        Outcome getBestTag(Lattice &lattice, Lattice::EdgeDescriptor token,
                Context context);
        
        void applyTokenTags(Lattice &lattice, TokenEdgesMap tokenEdgesMap,
                std::vector<Outcome> tags);
        
        Context createContext(Lattice &lattice,
                TokenEdgesMap tokenEdgesMap,
                int currentIndex, int window);

        void addSampleSentences(Lattice &lattice);
        void addSampleSegment(Lattice &lattice, TokenEdgesMap tokenEdgesMap);
        
        void addCurrentTag(Lattice &lattice, Lattice::EdgeDescriptor token,
                Context context);
        
        std::string getFormLemma(Lattice &lattice,
                Lattice::EdgeDescriptor edge);
        
        std::string getFormPartOfSpeech(Lattice &lattice,
                Lattice::EdgeDescriptor edge);
        std::string getFormMorphoTag(Lattice &lattice,
                Lattice::EdgeDescriptor edge);
        std::string getPartOfSpeechFromMorphoTag(std::string tag);
        std::list<std::pair<std::string, std::string> >
            getAttributesFromMorphoTag(std::string tag);
        bool formMatchesTag(Lattice &lattice, Lattice::EdgeDescriptor form,
                MeTagger::Outcome tag);
        bool lemmaEdgeExists(Lattice &lattice, Lattice::EdgeDescriptor token,
                std::string lemma);
        bool lexemeEdgeExists(Lattice &lattice, Lattice::EdgeDescriptor token,
                std::string lemma, std::string partOfSpeech);
        void addLemmaEdge(Lattice &lattice, Lattice::EdgeDescriptor token,
                std::string lemma);
        Lattice::EdgeDescriptor addLexemeEdge(Lattice &lattice,
                Lattice::EdgeDescriptor token, std::string lemma,
                std::string partOfSpeech);
        void addFormEdge(Lattice &lattice, Lattice::EdgeDescriptor token,
                Lattice::EdgeDescriptor lexeme, std::string tag);
        bool isDiscarded(Lattice &lattice, Lattice::EdgeDescriptor edge);
        bool hasLexemeEdgeMatchingTag(Lattice &lattice,
                Lattice::EdgeDescriptor token, std::string tag);
        Lattice::EdgeDescriptor getLexemeEdgeMatchingTag(Lattice &lattice,
                Lattice::EdgeDescriptor token, std::string tag);
        bool lexemeEdgeMatchesTag(Lattice &lattice, Lattice::EdgeDescriptor lexeme,
                std::string tag);
};

#endif
