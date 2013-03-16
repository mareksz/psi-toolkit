#include "dictionary.hpp"

Dictionary::Dictionary(bool has_pos, bool has_morpho)
 : m_has_pos(has_pos || has_morpho), m_has_morpho(has_morpho),
   m_lemma_tree(0), m_pos_tree(0), m_morpho_tree(0)
{ }

Dictionary::~Dictionary() {
    if(m_lemma_tree)
        delete m_lemma_tree;
    if(m_pos_tree)
        delete m_pos_tree;
    if(m_morpho_tree)
        delete m_morpho_tree;
}

void Dictionary::read_dictionary(std::string filename)
{   
    std::ifstream in;
    in.open(filename.c_str());
    
    std::map<std::string, std::vector<unsigned> > temp_map;
    
    std::cerr << "Reading dictionary data" << std::endl;
    
    typedef boost::unordered_map<unsigned, size_t> FreqMap;
    
    IdMap lemma_map;
    IdMap pos_map;
    IdMap morpho_map;
    
    FreqMap lemma_freq;
    FreqMap pos_freq;
    FreqMap morpho_freq;
    
    morpho_map["###END###"] = 0;
    morpho_freq[0] = 1;
    
    std::string line;
    while(std::getline(in, line))
    {            
        std::stringstream linestream(line);
        std::stringstream interpretation;
        std::string orth, lemma, pos, morpho;
        linestream >> orth >> lemma;
        
        std::vector<unsigned>& interpretations = temp_map[orth];
        
        unsigned lemma_id = get_or_add_id(lemma_map, lemma);
        lemma_freq[lemma_id]++;
        interpretations.push_back(lemma_id);
        
        if(m_has_pos) {
            if(linestream >> pos) {
                unsigned pos_id = get_or_add_id(pos_map, pos);
                pos_freq[pos_id]++;
                interpretations.push_back(pos_id);
            }
            else {
                //@TODO throw some exception
            }
        }
        
        if(m_has_morpho) {
            while(linestream >> morpho) {
                unsigned morpho_id = get_or_add_id(morpho_map, morpho);
                morpho_freq[morpho_id]++;
                interpretations.push_back(morpho_id);
            }
        }
        
        interpretations.push_back(0);
        morpho_freq[0]++;
    }
    
    std::cerr << "Compressing dictionary data" << std::endl;
    
    map_to_stringvector(lemma_map, m_lemmas);
    map_to_stringvector(pos_map, m_pos);
    map_to_stringvector(morpho_map, m_morphos);
    
    m_lemma_tree = new CanonicalHuffman<unsigned>(lemma_freq.begin(),
                                                  lemma_freq.end());
    
    m_pos_tree = new CanonicalHuffman<unsigned>(pos_freq.begin(),
                                                pos_freq.end());
    
    m_morpho_tree = new CanonicalHuffman<unsigned>(morpho_freq.begin(),
                                                   morpho_freq.end());
    
    std::cerr << lemma_freq.size() << std::endl;
    std::cerr << pos_freq.size() << std::endl;

    enum State { Lemma, Pos, Morpho } state = Lemma;
    
    KeyValueStore::Builder builder;
    typedef std::pair<std::string, std::vector<unsigned> > Pair;
    BOOST_FOREACH(Pair kv, temp_map) {
        std::string value;
        BitWrapper<> bit_wrapper(value);
        BOOST_FOREACH(unsigned id, kv.second) {
            switch(state) {
                case Lemma:
                    m_lemma_tree->Put(bit_wrapper, id);
                    state = m_has_pos ? Pos : Morpho;
                    break;
                case Pos:
                    m_pos_tree->Put(bit_wrapper, id);
                    state = Morpho;
                    break;
                case Morpho:
                    m_morpho_tree->Put(bit_wrapper, id);
                    state = id == 0 ? Lemma : Morpho;
            }
        }
        builder.add(kv.first, value);       
    }
    m_store = builder.build();
}

unsigned Dictionary::get_or_add_id(IdMap &id_map, std::string& symbol)
{
    IdMap::iterator it = id_map.find(symbol);
    if(it != id_map.end())   
        return it->second;
    else
    {
        unsigned value = id_map.size();
        id_map[symbol] = value;
        return value;
    }
}

void Dictionary::map_to_stringvector(IdMap& id_map, StringVector<>& string_vec) {
    std::vector<std::string> temp_vec;
    temp_vec.resize(id_map.size());
    
    typedef std::pair<std::string, unsigned> StringId;
    BOOST_FOREACH(StringId string_id, id_map)
        temp_vec[string_id.second] = string_id.first;
        
    BOOST_FOREACH(std::string item, temp_vec)
        string_vec.push_back(item);
}

void Dictionary::load(std::string file_name) {
    std::FILE* p_file = std::fopen(file_name.c_str(), "r");
    load(p_file);
    std::fclose(p_file);        
}

void Dictionary::load(std::FILE* p_file) {
    std::cerr << "Loading dictionary data" << std::endl;
    std::fread((void*)&m_has_pos, 1, sizeof(bool), p_file);
    std::fread((void*)&m_has_morpho, 1, sizeof(bool), p_file);
    m_lemma_tree = new CanonicalHuffman<unsigned>(p_file);
    m_lemma_tree->Load(p_file);
    m_pos_tree = new CanonicalHuffman<unsigned>(p_file);
    m_pos_tree->Load(p_file);
    m_morpho_tree = new CanonicalHuffman<unsigned>(p_file);
    m_morpho_tree->Load(p_file);
    m_store = new KeyValueStore();
    m_store->load(p_file);
    std::cerr << "Done" << std::endl;
}


void Dictionary::save(std::string file_name) {
    std::FILE* p_file = std::fopen(file_name.c_str(), "w");
    
    save(p_file);
    std::fclose(p_file);
}

void Dictionary::save(std::FILE* p_file) {
    std::cerr << "Saving dictionary data" << std::endl;
    std::fwrite((void*)&m_has_pos, 1, sizeof(bool), p_file);
    std::fwrite((void*)&m_has_morpho, 1, sizeof(bool), p_file);
    m_lemmas.save(p_file);
    m_pos.save(p_file);
    m_morphos.save(p_file);
    m_lemma_tree->Save(p_file);
    m_pos_tree->Save(p_file);
    m_morpho_tree->Save(p_file);
    m_store->save(p_file);
    std::cerr << "Done" << std::endl;
}
    
DictionaryItem Dictionary::look_up(std::string token)
{
    typedef boost::optional<std::string> OptString;
  
    OptString data = m_store->get(token);
    if(data)
        return DictionaryItem(token, false,
                              decompress_interpretations(data.get()));

    Interpretations interpretations;
    interpretations.push_back(Interpretation(token, "unknown"));
    return DictionaryItem(token, false, interpretations);
}
    
Interpretations Dictionary::decompress_interpretations(std::string compressed) {
    Interpretations interpretations;

    char separator  = 1;
    char next_inter = 0;
    
    enum { Lemma, Pos, Morpho } state = Lemma;
    std::stringstream lemma, pos, morpho;
    std::vector<std::string> morphologies;
    
    //BitWrapper<> bit_wrapper(compressed);
    //boost::optional<char> c;
    //while(c = m_lemma_tree->Read(bit_wrapper)) {
    //    if(c.get() == separator) {
    //        switch(state) {
    //            case Lemma:
    //                state = Pos;
    //                break;
    //            case Pos:
    //                state = Morpho;
    //                break;
    //            case Morpho:
    //                morphologies.push_back(morpho.str());
    //                morpho.str(std::string());
    //                state = Morpho;
    //                break;
    //        }
    //    }
    //    else if (c.get() == next_inter) {
    //        state = Lemma;
    //        std::string morpho_string = morpho.str();
    //        if(morpho_string.size())
    //            morphologies.push_back(morpho_string);
    //        
    //        interpretations.push_back(
    //            Interpretation(lemma.str(),
    //                           pos.str(),
    //                           morphologies));
    //        
    //        lemma.str(std::string());
    //        pos.str(std::string());
    //        morpho.str(std::string());
    //        morphologies.clear();
    //    }
    //    else {
    //        switch(state) {
    //            case Lemma:
    //                lemma << c.get();
    //                break;
    //            case Pos:
    //                pos << c.get();
    //                break;
    //            case Morpho:
    //                morpho << c.get();
    //                break;
    //        }
    //    }
    //}
    //
    return interpretations;
}
