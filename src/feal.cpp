#include <stdexcept>
#include <cstdint>
#include <iostream>
#include <iomanip>

#include "feal.h"

template <typename T>
auto concatIntegers(T high, T low) {
    static_assert(std::is_integral<T>::value, "T doit être un type entier");

    using DoubleT = typename std::conditional<sizeof(T) == 1, uint16_t,
                    typename std::conditional<sizeof(T) == 2, uint32_t,
                    typename std::conditional<sizeof(T) == 4, uint64_t,
                    void>::type>::type>::type;

    static_assert(!std::is_same<DoubleT, void>::value, "Type non pris en charge");

    return (static_cast<DoubleT>(high) << (8 * sizeof(T))) | static_cast<DoubleT>(low);
}

template <typename T, typename... Args>
auto concatIntegers(T first, Args... rest) {
    return (static_cast<typename std::make_unsigned<T>::type>(first) << (8 * sizeof...(rest) * sizeof(T))) 
           | concatIntegers(rest...);
}


/*

This function is used to compute the SBOX.

*/

FEALNX::FEALNX(int N, std::array<uint8_t, 16> initKey) : N(N) {
    if (initKey.size() == 8) {
        
        for (int i = 8; i < 16; ++i) {
            key[i] = 0;  
        }
        
        for (int i = 0; i < 8; ++i) {
            key[i] = initKey[i];
        }
    } else if (initKey.size() == 16) {
       
        key = initKey;
    } else {
        throw std::invalid_argument("La taille de la clé doit être de 64 ou 128 bits.");
    }
}

uint8_t FEALNX::rot(uint8_t data, uint8_t shift){
    if(shift > 8){
        throw std::invalid_argument("Shift cannot be greater than 8 (byte length).");
    }

    return ((data << shift) | (data >> (8 - shift))) & 0xFF;
}


uint8_t FEALNX::S(uint8_t x, uint8_t y, uint8_t i){
    if(i > 1){
        throw std::invalid_argument("S-BOX increment is too high (must be 0 or 1)");
    }

    uint8_t result = (x + y + i) % 256;

    // Performs Rot2 as indicated in the specification paper
    return rot(result, 2);
}

uint32_t FEALNX::f(uint32_t alpha, uint16_t beta){

   // Alpha bit extraction
   uint8_t alpha0 = (alpha >> 24) & 0xFF; 
   uint8_t alpha1 = (alpha >> 16) & 0xFF; 
   uint8_t alpha2 = (alpha >> 8) & 0xFF; 
   uint8_t alpha3 = alpha & 0xFF; // alpha >> 0

   // Beta bit extraction
   uint8_t beta0 = (beta >> 8) & 0xFF; 
   uint8_t beta1 = beta & 0xFF; // beta >> 0

   uint8_t f1 = alpha1 ^ beta0;
   uint8_t f2 = alpha2 ^ beta1;

   f1 = f1 ^ alpha0;
   f2 = f2 ^ alpha3;
   f1 = S(f1,f2,1);
   f2 = S(f2,f1,0);
   uint8_t f0 = S(alpha0, f1, 0);
   uint8_t f3 = S(alpha3, f2, 1);

   uint32_t result = (f0 << 24) | (f1 << 16) | (f2 << 8) | f3;

   return result;

}

uint32_t FEALNX::fk(uint32_t alpha, uint32_t beta){

    
    // Alpha bit extraction
   uint8_t alpha0 = (alpha >> 24) & 0xFF; 
   uint8_t alpha1 = (alpha >> 16) & 0xFF; 
   uint8_t alpha2 = (alpha >> 8) & 0xFF; 
   uint8_t alpha3 = alpha & 0xFF;

   // Beta bit extraction
   uint8_t beta0 = (beta >> 24) & 0xFF; 
   uint8_t beta1 = (beta >> 16) & 0xFF; 
   uint8_t beta2 = (beta >> 8) & 0xFF; 
   uint8_t beta3 = beta & 0xFF;

   uint8_t fk1 = alpha1 ^ alpha0;
   uint8_t fk2 = alpha2 ^ alpha3;
   fk1 = S(fk1,fk2 ^ beta0, 1);
   fk2 = S(fk2, fk1 ^ beta1, 0);
   uint8_t fk0 = S(alpha0, fk1 ^ beta2,0);
   uint8_t fk3 = S(alpha3, fk2 ^ beta3,1);

   uint32_t result = (fk0 << 24) | (fk1 << 16) | (fk2 << 8) | fk3;

   return result;
 
 }


 void FEALNX::generateDerivedKeys(){

    
    std::array<uint8_t, 8> Kleft = { key[0], key[1], key[2], key[3], key[4], key[5], key[6], key[7] };
    std::array<uint8_t, 8> Kright = { key[8], key[9], key[10], key[11], key[12], key[13], key[14], key[15] };


    uint32_t Kright1 = (static_cast<uint32_t>(Kright[0]) << 24) | 
                    (static_cast<uint32_t>(Kright[1]) << 16) | 
                    (static_cast<uint32_t>(Kright[2]) << 8) | 
                    static_cast<uint32_t>(Kright[3]);

    uint32_t Kright2 = (static_cast<uint32_t>(Kright[4]) << 24) | 
                    (static_cast<uint32_t>(Kright[5]) << 16) | 
                    (static_cast<uint32_t>(Kright[6]) << 8) | 
                    static_cast<uint32_t>(Kright[7]);

    uint32_t A0 = (static_cast<uint32_t>(Kleft[0]) << 24) | 
                    (static_cast<uint32_t>(Kleft[1]) << 16) | 
                    (static_cast<uint32_t>(Kleft[2]) << 8) | 
                    static_cast<uint32_t>(Kleft[3]);

    uint32_t B0 = (static_cast<uint32_t>(Kleft[4]) << 24) | 
                    (static_cast<uint32_t>(Kleft[5]) << 16) | 
                    (static_cast<uint32_t>(Kleft[6]) << 8) | 
                    static_cast<uint32_t>(Kleft[7]);

    uint32_t Qr = 0;

    uint32_t D0 = 0;

    for(int r = 1; r<= (N/2)+4; r++){
        switch(r%3){

            case 0:
                Qr = Kright2;
                break;
            case 1:
                Qr = Kright1 ^ Kright2;
                break;
            case 2:
                Qr = Kright1;
                break;
            default:
                break;

        }

        
        uint32_t tmp1 = D0;
        uint32_t tmp2 = A0;



        D0 = A0;
        
        A0 = B0;
        
        B0 = fk(tmp2, B0^tmp1^Qr);
        

        std::array<uint8_t, 2> K21 = {static_cast<uint8_t>((B0 >> 24) & 0xFF) , static_cast<uint8_t>((B0 >> 16) & 0xFF)};
        std::array<uint8_t, 2> K22 = {static_cast<uint8_t>((B0 >> 8) & 0xFF) , static_cast<uint8_t>(B0 & 0xFF)};

        subkeys.push_back(K21);
        subkeys.push_back(K22);
        
    } 

 }





 uint64_t FEALNX::encrypt(uint64_t pt){

    if(subkeys.empty()){
        throw std::runtime_error("Subkeys array is empty. Please generate them");  
    }

    uint32_t L0 = (static_cast<uint32_t>(pt >> 32)) & 0xFFFFFFFF;
    uint32_t R0 = static_cast<uint32_t>(pt) & 0xFFFFFFFF;


    // Preprocessing
    L0 = L0 ^ concatIntegers(subkeys[N][0], subkeys[N][1], subkeys[N+1][0], subkeys[N+1][1]);
    R0 = R0 ^ concatIntegers(subkeys[N+2][0], subkeys[N+2][1], subkeys[N+3][0], subkeys[N+3][1]);
    
    R0 = R0 ^ L0;
    L0 = L0 ^ 0;
   
    

    //Iterative calculation

    
    
    for (int r = 1; r <= N; r++) {
        uint32_t tmp = R0;
        R0 = L0 ^ f(R0, concatIntegers(subkeys[r-1][0], subkeys[r-1][1])); // update Rr
        L0 = tmp; // update Lr
    }

    //Post-processing
    
    L0 = L0 ^ R0;
    R0 = R0 ^ 0;
    
    
    

    R0 = R0 ^ concatIntegers(subkeys[N+4][0], subkeys[N+4][1], subkeys[N+5][0], subkeys[N+5][1]);
    L0 = L0 ^ concatIntegers(subkeys[N+6][0], subkeys[N+6][1], subkeys[N+7][0], subkeys[N+7][1]);

    uint64_t result = concatIntegers(R0, L0);

    return result;

 }

 uint64_t FEALNX::decrypt(uint64_t ct){


    uint32_t R0 = (static_cast<uint32_t>(ct >> 32)) & 0xFFFFFFFF;
    uint32_t L0 = static_cast<uint32_t>(ct) & 0xFFFFFFFF;

    R0 = R0 ^ concatIntegers(subkeys[N+4][0], subkeys[N+4][1], subkeys[N+5][0], subkeys[N+5][1]);
    L0 = L0 ^ concatIntegers(subkeys[N+6][0], subkeys[N+6][1], subkeys[N+7][0], subkeys[N+7][1]);

 
    L0 = L0 ^ R0;
    R0 = R0 ^ 0;

    

    for(int r = N; r>= 1; r--){
        uint32_t tmp = R0;
        R0 = L0;
        L0 = tmp ^ f(L0, concatIntegers(subkeys[r-1][0], subkeys[r-1][1]));
    }

    uint32_t t = L0;

    L0 = L0 ^ 0;
    R0 = R0 ^ t;

    L0 = L0 ^ concatIntegers(subkeys[N][0], subkeys[N][1], subkeys[N+1][0], subkeys[N+1][1]);
    R0 = R0 ^ concatIntegers(subkeys[N+2][0], subkeys[N+2][1], subkeys[N+3][0], subkeys[N+3][1]);
    uint64_t result = concatIntegers(L0, R0);

    return result;

 }


 void FEALNX::displayDerivedKeys(){

    std::cout << "--------------------------------------------\n";
    std::cout << "|  Subkey  |  Byte 1  |  Byte 2  |\n";
    std::cout << "--------------------------------------------\n";

    int index = 1; 

    for (const auto& subkey : subkeys) {
        std::cout << "|    K" << std::setw(2) << index << "    |  "
                  << std::setw(4) << std::right << std::hex << static_cast<uint16_t>(subkey[0]) << "    |  "
                  << std::setw(4) << std::right << std::hex << static_cast<uint16_t>(subkey[1]) << "    |\n";
        ++index;
    }

    std::cout << "--------------------------------------------\n";


 }

 void FEALNX::displayInformation(){

    std::cout << "Number of Feistel rounds : " << N << '\n';
    std::cout << "Size of the key : " << key.size()*8 << " bits" << '\n';


 }




//https://link.springer.com/content/pdf/10.1007/3-540-38424-3_46.pdf
//http://info.isl.ntt.co.jp/crypt/eng/archive/dl/feal/call-3e.pdf