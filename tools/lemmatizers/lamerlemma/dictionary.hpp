#ifndef DICTIONARY_HDR
#define DICTIONARY_HDR

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>

#include <boost/foreach.hpp>

#include "key_value_store.hpp"
#include "canonical_huffman.hpp"

#include "simple_convert.hpp"
#include "string_case_converter_manager.hpp"

class Interpretation
{
  public:
    Interpretation(std::string lemma, std::string tag = std::string(), std::string morpho = std::string())
     : m_lemma(lemma), m_tag(tag), m_morpho(morpho)
    { }
    
    std::string& get_lemma() { return m_lemma; }
    std::string& get_pos() { return m_tag; }
    std::string& get_morpho() { return m_morpho; }
    bool is_unknown() { return m_tag == "unknown"; }
    
  private:
    std::string m_lemma;
    std::string m_tag;
    std::string m_morpho;
};

typedef std::vector<Interpretation> Interpretations;

class DictionaryItem
{
  public:
    DictionaryItem() : m_orth("empty"), m_is_special(true) { }
    
    DictionaryItem(std::string orth, bool is_special = false,
                   Interpretations interpretations = Interpretations())
     : m_orth(orth), m_is_special(is_special),
       m_interpretations(interpretations)
    { }
    
    std::string& get_orth() { return m_orth; }    
    std::string& get_pos() { return m_is_special ? m_orth : m_interpretations[0].get_pos(); }
    bool is_special() { return m_is_special; }
    
    Interpretations& get_interpretations() { return m_interpretations; }

  private:
    std::string m_orth;
    bool m_is_special;
    
    Interpretations m_interpretations;
};

class Dictionary
{
  public:
    Dictionary(bool has_morpho = false);
    DictionaryItem look_up(std::string);
    
    void read_dictionary(std::string);
    
    void load(std::string);
    void load(std::FILE*);
    
    void save(std::string);
    void save(std::FILE*);
    
    bool has_pos() { return m_has_pos; }
    bool has_morpho() { return m_has_morpho; }
    
  private:
    
    Interpretations decompress_interpretations(std::string);
    
    bool m_has_pos;
    bool m_has_morpho;
    
    CanonicalHuffman<char>* m_huffman;
    KeyValueStore* m_store;
};

#endif
