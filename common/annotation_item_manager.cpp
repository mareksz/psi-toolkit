#include "annotation_item_manager.hpp"


#include <sstream>


AnnotationItemManager::AnnotationItemManager() {
    zObjectsHolder_ = zvector::generate(EMPTY_ZOBJECTS_HOLDER);
    zSymbolTable_ = zsymboltable::generate(zObjectsHolder_);
    zSymbolFactory_ = new zsymbolfactory(zSymbolTable_);
}


AnnotationItemManager::AnnotationItemManager(AnnotationItemManager & other) {
    zObjectsHolder_ = zvector::generate(EMPTY_ZOBJECTS_HOLDER);
    *zObjectsHolder_ = *(other.zObjectsHolder_);
    zSymbolTable_ = zsymboltable::generate(zObjectsHolder_);
    *zSymbolTable_ = *(other.zSymbolTable_);
    zSymbolFactory_ = new zsymbolfactory(zSymbolTable_);
    *zSymbolFactory_ = *(other.zSymbolFactory_);
}


AnnotationItemManager::~AnnotationItemManager() {
    delete zSymbolFactory_;
    zobject::freeZObjects(zObjectsHolder_);
    zObjectsHolder_->annihilate();
}


void AnnotationItemManager::setStringValue(
    AnnotationItem & annotationItem,
    std::string attribute,
    std::string value
) {
    setValue(annotationItem, attribute, zSymbolFactory_->get_symbol(value.c_str()));
}


void AnnotationItemManager::setValue(
    AnnotationItem & annotationItem,
    std::string attribute,
    std::string value
) {
    int intVal;
    std::stringstream valSs(value);
    if (valSs >> intVal && valSs.eof()) {
        setValue(annotationItem, attribute, intVal);
    } else {
        setStringValue(annotationItem, attribute, value);
    }
}


void AnnotationItemManager::setValue(
    AnnotationItem & annotationItem,
    std::string attribute,
    int value
) {
    setValue(annotationItem, attribute, INTEGER_TO_ZVALUE(value));
}


void AnnotationItemManager::setValue(
    AnnotationItem & annotationItem,
    std::string attribute,
    zvalue value
) {
    m_.insert(StringBimapItem(attribute, m_.size()));
    size_t ix = m_.left.at(attribute);
    if (ix >= annotationItem.values_.size()) {
        annotationItem.resize_(ix + 1);
    }
    annotationItem.values_[ix] = value;
    annotationItem.attributes_[ix] = true;
}


zvalue AnnotationItemManager::getValue(
    AnnotationItem & annotationItem,
    std::string attribute
) {
    if (m_.left.find(attribute) == m_.left.end()) {
        return NULL_ZVALUE;
    } else {
        size_t ix = m_.left.at(attribute);
        if (annotationItem.attributes_[ix]) {
            return annotationItem.values_[m_.left.at(attribute)];
        } else {
            return NULL_ZVALUE;
        }
    }
}


std::string AnnotationItemManager::getValueAsString(
    AnnotationItem & annotationItem,
    std::string attribute
) {
    return zvalueToString(getValue(annotationItem, attribute));
}


std::list< std::pair<std::string, zvalue> > AnnotationItemManager::getValuesAsZvalues(
    const AnnotationItem & annotationItem
) {
    std::list< std::pair<std::string, zvalue> > result;
    for (
        boost::dynamic_bitset<>::size_type i = annotationItem.attributes_.find_first();
        i != boost::dynamic_bitset<>::npos && i < m_.size();
        i = annotationItem.attributes_.find_next(i)
    ) {
        result.push_back(std::pair<std::string, zvalue>(
            m_.right.at(i),
            annotationItem.values_[i]
        ));
    }
    return result;
}

std::list< std::pair<std::string, std::string> > AnnotationItemManager::getValues(
    const AnnotationItem & annotationItem
) {
    std::list< std::pair<std::string, std::string> > result;
    for (
        boost::dynamic_bitset<>::size_type i = annotationItem.attributes_.find_first();
        i != boost::dynamic_bitset<>::npos && i < m_.size();
        i = annotationItem.attributes_.find_next(i)
    ) {
        result.push_back(std::pair<std::string, std::string>(
            m_.right.at(i),
            zvalueToString(annotationItem.values_[i])
        ));
    }
    return result;
}

std::map<std::string, std::string> AnnotationItemManager::getAVMap(
    const AnnotationItem & annotationItem
) {
    std::map<std::string, std::string> result;
    std::list< std::pair<std::string, std::string> > list = getValues(annotationItem);
    result.insert(list.begin(), list.end());
    return result;
}

zvalue AnnotationItemManager::stringToZvalue(std::string s) const {
    return zSymbolFactory_->get_symbol(s.c_str());
}

std::string AnnotationItemManager::zvalueToString(zvalue z) const {
    char * resultCStr = zvalue_to_string(z);
    std::string result(resultCStr);
    delete [] resultCStr;
    return result;
}


//Master stuff


bool AnnotationItemManager::is_int(zvalue value) const {
    return INTEGERP(value);
}


bool AnnotationItemManager::is_string(zvalue value) const {
    return !INTEGERP(value);
}


int AnnotationItemManager::to_int(zvalue value) const {
    assert(is_int(value));
    return ZVALUE_TO_INTEGER(value);
}


std::string AnnotationItemManager::to_string(zvalue value) const {
    assert(is_string(value));
    if (is_any(value)) {
        return NIL_STRING;
    }
    if (is_false(value)) {
        return FAIL_STRING;
    }
    return zvalueToString(value);
}


std::string AnnotationItemManager::string_representation(zvalue value) const {
    return zvalueToString(value);
}


zvalue AnnotationItemManager::from_int(int i) {
    return INTEGER_TO_ZVALUE(i);
}


zvalue AnnotationItemManager::from_string(const std::string& s) {
    if (s == ANY_STRING || s == NIL_STRING) {
        return any_value();
    }
    if (s == FAIL_STRING || s == FALSE_STRING || s == EMPTY_STRING) {
        return false_value();
    }
    return stringToZvalue(s);
}


zvalue AnnotationItemManager::from_bool(bool b) {
    return (b ? any_value() : false_value());
}

zvalue AnnotationItemManager::false_value() const {
    return NULL_ZVALUE;
}


zvalue AnnotationItemManager::any_value() const {
    return DEFAULT_ZVALUE;
}

bool AnnotationItemManager::is_false(zvalue value) const {
    return NULLP(value);
}


bool AnnotationItemManager::is_true(zvalue value) const {
    return !NULLP(value);
}


bool AnnotationItemManager::is_any(zvalue value) const {
    return DEFAULTP(value);
}

const std::string AnnotationItemManager::FAIL_STRING = "fail";
const std::string AnnotationItemManager::FALSE_STRING = "false";
const std::string AnnotationItemManager::EMPTY_STRING = "empty";
const std::string AnnotationItemManager::ANY_STRING = "any";
const std::string AnnotationItemManager::NIL_STRING = "nil";
