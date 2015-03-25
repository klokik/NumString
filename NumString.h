#ifndef NUMSTRING_H
#define NUMSTRING_H

#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>

template <typename T,unsigned int C_min,unsigned int C_max>
class NumString: std::vector<T>
{
    typedef std::vector<T> vector_type;
public:
    typedef typename vector_type::reference	reference;
    typedef typename vector_type::const_reference const_reference;
    typedef typename vector_type::iterator iterator;
    typedef typename vector_type::const_iterator const_iterator;
    typedef typename vector_type::size_type size_type;
    typedef typename vector_type::value_type value_type;

    using vector_type::begin;
    using vector_type::end;
    using vector_type::size;
    using vector_type::rbegin;
    using vector_type::rend;
    using vector_type::operator[];
    using vector_type::erase;
    using vector_type::empty;
    using vector_type::push_back;
    using vector_type::insert;
    using vector_type::resize;

    /* insert a new element into position
        @param pos: iterator - position
        @param val: value to insert
        @result: true if holds
    */
    iterator insert(iterator pos,value_type const &val = value_type())
    {
        if(val >= C_min || val <= C_max)
            vector_type::insert(pos,val);
        else
            throw std::invalid_argument();
    }

    /* check if dat numstring is a number in given base
        @param base: given base
        @result: true if holds
    */
    bool isBase(unsigned int base) const
    {
        return std::all_of(vector_type::begin(),vector_type::end(),[base](auto a){return (a >= 0 || a < base);});
    }

    /* check if dat numstring is binary
        @result: true if holds
    */
    bool isBinary()
    {
        isBase(2);
    }

    /* check if dat numstring is decimal
        @result: true if holds
    */
    bool isDecimal()
    {
        isBase(10);
    }

    /* check if dat numstring is hexadecimal
        @result: true if holds
    */
    bool isHexadecimal()
    {
        isBase(16);
    }

    /* get the length
        @result: length of dat number
    */
    size_type dim() const
    {
        return vector_type::size();
    }

    /* append given number to dat number
        @param val: number to append
    */
    template <typename nT>
    void append(std::vector<nT> const &val)
    {
        std::for_each(val.begin(),val.end(),[](auto a){vector_type::push_back(a);});
    }

    /* converts from arbitrary base to arbitrary base
        @templateparam T: type of a digit
        @templateparam new_base: new base
        @result: dat number in given base
    */
    template <unsigned int new_base>
    NumString<T,0,new_base> toBase(unsigned int src_base)
    {
        static_assert(new_base > 1,"conversion to base 1 or 0 unsupported");

        if(!isBase(src_base))
            throw std::logic_error("source is not in desired base");

        NumString<T,0,new_base> result;
        NumString<T,C_min,C_max> copy = *this;

        copy.truncate();
        while(copy.size()!=0)
        {
            uint val = copy.modInBase(new_base,src_base);
            copy = copy.divideInBaseBy(new_base,src_base);

            result.push_back(val);
        }

        return result;
    }

    /* remove most significant zeroes from dat numstring */
    void truncate()
    {
        while(vector_type::size() && *vector_type::rbegin() == 0)
            vector_type::resize(vector_type::size()-1);
    }

    /* divides dat number in given base by given number
        @param divisor
        @param base
    */
    NumString<T,C_min,C_max> divideInBaseBy(unsigned int divisor,unsigned int base)
    {
        NumString<T,C_min,C_max> result;

        unsigned int cnum = 0;
        int i=0;
        for(auto it=vector_type::rbegin();it!=vector_type::rend();)
        {
            cnum += (*it++);
            i++;
            while(cnum < divisor)
            {
                if(it == vector_type::rend())
                    break;

                cnum = cnum*base + *(it++);
                i++;

                result.push_back(0);
            }
            if(cnum < divisor) // still not reached
            {
                result.push_back(0);
                break;
            }

            result.push_back(cnum/divisor);
            cnum = (cnum%divisor)*base;
        }

        // convert endiannes
        std::reverse(result.begin(),result.end());
        // remove trailing zeroes
        result.truncate();

        return result;
    }

    /* subtract given number from dat number in given base
        @param b
        @param base
        @result: new numbstring dat-b
    */
    NumString<T,C_min,C_max> subtractInBase(NumString<T,C_min,C_max> b,unsigned int base)
    {
        if(b.size()>vector_type::size())
            throw std::logic_error("result is negative");

        NumString<T,C_min,C_max> result;

        int carry_flag = 0;
        for(uint q=0;q<vector_type::size();q++)
        {
            int cval = vector_type::at(q)-carry_flag;
            int ex_b_val = (q<b.size())?b[q]:0;
            if(cval < ex_b_val)
            {
                carry_flag = 1;
                cval += base;
            }
            else
                carry_flag = 0;
            result.push_back(cval-ex_b_val);
        }

        return result;
    }

    /* multiply dat number by given number in given base
        @param b
        @param base
        @result: new numbstring dat*b
    */
    NumString<T,C_min,C_max> multiplyInBase(unsigned int b,unsigned int base)
    {
        NumString<T,C_min,C_max> result;

        int carry_val = 0;
        for(uint q=0;q<vector_type::size();q++)
        {
            uint cval = (vector_type::at(q)*b+carry_val);
            result.push_back(cval%base);

            carry_val = cval/base;
        }

        while(carry_val)
        {
            result.push_back(carry_val%base);
            carry_val /= base;
        }

        return result;
    }

    /* get quotient
        @param b
        @param base
        @result: dat mod b
    */
    unsigned int modInBase(unsigned int b,unsigned int base)
    {
        auto a = divideInBaseBy(b,base);
        auto k = a.multiplyInBase(b,base);
        auto c = subtractInBase(k,base);

        c.truncate();
        int q = 1;
        uint sum = 0;
        for(auto item:c)
        {
            sum += item*q;
            q *= base;
        }
        return sum;
    }

    /* checks if given number is substring of dat number
        @param subs: given string
        @result: true if it is false if not
    */
    bool containsSubstring(NumString<T,C_min,C_max> subs)
    {
        return (std::search(vector_type::begin(),vector_type::end(),subs.begin(),subs.end()) != vector_type::end());
    }
};

#endif // NUMSTRING_H
