#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

////////////////////////////////////// READ and Validate Command Line ARGs  /////////////////////////////////////////////////////////

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo){

    FILE* read_enc_check;
    char s[50];
    read_enc_check= fopen("enc_completion_check.txt","r");
    fgets(s,50,read_enc_check);
    //printf("File content is %s ",s);

    if  ( (argv[2] != NULL) && !(strcmp (argv[2],s))){
        decInfo->stego_image_fname = argv[2];
    }

    else{
        return e_failure;
    }

    if  ( (argv[3] != NULL)){
        decInfo ->output_fname = argv[3];
    }

    else if ( (argv[3] == NULL)){

        decInfo ->output_fname = "Output.txt";

    }
    return e_success;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status do_decoding(DecodeInfo *decInfo)

{
    //printf("LETS DECODE NOW\n");

    if(open_decode_files(decInfo) == e_success)

    {
        printf("Files opened successfully\n");

        if(decode_magic_string(decInfo) == e_success)
        {

            printf("Magic string is now decoded\n");

            if(decode_output_file_extn_size(decInfo) == e_success)
            
            {
                printf("Decoded file extension size \n");

                if(decode_secret_file_extn(decInfo) == e_success)

                {
                    printf("Decoded secret file extension\n");
                    
                    if(decode_secret_file_size(decInfo) == e_success)
                    {
                        printf("Output file size decoded\n");

                        if(decode_secret_data(decInfo) == e_success)
                        {
                            printf("DATA HAS BEEN Decoded\n");
                        }
                        else
                        {
                            printf("SO CLOSE,  YET SO FAR");
                        }
                    }
                    else
                    {
                        printf("Unable to decode secret file size\n");
                    }
                }
                else
                {
                    printf("Unable to decode secret file extension\n");
                }
            }
            else
            {
                printf("Unable to decode file extension size\n");
            }
        }
        else
        {
            printf("Unable to decode magic string\n");
        }
    }
    else
    {
         printf("Failed to open files\n");
    }
    return e_success;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 Status open_decode_files(DecodeInfo* decInfo)

{

    FILE* read_enc_check;
char st[50];
read_enc_check= fopen("enc_completion_check.txt","r");
fgets(st,50,read_enc_check);
//printf("File content is %s ",st);

    // Stego Image file
    decInfo->fptr_stego_image=fopen(st,"r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }

    //Opening Output.txt file

    decInfo->fptr_output=fopen(decInfo->output_fname,"w");

    // Do Error handling
    if (decInfo->fptr_output == NULL)
    {
        printf("Unable to open Output.txt\n");
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_magic_string(DecodeInfo *decInfo)

{
    char ch;
    char magic_string[3]={0x00,0x00,0x00};
    magic_string[2]='\0';
    char user_key[10];

    /*User input for the key*/
    printf("Enter the key: ");
    fgets(user_key,10,stdin);
    user_key[strlen(user_key)-1]='\0';
    /*****************************/

    fseek(decInfo->fptr_stego_image,54,SEEK_SET);

    for(int i=0;i<8;i++){
        fread(&ch,sizeof(char),1,decInfo->fptr_stego_image);
        magic_string[0] = magic_string[0] | (ch & 0x01)<<(7-i);
    }

     for(int i=0;i<8;i++){
        fread(&ch,sizeof(char),1,decInfo->fptr_stego_image);
        magic_string[1] = magic_string[1] | (ch & 0x01)<<(7-i);
    }

    if (strcmp(magic_string,user_key) == 0){
        return e_success;
    }
    else{
        return e_failure;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_output_file_extn_size(DecodeInfo *decInfo)

{

    unsigned int temp = 0x00;    
    char ch;

    for(int i=0;i<32;i++){

        fread(&ch,sizeof(char),1,decInfo->fptr_stego_image);
        temp = temp | (ch & 0x01)<<(31-i);
    }
    decInfo->file_extn_size = temp;
    
    if(temp == strlen(".txt")){
        return e_success; 
    }
    return e_failure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_secret_file_extn(DecodeInfo *decInfo)

{
    char extn[decInfo->file_extn_size];

    for(int i=0;i<decInfo->file_extn_size;i++){
        extn[i] = 0x00;
    }

    extn[decInfo->file_extn_size] ='\0';
    char ch;    
    for(int j=0;j<decInfo->file_extn_size;j++)

    {
        for (int i=0;i<8;i++ )
        {
        fread(&ch,sizeof(char),1,decInfo->fptr_stego_image);
        extn[j] = extn[j] | (ch & 0x01)<<(7-i);
        }
    }



    if(strcmp(extn,".txt")==0){
        return e_success;
    }

    return e_failure;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_secret_file_size(DecodeInfo *decInfo)

{

unsigned long x=0x00;
char ch;

for(int i=0;i<32;i++){

        fread(&ch,sizeof(char),1,decInfo->fptr_stego_image);
        x = x | (ch & 0x01)<<(31-i);
    }

    decInfo->size_output_file=x;
    return e_success;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status decode_secret_data(DecodeInfo *decInfo)

{

    char arr[decInfo->size_output_file];

    for(int i=0;i<decInfo->size_output_file;i++)

    {

        char ch[8];
        char x =0x00;

        fread(ch,sizeof(char),8,decInfo->fptr_stego_image);        
        for(int j=0;j<8;j++)
        {   
            x = x | ((ch[j] & 0x01) <<(7-j));
        }  
        arr[i]=x;     

    }


    fwrite(arr,sizeof(char),decInfo->size_output_file-1,decInfo->fptr_output);
    return e_success;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
