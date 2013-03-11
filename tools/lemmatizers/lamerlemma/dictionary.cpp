#include "dictionary.hpp"

Dictionary::Dictionary(bool has_morpho)
 : m_has_pos(true), m_has_morpho(has_morpho)
{ }

void Dictionary::read_dictionary(std::string filename)
{   
    std::ifstream in;
    in.open(filename.c_str());
    
    std::map<char, size_t> char_map;
    char zero = '\0';
    
    std::map<std::string, std::string> temp_map;
    
    std::cerr << "Reading dictionary data" << std::endl;
    
    std::string line;
    while(std::getline(in, line))
    {            
        std::stringstream linestream(line);
        std::string orth, lemma, tag, morpho;
        linestream >> orth >> lemma >> tag;
        
        std::string interpretation = lemma + zero + tag + zero;
        
        if(m_has_morpho) {
            linestream >> morpho;
            interpretation += morpho + zero;
        }
        
        BOOST_FOREACH(char c, interpretation)
          char_map[c]++;
        
        temp_map[orth] += interpretation;
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
    std::fwrite((void*)&m_has_morpho, 1, sizeof(bool), p_file);
    m_huffman->Save(p_file);
    m_store->save(p_file);
    std::cerr << "Done" << std::endl;
}
    
DictionaryItem Dictionary::look_up(std::string token)
{
    typedef boost::optional<std::string> OptString;
  
    boost::shared_ptr<StringGeneralCaseConverter> lowerConverter =
        StringCaseConverterManager::getInstance().getLowerCaseConverter("en");
    
    std::string token_lowercased =
        simpleConvert(*lowerConverter, token);
    
    if(token == token_lowercased)
    {
        OptString data = m_store->get(token);
        if(data)
            return DictionaryItem(token, false,
                                  decompress_interpretations(data.get()));
    }
    else
    {
        //@TODO: Add uc_first check
      
        OptString data = m_store->get(token);
        if(data)
        {
            DictionaryItem item(token, false,
                                decompress_interpretations(data.get()));
            
            OptString data_lowercased = m_store->get(token_lowercased);
            if(data_lowercased)
                BOOST_FOREACH(Interpretation i,
                              decompress_interpretations(data_lowercased.get()))
                    item.get_interpretations().push_back(i);
            return item;
        }
        
        OptString data_lowercased = m_store->get(token_lowercased);
        if(data_lowercased)
        {
            DictionaryItem item_lowercased(token, false,
                                           decompress_interpretations(data_lowercased.get()));
            return item_lowercased;
        }
    }
    
    Interpretations interpretations;
    interpretations.push_back(Interpretation(token, "unknown", "unknown"));
    return DictionaryItem(token, false, interpretations);

}
    
Interpretations Dictionary::decompress_interpretations(std::string compressed) {
    Interpretations interpretations;

    char zero = '\0';
    enum { Lemma, Tag, Morpho } state = Lemma;
    std::stringstream lemma, tag, morpho;
    
    BitWrapper<> bit_wrapper(compressed);
    boost::optional<char> c;
    while(c = m_huffman->Read(bit_wrapper)) {
        if(c.get() != zero) {
            switch(state) {
                case Lemma:
                    lemma << c.get();
                    break;
                case Tag:
                    tag << c.get();
                    break;
                case Morpho:
                    morpho << c.get();
                    break;
            }
        }
        else {
            if(state == Lemma) {
                state = Tag;
            }
            else if(state == Tag) {
                if(m_has_morpho)
                    state = Morpho;
                else {
                    state = Lemma;
                    interpretations.push_back(Interpretation(lemma.str(), tag.str()));
                    lemma.str(std::string());
                    tag.str(std::string());
                }
            }
            else if(state == Morpho) {
                state = Lemma;
                interpretations.push_back(Interpretation(lemma.str(), tag.str(), morpho.str()));
                lemma.str(std::string());
                tag.str(std::string());
                morpho.str(std::string());
            }
        }
    }
    return interpretations;
}
