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

//Definition for reading validating input arguments

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{   
    //validation for .bmp file
    if(strstr(argv[2],".bmp")!=NULL)
    {
	encInfo->src_image_fname=argv[2];
    }
    else
    { 
	return e_failure;
    }

    //validation for secret_file
    if(argv[3]!=NULL)
    {
	int flag=1,len,num=0;
	encInfo->secret_fname=argv[3];
	len=strlen(argv[3]);
	for(int i=len-1;i>=0;i-- )
	{
	    if(argv[3][i]=='.')
	    {
		flag=0;
		for(int j=i;argv[3][j]!='\0';j++)
		{                          
		    encInfo->extn_secret_file[num++]=argv[3][j];
		}
		encInfo->extn_secret_file[num]='\0';
		break;
	    }
	}
	if(flag==1)
	    return e_failure;

    }
    else 
    {
	return e_failure;
    }
    //validation for output image
    if(argv[4]==NULL)
    {
	encInfo->stego_image_fname="stego.bmp";
    }
    else
    {    
	if(strstr(argv[4],".bmp")!=NULL)
	{
	    encInfo->stego_image_fname=argv[4];
	}
	else
	{
	    int len=strlen(argv[4]);
		argv[4][len]='.';
		argv[4][len+1]='b';
		argv[4][len+2]='m';
		argv[4][len+3]='p';
		argv[4][len+4]='\0';
		encInfo->stego_image_fname=argv[4];
		printf("%s",encInfo->stego_image_fname);
	}
    }
    return e_success;
}

//Definition for to get file size

uint get_file_size(FILE *ptr)
{
    fseek(ptr,0,SEEK_END);
    //returing size of a file
    return ftell(ptr); 
}

//Definition for check Capacity 

Status check_capacity(EncodeInfo *encInfo)
{
    //checking size of a src image is enough to hold the data of the secret file data 
    if(get_image_size_for_bmp(encInfo->fptr_src_image) >= (54+(strlen(MAGIC_STRING))+4+strlen(encInfo->extn_secret_file)+4+get_file_size(encInfo->fptr_secret))*8)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

//definition for encode bmp header  

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char arr[54],ch1,ch2;

    fseek(fptr_src_image,0,SEEK_SET);
    //reading starting 54 bytes of data from src image 
    fread(arr,54,1,fptr_src_image);       
    fseek(fptr_dest_image,0,SEEK_SET);
     //Writing starting 54 bytes of data to stego image 
    fwrite(arr,54,1,fptr_dest_image);
   
    return e_success;
}

// Definition for encode Magic string

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    int size=strlen(MAGIC_STRING);
    char *data;
    data=magic_string;
    encode_data_to_image(data, size,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

//Definition for encode data to image 

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[8];
    for(int i=0;i<size;i++)
    {
       //reading 8 bytes data from src image every iteration
	fread(image_buffer,8,1,fptr_src_image);
	
	//calling byte to lsb function
	encode_byte_to_lsb(data[i], image_buffer);
	
	//writinig 8 bytes data to stego image
	fwrite(image_buffer,8,1,fptr_stego_image);
    }
}

//Definition for encode byte to lsb

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {  
    //clearing 0th bit
	image_buffer[i]=(image_buffer[i]&~(1));
    //geting a bit
	if(data&(1<<i))
	    image_buffer[i]|=1;  //replaceing a bit if 1
	else
	    image_buffer[i]|=0;  //replacing a bit if 0
    }
}

//Definition for encode secret file size

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char image_buffer[32];
     //reading 32 bytes data from src image 
    fread(image_buffer,32,1,encInfo->fptr_src_image);
    
    //calling size to lsb function
    encode_size_to_lsb(file_size,image_buffer);
    
     //reading 32 bytes data to stego image 
    fwrite(image_buffer,32,1,encInfo->fptr_stego_image);
    return e_success;
}

//Definition for encode size to lsb

Status encode_size_to_lsb(int data, char *image_buffer)
{
    for(int i=0;i<32;i++)
    {  
    //clearing 0th bit
	image_buffer[i]=(image_buffer[i]&~(1));
	//geting a bit
	if(data&(1<<i))
	    image_buffer[i]|=1;   //replaceing a bit if 1
	else
	    image_buffer[i]|=0;   //replaceing a bit if 0
    }
}

//Definition for encode secret file extension

Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo)
{
    int size=strlen(file_extn);
  
    encode_data_to_image(file_extn, size,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    /*
    for(int i=0;i<size;i++)
    {
    //reading 8 bytes data from src image every iteration
	fread(image_buffer,8,1,encInfo->fptr_src_image);
	
	//calling byte to lsb function
	encode_byte_to_lsb(file_extn[i], image_buffer);
	
    //writing 8 bytes data to stego image every iteration
	fwrite(image_buffer,8,1,encInfo->fptr_stego_image);
    }
    */
    return e_success;
}

//Definition for encode secret file data

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    int size=encInfo->size_secret_file;
    char image_buffer[8];
    char data[size];
    //making file pointer to point to the 0th position
    fseek(encInfo->fptr_secret,0,SEEK_SET);
    
    //reading data size times from the secret file
    fread(data,size,1,encInfo->fptr_secret);
    
    for(int i=0;i<size;i++)
    {   
     //reading 8 bytes data from src image every iteration
	fread(image_buffer,8,1,encInfo->fptr_src_image);
	
	//calling byte to lsb function
	encode_byte_to_lsb(data[i], image_buffer);
	
	 //reading 8 bytes data to stego image every iteration
	fwrite(image_buffer,8,1,encInfo->fptr_stego_image);
    }
    return e_success;

}

//Definition for copy remaining img data

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    //reading each byte data from src image untill it reaches EOF
    while(fread(&ch,1,1,fptr_src)!=0)
    {
	fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}

//Definition for do_encoding

Status do_encoding(EncodeInfo *encInfo)
{
    printf("Started Encoding\n");
    if(open_files(encInfo)==e_success)
    {
	printf("Open files successful\n");
    }
    else
    {
	printf("Open files failure\n");
	return e_failure;
    }
    if(check_capacity(encInfo)==e_success)
    {
	printf("Check capacity successful\n");
    }
    else
    {
	printf("check capacity failure\n");
	return e_failure;
    }
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
	printf("Copy header successfull\n");
    }
    else
    {
	printf("Copy header failure\n");
	return e_failure;
    }
    if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
    {
	printf("Encoding of Magic_String Successfull\n");
    }
    else
    {
	printf("Encoding of Magic_String is unSuccessfull\n");
	return e_failure;
    }
    if(encode_secret_file_size(strlen(encInfo->extn_secret_file), encInfo)==e_success)
    {
	printf("Encoding of secret file extension size successful\n"); 
    }
    else
    {
	printf("Encoding of secret file extension size unsuccessful\n");
    }
    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
    {
	printf("Encoding of secret file extension is successful\n");
    }
    else
    {
	printf("Encoding of secret file extension is unsuccessful\n");
    }
    if(encode_secret_file_size(encInfo->size_secret_file=get_file_size(encInfo->fptr_secret),encInfo)==e_success)
    {
	printf("Encoding of secret file size is successful\n");
    }
    else
    {
	printf("Encoding of secret file size is unsuccessful\n");
    }
    if(encode_secret_file_data(encInfo)==e_success)
    {
	printf("Encoding of secret file data is successful\n");
    }
    else
    {
	printf("Encoding of secret file data is unsuccessful\n");
    }
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
	printf("Copying remaining image data is successfull\n");
    }
    else
    {
	printf("Copying remaining image data is unsuccessfull\n");
    }
    return e_success;
}
