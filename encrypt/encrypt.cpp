#include <stdio.h>
#include <stdlib.h> // Thêm header này để dùng malloc và free

int main() {
    FILE* inputFile = NULL;
    FILE* outputFile = NULL;
    errno_t err;

    // Mở file shellcode gốc
    err = fopen_s(&inputFile, "shellcode.bin", "rb");
    if (err != 0 || inputFile == NULL) {
        printf("Failed to open shellcode.bin\n");
        return 1;
    }

    // Mở file để lưu shellcode đã mã hóa
    err = fopen_s(&outputFile, "shellcode_encrypted.bin", "wb");
    if (err != 0 || outputFile == NULL) {
        fclose(inputFile);
        printf("Failed to open shellcode_encrypted.bin\n");
        return 1;
    }

    // Lấy kích thước file
    fseek(inputFile, 0, SEEK_END);
    size_t shellcodeSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    // Đọc shellcode
    unsigned char* shellcode = (unsigned char*)malloc(shellcodeSize);
    if (!shellcode) {
        fclose(inputFile);
        fclose(outputFile);
        printf("Failed to allocate memory\n");
        return 1;
    }
    fread(shellcode, 1, shellcodeSize, inputFile);
    fclose(inputFile);

    // Mã hóa shellcode bằng XOR với key
    unsigned char key = 0x5A; // Key mã hóa
    for (size_t i = 0; i < shellcodeSize; i++) {
        shellcode[i] ^= key;
    }

    // Ghi shellcode đã mã hóa vào file mới
    fwrite(shellcode, 1, shellcodeSize, outputFile);
    fclose(outputFile);
    free(shellcode);

    printf("Shellcode encrypted and saved to shellcode_encrypted.bin\n");
    return 0;
}