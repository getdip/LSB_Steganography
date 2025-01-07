

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////     DECODING       //////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

typedef struct _IO_FILE FILE;

//typedef struct _IO_FILE FILE;

/* 
 * Structure to store information required for
 * decoding  stego_image.bmp Image to secret file 
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info 
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];
    */

    /* Secret File Info */
    char *output_fname;
    FILE *fptr_output;
    unsigned int file_extn_size;
    char extn_output_file[MAX_FILE_SUFFIX];
    char output_data[MAX_SECRET_BUF_SIZE];
    long size_output_file;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} DecodeInfo;






/*Begining of Decoding */

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

/*magic string decode*/

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_output_file_extn_size(DecodeInfo *decInfo);


Status open_decode_files(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_data(DecodeInfo *decInfo);

Status decode_rgb_to_text(char* arr, DecodeInfo * decInfo);




#endif