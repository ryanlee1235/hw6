#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>
#include <cstdlib>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        // store each group of 6 in array
        unsigned long long w[5];
        int r = 36; // 36 constant
        // number of full groups of 6
        int runNum = (int)k.length() / 6;
        int length = k.length();
        // number of slots filled in a non-filled group
        int remainder = (int)k.length() % 6;

        int count = runNum;
        // loop through w[]
        for(int i = 0; i < 5; i++)
        {
            // start w[4 - i] as 0
            w[4 - i] = 0;
            if(i <= runNum)
            {
                // loop through substring of size 6
                for(int j = 0; j < 6; j++)
                {
                    HASH_INDEX_T kHash;
                    if(count > 0)
                    {
                        // formula for if its a full container
                        kHash = letterDigitToNumber(k[j + (length - (6 * (i + 1)))]);
                    }
                    else
                    {
                        // leading 0's
                        if(j < (6 - remainder))
                        {
                            kHash = 0;
                        }
                        else
                        {
                            kHash = letterDigitToNumber(k[j - (6 - remainder)]);
                        }
                    }
                    // std::cout << "kHash: " << kHash << std::endl;
                    // linear power code
                    w[4 - i] *= r;
                    w[4 - i] += kHash;
                }
            }
            // else
            // {
            //     w[4 - i] = 0;
            // }
            if(count > 0)
            {
                count--;
            }
        }

        HASH_INDEX_T finalHash = 0;
        for(int i = 0; i < 5; i++)
        {
            finalHash += rValues[i] * w[i];
            // std::cout << "w[" << i << "]: " << w[i] << std::endl;
        }
        return finalHash;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        char newLetter = letter;
        if(letter <= 'Z' && letter >= 'A')
        {
            newLetter = letter + ('a' - 'A'); // Converts Uppercase to lowercase
        }
        HASH_INDEX_T index;
        if(newLetter <= 'z' && newLetter >= 'a')
        {
            index = std::size_t(newLetter - 'a');
        }
        else if(newLetter <= '9' && newLetter >= '0')
        {
            index = std::size_t(newLetter - '0') + 26;
        }
        return index;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
