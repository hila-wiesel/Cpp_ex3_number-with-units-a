
#include "NumberWithUnits.hpp"
#include <sstream>
#include <fstream>

using namespace std;


namespace ariel{
    static map<std::string, map<std::string, double>> convertion_map;


    NumberWithUnits::NumberWithUnits(double num, string type){
        _num = num;
        _type = type;
        //cout << "new NumberWithUnits(" << _num << ", "<< _type<< ")" << endl << endl;
    }
    
    void NumberWithUnits::read_units(ifstream& units_file){
        string line;
        while (getline(units_file, line)){
            istringstream iss(line);
            double garb_1, val;
            string conv_unit, orig_unit, garb_2;

            if (!(iss >> garb_1 >> orig_unit >> garb_2 >> val >> conv_unit)) {  // error
                break; 
            } 
            
            if (convertion_map.count(orig_unit) != 0){
                convertion_map[orig_unit][conv_unit] = val;
            }
            else{
                map<string, double> new_convert1;
                new_convert1[conv_unit] = val;
                convertion_map[orig_unit] = new_convert1;

            if (convertion_map.count(conv_unit) != 0){
                convertion_map[conv_unit][orig_unit] = 1/val;
            }
            else{
                map<string, double> new_convert2;
                new_convert2[orig_unit] = 1/val;
                convertion_map[conv_unit] = new_convert2;
            }
        }
        // debug:
        // cout << "convertion_map:" << endl;
        // for(map<string, map<string, double>>::const_iterator it = convertion_map.begin();
        //     it != convertion_map.end(); ++it){
        //     cout << it->first << ":" << endl; 
        //     for(map<string, double>::const_iterator inner_it = it->second.begin();
        //     inner_it != it->second.end(); ++inner_it){
        //         cout << inner_it->first << ", " << inner_it->second << endl;
        //     }
        } 
    }
        

    NumberWithUnits NumberWithUnits::convert_type(std::string newType) const{
        map<string, double> convertions = convertion_map.at(this->_type);
        string error_message = "Units do not match - [" + this->_type + "] cant convert to [" + newType + "]" ;
        //cout<< "convert " << _type << " to " << newType << endl;
        if (convertions.empty()){
            throw invalid_argument(error_message);
        }

        if (convertions.count(newType) == 0){
            throw invalid_argument(error_message);
        }
        
        double val = convertions.at(newType);
        return NumberWithUnits(this->_num*val, newType);
    }


    //-------------------------------------
    // binary operators:
    //-------------------------------------

    const NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return NumberWithUnits(_num+new_other.get_num(), _type);
    }

    const NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return NumberWithUnits(_num-new_other.get_num(), _type);
    }


    const NumberWithUnits NumberWithUnits::operator+() const{   //??
        return NumberWithUnits(+_num, _type);
    }
    
    const NumberWithUnits NumberWithUnits::operator-() const{
        return NumberWithUnits(-_num, _type);
    }


    bool NumberWithUnits::operator==(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return _num == new_other.get_num();
    }
    bool NumberWithUnits::operator!=(const NumberWithUnits& other) const{
        return (!(*this == other));
    }


    bool NumberWithUnits::operator>(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return _num > new_other.get_num();

    }
    bool NumberWithUnits::operator>=(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return _num >= new_other.get_num();

    }
    bool NumberWithUnits::operator<(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return _num < new_other.get_num();

    }
    bool NumberWithUnits::operator<=(const NumberWithUnits& other) const{
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        return _num <= new_other.get_num();

    } 

    //-------------------------------------
    // placament operators:
    //-------------------------------------

    NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& other){
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        _num += new_other.get_num();
        return *this;
    }
    NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits& other){
        NumberWithUnits new_other = other;
        if (_type != other.get_type()){
            new_other = other.convert_type(_type);
        }
        _num -= new_other.get_num();
        return *this;
    }


    // prefix:
    NumberWithUnits& NumberWithUnits::operator++() {
        _num++;
        return *this;
    }
    NumberWithUnits& NumberWithUnits::operator--() {
        _num--;
        return *this;
    }

    // postfix:
    const NumberWithUnits NumberWithUnits::operator++(int dummy_flag_for_postfix_increment) {
        NumberWithUnits copy = *this;
        _num++;
        return copy;
    }
    const NumberWithUnits NumberWithUnits::operator--(int dummy_flag_for_postfix_increment) {
        NumberWithUnits copy = *this;
        _num--;
        return copy;
    }


    //-------------------------------------
    // friend binary operators
    //-------------------------------------
            
    const NumberWithUnits operator*(const NumberWithUnits& n, double d){
        return NumberWithUnits(n.get_num()*d, n.get_type());
    }
    const NumberWithUnits operator*(double d, const NumberWithUnits& n){
        return n*d;
    }

    
    //----------------------------------
    // friend global IO operators
    //----------------------------------


    
    ostream& operator<< (ostream& os, const NumberWithUnits& n){
        return (os << n._num << '[' << n._type << ']');
    }
    istream& operator>> (istream& is, NumberWithUnits& n){    // ??
        // Checks format, with rewind on failure.
        double new_num;
        string new_type;

        // remember place for rewinding
        ios::pos_type startPosition = is.tellg();

        if ( (!(is >> new_num))                 ||
            (!NumberWithUnits::getAndCheckNextCharIs(is,'['))  ||
            (!(is >> new_type))                 ||
            (!(NumberWithUnits::getAndCheckNextCharIs(is,']'))) ) {

            // rewind on error
            auto errorState = is.rdstate(); // remember error state
            is.clear(); // clear error so seekg will work
            is.seekg(startPosition); // rewind
            is.clear(errorState); // set back the error flag
        }
        else {
            n._num = new_num;
            n._type = new_type;
        }

        return is;
    }


    istream& NumberWithUnits::getAndCheckNextCharIs(istream& input, char expectedChar) {
        char actualChar;
        input >> actualChar;
        if (!input) return input;

        if (actualChar != expectedChar) 
            // failbit is for format error
            input.setstate(ios::failbit);
        return input;
    }
}
