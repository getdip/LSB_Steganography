#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}


////////////////////////////        READ-VALIDATE CLA ARGUMENTS IMPLEMENTATION        ///////////////////////////////////////


Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)

{

    if  ( (argv[2] != NULL) && (strcmp (strstr(argv[2],"."),".bmp")  == 0)){
        encInfo ->src_image_fname = argv[2];
    }

    else{
        return e_failure;
    }

    if  ( (argv[3] != NULL)){
        encInfo ->secret_fname = argv[3];
    }

    else{
        return e_failure;
    }

    if  ( argv[4] != NULL){
        encInfo ->stego_image_fname = argv[4];
    }

    else{
        encInfo ->stego_image_fname = "stego_image.bmp";
    }

    return e_success;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// IMPLEMENTING do_ENCODING()  //////////////////////////////////////////////////////////////////

Status do_encoding(EncodeInfo *encInfo)

{

if(open_files(encInfo)== e_success)
{

    printf("Files opened successfully\n");

    if(check_capacity(encInfo) == e_success)
    
    {

        printf("Possible to encode\n");

        if(copy_bmp_header(encInfo->fptr_src_image , encInfo -> fptr_stego_image) == e_success)
        {

            printf("Copied Header successfully\n");

            if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
            
            {
                    printf("Magic string encoded successfully\n");

                    int hg=4;

                    if(encode_size(hg, encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
                    {

                            printf("Successfully encoded secret file extension size\n");

                            if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                            
                            {

                                printf("Successfully encoded secret file extension\n" );

                                if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
                                
                                {

                                    printf("Encoded secret file size successfully\n");

                                    if(encode_secret_file_data(encInfo) == e_success)
                                    {

                                        printf("Encoded secret data successfully\n");


                                        if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)

                                        {

                                            printf("Copied rest of the RGB dta from source to output file\n");

                                        }
                                        else
                                        {

                                            printf("Unable to display the output image\n");
                                        }
                                    }
                                    else    
                                    {

                                        printf("Failed to encode Secret data\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("Failes to encode secret file size\n");
                                    return e_failure;
                                }
                            }
                            else{

                                printf("failed to encode secret file extension\n");

                                return e_failure;
                            }
                    }
                    else
                    {
                        printf("Failes to encode secret file extension size\n");

                        return e_failure;
                    }
            }
            else
            {
                printf("Failed to encode the magic string\n");
            }
        }
        else
        {
            printf("Failed to copy the heade from source image\n");
            return e_failure;
        }
    }
    else
    {
        printf("Image file size is not adequate\n");
        return e_failure;
    }
}
else
{
    printf("Failed to open the files\n");
    return e_failure;
}

return e_success;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////      Check-capacity Implementation       /////////////////////////////////////////////////////

uint get_file_size(FILE *fptr)             ///////////  Returns Size of the SECRET FILE in BYTES 

{
    fseek(fptr, 0,SEEK_END);
    
    return ftell(fptr); 
}

Status check_capacity(EncodeInfo *encInfo)

{
    //Size of input .bmp file
    encInfo ->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    // size of the secret file
    encInfo ->size_secret_file = get_file_size(encInfo->fptr_secret);

    if( encInfo->image_capacity > ((54 + 2 + 4 + 4 + 4 + encInfo->size_secret_file)*8)){  //First 54 bytes are metadata of the image file
        return e_success;
    }
    else
    {
        e_failure;
    }
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////// copy bmp header implementation ////////////////////////////////////////////////////////////////
Status copy_bmp_header(FILE *fptr_src, FILE *fptr_dest)       ///// FIRST 54 BYTES OF A THE IMAGE FILE SHOULD BE COPIED AS IT IS

{
    char header[54];

    //ALWAYS RESET THE FILE POINTER OF A FILE

    fseek(fptr_src, 0, SEEK_SET);
    fread(header,sizeof(char),54,fptr_src);
    fwrite(header,sizeof(char),54,fptr_dest);
    
    return e_success;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////   Magic String Encoding ////////////////////////////////////////////

Status encode_magic_string(char *s, EncodeInfo *encInfo)
{
    encode_data_to_image(s, strlen(s),encInfo);     // calling encode_to_data function
    return e_success;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////// Data - TO -Image encoding implemetation   //////////////////////////////////////////////


Status encode_data_to_image(char *data, int s  ,EncodeInfo *encInfo)

{
    //calling encode_byte_to_lsb to encode the character
    //fetch 8bytes of RGB data for the s number of times i.e 8*s total RGB byte data

    for(int i= 0;i< s;i++)
    {
        fread(encInfo->image_data,sizeof(char),8,encInfo->fptr_src_image);
        encode_byte_to_lsb(data[i],encInfo->image_data);
        fwrite(encInfo->image_data,sizeof(char),8,encInfo->fptr_stego_image);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////          Encode_byte_to_lsb       /////////////////////////////////////////////////////////////

Status encode_byte_to_lsb(char data, char *image_buffer)

{
    unsigned int mask = 1 << 7;

for(int i=0;i<8;i++)
{

    image_buffer[i] = (image_buffer[i] & 0xFE) | ((data & mask) >> (7-i));

    mask = mask >> 1;
}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////// Secret file extension SIZE ENCODING /////////////////////////////////////////////////////////////

Status encode_size(int size, FILE *fptr_src, FILE *fptr_stego)

{
    char str[32];
    fread(str,sizeof(char),32,fptr_src);
    encode_size_to_lsb(str,size);
    fwrite(str,sizeof(char),32,fptr_stego);
    return e_success;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////        Encode_size_to_lsb Implemetation        ////////////////////////////////////////////////////

Status encode_size_to_lsb(char *ch, int s){
    unsigned int mask = 1 << 31;

    for(int i=0;i<32;i++){
        ch[i] = (ch[i] & 0xFE) | ((s & mask) >> (31-i));

        mask = mask >> 1;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////        ENcode secret File Extension     ////////////////////////////////////////////////////////

Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo){

    file_extn = ".txt";
    encode_data_to_image(file_extn,strlen(file_extn), encInfo);
    return e_success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////    Encoding secret file size       ////////////////////////////////////////////////////

Status encode_secret_file_size(long int  file_size, EncodeInfo *encInfo){
    
    char str[32];
    fread(str,sizeof(char),32,encInfo->fptr_src_image);
    encode_size_to_lsb(str,file_size);
    fwrite(str,sizeof(char),32,encInfo->fptr_stego_image);
    return e_success;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////        Encode secret data content to image    //////////////////////////////////////////////////////

Status encode_secret_file_data(EncodeInfo *encInfo){
    
    char ch;

    //bring the file pointer to the start of the secret.txt
    fseek(encInfo->fptr_secret,0,SEEK_SET);

    for(int i =0 ; i<encInfo ->size_secret_file;i++)
    {
        //read 8byte of RGB data from the beautiful.bmp file
        fread(encInfo->image_data,sizeof(char),8,encInfo->fptr_src_image);

        //read a character from the secret file
        fread(&ch,sizeof(char),1,encInfo->fptr_secret);
        encode_byte_to_lsb(ch,encInfo->image_data);
        fwrite(encInfo->image_data,sizeof(char),8,encInfo->fptr_stego_image);
    }
    return e_success;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////      Copying remaining data from src to output Image      //////////////////////////////////////       

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest){
    
    char ch;
    while(fread(&ch,1,1,fptr_src)>0)
    {
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}
