#include "dictionary.hpp"

Dictionary::Dictionary(bool has_pos, bool has_morpho)
 : m_has_pos(has_pos || has_morpho), m_has_morpho(has_morpho)
{ }

void Dictionary::read_dictionary(std::string filename)
{   
    std::ifstream in;
    in.open(filename.c_str());
    
    std::map<char, size_t> char_map;
    char separator  = 1;
    char next_inter = 0;
    
    std::map<std::string, std::string> temp_map;
    
    std::cerr << "Reading dictionary data" << std::endl;
    
    std::string line;
    while(std::getline(in, line))
    {            
        std::stringstream linestream(line);
        std::stringstream interpretation;
        std::string orth, lemma, pos, morpho;
        linestream >> orth >> lemma;
        
        interpretation << lemma;
        
        if(m_has_pos) {
            if(linestream >> pos)
                interpretation << separator << pos;
            else {
                //@TODO throw some exception
            }
        }
        
        if(m_has_morpho) {
            while(linestream >> morpho)
                interpretation << separator << morpho;
        }
        
        std::string interpretation_string = interpretation.str();
        interpretation_string.push_back(next_inter);
        BOOST_FOREACH(char c, interpretation_string)
          char_map[c]++;
        
        temp_map[orth] += interpretation_string;
    }
    
    std::cerr << "Compressing dictionary data" << std::endl;
    
    m_huffman = new CanonicalHuffman<char>(char_map.begin(), char_map.end());
    
    KeyValueStore::Builder builder;
    typedef std::pair<std::string, std::string> StringPair;
    BOOST_FOREACH(StringPair kv, temp_map) {
        std::string value;
        BitWrapper<> bit_wrapper(value);
        BOOST_FOREACH(char c, kv.second)
            m_huffman->Put(bit_wrapper, c);
        builder.add(kv.first, value);       
    }
    
    m_store = builder.build();
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
    m_huffman = new CanonicalHuffman<char>(p_file);
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
    m_huffman->Save(p_file);
    m_store->save(p_file);
    std::cerr << "Done" << std::endl;
}
    
DictionaryItem Dictionary::look_up(std::string token)
{
    typedef boost::optional<std::string> OptString;
  
    //boost::shared_ptr<StringGeneralCaseConverter> lowerConverter =
    //    StringCaseConverterManager::getInstance().getLowerCaseConverter("en");
    //
    //std::string token_lowercased =
    //    simpleConvert(*lowerConverter, token);
    
    //if(token == token_lowercased)
    //{
        OptString data = m_store->get(token);
        if(data)
            return DictionaryItem(token, false,
                                  decompress_interpretations(data.get()));
    //}
    //else
    //{
    //    //@TODO: Add uc_first check
    //  
    //    OptString data = m_store->get(token);
    //    if(data)
    //    {
    //        DictionaryItem item(token, false,
    //                            decompress_interpretations(data.get()));
    //        
    //        OptString data_lowercased = m_store->get(token_lowercased);
    //        if(data_lowercased)
    //            BOOST_FOREACH(Interpretation i,
    //                          decompress_interpretations(data_lowercased.get()))
    //                item.get_interpretations().push_back(i);
    //        return item;
    //    }
    //    
    //    OptString data_lowercased = m_store->get(token_lowercased);
    //    if(data_lowercased)
    //    {
    //        DictionaryItem item_lowercased(token, false,
    //                                       decompress_interpretations(data_lowercased.get()));
    //        return item_lowercased;
    //    }
    //}
    //
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
    
    BitWrapper<> bit_wrapper(compressed);
    boost::optional<char> c;
    while(c = m_huffman->Read(bit_wrapper)) {
        if(c.get() == separator) {
            switch(state) {
                case Lemma:
                    state = Pos;
                    break;
                case Pos:
                    state = Morpho;
                    break;
                case Morpho:
                    morphologies.push_back(morpho.str());
                    morpho.str(std::string());
                    state = Morpho;
                    break;
            }
        }
        else if (c.get() == next_inter) {
            state = Lemma;
            std::string morpho_string = morpho.str();
            if(morpho_string.size())
                morphologies.push_back(morpho_string);
            
            interpretations.push_back(
                Interpretation(lemma.str(),
                               pos.str(),
                               morphologies));
            
            lemma.str(std::string());
            pos.str(std::string());
            morpho.str(std::string());
            morphologies.clear();
        }
        else {
            switch(state) {
                case Lemma:
                    lemma << c.get();
                    break;
                case Pos:
                    pos << c.get();
                    break;
                case Morpho:
                    morpho << c.get();
                    break;
            }
        }
    }
    
    return interpretations;
}
