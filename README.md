# LSB_Steganography

 Stegonographical methods to hide files in images written in C programming language. 

Plain simple text hidden within an image.

# Project Overview

An image and a secret text are provided as an input to the application. The application encodes the secret text (secret.txt) within the image file (beautiful.bmp) and an encoded image file is produced(stego_image.bmp). Once encoding is completed the encoded image file (stego_image.bmp) is then decoded to get the secret text (Output.txt).


# Encoding 

A byte of data is fetched from the **secret.txt** file, lets name this byte as **S_byte**. Also, a whole byte of RGB data is fetched from the image file **beautiful.bmp**, lets call it **I_byte**. Now lets consider one bit **s1** of this byte **S_byte**, this bit **s1** is encoded as the LSB(Least Significant Bit) for the byte **I_byte**. That basically means the LSB of the **I_byte** is **s1**.  **Thus, in order to encode a single bit of the text file a whole byte of RGB data is required.**

Following this pattern, each and every bytes of the text file **secret.txt** is encoded.




