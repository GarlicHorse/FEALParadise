#ifndef FEAL_H
#define FEAL_H

#include <array>
#include <cstdint>
#include <vector>

class FEALNX {

    public:

        // Constructor that initializes the Feistel cipher with a given number of rounds (N) and encryption key
        // N: Number of Feistel rounds (integer)
        // key: 16-byte encryption key (std::array<uint8_t, 16>)
        FEALNX(int N, std::array<uint8_t, 16> key);

        // Generates derived keys based on the given key (encryption key) and N (number of Feistel rounds)
        void generateDerivedKeys();

        // Encrypts the plaintext (pt) and returns the ciphertext (ct)
        uint64_t encrypt(uint64_t pt);

        // Decrypts the ciphertext (ct) and returns the plaintext (pt)
        uint64_t decrypt(uint64_t ct);

        // / Displays various information, such as the key and the number of Feistel rounds (N)
        void displayInformation();

        // Displays all of the derived keys in a table.
        void displayDerivedKeys();


        // Returs the encryption key
        std::array<uint8_t, 16> getKey() const;

        // Sets a new encryption key
        void setKey(const std::array<uint8_t, 16> key);
        

    private:

        // Number of Feistel rounds
        int N; 
    
        // Encryption key (16 bytes)
        std::array<uint8_t, 16> key; 
    
        // Derived keys (stored as pairs of 2 bytes)
        std::vector<std::array<uint8_t, 2>> subkeys; 
    
        // Performs a bit rotation by shifting bits to the left
        uint8_t rot(uint8_t data, uint8_t shift);
    
        // Performs the SBOX operation (See specification paper for details)
        uint8_t S(uint8_t x, uint8_t y, uint8_t i);
    
        // Feistel function that operates on the input (alpha, beta)
        uint32_t f(uint32_t alpha, uint16_t beta);
    
        // Performs key generation operations to create derived keys
        uint32_t fk(uint32_t alpha, uint32_t beta);

};

#endif