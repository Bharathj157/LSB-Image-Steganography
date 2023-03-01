#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files_decode(DecodeInfo *decInfo)
{
    
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_src_name, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_src_name);

        return e_failure;
    }
    // No failure return e_success
    return e_success;
}

//Definition for validate input arguments  

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
  //validation for .bmp file 
    if(strstr(argv[2],".bmp")!=NULL)
    {
        decInfo->stego_src_name=argv[2];
    }
    else
        return e_failure;

  //validation for output file 
    if(argv[3]==NULL)
    {
        decInfo->output_fname[0]='o';
        decInfo->output_fname[1]='u';
        decInfo->output_fname[2]='t';
        decInfo->output_fname[3]='p';
        decInfo->output_fname[4]='u';
        decInfo->output_fname[5]='t';
        decInfo->output_fname[6]='\0';
    }
    else
    {   
          for(int i=0;i<strlen(argv[3]);i++)	
          decInfo->output_fname[i]=argv[3][i];
    }

    return e_success;
}

//Definitiion for decoding Magic string

Status decode_magic_string(char *data,DecodeInfo *decInfo)
{
    char arr[strlen(data)];
    char buffer[8];
    int i;
    
    //using fseek function to skip 54 bytes (header)
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);

    for(i=0;i<strlen(data);i++)
    { 
    //reading 8 bytes of data from stego image
	fread(buffer,8,1,decInfo->fptr_stego_image);
	
   //storing each charcter in a array in each iteration 	
	arr[i]=decode_byte_from_lsb(buffer);
    }
    arr[i]='\0';
    
    //cmp magic string value with decoded magic string 
    if(strcmp(arr,data)==0)
	return e_success;
    else
	return e_failure;
}

//Definition Decode byte from lsb

char decode_byte_from_lsb(char* buffer)
{
    char ch=0;
    for(int i=0;i<8;i++)
    { 
        //logic to get a bit of a encoded data of 8bit data
	ch=(buffer[i]&1)<<i|ch; 
    }
    return ch;
}

//Definition for decode Extension Size

Status decode_extension_size(DecodeInfo *decInfo)
{
   char buffer[32];
   
   //reading 32 bytes of data from stego image
   fread(buffer,32,1,decInfo->fptr_stego_image);
   
   //storing the size after decoding is done
   int size=decode_size_from_lsb(buffer);

   //storing size in a structure
   decInfo->dsize_secret_file=size;

   return e_success;
}

//Definition for Decoding size from lsb 

int decode_size_from_lsb(char* buffer)
{
    int size=0;
    for(int i=0;i<32;i++)
    { 
     //logic to get a bit of a encoded data of 32bit data
	size=(buffer[i]&1)<<i|size;
    }
    return size;
}

//Definition for Decode secret file extension

Status decode_secret_extension(DecodeInfo *decInfo)
{
    int i;
    char arr[decInfo->dsize_secret_file+1];
    char buffer[8];
  
    for(i=0;i<decInfo->dsize_secret_file;i++)
    {
      //reading 8 bytes of data from stego image
        fread(buffer,8,1,decInfo->fptr_stego_image);
        
      //storing each charcter in an array after decoding
 	arr[i]=decode_byte_from_lsb(buffer);
 	
    }
    arr[i]='\0';
  
  //storing extension in a structure 
    decInfo->extn_output_file=strcat(decInfo->output_fname,arr);

    return e_success; 
}

//Definition for Decode secret file size

Status decode_secret_file_size(DecodeInfo *decInfo)
{
   char buffer[32];
   //reading 8 bytes of data from stego image
   fread(buffer,32,1,decInfo->fptr_stego_image);
   
   int size=decode_size_from_lsb(buffer);
   
   //storing decoded vale in a struct
   decInfo->dsize_secret_file_data=size;
 
   return e_success;
}

//Definition for decode secret file data

Status decode_secret_file_data(DecodeInfo *decInfo)
{
   //opening output file in a write mode
    decInfo->fptr_output=fopen(decInfo->output_fname, "w");
    char arr[decInfo->dsize_secret_file_data];
    char buffer[8];
    int i;
    for(i=0;i<decInfo->dsize_secret_file_data;i++)
    {
       //reading 8 bytes from stego image
	fread(buffer,8,1,decInfo->fptr_stego_image);
	
	arr[i]=decode_byte_from_lsb(buffer);
	
	//after decoding writing data into a output file
	fwrite(&arr[i],1,1,decInfo->fptr_output);
    }
    return e_success;
}

//Definition for decoding

Status do_decoding(DecodeInfo *decInfo)
{
    printf("Started Decoding\n");
    if(open_files_decode(decInfo)==e_success)
    {
        printf("Open files successful\n");
    }
    else
    {
        printf("Open files failure\n");
        return e_failure;
    }

    if(decode_magic_string(MAGIC_STRING,decInfo)==e_success)
    {
        printf("Decoding Magic String Successful\n");
    }
    else
    {
	printf("Decoding Magic String Unseccessful\n");
	return e_failure;
    }

    if(decode_extension_size(decInfo)==e_success)
    {
	printf("Decoding of extension size in Successful\n");
    }
    else
    {
	printf("Decoding of extension size is unSuccessful\n");
    }

    if(decode_secret_extension(decInfo)==e_success)
    {
	printf("Decoding of secret extension is Successful\n");
    }
    else
    {
	printf("Decoding of secret extension is unsuccessful\n");
    
    }

    if(decode_secret_file_size(decInfo)==e_success)
    {
	printf("Decoding of secret file size is successful\n");
    }
    else
    {
       printf("Decoding of secret file size is unsuccessful\n");	
    }

    if(decode_secret_file_data(decInfo)==e_success)
    {
	printf("Decoding of secret file data  is successful\n");
    }
    else
    {
       printf("Decoding of secret file data is unsuccessful\n");	
    }

    return e_success;
}
