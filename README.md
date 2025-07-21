# LSB_Steganography

 Steganographic methods to hide files in images written in C programming language. 

Plain simple text hidden within an image.

# Project Overview

An image and a secret text are provided as an input to the application. The application encodes the secret text (secret.txt) within the image file (beautiful.bmp) and an encoded image file is produced(stego_image.bmp). Once encoding is completed the encoded image file (stego_image.bmp) is then decoded to get the secret text (Output.txt).


# Encoding 

A byte of data is fetched from the **secret.txt** file, lets name this byte as **S_byte**. A whole byte of RGB data is also fetched from the image file **beautiful.bmp**, lets call it **I_byte**. Now lets consider one bit **s1** of this byte **S_byte**, this bit **s1** is encoded as the LSB(Least Significant Bit) for the byte **I_byte**. That basically means the LSB of the **I_byte** is **s1**.  **Thus, in order to encode a single bit of the text file a whole byte of RGB data is required.**

Following this pattern, each and every bytes of the text file **secret.txt** is encoded.

Although the whole idea of steganography is to hide the content of the secret file, but in order to produce the output file we need to encode the input file **extension** and **extension string's length**as well. Apart from these encodings, a magic string is also encoded in the encoded image. This **magic string** acts as a layer of authetication, which must be encoded at the very begining followed by **extension**, **extension string's length** and then the actual content of the secret message file **secret.txt**.

**ENCODING BYTE ORDER (in the encoded image file)**

a) 54 bytes of header (Metadata)  
b) Magic-string bytes  
c) Extension string's length bytes  
d) Extension string bytes  
e) Secret message file bytes  
f) Secret message file data size bytes  
g) Copy rest of input image file bytes to encoded image file bytes  

# Decoding

With the data of the encoded image as per the ENCODING BYTE ORDER, the following steps are executed

a) Skip first 54 bytes of metadata, from next byte onwards decode for the magic string. If it matches, decode futher otherwise terminate  
b) Decode the output file extension string length bytes  
c) Decode the output file extension string bytes  
e) Decode the secret file content bytes  
f) Decode the secret file size bytes  

With above decoded informations the Output.txt is created.


# Build process and 'make' commands  

REQUIREMENTS

**1) C compiler (gcc)**  
**2) Make**


EXECUTE THE BELOW COMMAND TO PRODUCE THE EXECUTABLE 
>**make**  

HOW TO ENCODE  
>**make ENCODE INFILE=<secret_filename_with_extension>**  

Note: Default value passed to INFILE variable is **secret.txt**    

HOW TO DECODE  
>**make DECODE OUTFILE=<output_filename_with_extension>**  

Note: Default value passed to OUTFILE variable is **Output.txt**   
User have to enter the key i.e the magic-string(refer to the **encode.h**)  
On successfull authentication, an Output.txt is created.  

HOW TO CLEAN  
>**make clean**  

This command removes all intermediate files along wtih user given files as per INFILE=<filename_with_extension> and OUTFILE=<output_filename_with_extension>  



 


