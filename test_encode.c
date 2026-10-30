#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include <string.h>
#include "types.h"

//NAME   : Dipak
//DATE   : 30/7/2026
//PROJECT : LSB IMAGE STEGANOGRAPHY
//DESCRIPTION : TAKING ONE BMP IMG FILE ENCODING IT WITH SECRET DATA  AND DECODING THE ENCODED IMAGE TO GET SECRET DATA .

//Function prototype
OperationType check_operation_type(int argc, char *argv[]);
int main(int argc,char *argv[])
{
    EncodeInfo E1;
    DecodeInfo D1;
    if(argc<2)
    {
        printf("INFO  :  Please pass valid arguments.\n");
        printf("INFO  :  Encoding - minimum 4 arguments.\n");
        printf("INFO  :  Decoding - minimum 3 arguments.\n"); 
        return 1;
    }
    int res = check_operation_type(argc,argv);
    if(res == e_encode)
    {
        if(argc < 4)   
        {
            printf("ERROR: Encoding requires at least 4 arguments.\n");
            printf("\n");
            printf("INFO  :  Please enter like this %s -e beautiful.bmp secret.txt [stegno.bmp]\n", argv[0]);
            return 1;
        }
         printf("INFO  :  Selected Encoding. Encoding Started\n");
         printf("\n");
        if(read_and_validate_encode_args(argv, &E1)==e_success)
        {
            printf("Info:Read and validate encode args is success\n");
            if(open_files(&E1) == e_success)
            {
                printf("......................................\n");
                printf("Info:Files are opened successfuly\n");
                printf(".......................................\n");
                if(do_encoding(&E1) == e_success)
                {
                    printf("Info: Encoding is success\n");
                }
                else
                {
                    printf("Failed to encode\n");
                    return 0;
                }
            }
            else
            {
                printf("open files is failure\n");
                return 0;
            }
        }
        else
        {
            printf("Read and validate encode args is failure\n");
            return 0;
        }
    }
    else if(res == e_decode)
    {
          if(argc < 3)   
        {
            printf("ERROR: decoding requires at least 3 arguments.\n");
            printf("\n");
            printf("INFO  :  Please enter like this %s -d stegno.bmp [output.txt]\n", argv[0]);
            return 1;
        }
        printf("INFO  :  Selected Decoding. Decoding Started\n");
        printf("\n");
         if(read_and_validate_decode_args(argc,argv,&D1)==e_success)
        {
            printf("INFO  :  Read and validate is  successully completed for Decoding\n");
            printf("\n");
            if(open_decode_files(&D1) == e_success)
            {
                printf("......................................\n");
                printf("Info:Files are opened successfuly\n");
                printf(".......................................\n"); 
                if(do_decoding (&D1)==e_success)
                {
                  printf("INFO: Decoding Completed successfully\n");
                }
                else
                {
                  printf("decoding is not done correctly\n");
                }
            }
        }
    }
    else
    {
        printf("Invalid option\n");
        printf("For Encoding: ./a.out -e beautiful.bmp secret.txt [stegno.bmp]\n");
        printf("For Decoding: ./a.out -d be stegno.bmp default.txt\n");
    }
    return 0;
}
OperationType check_operation_type(int argc,char *argv[])
{
    //step1:check argv is -e or  not ,  if yes goto step2,if not goto step 3 
    //step 2:return e_encode
    //step 3: check argv is -d or not if yes goto step4,if not goto step 5
    //step 4 : return e_decode
    //step 5: return e_unsuppoerted
    
  if(strcmp(argv[1],"-e") == 0)
     return e_encode;
  else if(strcmp(argv[1],"-d") == 0)
    return e_decode;
  else
    return e_unsupported;
}