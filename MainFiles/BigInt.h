#pragma once
/*
 * BigInt.h
 *
 *  Created on: Apr 1, 2023
 *      Author: zubair
 *  q2, assignment 3, 22i-0789
 */
#include <iostream>
#include <string>
using namespace std;
#ifndef BIGINT_H_
#define BIGINT_H_

class BigInt {
private:
    int len;
    char* arr;
    bool positive;
    bool isNumPositive() const;
    void changeSign();
public:
    //BigInt();
    BigInt(int val);
    BigInt(const string& text);
    BigInt(const BigInt& copy);
    BigInt(char* arr);

    int getLength() const;
    char getIndex(int index) const;
    char* getArray() const;

    BigInt& operator=(const BigInt& val);
    BigInt& operator=(char* arr);
    BigInt operator+(const BigInt& val);
    BigInt operator+(int val) const;

    BigInt decimalAddition(const BigInt& val) const;
    BigInt decimalAddition(int val) const;

    BigInt operator-(const BigInt& val) const;
    BigInt operator-(int val) const;

    BigInt decimalSubtraction(const BigInt& val) const;
    BigInt decimalSubtraction(int val) const;

    BigInt operator*(const BigInt& val) const;

    BigInt decimalMultiplication(const BigInt& val) const;

    BigInt power(BigInt base, BigInt exponent);

    BigInt decimalPower(BigInt base, BigInt exponent);

    int getEquivalentValue(char letter1, char letter2);
    int getEquivalentValue(char letter1);
    BigInt operator+=(const BigInt& rhs);
    BigInt operator-=(const BigInt& rhs);
    //BigInt operator*=(const BigInt& rhs);

    bool operator==(const BigInt& val) const;
    bool operator==(const char* val) const;
    bool operator<(const BigInt& val) const;
    bool operator>(const BigInt& val) const;
    bool operator<=(const BigInt& val) const;
    bool operator>=(const BigInt& val) const;
    bool operator!=(const BigInt& val) const;

    BigInt& operator++(); // Pre-increment Operator
    BigInt operator++(int); // Post-increment Operator
    BigInt& operator--(); // Pre-decrement Operator
    BigInt operator--(int); // Post-decrement Operator
    BigInt modulus(BigInt&, BigInt&);
    string modHelp(string, BigInt);
    string hexaToBin(string);
    string BinarytoHexa(string bin);

    operator string();
    operator int();

    ~BigInt();
};
//istream& operator>>(istream& input, BigInt& val); // inputs the BigInt

#endif /* BIGINT_H_ */

//Constructors
BigInt::BigInt(int val = 0) {
    this->len = 0;
    long long int temp = val;
    while (temp != 0)
    {
        ++this->len;
        temp = temp / 10;
    }
    if (this->len == 0)
        this->len = 1;
    temp = val;
    if (val >= 0)
    {
        this->positive = true;
        this->arr = new char[this->len + 1] {};
        for (int i = this->len - 1; i >= 0; --i)
        {
            *(this->arr + i) = char((temp % 10) + 48);
            temp = temp / 10;
        }
    }
    else
    {
        ++this->len;
        this->positive = false;
        temp *= -1;
        this->arr = new char[this->len + 1] {};
        for (int i = this->len - 1; i > 0; --i)
        {
            *(this->arr + i) = char((temp % 10) + 48);
            temp = temp / 10;
        }
        *(this->arr + 0) = '-';
    }
}
BigInt::BigInt(const string& text) {
    this->len = 0;
    while (text[len] != '\0')
        ++this->len;
    this->arr = new char[this->len + 1] {};
    for (int i = 0; i < this->len; ++i)
        *(this->arr + i) = text[i];
    if (*(this->arr + 0) == '-')
        this->positive = false;
    else
        this->positive = true;
}
BigInt::BigInt(const BigInt& copy) {
    this->len = copy.getLength();
    this->arr = new char[this->len + 1] {};
    for (int i = 0; i < this->len; ++i)
        *(this->arr + i) = copy.getIndex(i);
    this->positive = copy.isNumPositive();
}
BigInt::BigInt(char* arr) {
    this->len = 0;
    while (*(arr + this->len) != '\0')
        ++this->len;
    this->arr = new char[this->len + 1] {};
    for (int i = 0; i < this->len; ++i)
        *(this->arr + i) = *(arr + i);
    if (*(this->arr + 0) == '-')
        this->positive = false;
    else
        this->positive = true;
}
//Private Helper Member Functions
bool BigInt::isNumPositive() const {
    return this->positive;
}
void BigInt::changeSign() {
    if (this->positive)
    {
        this->positive = false;
        char* temp = new char[this->len + 2] {};
        for (int i = 0; i < this->len; ++i)
            *(temp + i + 1) = *(this->arr + i);
        *(temp + 0) = '-';
        ++this->len;
        delete[] this->arr;
        this->arr = temp;
        temp = NULL;
    }
    else
    {
        this->positive = true;
        char* temp = new char[this->len] {};
        for (int i = 0; i < this->len - 1; ++i)
            *(temp + i) = *(this->arr + i + 1);
        --this->len;
        delete[] this->arr;
        this->arr = temp;
        temp = NULL;
    }
}
//Getters
int BigInt::getLength() const {
    return this->len;
}
char BigInt::getIndex(int index) const {
    if (index < this->len)
        return *(this->arr + index);
    else
    {
        cout << "The given index is out of bound\n";
        char temp = '\0';
        return temp;
    }
}
char* BigInt::getArray() const {
    return this->arr;
}
int BigInt::getEquivalentValue(char letter1, char letter2) {
    int value1 = 0, value2 = 0;
    switch (letter1) {
    case 'a':
    case 'A':
        value1 = 10;
        break;
    case 'b':
    case 'B':
        value1 = 11;
        break;
    case 'c':
    case 'C':
        value1 = 12;
        break;
    case 'd':
    case 'D':
        value1 = 13;
        break;
    case 'e':
    case 'E':
        value1 = 14;
        break;
    case 'f':
    case 'F':
        value1 = 15;
        break;
    default:
        value1 = letter1 - 48;
        break;
    }
    switch (letter2) {
    case 'a':
    case 'A':
        value2 = 10;
        break;
    case 'b':
    case 'B':
        value2 = 11;
        break;
    case 'c':
    case 'C':
        value2 = 12;
        break;
    case 'd':
    case 'D':
        value2 = 13;
        break;
    case 'e':
    case 'E':
        value2 = 14;
        break;
    case 'f':
    case 'F':
        value2 = 15;
        break;
    default:
        value2 = letter2 - 48;
        break;
    }
    return value1 + value2;
}
int BigInt::getEquivalentValue(char letter1) {
    int value1 = 0;
    switch (letter1) {
    case 'a':
    case 'A':
        value1 = 10;
        break;
    case 'b':
    case 'B':
        value1 = 11;
        break;
    case 'c':
    case 'C':
        value1 = 12;
        break;
    case 'd':
    case 'D':
        value1 = 13;
        break;
    case 'e':
    case 'E':
        value1 = 14;
        break;
    case 'f':
    case 'F':
        value1 = 15;
        break;
    default:
        value1 = letter1 - 48;
        break;
    }
    return value1;
}
// arithmetic operators
BigInt& BigInt::operator=(const BigInt& val) {
    this->len = val.getLength();
    delete[] this->arr;
    this->arr = NULL;
    this->arr = new char[this->len + 1] {};
    for (int i = 0; i < this->len; ++i)
        *(this->arr + i) = val.getIndex(i);
    this->positive = val.isNumPositive();
    return *this;
}
BigInt& BigInt::operator=(char* arr) {
    this->len = 0;
    while (*(arr + this->len) != '\0')
        ++this->len;
    delete[] this->arr;
    this->arr = NULL;
    this->arr = new char[this->len + 1] {};
    for (int i = 0; i < this->len; ++i)
        *(this->arr + i) = *(arr + i);
    if (*(this->arr + 0) == '-')
        this->positive = false;
    else
        this->positive = true;
    return *this;
}
BigInt BigInt::operator+(const BigInt& val) {
    if (this->positive == false || val.isNumPositive() == false)
    {
        BigInt result;
        if (this->positive == false && val.isNumPositive() == false)
        {
            BigInt temp1(*this), temp2(val);
            temp1.changeSign();
            temp2.changeSign();
            result = temp1 + temp2;
            result.changeSign();
        }
        else if (this->positive == false)
        {
            BigInt temp1(*this);
            temp1.changeSign();
            result = val - temp1;
        }
        else
        {
            BigInt temp1(val);
            temp1.changeSign();
            result = *this - temp1;
        }
        return result;
    }
    else
    {
        int newLen = 0;
        if (this->len > val.getLength())
            newLen = this->len;
        else
            newLen = val.getLength();
        char* result = new char[newLen + 1] {};
        int carry = 0, sum = 0;
        int count1 = this->len - 1;
        int count2 = val.getLength() - 1;
        int count3 = newLen - 1;
        while (count1 >= 0 && count2 >= 0)
        {
            sum = 0;
            //sum = carry + ((int(*(this->arr + count1)) - 48)) + (int(val.getIndex(count2)) - 48);
            sum = carry + this->getEquivalentValue(*(this->arr + count1), val.getIndex(count2));
            carry = sum / 16;
            sum = sum % 16;
            //*(result + count3) = char(sum + 48);
            switch (sum) {
            case 10:
                *(result + count3) = 'a';
                break;
            case 11:
                *(result + count3) = 'b';
                break;
            case 12:
                *(result + count3) = 'c';
                break;
            case 13:
                *(result + count3) = 'd';
                break;
            case 14:
                *(result + count3) = 'e';
                break;
            case 15:
                *(result + count3) = 'f';
                break;
            default:
                *(result + count3) = char(sum + 48);
                break;
            }
            --count1;
            --count2;
            --count3;
        }
        while (count1 >= 0)
        {
            sum = 0;
            //sum = carry + ((int(*(this->arr + count1)) - 48));
            sum = carry + this->getEquivalentValue(*(this->arr + count1));
            carry = sum / 16;
            sum = sum % 16;
            //*(result + count3) = char(sum + 48);
            switch (sum) {
            case 10:
                *(result + count3) = 'a';
                break;
            case 11:
                *(result + count3) = 'b';
                break;
            case 12:
                *(result + count3) = 'c';
                break;
            case 13:
                *(result + count3) = 'd';
                break;
            case 14:
                *(result + count3) = 'e';
                break;
            case 15:
                *(result + count3) = 'f';
                break;
            default:
                *(result + count3) = char(sum + 48);
                break;
            }
            --count1;
            --count3;
        }
        while (count2 >= 0)
        {
            //sum = carry + (int(val.getIndex(count2)) - 48);
            sum = carry + this->getEquivalentValue(val.getIndex(count2));
            carry = sum / 16;
            sum = sum % 16;
            //*(result + count3) = char(sum + 48);
            switch (sum) {
            case 10:
                *(result + count3) = 'a';
                break;
            case 11:
                *(result + count3) = 'b';
                break;
            case 12:
                *(result + count3) = 'c';
                break;
            case 13:
                *(result + count3) = 'd';
                break;
            case 14:
                *(result + count3) = 'e';
                break;
            case 15:
                *(result + count3) = 'f';
                break;
            default:
                *(result + count3) = char(sum + 48);
                break;
            }
            --count2;
            --count3;
        }
        if (carry != 0)
        {
            int len = 0;
            while (*(result + len) != '\0')
                ++len;
            char* tempResult = new char[len + 2] {};
            for (int i = len - 1; i >= 0; --i)
                *(tempResult + i + 1) = *(result + i);
            *(tempResult + 0) = char(carry + 48);
            delete[] result;
            result = NULL;
            result = tempResult;
            tempResult = NULL;
        }
        BigInt temp(result);
        delete[] result;
        result = NULL;
        return temp;
    }
}
BigInt BigInt::operator+(int val) const {
    BigInt temp(val);
    return *this + temp;
}
// performs addition according to base 10
BigInt BigInt::decimalAddition(const BigInt& val) const {
    if (this->positive == false || val.isNumPositive() == false)
    {
        BigInt result;
        if (this->positive == false && val.isNumPositive() == false)
        {
            BigInt temp1(*this), temp2(val);
            temp1.changeSign();
            temp2.changeSign();
            result = temp1 + temp2;
            result.changeSign();
        }
        else if (this->positive == false)
        {
            BigInt temp1(*this);
            temp1.changeSign();
            result = val - temp1;
        }
        else
        {
            BigInt temp1(val);
            temp1.changeSign();
            result = *this - temp1;
        }
        return result;
    }
    else
    {
        int newLen = 0;
        if (this->len > val.getLength())
            newLen = this->len;
        else
            newLen = val.getLength();
        char* result = new char[newLen + 1] {};
        int carry = 0, sum = 0;
        int count1 = this->len - 1;
        int count2 = val.getLength() - 1;
        int count3 = newLen - 1;
        while (count1 >= 0 && count2 >= 0)
        {
            sum = 0;
            sum = carry + ((int(*(this->arr + count1)) - 48)) + (int(val.getIndex(count2)) - 48);
            carry = sum / 10;
            sum = sum % 10;
            *(result + count3) = char(sum + 48);
            --count1;
            --count2;
            --count3;
        }
        while (count1 >= 0)
        {
            sum = 0;
            sum = carry + ((int(*(this->arr + count1)) - 48));
            carry = sum / 10;
            sum = sum % 10;
            *(result + count3) = char(sum + 48);
            --count1;
            --count3;
        }
        while (count2 >= 0)
        {
            sum = carry + (int(val.getIndex(count2)) - 48);
            carry = sum / 10;
            sum = sum % 10;
            *(result + count3) = char(sum + 48);
            --count2;
            --count3;
        }
        if (carry != 0)
        {
            int len = 0;
            while (*(result + len) != '\0')
                ++len;
            char* tempResult = new char[len + 2] {};
            for (int i = len - 1; i >= 0; --i)
                *(tempResult + i + 1) = *(result + i);
            *(tempResult + 0) = char(carry + 48);
            delete[] result;
            result = NULL;
            result = tempResult;
            tempResult = NULL;
        }
        BigInt temp(result);
        delete[] result;
        result = NULL;
        return temp;
    }
}
// performs addition according to base 10
BigInt BigInt::decimalAddition(int val) const {
    BigInt temp(val);
    return this->decimalAddition(temp);
}

BigInt BigInt::operator-(const BigInt& val) const {
    if (*this == val)
    {
        BigInt result;
        return result;
    }
    else if (this->positive == false || val.isNumPositive() == false)
    {
        BigInt result;
        if (this->positive == false && val.isNumPositive() == false)
        {
            BigInt temp1(*this), temp2(val);
            temp1.changeSign();
            temp2.changeSign();
            result = temp2 - temp1;
        }
        else if (this->positive == false)
        {
            BigInt temp1(*this);// temp2( val );
            temp1.changeSign();
            //temp2.changeSign();
            result = temp1 + val;
            result.changeSign();
        }
        else
        {
            BigInt temp1(val);
            temp1.changeSign();
            result = *this + temp1;
            //result.changeSign();
        }
        return result;
    }
    else
    {
        char* result, * complement;
        if (this->len >= val.getLength())
        {
            result = new char[this->len + 1] {};
            complement = new char[this->len + 1] {};
            for (int i = 0; i < this->len; ++i)
                *(complement + i) = '9';
            int counter = this->len - 1;
            for (int i = val.getLength() - 1; i >= 0; --i)
            {
                *(complement + counter) = char((9 - ((int(val.getIndex(i))) - 48)) + 48);
                --counter;
            }
            BigInt temp(complement);
            temp += 1;
            delete[] complement;
            complement = NULL;
            complement = temp.getArray();
            int sum = 0;
            int carry = 0;
            for (int i = this->len - 1; i >= 0; --i)
            {
                sum = 0;
                sum = carry + (((int(*(this->arr + i))) - 48)) + ((int(*(complement + i))) - 48);
                carry = sum / 10;
                sum = sum % 10;
                *(result + i) = char(sum + 48);
            }
            if (carry != 0)
            {
                while (*(result + 0) == '0') //&& carry != 0)
                {
                    char* temp = new char[this->len] {};
                    int counter = 0;
                    for (int i = 1; i < this->len; ++i)
                    {
                        *(temp + counter) = *(result + i);
                        ++counter;
                    }
                    delete[] result;
                    result = NULL;
                    result = temp;
                    temp = NULL;
                }
                temp = result;
                return temp;
            }
            else
            {
                for (int i = 0; i < this->len; ++i)
                    *(result + i) = char((9 - (char(*(result + i)) - 48)) + 48);
                temp = result;
                temp += 1;
                temp.changeSign();
                return temp;
            }
        }
        else
        {
            result = new char[val.getLength() + 1] {};
            complement = new char[val.getLength() + 1] {};
            for (int i = 0; i < this->len; ++i)
                *(complement + i) = '9';
            int counter = val.getLength() - 1;
            for (int i = this->len - 1; i >= 0; --i)
            {
                *(complement + counter) = char((9 - ((int(*(this->arr + i))) - 48)) + 48);
                --counter;
            }
            BigInt temp(complement);
            temp += 1;
            delete[] complement;
            complement = NULL;
            complement = temp.getArray();
            int sum = 0;
            int carry = 0;
            for (int i = val.getLength() - 1; i >= 0; --i)
            {
                sum = 0;
                sum = carry + ((int(val.getIndex(i))) - 48) + ((int(*(complement + i))) - 48);
                carry = sum / 10;
                sum = sum % 10;
                *(result + i) = char(sum + 48);
            }
            if (carry != 0)
            {
                while (*(result + 0) == '0') //&& carry != 0)
                {
                    char* temp = new char[this->len] {};
                    int counter = 0;
                    for (int i = 1; i < this->len; ++i)
                    {
                        *(temp + counter) = *(result + i);
                        ++counter;
                    }
                    delete[] result;
                    result = NULL;
                    result = temp;
                    temp = NULL;
                }
                temp = result;
                return temp;
            }
            else
            {
                for (int i = 0; i < val.getLength(); ++i)
                    *(result + i) = char((9 - (char(*(result + i)) - 48)) + 48);
                temp = result;
                temp += 1;
                temp.changeSign();
                return temp;
            }
        }
    }
}
BigInt BigInt::operator-(int val) const {
    BigInt temp(val);
    return *this - temp;
}

// performs subtraction according to base 10
BigInt BigInt::decimalSubtraction(const BigInt& val) const {
    if (*this == val)
    {
        BigInt result;
        return result;
    }
    else if (this->positive == false || val.isNumPositive() == false)
    {
        BigInt result;
        if (this->positive == false && val.isNumPositive() == false)
        {
            BigInt temp1(*this), temp2(val);
            temp1.changeSign();
            temp2.changeSign();
            result = temp2 - temp1;
        }
        else if (this->positive == false)
        {
            BigInt temp1(*this);// temp2( val );
            temp1.changeSign();
            //temp2.changeSign();
            result = temp1 + val;
            result.changeSign();
        }
        else
        {
            BigInt temp1(val);
            temp1.changeSign();
            result = *this + temp1;
            //result.changeSign();
        }
        return result;
    }
    else
    {
        char* result, * complement;
        if (this->len >= val.getLength())
        {
            result = new char[this->len + 1] {};
            complement = new char[this->len + 1] {};
            for (int i = 0; i < this->len; ++i)
                *(complement + i) = '9';
            int counter = this->len - 1;
            for (int i = val.getLength() - 1; i >= 0; --i)
            {
                *(complement + counter) = char((9 - ((int(val.getIndex(i))) - 48)) + 48);
                --counter;
            }
            BigInt temp(complement);
            temp += 1;
            delete[] complement;
            complement = NULL;
            complement = temp.getArray();
            int sum = 0;
            int carry = 0;
            for (int i = this->len - 1; i >= 0; --i)
            {
                sum = 0;
                sum = carry + (((int(*(this->arr + i))) - 48)) + ((int(*(complement + i))) - 48);
                carry = sum / 10;
                sum = sum % 10;
                *(result + i) = char(sum + 48);
            }
            if (carry != 0)
            {
                while (*(result + 0) == '0') //&& carry != 0)
                {
                    char* temp = new char[this->len] {};
                    int counter = 0;
                    for (int i = 1; i < this->len; ++i)
                    {
                        *(temp + counter) = *(result + i);
                        ++counter;
                    }
                    delete[] result;
                    result = NULL;
                    result = temp;
                    temp = NULL;
                }
                temp = result;
                return temp;
            }
            else
            {
                for (int i = 0; i < this->len; ++i)
                    *(result + i) = char((9 - (char(*(result + i)) - 48)) + 48);
                temp = result;
                temp += 1;
                temp.changeSign();
                return temp;
            }
        }
        else
        {
            result = new char[val.getLength() + 1] {};
            complement = new char[val.getLength() + 1] {};
            for (int i = 0; i < this->len; ++i)
                *(complement + i) = '9';
            int counter = val.getLength() - 1;
            for (int i = this->len - 1; i >= 0; --i)
            {
                *(complement + counter) = char((9 - ((int(*(this->arr + i))) - 48)) + 48);
                --counter;
            }
            BigInt temp(complement);
            temp += 1;
            delete[] complement;
            complement = NULL;
            complement = temp.getArray();
            int sum = 0;
            int carry = 0;
            for (int i = val.getLength() - 1; i >= 0; --i)
            {
                sum = 0;
                sum = carry + ((int(val.getIndex(i))) - 48) + ((int(*(complement + i))) - 48);
                carry = sum / 10;
                sum = sum % 10;
                *(result + i) = char(sum + 48);
            }
            if (carry != 0)
            {
                while (*(result + 0) == '0') //&& carry != 0)
                {
                    char* temp = new char[this->len] {};
                    int counter = 0;
                    for (int i = 1; i < this->len; ++i)
                    {
                        *(temp + counter) = *(result + i);
                        ++counter;
                    }
                    delete[] result;
                    result = NULL;
                    result = temp;
                    temp = NULL;
                }
                temp = result;
                return temp;
            }
            else
            {
                for (int i = 0; i < val.getLength(); ++i)
                    *(result + i) = char((9 - (char(*(result + i)) - 48)) + 48);
                temp = result;
                temp += 1;
                temp.changeSign();
                return temp;
            }
        }
    }
}
// performs subtraction according to base 10
BigInt BigInt::decimalSubtraction(int val) const {
    BigInt temp(val);
    return this->decimalSubtraction(temp);
}

BigInt BigInt::operator*(const BigInt& val) const {
    BigInt temp("1");
    BigInt result(*this);
    for (; temp < val; ++temp) {
        result += *this;
    }
    return result;
}

// performs multiplication according to base 10
BigInt BigInt::decimalMultiplication(const BigInt& val) const {
    BigInt temp("1");
    BigInt result(*this);
    for (; temp < val; temp = temp.decimalAddition(1)) {
        result = result.decimalAddition(*this);
    }
    return result;

}

BigInt BigInt::power(BigInt base, BigInt exponent) {

    BigInt temp("1");
    if (exponent == "0") return temp;
    BigInt newtemp(base);
    for (; temp < exponent; ++temp) {
        base = base * newtemp;
    }
    return base;
}

// performs power according to base 10
BigInt BigInt::decimalPower(BigInt base, BigInt exponent) {
    BigInt temp("1");
    if (exponent == "0") return temp;
    BigInt newtemp(base);
    for (; temp < exponent; temp = temp.decimalAddition(1)) {
        base = base.decimalMultiplication(newtemp);
    }
    return base;

}
BigInt BigInt::operator+=(const BigInt& rhs) {
    *this = *this + rhs;
    return *this;
}
BigInt BigInt::operator-=(const BigInt& rhs) {
    *this = *this - rhs;
    return *this;
}
// BigInt BigInt::operator*=(const BigInt& rhs) {
//   BigInt temp = (*this) * (rhs);
//   *this = temp;
//   return *this;
// } 

bool BigInt::operator==(const BigInt& val) const {
    if (this->len != val.getLength())
        return false;
    else
    {
        for (int i = 0; i < this->len; ++i)
            if (*(this->arr + i) != val.getIndex(i))
                return false;
        return true;
    }
}
bool BigInt::operator==(const char* val) const {
    int len = 0;
    while (*(val + len) != '\0')
        ++len;
    if (this->len != len)
        return false;
    else
    {
        for (int i = 0; i < this->len; ++i)
            if (*(this->arr + i) != *(val + i))
                return false;
        return true;
    }
}
bool BigInt::operator<(const BigInt& val) const {
    if (this->len > val.getLength())
        return false;
    else if (this->len < val.getLength())
        return true;
    else if (*this == val)
        return false;
    else
    {
        for (int i = 0; i < this->len; ++i) {
            if (int(*(this->arr + i)) > int(val.getIndex(i)))
                return false;
            else if(int(*(this->arr + i)) < int(val.getIndex(i)))
                return true;
        }
        return true;
    }
}
bool BigInt::operator>(const BigInt& val) const {
    if (this->len > val.getLength())
        return true;
    else if (this->len < val.getLength())
        return false;
    else if (*this == val)
        return false;
    else
    {
        for (int i = 0; i < this->len; ++i) {
          //  cout << int(*(this->arr + i)) << " " << int(val.getIndex(i)) << endl;
            if (int(*(this->arr + i)) < int(val.getIndex(i)))
                return false;
            else if (int(*(this->arr + i)) > int(val.getIndex(i)))
                return true;
        }
        return true;
    }
}
bool BigInt::operator<=(const BigInt& val) const {
    if (*this == val)
        return true;
    else if (*this < val)
        return true;
    else
        return false;
}
bool BigInt::operator>=(const BigInt& val) const {
    if (*this == val)
        return true;
    else if (*this > val)
        return true;
    else
        return false;
}
bool BigInt::operator!=(const BigInt& val) const {
    if (*this == val)
        return false;
    else
        return true;
}

BigInt& BigInt::operator++() {
    *this += 1;
    return *this;
}
BigInt BigInt::operator++(int) {
    BigInt temp(*this);
    *this += 1;
    return temp;
}
BigInt& BigInt::operator--() {
    *this -= 1;
    return *this;
}
BigInt BigInt::operator--(int) {
    BigInt temp(*this);
    *this -= 1;
    return temp;
}

string BigInt::hexaToBin(string hexaNum) {
    string result = "";
    int size = hexaNum.size();
    for (int i = 0; i < size; ++i) {
        switch (hexaNum[i]) {
        case '0':
            result += "0000";
            break;
        case '1':
            result += "0001";
            break;
        case '2':
            result += "0010";
            break;
        case '3':
            result += "0011";
            break;
        case '4':
            result += "0100";
            break;
        case '5':
            result += "0101";
            break;
        case '6':
            result += "0110";
            break;
        case '7':
            result += "0111";
            break;
        case '8':
            result += "1000";
            break;
        case '9':
            result += "1001";
            break;
        case 'A':
        case 'a':
            result += "1010";
            break;
        case 'B':
        case 'b':
            result += "1011";
            break;
        case 'C':
        case 'c':
            result += "1100";
            break;
        case 'D':
        case 'd':
            result += "1101";
            break;
        case 'E':
        case 'e':
            result += "1110";
            break;
        case 'F':
        case 'f':
            result += "1111";
            break;
        default:
            cout << "\nInvalid hexaDecimal...\n";
            break;
        }
    }
    return result;
}
string BigInt::BinarytoHexa(string bin) {
    string final = bin, result = "";
    bool leadingZeroes = true;
    if (bin.size() % 4 != 0) {
        int mod = bin.size() % 4;
        for (int i = 0; i < 4 - mod; ++i) {
            char ch = '0';
            final = ch + final;
        }
    }
    while (final.size() > 0) {
        if (final.substr(0, 4) == "0000") {
            if (leadingZeroes == false)
                result += '0';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "0001") {
            leadingZeroes = false;
            result += '1';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "0010") {
            leadingZeroes = false;
            result += '2';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "0011") {
            leadingZeroes = false;
            result += '3';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "0100") {
            leadingZeroes = false;
            result += '4';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "0101") {
            leadingZeroes = false;
            result += '5';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "0110") {
            leadingZeroes = false;
            result += '6';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "0111") {
            leadingZeroes = false;
            result += '7';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "1000") {
            leadingZeroes = false;
            result += '8';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "1001") {
            leadingZeroes = false;
            result += '9';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "1010") {
            leadingZeroes = false;
            result += 'a';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "1011") {
            leadingZeroes = false;
            result += 'b';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "1100") {
            leadingZeroes = false;
            result += 'c';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "1101") {
            leadingZeroes = false;
            result += 'd';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "1110") {
            leadingZeroes = false;
            result += 'e';
            final.erase(0, 4);
        }
        else if (final.substr(0, 4) == "1111") {
            leadingZeroes = false;
            result += 'f';
            final.erase(0, 4);
        }
    }
    if (leadingZeroes == true)
        result += "0";
    return result;
}
string BigInt::modHelp(string hexa, BigInt num) {
    string Binary = hexaToBin(hexa);

    int size = Binary.size();
    string num1 = string(num);
    int numInteger = stoi(num1);

    string substring;
    if (size < numInteger)
        substring = Binary;
    else
        substring = Binary.substr(size - numInteger, numInteger);

    return BinarytoHexa(substring);

}
BigInt BigInt::modulus(BigInt& hexa, BigInt& num) {
    BigInt temp(modHelp(string(hexa), num));
    return temp;
}

//Type Conversion
BigInt::operator string() {
    if (this->arr == NULL)
        return " ";
    else 
        return this->arr;
}
BigInt::operator int() {
    if (this->positive)
        return this->len;
    else
        return this->len - 1;
}
BigInt::~BigInt() {
    delete[] this->arr;
    this->arr = NULL;
}
ostream& operator<<(ostream& output, const BigInt& val) {
    output << val.getArray();
    return output;
}
istream& operator>>(istream& input, BigInt& val) {
    cout << "How many digits in the number to be input, including -ve sign if it applies: ";
    int temp;
    cin >> temp;
    char* tempInput = new char[temp + 1] {};
    char temp2;
    for (int i = 0; i < temp; ++i)
    {
        input >> temp2;
        *(tempInput + i) = temp2;
    }
    val = tempInput;
    delete[] tempInput;
    tempInput = NULL;
    return input;
}