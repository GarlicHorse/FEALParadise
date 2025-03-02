#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "feal.h"

TEST_CASE("Testing FEALNX", "[FEAL]") {

    std::array<uint8_t, 16> key = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

    int N = 32; // Number of rounds

    FEALNX feal(N,key); // Instanciation de l'objet (ajoute une clé si nécessaire)
    
    feal.generateDerivedKeys();

    SECTION("Ensure Correctness") {

        uint64_t plaintext = 0x1234567812345678;
        
        auto ciphertext = feal.encrypt(plaintext);
        auto decrypted = feal.decrypt(ciphertext);

        REQUIRE(decrypted == plaintext);
    }

    SECTION("Plaintext with min value") {

        uint64_t plaintext = 0;
        
        auto ciphertext = feal.encrypt(plaintext);
        auto decrypted = feal.decrypt(ciphertext);

        REQUIRE(decrypted == plaintext);
    }

    SECTION("Plaintext with max value") {

        uint64_t plaintext = 0xFFFFFFFFFFFFFFFF;
        
        auto ciphertext = feal.encrypt(plaintext);
        auto decrypted = feal.decrypt(ciphertext);

        REQUIRE(decrypted == plaintext);
    }

    SECTION("Plaintext with pattern value") {

        uint64_t plaintext = 0x0101010101010101;
        
        auto ciphertext = feal.encrypt(plaintext);
        auto decrypted = feal.decrypt(ciphertext);

        REQUIRE(decrypted == plaintext);
    }





}


