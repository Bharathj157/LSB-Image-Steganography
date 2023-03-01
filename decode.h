#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define DMAX_FILE_SUFFIX 15

typedef struct _DecodeInfo
{
  /* Stego Image Info */
    char* stego_src_name;
    FILE* fptr_stego_image;

    /*output file*/
    char output_fname[DMAX_FILE_SUFFIX];
    FILE* fptr_output;
    long dsize_secret_file;
    long dsize_secret_file_data;
    char *extn_output_file;
    
}DecodeInfo;

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the Decoding */
Status do_decoding(DecodeInfo *decInfo);

/*File open */
Status open_files_decode(DecodeInfo *decInfo);

/* Extract Magic String */
Status decode_magic_string(char * data,DecodeInfo *decInfo);

/* Decode byte from lsb */
char decode_byte_from_lsb(char* buffer);

/*Decode size fro lsb */
int decode_size_from_lsb(char* buffer);

/*Decode extension size */
Status decode_extension_size(DecodeInfo *decInfo);

/*Decode secret file size*/
 Status decode_secret_file_size(DecodeInfo *decInfo);

/*Decode secret file extension*/
Status decode_secret_extension(DecodeInfo *decInfo);

/*Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif
