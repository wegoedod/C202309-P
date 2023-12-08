#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>

// �ƽ�Ű ���� ��Ʈ
struct ImageInfo{
    char ascii_chars[11];
    char filename[256];
};


// �̹����� �ƽ�Ű ��Ʈ�� ��ȯ�ϴ� �Լ�
void changeToAscii(unsigned char* image, int width, int height, struct ImageInfo *info) {
    int gray;
    int index;

    for (int i = height; i > 0; i-=10) {
        for (int j = 0; j < width; j+=10) {
            int sum = 0;
            //��� �� ����
            for (int k = 0; k < 10; k++) {
                for (int l = 0; l < 10; l++) {
                    int row = i + k;
                    int col = j + l;
                    if (row < height && col < width) {
                        index = (row * width + col) * 3;
                                // RGB ���� ������� ��ȯ
                        sum += (int)(0.2989 * image[index] + 0.5870 * image[index + 1] + 0.1140 * image[index + 2]);
                    }
                }
            }

            //��� ��� ���
            int average = sum / 100;
            // ��⿡ ���� �ƽ�Ű ���� �����Ͽ� ���
            int char_index = (average * (sizeof(info->ascii_chars) - 1)) / 255;
            printf("%c", info->ascii_chars[char_index]);

        }
        printf("\n");
    }
}



int main() {
    // JPEG �̹��� ���� ���

    struct ImageInfo info;
    strcpy_s(info.ascii_chars, sizeof(info.ascii_chars), "@%#*+=-:. ");
    strcpy_s(info.filename, sizeof(info.filename), "image.jpg");
    char changeAscii;
    int charIndex;
    char changeAsciiword;
    while (1)
    {
        printf("�ƽ�Ű ��Ʈ ������ ����� ���ڸ� �����Ͻðڽ��ϱ�?(Y/N)\n���� ����(%s) ", info.ascii_chars, sizeof(info.ascii_chars));
        scanf_s(" %c", &changeAscii,sizeof(changeAscii));
        if (changeAscii == 'Y' || changeAscii == 'y') {
            printf("������ ������ ��ȣ�� �Է��ϼ���.\n���� ����(%s) ", info.ascii_chars, sizeof(info.ascii_chars));
            scanf_s("%d", &charIndex);
            printf("������ ���ڸ� �Է��ϼ���.\n���� ����(%s) ", info.ascii_chars, sizeof(info.ascii_chars));
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

    printf("jpeg ������ ��θ� �Է����ּ���. ");
    scanf_s("%s", info.filename, sizeof(info.filename));

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
