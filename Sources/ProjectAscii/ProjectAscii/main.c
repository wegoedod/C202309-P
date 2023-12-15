#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include "chAscii.h"

int main() {
    // JPEG �̹��� ���� ���

    struct ImageInfo info;
    strcpy_s(info.ascii_chars, sizeof(info.ascii_chars), " .:-=+*#%@");
    char changeAscii;
    int charIndex;
    char changeAsciiword;
    while (1)
    {
        printf("�ƽ�Ű ��Ʈ ������ ����� ���ڸ� �����Ͻðڽ��ϱ�?(Y/N)\n���� ����(%s) ", info.ascii_chars, sizeof(info.ascii_chars));
        scanf_s(" %c", &changeAscii,sizeof(changeAscii));
        if (changeAscii == 'Y' || changeAscii == 'y') {
            printf("������ ������ ��ȣ�� �Է��ϼ���.\n���� ����(%s) ", &info.ascii_chars, sizeof(info.ascii_chars));
            scanf_s("%d", &charIndex);
            printf("������ ���ڸ� �Է��ϼ���.\n���� ����(%s) ", &info.ascii_chars, sizeof(info.ascii_chars));
            scanf_s(" %c", &changeAsciiword,sizeof(changeAsciiword));
            info.ascii_chars[charIndex - 1] = changeAsciiword;
        }
        else if (changeAscii == 'N' || changeAscii == 'n') {
            break;
        }
        else
        {
            printf("Y �Ǵ� N�� �Է����ּ���.");
        }

        // ���� ���� ó��
        while (getchar() != '\n');
    }

    printf("jpeg ������ �̸��� �Է����ּ���. ");
    scanf_s("%s", &info.filename, sizeof(info.filename));

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

    //������ ����
    while (1)
    {
        printf("�� ���ϱ� �� �ȼ� �� �ϳ��� ���ڸ� �Ҵ��� ������ �Է����ּ���(���� �ϳ��� �Է�): ");
        scanf_s("%d", &info.size);
        if (info.size > cinfo.output_width || info.size > cinfo.output_height) {
            printf("������ ũ�⺸�� Ŭ �� �����ϴ�.\n");
        }
        else {
            break;
        }
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