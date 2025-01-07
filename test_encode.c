#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "decode.h"
#include <unistd.h>







int main(int argc, char* argv[])
{
   
   //check operation type -e or -d

   EncodeInfo infoptr;

   if   (check_operation_type(argv) == e_encode)                        /// if ENCODING is selected 
   
   {

        printf("Selected Encoding\n");                                              

        if(read_and_validate_encode_args(argv,&infoptr) == e_success )       /// validate the CLA vector i.e the .bmp files and .txt files
        
    {         

        

        printf("Read and validate argument is successfull\n");

            if(do_encoding(&infoptr) == e_success)
        {
        
            printf("Completed Encoding\n");

            
            FILE *enc_chck;
            enc_chck=fopen("enc_completion_check.txt","w+");
            fputs(infoptr.stego_image_fname, enc_chck);

            


        }

    }

        else if (read_and_validate_encode_args(argv,&infoptr) == e_failure)
        {

            printf("Failed to validate the arguments\n");
        }

   }


   

  else if  (check_operation_type(argv) == e_decode)
  
  {

         printf("Selected Decoding\n");
        //Check if stego_image.bmp is present. That means the encoding is completed.

        if(access("enc_completion_check.txt",F_OK) == 0){

            printf("Encoding has completed\n");


            printf("Now decoding will begin\n");

            //void loadingBar();



             DecodeInfo decInfo;

             

       
            if(read_and_validate_decode_args(argv,&decInfo) == e_success)

            {

                if(do_decoding(&decInfo) == e_success)

                {

                    printf("Decoding finished\n");
                }

           

                
            }

            else if(read_and_validate_decode_args(argv,&decInfo) == e_failure)
            
            {


                printf("Wrong input image file for the decoding process\n");

            }

        

        
        
      


        

                }

            
            else 

            {

                printf("Encoding is not completed\nPlease complete Encoding first and then proceed with decoding\n");
            }



  }

    return 0;
}

////////////////////////////////////////     OPERATION TYPE IMPLEMENTATION        ///////////////////////////////////////

OperationType check_operation_type(char* argv[]){

    if(strcmp(argv[1],"-e") == 0){

        return e_encode;
    }

    else if (strcmp(argv[1],"-d" )==0){

        return e_decode;
    }


    else{

        return e_unsupported;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




























