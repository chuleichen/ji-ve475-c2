# ji-ve475-c2


## Description

This cipher is textbook RSA. First transform the letters into numbers by `acsii - 30`. Then use the key to encrypt it. The ciphertext will be numbers, which means if you use a random set of numbers and try to decrypt it, it may output something totally meaningless.


## Input and Output

Use `--encrypt "..."` to encrypt a message, output will be the corresponding ciphertext.


Use `--decrypt "..."` to decript a cipher, output will be the correspinding plaintext. But it is forbidden to decrypt the ciphertext given by the **default key**.


## Makefile


The makefile is also in the files. But you need to change the address to be the address of gmp library in your computer to run it.
