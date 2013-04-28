#include "me_tagger.hpp"
#include <sstream>

Annotator* MeTagger::Factory::doCreateAnnotator(
        const boost::program_options::variables_map& options) {
    std::string lang = options["lang"].as<std::string>();
    bool train = false;
    
    LangSpecificProcessorFileFetcher fileFetcher(__FILE__, lang);
    std::string modelPathString = "";
    std::string trainPathString = "";
    
    if (options.count("model")) {
        std::string modelFilename = options["model"].as<std::string>();
        if (!options.count("train")) {
            boost::filesystem::path modelPath;
            modelPath = fileFetcher.getOneFile(modelFilename);
            modelPathString = modelPath.string();
        } else {
            train = true;
            modelPathString = modelFilename;
            trainPathString
                = options["train-tags"].as<std::string>();
        }
    }
    
    MeTagger* tagger = new MeTagger(
            lang, modelPathString != "" ? modelPathString : DEFAULT_MODEL_FILE,
            train, trainPathString);
    
    if (!options.count("train")) {
        tagger->loadModel(tagger->getModelFile());
    }
    return tagger;
}

void MeTagger::Factory::doAddLanguageIndependentOptionsHandled(
        boost::program_options::options_description& optionsDescription) {

    optionsDescription.add_options()
        ("model", boost::program_options::value<std::string>()
         ->default_value(DEFAULT_MODEL_FILE), "model file")
        ("iterations", boost::program_options::value<int>()
         ->default_value(DEFAULT_ITERATIONS), "number of iterations")
        ("unknown-pos", boost::program_options::value<std::string>()
         ->default_value(DEFAULT_UNKNOWN_POS_LABEL),
            "unknown part of speech label")
        ("open-class-labels",
         boost::program_options::value<std::vector<std::string> >()
         ->multitoken(), "open class labels")
        ("train", "training mode")
        ("train-tags", boost::program_options::value<std::string>(),
         "train tags file")
        ("save-model", "saves model in training mode")
        ;
}

std::string MeTagger::Factory::doGetName() const {
    return "metagger";
}

boost::filesystem::path MeTagger::Factory::doGetFile() const {
    return __FILE__;
}

std::list<std::list<std::string> > MeTagger::Factory::doRequiredLayerTags() {
    return boost::assign::list_of(
        boost::assign::list_of
            (std::string("lemma"))
    );
}

std::list<std::list<std::string> > MeTagger::Factory::doOptionalLayerTags() {
    return std::list<std::list<std::string> >();
}

std::list<std::string> MeTagger::Factory::doProvidedLayerTags() {
    return std::list<std::string>();
}

const std::string MeTagger::Factory::DEFAULT_MODEL_FILE
= "%ITSDATA%/%LANG%.blm";
const int MeTagger::Factory::DEFAULT_ITERATIONS = 100;
const std::string MeTagger::Factory::DEFAULT_UNKNOWN_POS_LABEL = "unknown";

LatticeWorker* MeTagger::doCreateLatticeWorker(Lattice& lattice) {
    return new Worker(*this, lattice);
}

MeTagger::Worker::Worker(MeTagger& processor, Lattice& lattice):
    LatticeWorker(lattice), processor_(processor) {  }

void MeTagger::Worker::doRun() {
    if (processor_.training()) {
        processor_.train(lattice_);
    }
    else
        processor_.tag(lattice_);
}

std::string MeTagger::doInfo() {
    return "maximum entropy pos tagger";
}

MeTagger::MeTagger(const std::string& langCode, const std::string& modelFile,
                   bool trainMode, const std::string& trainFile) :
    langCode_(langCode),
    openForEvents_(false), posModelLoaded_(false),
    trainMode_(trainMode), trainIterations_(100), trainTagsFile_(trainFile),
    modelFile_(modelFile),
    tokenNo_(0),
    unknownPosLabel_("unknown"),
    rxUpperCaseFirst_("^\\p{Lu}"),
    rxUpperCaseAll_("^\\p{Lu}+$"),
    rxContainsNumber_("\\d"),
    rxIsNumber_("^\\d+([.,]\\d+)(%)?$"),
    rxContainsLetter_("\\p{L}"),
    rxContainsPunct_("[.,;:?!<>%&'\\-\"/()\\[\\]+=_*$#@~|{}\\\\]"),
    rxHasFix_("\\-$") {
        
        if (training()) {
            prepareTraining();
        }  
    }
    
MeTagger::~MeTagger() {
    if (training()) {
        finalizeTraining();
        saveModel(getModelFile());
    }   
}

bool MeTagger::training() {
    return trainMode_;
}

void MeTagger::saveModel(std::string path) {

    model_.save(path, true);
}

void MeTagger::prepareTraining() {
    readOutcomes();
}

void MeTagger::finalizeTraining() {
    model_.end_add_event();
    openForEvents_ = false;
    maxent::verbose = 1;
    model_.train(100);
    maxent::verbose = 0;
}

void MeTagger::train(Lattice &lattice) {
    addSampleSentences(lattice);
}

void MeTagger::readOutcomes() {
    std::ifstream in(trainTagsFile_.c_str());
    Outcome tag;
    while(in >> tag)
        outcomes_.push_back(tag);
}

void MeTagger::addSampleSentences(Lattice &lattice) {
    if (!openForEvents_) {
        model_.begin_add_event();
        openForEvents_ = true;
    }
    
    LayerTagMask segmentMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "segment", langCode_);
    Lattice::EdgesSortedBySourceIterator segmentIt =
        lattice.edgesSortedBySource(segmentMask);
    if (!segmentIt.hasNext()) {
        TokenEdgesMap tokenEdgesMap = createTokenEdgesMap(lattice,
                lattice.getFirstVertex(), lattice.getLastVertex()
                    );
        addSampleSegment(lattice, tokenEdgesMap);
    }
    while (segmentIt.hasNext()) {
        Lattice::EdgeDescriptor segment = segmentIt.next();

        TokenEdgesMap tokenEdgesMap = createTokenEdgesMap(lattice,
                lattice.getEdgeSource(segment), lattice.getEdgeTarget(segment)
                    );
        addSampleSegment(lattice, tokenEdgesMap);
    }
}

void MeTagger::addSampleSegment(Lattice &lattice,
        TokenEdgesMap tokenEdgesMap) {
    
    for (TokenEdgesMap::iterator tokenIt = tokenEdgesMap.begin();
            tokenIt != tokenEdgesMap.end(); ++ tokenIt) {
        int tokenIndex = tokenIt->first;
        Lattice::EdgeDescriptor token = tokenIt->second;
        
        Context context = createContext(lattice, tokenEdgesMap,
                tokenIndex, 2);
        
        if (tokenIndex > 0) {
            Outcome prevTag = outcomes_[tokenIndex-1];
            context.push_back(Feature("prevtag=" + prevTag));
        }
       
        Outcome currentTag = outcomes_[tokenIndex];
        
        model_.add_event(context, currentTag);
    }
}

std::string MeTagger::getModelFile() {
    return modelFile_;
}

void MeTagger::loadModel(std::string path) {
    model_.load(path);
    posModelLoaded_ = true;
}

void MeTagger::tag(Lattice &lattice) {
    if (!posModelLoaded_) {
        std::cerr << "no model loaded. metagger will do nothing" << std::endl;
        return;
    }
    LayerTagMask segmentMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "segment", langCode_);
    Lattice::EdgesSortedBySourceIterator segmentIt =
        lattice.edgesSortedBySource(segmentMask);
    if (!segmentIt.hasNext()) {
        TokenEdgesMap tokenEdgesMap = createTokenEdgesMap(lattice,
                lattice.getFirstVertex(), lattice.getLastVertex()
                    );
        tagSegment(lattice, tokenEdgesMap);
    }
    while (segmentIt.hasNext()) {
        Lattice::EdgeDescriptor segment = segmentIt.next();
    
        TokenEdgesMap tokenEdgesMap = createTokenEdgesMap(lattice,
                lattice.getEdgeSource(segment), lattice.getEdgeTarget(segment)
                    );
        tagSegment(lattice, tokenEdgesMap);
    }
}

void MeTagger::tagSegment(Lattice &lattice, TokenEdgesMap tokenEdgesMap) {
    std::vector<Outcome> tags = getTokenTags(lattice, tokenEdgesMap);
    applyTokenTags(lattice, tokenEdgesMap, tags);
}

MeTagger::TokenEdgesMap MeTagger::createTokenEdgesMap(
        Lattice &lattice, Lattice::VertexDescriptor start,
        Lattice::VertexDescriptor end) {
    TokenEdgesMap tokenEdgesMap;
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "token", langCode_);
    Lattice::VertexDescriptor vertex = start;
    
    if(!training())
        tokenNo_ = 0;
    
    while (vertex < end) {
        Lattice::InOutEdgesIterator tokenIt = lattice.outEdges(vertex, tokenMask);
        if (! tokenIt.hasNext()) {
            ++ vertex;
            continue;
        }
        if (tokenIt.hasNext()) {
            Lattice::EdgeDescriptor token = tokenIt.next();

            if (!lattice.isBlank(token)) { //skip white spaces
                tokenEdgesMap.insert(std::pair<int, Lattice::EdgeDescriptor>(
                            tokenNo_,
                            token
                            ));
                tokenNo_ ++;
            }
            //std::cerr << tokenNo_ << std::endl;
            vertex = lattice.getEdgeTarget(token);
        }
    }
    return tokenEdgesMap;
}

MeTagger::Context MeTagger::createContext(Lattice &lattice,
        TokenEdgesMap tokenEdgesMap,
        int currentIndex, int window) {
    Context context;

    LayerTagMask formMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "form", langCode_);

    std::string currentOrth = "";

    TokenEdgesMap::iterator tokenIt =
        tokenEdgesMap.find(currentIndex);
    if (tokenIt == tokenEdgesMap.end())
        return context;

    Lattice::EdgeDescriptor token = tokenIt->second;
    currentOrth = lattice.getEdgeText(token);
    context.push_back(Feature("curr_word=" + currentOrth));
    context.push_back(Feature("curr_word_length=" +
                boost::lexical_cast<std::string>( currentOrth.size() )));

    Lattice::VertexDescriptor currentVertex =
            lattice.getEdgeSource(token);
    Lattice::InOutEdgesIterator formIt =
        lattice.outEdges(currentVertex, formMask);
    while (formIt.hasNext()) {
        Lattice::EdgeDescriptor form = formIt.next();
        if (isDiscarded(lattice, form))
            continue;
        std::string tag = getFormMorphoTag(lattice, form);
        std::string lemma = getFormLemma(lattice, form);
        context.push_back(Feature("curr_has_tag=" + tag));
        context.push_back(Feature("curr_has_lemma=" + lemma));
    }

    if (RegExp::PartialMatch(currentOrth, rxUpperCaseFirst_))
        context.push_back("UpperCaseFirst");
    if (RegExp::PartialMatch(currentOrth, rxUpperCaseAll_))
        context.push_back("UpperCaseAll");
    if (RegExp::PartialMatch(currentOrth, rxContainsNumber_))
        context.push_back("ContainsNumber");
    if (RegExp::PartialMatch(currentOrth, rxIsNumber_))
        context.push_back("IsNumber");
    if (RegExp::PartialMatch(currentOrth, rxContainsLetter_))
        context.push_back("ContainsLetter");
    if (RegExp::PartialMatch(currentOrth, rxContainsPunct_) &&
            RegExp::PartialMatch(currentOrth, rxContainsNumber_))
        context.push_back("ContainsPunctAndNumber");
    if (!RegExp::PartialMatch(currentOrth, rxContainsLetter_) &&
            !RegExp::PartialMatch(currentOrth, rxContainsNumber_))
        context.push_back("ContainsNoLetterAndNoNumber");

    if (currentOrth.size() > 6) {
        context.push_back(Feature("curr_prefix=" + currentOrth.substr(0, 1)));
        context.push_back(Feature(
                    "curr_suffix=" + currentOrth.substr(currentOrth.size() - 1)
                    ));

        context.push_back(Feature("curr_prefix=" + currentOrth.substr(0, 2)));
        context.push_back(Feature(
                    "curr_suffix=" + currentOrth.substr(currentOrth.size() - 2)
                    ));

        context.push_back(Feature("curr_prefix=" + currentOrth.substr(0, 3)));
        context.push_back(Feature(
                    "curr_suffix=" + currentOrth.substr(currentOrth.size() - 3)
                    ));

        context.push_back(Feature(
                    "curr_suffix=" + currentOrth.substr(currentOrth.size() - 4)
                    ));
    }

    for (int i = 1; i < window; i ++) {
        int prevIndex = 0;
        if (currentIndex >= i)
            prevIndex = currentIndex - i;

        TokenEdgesMap::iterator prevIt =
            tokenEdgesMap.find(prevIndex);
        if (prevIt != tokenEdgesMap.end()) {
            Lattice::EdgeDescriptor prevToken = prevIt->second;
            std::stringstream prevWord;
            prevWord << "prev" << i << "_word=" <<
                lattice.getEdgeText(prevToken);
            context.push_back(Feature(prevWord.str()));

            Lattice::VertexDescriptor prevVertex =
                lattice.getEdgeSource(prevToken);
            Lattice::InOutEdgesIterator prevFormIt =
                lattice.outEdges(prevVertex, formMask);
            while (prevFormIt.hasNext()) {
                Lattice::EdgeDescriptor prevForm = prevFormIt.next();
                if (isDiscarded(lattice, prevForm))
                    continue;
                std::stringstream prevTag, prevLemma;
                prevTag << "prev" << i << "_has_tag=" <<
                    getFormMorphoTag(lattice, prevForm);
                prevLemma << "prev" << i << "_has_lemma=" <<
                    getFormLemma(lattice, prevForm);
                context.push_back(Feature(prevTag.str()));
                context.push_back(Feature(prevLemma.str()));
            }
        }

        int nextIndex = currentIndex + i;
        if ((size_t)nextIndex >= tokenEdgesMap.size())
            nextIndex = tokenEdgesMap.size() - 1;

        TokenEdgesMap::iterator nextIt =
            tokenEdgesMap.find(nextIndex);
        if (nextIt != tokenEdgesMap.end()) {
            Lattice::EdgeDescriptor nextToken = nextIt->second;
            std::stringstream nextWord;
            nextWord << "next" << i << "_word=" <<
                lattice.getEdgeText(nextToken);
            context.push_back(Feature(nextWord.str()));

            Lattice::VertexDescriptor nextVertex =
                lattice.getEdgeSource(nextToken);
            Lattice::InOutEdgesIterator nextFormIt =
                lattice.outEdges(nextVertex, formMask);
            while (nextFormIt.hasNext()) {
                Lattice::EdgeDescriptor nextForm = nextFormIt.next();
                if (isDiscarded(lattice, nextForm))
                    continue;
                std::stringstream nextTag, nextLemma;
                nextTag << "next" << i << "_has_tag=" <<
                    getFormMorphoTag(lattice, nextForm);
                nextLemma << "next" << i << "_has_lemma=" <<
                    getFormLemma(lattice, nextForm);
                context.push_back(Feature(nextTag.str()));
                context.push_back(Feature(nextLemma.str()));
            }
        }

    }

    std::set<std::string> uniq;
    uniq.insert( context.begin(), context.end() );
    context.clear();
    context.insert( context.begin(), uniq.begin(), uniq.end() );
    
    return context;
}

std::vector<MeTagger::Outcome> MeTagger::getTokenTags(Lattice &lattice,
        TokenEdgesMap tokenEdgesMap) {
    std::vector<Outcome> tags;
    
    for (TokenEdgesMap::iterator tokenIt = tokenEdgesMap.begin();
            tokenIt != tokenEdgesMap.end(); ++ tokenIt) {
        int tokenIndex = tokenIt->first;
        Lattice::EdgeDescriptor token = tokenIt->second;
        std::string prevTag = "";
        if (!tags.empty()) {
            prevTag = tags.back();
        }
        Context context = createContext(lattice, tokenEdgesMap,
                tokenIndex, 2);
        if (prevTag != "") {
            context.push_back(Feature("prevtag=" + prevTag));
        }
    
        Outcome bestTag = getBestTag(lattice, token, context);
        tags.push_back(bestTag);
        
    }
    return tags;
}

MeTagger::Outcome MeTagger::getBestTag(Lattice &lattice,
        Lattice::EdgeDescriptor token, MeTagger::Context context) {
    LayerTagMask formMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "form", langCode_);
    Outcome bestTag = "empty";
    double bestProb = 0;

    Lattice::VertexDescriptor currentVertex =
        lattice.getEdgeSource(token);
    Lattice::InOutEdgesIterator formIt =
        lattice.outEdges(currentVertex, formMask);
    while (formIt.hasNext()) {
        Lattice::EdgeDescriptor form = formIt.next();
        if (getFormPartOfSpeech(lattice, form) == unknownPosLabel_) {
            std::string currentOrth = lattice.getEdgeText(token);
            if (!openClasses_.empty()) {
                for (std::vector<std::string>::iterator openClassIt =
                        openClasses_.begin();
                        openClassIt != openClasses_.end();
                        ++ openClassIt) {
                    Outcome openClassTag(*openClassIt);
        
                    double prob = model_.eval(context, openClassTag);
                    if ( prob > bestProb ) {
                        bestTag  = openClassTag;
                        bestProb = prob;
                    }
                }
            }
        } else {
            Outcome tag( getFormMorphoTag(lattice, form) );
            double prob = model_.eval(context, tag);
            if (prob > bestProb) {
                bestTag = tag;
                bestProb = prob;
            }
        }
    }
    if (bestTag == "empty") {
        bestTag = model_.predict(context);
    }

    return bestTag;
}

void MeTagger::applyTokenTags(Lattice &lattice, TokenEdgesMap tokenEdgesMap,
        std::vector<MeTagger::Outcome> tags) {
    LayerTagMask formMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "form", langCode_);
    size_t i = 0;
    while (i < tags.size()) {
        Outcome tag = tags[i];
        TokenEdgesMap::iterator tokenIt = tokenEdgesMap.find(i);
        if (tokenIt != tokenEdgesMap.end()) {
            Lattice::EdgeDescriptor token = tokenIt->second;
            Lattice::VertexDescriptor currentVertex =
                lattice.getEdgeSource(token);
            Lattice::InOutEdgesIterator formIt =
                lattice.outEdges(currentVertex, formMask);
            bool allFormsDiscarded = true;
            while (formIt.hasNext()) {
                Lattice::EdgeDescriptor form = formIt.next();
                if (isDiscarded(lattice, form))
                    continue;
                //if (getFormMorphoTag(lattice, form) != tag) {
                if (!formMatchesTag(lattice, form, tag)) {
                    lattice.discard(form);
                } else {
                    allFormsDiscarded = false;
                }
            }
            if (allFormsDiscarded) {
                if (lattice.getEdgeAnnotationItem(token).getCategory() == "T") {
                    if (hasLexemeEdgeMatchingTag(lattice, token, tag)) {
                        Lattice::EdgeDescriptor lexemeEdge =
                            getLexemeEdgeMatchingTag(lattice, token, tag);
                        addFormEdge(lattice, token, lexemeEdge, tag);
                    } else {
                        std::string lemma = lattice.getEdgeText(token);
                        if (!lemmaEdgeExists(lattice, token, lemma)) {
                            addLemmaEdge(lattice, token, lemma);
                        }
                        std::string partOfSpeech = getPartOfSpeechFromMorphoTag(tag);
                        Lattice::EdgeDescriptor lexemeEdge = addLexemeEdge(
                                lattice, token, lemma, partOfSpeech);
                        addFormEdge(lattice, token, lexemeEdge, tag);
                    }

                }
            }
        }
        i ++;
    }
}

bool MeTagger::formMatchesTag(Lattice &lattice,
        Lattice::EdgeDescriptor form, MeTagger::Outcome tag) {
    if (getFormPartOfSpeech(lattice, form) ==
            getPartOfSpeechFromMorphoTag(tag)) {
        AnnotationItem ai = lattice.getEdgeAnnotationItem(form);
        std::list< std::pair<std::string, std::string> > av
            = lattice.getAnnotationItemManager().getValues(ai);
        std::list<std::pair<std::string, std::string> > attributes =
            getAttributesFromMorphoTag(tag);
        if (!av.empty()) {
            bool allAttributesMatched = true;
            for (std::list<std::pair<std::string, std::string> >::iterator
                    avIt = av.begin();
                    avIt != av.end();
                    ++ avIt) {
                bool attributeMatched = false;
                for (std::list<std::pair<std::string, std::string> >::iterator
                        attrIt = attributes.begin();
                        attrIt != attributes.end();
                    ++ attrIt) {
                    if (avIt->first == attrIt->first &&
                            avIt->second == attrIt->second) {
                        attributeMatched = true;
                        break;
                    }
                }
                if (!attributeMatched) {
                    allAttributesMatched = false;
                    break;
                }
            }
            return allAttributesMatched;
        } else {
            if (attributes.empty()) {
                return true;
            }
        }
    }
    return false;
}

std::string MeTagger::getFormLemma(Lattice &lattice,
        Lattice::EdgeDescriptor edge) {
    std::list<Lattice::Partition> partitions = lattice.getEdgePartitions(edge);
    if (! partitions.empty()) {
        Lattice::Partition firstPartition = partitions.front();
        Lattice::EdgeDescriptor parentEdge = firstPartition.firstEdge(lattice);

        LayerTagCollection tags = lattice.getEdgeLayerTags(parentEdge);
        if (lattice.getLayerTagManager().isThere("lexeme", tags)) {
            std::string lemma_pos =
                lattice.getEdgeAnnotationItem(parentEdge).getText();
            if (lemma_pos.find(LEMMA_CATEGORY_SEPARATOR)
                    != std::string::npos) {
                return lemma_pos.substr(0,
                        lemma_pos.find(LEMMA_CATEGORY_SEPARATOR));
            } else {
                return lemma_pos;
            }
        }
    }
    return lattice.getEdgeText(edge);
}

std::string MeTagger::getFormPartOfSpeech(Lattice &lattice,
        Lattice::EdgeDescriptor edge) {
    std::list<Lattice::Partition> partitions = lattice.getEdgePartitions(edge);
    if (! partitions.empty()) {
        Lattice::Partition firstPartition = partitions.front();
        Lattice::EdgeDescriptor parentEdge = firstPartition.firstEdge(lattice);

        LayerTagCollection tags = lattice.getEdgeLayerTags(parentEdge);
        if (lattice.getLayerTagManager().isThere("lexeme", tags)) {
            std::string lemma_pos =
                lattice.getEdgeAnnotationItem(parentEdge).getText();
            if (lemma_pos.find(LEMMA_CATEGORY_SEPARATOR)
                    != std::string::npos) {
                return lemma_pos.substr(
                        lemma_pos.find(LEMMA_CATEGORY_SEPARATOR) + 1,
                        std::string::npos);
            } else {
                return unknownPosLabel_;
            }
        }
    }
    return unknownPosLabel_;
}

std::string MeTagger::getFormMorphoTag(Lattice &lattice,
        Lattice::EdgeDescriptor edge) {
    std::string morphoTag = getFormPartOfSpeech(lattice, edge);
    AnnotationItem ai = lattice.getEdgeAnnotationItem(edge);
    std::list< std::pair<std::string, std::string> > av
        = lattice.getAnnotationItemManager().getValues(ai);
    for (std::list< std::pair<std::string, std::string> >::iterator avIt =
            av.begin(); avIt != av.end(); ++ avIt) {
        if (avIt->first == "head" || avIt->first == "orth")
            continue;
        if (morphoTag != "")
            morphoTag += ",";
        morphoTag += avIt->first;
        morphoTag += "=";
        morphoTag += avIt->second;
    }
    return morphoTag;
}

std::string MeTagger::getPartOfSpeechFromMorphoTag(std::string tag) {
    std::string partOfSpeech = tag;
    if (tag.find(",") > 0)
        partOfSpeech = tag.substr(0, tag.find(","));
    return partOfSpeech;
}

std::list<std::pair<std::string, std::string> >
    MeTagger::getAttributesFromMorphoTag(std::string tag) {
    std::list<std::pair<std::string, std::string> > attributes;
    std::string tempTag = "";
    if (tag.find(",") > 0) {
        tempTag = tag.substr(tag.find(",") + 1, std::string::npos);
    }
    while (tempTag.find(",") != std::string::npos) {
        size_t sepPos = tempTag.find(",");
        std::string attributeString = tempTag.substr(0, sepPos);
        if (attributeString.find("=") != std::string::npos) {
            std::string key = attributeString.substr(0,
                    attributeString.find("="));
            std::string value = attributeString.substr(attributeString.find(
                        "=") + 1, std::string::npos);
            attributes.push_back(std::pair<std::string, std::string>(
                        key, value));
        }
        if (sepPos < tempTag.size())
            tempTag = tempTag.substr(sepPos + 1, std::string::npos);
        else
            tempTag = "";
    }
    if (tempTag != "") {
        if (tempTag.find("=") != std::string::npos) {
            std::string key = tempTag.substr(0,
                    tempTag.find("="));
            std::string value = tempTag.substr(tempTag.find(
                        "=") + 1, std::string::npos);
            attributes.push_back(std::pair<std::string, std::string>(
                        key, value));
        }
    }
    return attributes;
}

bool MeTagger::hasLexemeEdgeMatchingTag(Lattice &lattice,
        Lattice::EdgeDescriptor token, std::string tag) {
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "lexeme", langCode_);
    Lattice::VertexDescriptor vertex =
        lattice.getEdgeSource(token);
    Lattice::InOutEdgesIterator lexemeIt =
        lattice.outEdges(vertex, lexemeMask);
    while (lexemeIt.hasNext()) {
        Lattice::EdgeDescriptor lexeme = lexemeIt.next();
        if (lexemeEdgeMatchesTag(lattice, lexeme, tag))
            return true;
    }
    return false;
}

Lattice::EdgeDescriptor MeTagger::getLexemeEdgeMatchingTag(Lattice &lattice,
        Lattice::EdgeDescriptor token, std::string tag) {
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "lexeme", langCode_);
    Lattice::VertexDescriptor vertex =
        lattice.getEdgeSource(token);
    Lattice::InOutEdgesIterator lexemeIt =
        lattice.outEdges(vertex, lexemeMask);
    while (lexemeIt.hasNext()) {
        Lattice::EdgeDescriptor lexeme = lexemeIt.next();
        if (lexemeEdgeMatchesTag(lattice, lexeme, tag))
            return lexeme;
    }
    return Lattice::EdgeDescriptor();
}

bool MeTagger::lexemeEdgeMatchesTag(Lattice &lattice,
        Lattice::EdgeDescriptor lexeme, std::string tag) {
    std::string partOfSpeech = getPartOfSpeechFromMorphoTag(tag);
    std::list<std::pair<std::string, std::string> > attributes =
        getAttributesFromMorphoTag(tag);
    AnnotationItem ai = lattice.getEdgeAnnotationItem(lexeme);
    if (ai.getCategory() == partOfSpeech) {
        std::list< std::pair<std::string, std::string> > av
            = lattice.getAnnotationItemManager().getValues(ai);
        if (!av.empty()) {
            bool allAttributesFound = true;
            for (std::list<std::pair<std::string, std::string> >::iterator
                    avIt = av.begin();
                    avIt != av.end();
                    ++ avIt) {
                bool found = false;
                for (std::list<std::pair<std::string, std::string> >::iterator it =
                        attributes.begin();
                        it != attributes.end();
                        ++ it) {
                    if (it->first == avIt->first
                            && it->second == avIt->second) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    allAttributesFound = false;
                    break;
                }
            }
            if (allAttributesFound) {
                return true;
            }
        } else {
            return true;
        }
    }
    return false;
}

bool MeTagger::lemmaEdgeExists(Lattice &lattice,
        Lattice::EdgeDescriptor token, std::string lemma) {
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "lemma", langCode_);
    Lattice::InOutEdgesIterator lemmaIt =
        lattice.outEdges(lattice.getEdgeSource(token), lemmaMask);
    while (lemmaIt.hasNext()) {
        Lattice::EdgeDescriptor lemmaEdge = lemmaIt.next();
        if (lattice.getEdgeAnnotationItem(lemmaEdge).getText() == lemma)
                return true;
    }
    return false;
}

void MeTagger::addLemmaEdge(Lattice &lattice,
        Lattice::EdgeDescriptor token, std::string lemma) {
    Lattice::EdgeSequence::Builder seqBuilder(lattice);
    seqBuilder.addEdge(token);

    AnnotationItem annotationItem("word", lemma);
    LayerTagCollection lemmaTag
        = lattice.getLayerTagManager().createSingletonTagCollection("lemma");

    lattice.addEdge(
            lattice.getEdgeSource(token),
            lattice.getEdgeTarget(token),
            annotationItem,
            lemmaTag,
            seqBuilder.build()
            );
}

bool MeTagger::lexemeEdgeExists(Lattice &lattice,
        Lattice::EdgeDescriptor token, std::string lemma,
        std::string partOfSpeech) {
    LayerTagMask lexemeMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "lexeme", langCode_);
    Lattice::InOutEdgesIterator lexemeIt =
        lattice.outEdges(lattice.getEdgeSource(token), lexemeMask);
    while (lexemeIt.hasNext()) {
        Lattice::EdgeDescriptor lexeme = lexemeIt.next();
        if (lattice.getEdgeAnnotationItem(lexeme).getCategory()
                == partOfSpeech) {
            std::list<Lattice::Partition> partitions =
                lattice.getEdgePartitions(lexeme);
            if (! partitions.empty()) {
                Lattice::Partition firstPartition = partitions.front();
                Lattice::EdgeDescriptor parentEdge =
                    firstPartition.firstEdge(lattice);

                LayerTagCollection tags = lattice.getEdgeLayerTags(parentEdge);
                if (lattice.getLayerTagManager().isThere("lemma", tags)) {
                    if (lattice.getEdgeAnnotationItem(parentEdge).getText()
                            == lemma)
                        return true;
                }
            }
        }
    }
    return false;
}

Lattice::EdgeDescriptor MeTagger::addLexemeEdge(Lattice &lattice,
        Lattice::EdgeDescriptor token, std::string lemma,
        std::string partOfSpeech) {
    LayerTagMask lemmaMask = lattice.getLayerTagManager().getMaskWithLangCode(
        "lemma", langCode_);
    Lattice::InOutEdgesIterator lemmaIt =
        lattice.outEdges(lattice.getEdgeSource(token), lemmaMask);
    while (lemmaIt.hasNext()) {
        Lattice::EdgeDescriptor lemmaEdge = lemmaIt.next();
        if (lattice.getEdgeAnnotationItem(lemmaEdge).getText() == lemma) {
            Lattice::EdgeSequence::Builder seqBuilder(lattice);
            seqBuilder.addEdge(lemmaEdge);

            AnnotationItem annotationItem(partOfSpeech, StringFrag(
                        lemma + LEMMA_CATEGORY_SEPARATOR + partOfSpeech) );
            LayerTagCollection lexemeTag
                = lattice.getLayerTagManager().
                    createSingletonTagCollection("lexeme");

            return lattice.addEdge(
                    lattice.getEdgeSource(token),
                    lattice.getEdgeTarget(token),
                    annotationItem,
                    lexemeTag,
                    seqBuilder.build()
                    );
        }
    }
    return Lattice::EdgeDescriptor();
}

void MeTagger::addFormEdge(Lattice &lattice,
        Lattice::EdgeDescriptor token, Lattice::EdgeDescriptor lexeme,
        std::string tag) {
    Lattice::EdgeSequence::Builder seqBuilder(lattice);
    seqBuilder.addEdge(lexeme);

    AnnotationItem annotationItem(
            lattice.getEdgeAnnotationItem(lexeme).getCategory(),
            StringFrag(lattice.getEdgeText(token)) );
    std::list<std::pair<std::string, std::string> > attributes =
        getAttributesFromMorphoTag(tag);
    for (std::list<std::pair<std::string, std::string> >::iterator
            attrIt = attributes.begin();
            attrIt != attributes.end();
            ++ attrIt) {
        lattice.getAnnotationItemManager().setValue(
                annotationItem, attrIt->first, attrIt->second);
    }
    LayerTagCollection formTag
        = lattice.getLayerTagManager().
        createSingletonTagCollection("form");
        //@todo: tylko takie tagi? skopiowac jakies inne?
        //o tagsecie np? dodac wlasny tag "metagger"?

    lattice.addEdge(
            lattice.getEdgeSource(token),
            lattice.getEdgeTarget(token),
            annotationItem,
            formTag,
            seqBuilder.build()
            );
}

bool MeTagger::isDiscarded(Lattice &lattice, Lattice::EdgeDescriptor edge) {
    LayerTagCollection tags = lattice.getEdgeLayerTags(edge);
    if (lattice.getLayerTagManager().isThere("discarded", tags))
        return true;
    else
        return false;
}
