#include <iostream>
#include <string>

#include "feal.h"

int main() {

    std::array<uint8_t, 16> key = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

    int N = 32;

    FEALNX feal = FEALNX(N,key);

    feal.displayInformation();
    
    feal.generateDerivedKeys();

    feal.displayDerivedKeys();

    uint64_t pt = 0x0123456789ABCDEF;
    uint64_t ct = feal.encrypt(pt);

    std::cout << "The plaintext is : " << std::hex << pt << '\n';
    std::cout << "The ciphertext is : " << std::hex << ct << '\n';
    std::cout << "The decrypted ciphertext is : " << std::hex << feal.decrypt(ct) << '\n';

    return 0;

}
