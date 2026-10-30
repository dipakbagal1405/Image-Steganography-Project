#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        return e_failure;
    }
    return e_success;
}

Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo)
{
    if (argc < 3)
    {
        return e_failure;
    }
    if (strstr(argv[2], ".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }
    if (argc >= 4)
    {
        decInfo->secret_fname = argv[3];
    }
    else
    {
        decInfo->secret_fname = "decoded";
    }
    return e_success;
}

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    char ch = 0;
    for (int i = 0; i < 8; i++)
    {
        ch |= ((image_buffer[i] & 1) << i);
    }
    *data = ch;
    return e_success;
}

Status decode_size_from_lsb(long *size, char *image_buffer)
{
    unsigned int val = 0;
    for (int i = 0; i < 32; i++)
    {
        val |= ((unsigned int)(image_buffer[i] & 1) << i);
    }
    *size = (long)val;
    return e_success;
}

Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    char image_buffer[8];
    for (int i = 0; i < size; i++)
    {
        if (fread(image_buffer, 1, 8, decInfo->fptr_stego_image) < 8)
        {
            return e_failure;
        }
        decode_byte_from_lsb(&data[i], image_buffer);
    }
    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_str[3] = {0};
    if (decode_data_from_image(magic_str, strlen(MAGIC_STRING), decInfo) != e_success)
    {
        return e_failure;
    }
    else
    {
        return (strcmp(magic_str, MAGIC_STRING) == 0) ? e_success : e_failure;
    }
}

Status decode_secret_file_extn_size(int *size, DecodeInfo *decInfo)
{
    char buffer[32];
    if (fread(buffer, 1, 32, decInfo->fptr_stego_image) < 32)
    {
        return e_failure;
    }
    long temp_size = 0;
    decode_size_from_lsb(&temp_size, buffer);
    *size = (int)temp_size;
    return e_success;
}

Status decode_secret_file_extn(int extn_size, DecodeInfo *decInfo)
{
    if (decode_data_from_image(decInfo->extn_secret_file, extn_size, decInfo) != e_success)
    {
        return e_failure;
    }
    decInfo->extn_secret_file[extn_size] = '\0';
    return e_success;
}

Status decode_secret_file_size(long *secret_file_size, DecodeInfo *decInfo)
{
    char buffer[32];
    if (fread(buffer, 1, 32, decInfo->fptr_stego_image) < 32)
    {
        return e_failure;
    }
    decode_size_from_lsb(secret_file_size, buffer);
    return e_success;
}

Status decode_secret_file_data(long secret_file_size, DecodeInfo *decInfo)
{
    char single_byte, image_buffer[8];
    for (long i = 0; i < secret_file_size; i++)
    {
        if (fread(image_buffer, 1, 8, decInfo->fptr_stego_image) < 8)
        {
            return e_failure;
        }
        decode_byte_from_lsb(&single_byte, image_buffer);
        if (fwrite(&single_byte, 1, 1, decInfo->fptr_secret) < 1)
        {
            return e_failure;
        }
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) != e_success)
    {
        return e_failure;
    }
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    if (decode_magic_string(decInfo) == e_success)
    {
        int extn_size = 0;
        if (decode_secret_file_extn_size(&extn_size, decInfo) == e_success)
        {
            printf("Info: Decoded secret file extn size successfully: %d\n", extn_size);
            if (decode_secret_file_extn(extn_size, decInfo) == e_success)
            {
                printf("Info: Decoded secret file extn successfully: %s\n", decInfo->extn_secret_file);
                char final_output_fname[100];
                strcpy(final_output_fname, decInfo->secret_fname);
                if (strstr(decInfo->secret_fname, ".") == NULL)
                {
                    strcat(final_output_fname, decInfo->extn_secret_file);
                }
                decInfo->fptr_secret = fopen(final_output_fname, "w");
                if (decInfo->fptr_secret == NULL)
                {
                    return e_failure;
                }
                long file_size = 0;
                if (decode_secret_file_size(&file_size, decInfo) == e_success)
                {
                    printf("Info: Decoded secret file size successfully: %ld bytes\n", file_size);
                    if (decode_secret_file_data(file_size, decInfo) == e_success)
                    {
                        //printf("Info: Decoding completed successfully.\n");
                        fclose(decInfo->fptr_secret);
                        fclose(decInfo->fptr_stego_image);
                        return e_success;
                    }
                }
            }
        }
    }
    return e_failure;
}
