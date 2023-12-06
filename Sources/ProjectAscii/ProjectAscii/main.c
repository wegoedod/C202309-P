#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>

// �ƽ�Ű ���� ��Ʈ
struct ImageInfo{
    char *ascii_chars;
    char *filename;
}IMAGEINFO;


// �̹����� �ƽ�Ű ��Ʈ�� ��ȯ�ϴ� �Լ�
void changeToAscii(unsigned char* image, int width, int height, struct ImageInfo *info) {
    int gray;
    int index;

    for (int i = height; i > 0; i--) {
        for (int j = 0; j < width; j++) {
            index = (i * width + j) * 3;
            // RGB ���� ������� ��ȯ
            gray = (int)(0.2989 * image[index] + 0.5870 * image[index + 1] + 0.1140 * image[index + 2]);

            // ��⿡ ���� �ƽ�Ű ���� �����Ͽ� ���
            int char_index = (gray * (sizeof(info->ascii_chars) - 1)) / 255;
            printf("%c", info->ascii_chars[char_index]);
        }
        printf("\n");
    }
}



int main() {
    // JPEG �̹��� ���� ���

    struct ImageInfo info;
    info.ascii_chars = "@%#*+=-:. ";
    info.filename = "image.jpg";


    // JPEG ������ �б� ���� ����
    FILE* file = fopen(info.filename, "rb");
    if (!file) {
        printf("������ �� �� �����ϴ�.");
        return 1;
    }

    // JPEG ����ü ����
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // JPEG ���� ó���� ����
    cinfo.err = jpeg_std_error(&jerr);

    // JPEG ����ü �ʱ�ȭ
    jpeg_create_decompress(&cinfo);

    // �Է� ���� ����
    jpeg_stdio_src(&cinfo, file);

    // ������ ��� ���� �б�
    jpeg_read_header(&cinfo, TRUE);

    // �̹��� ���ڵ�
    jpeg_start_decompress(&cinfo);

    // �̹��� ������ ������ ���� ���� ����
    unsigned char* image = (unsigned char*)malloc(cinfo.output_width * cinfo.output_height * cinfo.output_components);

    // �̹��� �� ������ ��ĵ�ϸ� ������ �б�
    while (cinfo.output_scanline < cinfo.output_height) {
        unsigned char* row_pointer = &image[(cinfo.output_height - cinfo.output_scanline - 1) * cinfo.output_width * cinfo.output_components];
        jpeg_read_scanlines(&cinfo, &row_pointer, 1);
    }

    // �̹��� ���
    changeToAscii(image, cinfo.output_width, cinfo.output_height,&info);

    // �޸� ���� �� ���� �ݱ�
    free(image);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(file);

    return 0;
}
