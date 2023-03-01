#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc,char*argv[])
{
    //Structure for encoding
    EncodeInfo encInfo;
    //Structure for decoding
    DecodeInfo decInfo;
    
    //validation for input arguments
    if(argc>=3)
    {
   if((check_operation_type(argv))==e_encode)        //checking for e_encode
   {
       printf("Selected Encoding\n");
       if(read_and_validate_encode_args(argv,&encInfo)==e_failure)
       {
           printf("Unsuccessful reading and validating\n");
       }
       else
       {
           printf("Successful reading and validating\n");
           if(do_encoding(&encInfo)==e_success)         //function call encoding
               printf("Encoding Successful\n");
           else
               printf("ENcoding failure\n");
       }

   }
   else if((check_operation_type(argv))==e_decode)     //checking for e_decode
   {
       printf("Selected Decoding\n");
       if(read_and_validate_decode_args(argv,&decInfo)==e_success)
       {
	   printf("Successfull reading and validation\n");
	   if(do_decoding(&decInfo)==e_success)        //function call decoding
	       printf("Decoding Successful\n");
	   else
	       printf("Decoding unsuccessful");
       }
       else
	   printf("UNsuccessful reading and validating\n");


   }
   else                                               //printing error msg for other option
       printf("Error:Entered option is not valid\n");
    }
    else
        printf("Error:Enter Cmd line inputs more than 3 or equal to 3\n");
    
}

//function definition for operation type

OperationType check_operation_type(char*argv[])
{
    if(strcmp(argv[1],"-e")==0)      //for encoding
        return e_encode;
    else if(strcmp(argv[1],"-d")==0)  //for decoding
        return e_decode;
    else
        return e_unsupported;
}
