# FEALParadise 

🎤 *We KeeP spEnDiNg MoSt OuR LiVeS*  
🎶 *LiViNg in A* **FEALParadise** 🎵


## Description

FEAL (Fast Data Encipherment Algorithm) is a symmetric-key block cipher proposed in 1987 by Akihiro Shimizu and Shoji Miyaguchi. Initially designed as an alternative to DES, FEAL operates on 64-bit blocks and supports key sizes of 64 or 128 bits. The algorithm utilizes a Feistel network and has undergone multiple revisions, including FEAL-4, FEAL-8, and FEAL-NX. Although it has been broken by cryptanalysis, FEAL played a significant role in the development of differential and linear cryptanalysis techniques.

This repository contains an implementation of both FEALNX (128-bit key length) and FEALN (64-bit key length). It also includes an implementation of attacks against these cryptosystems. For more details, see the [Implemented Attacks](#implemented-attacks) chapter.

⚠️ **Warning:** This cryptosystem is only for educational purposes. Do not use it in any production environment.

## Features

- Key Generation
- Encryption
- Decryption
- Cryptanalysis (Soon)
- Unitary tests 

## Implemented Attacks


## Installation


## Usage


## Structureof the project

```
feal-implementation/
|__ include/
|   |__ feal.h 
│── src/
│   ├── feal.cpp                 
│   ├── main.cpp        
│
│── tests/
│   ├── test_feal.cpp   
│
│── CMakeLists.txt      
│── README.md           
```

## Tests

## Contributions

## Licence
