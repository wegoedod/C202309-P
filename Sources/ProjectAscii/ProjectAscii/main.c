#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>

// 아스키 문자 세트
struct ImageInfo{
    char *ascii_chars;
    char *filename;
}IMAGEINFO;


// 이미지를 아스키 아트로 변환하는 함수
void changeToAscii(unsigned char* image, int width, int height, struct ImageInfo *info) {
    int gray;
    int index;

    for (int i = height; i > 0; i-=10) {
        for (int j = 0; j < width; j+=10) {
            int sum = 0;
            //밝기 합 구함
            for (int k = 0; k < 10; k++) {
                for (int l = 0; l < 10; l++) {
                    int row = i + k;
                    int col = j + l;
                    if (row < height && col < width) {
                        index = (row * width + col) * 3;
                                // RGB 값을 흑백으로 변환
                        sum += (int)(0.2989 * image[index] + 0.5870 * image[index + 1] + 0.1140 * image[index + 2]);
                    }
                }
            }

            //평균 밝기 계산
            int average = sum / 100;
            // 밝기에 따라 아스키 문자 선택하여 출력
            int char_index = (average * (sizeof(info->ascii_chars) - 1)) / 255;
            printf("%c", info->ascii_chars[char_index]);

        }
        printf("\n");
    }
}



int main() {
    // JPEG 이미지 파일 경로

    struct ImageInfo info;
    info.ascii_chars = "@%#*+=-:. ";
    info.filename = "image.jpg";


    // JPEG 파일을 읽기 모드로 열기
    FILE* file = fopen(info.filename, "rb");
    if (!file) {
        printf("파일을 열 수 없습니다.");
        return 1;
    }

    // JPEG 구조체 선언
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // JPEG 오류 처리기 설정
    cinfo.err = jpeg_std_error(&jerr);

    // JPEG 구조체 초기화
    jpeg_create_decompress(&cinfo);

    // 입력 파일 지정
    jpeg_stdio_src(&cinfo, file);

    // 파일의 헤더 정보 읽기
    jpeg_read_header(&cinfo, TRUE);

    // 이미지 디코딩
    jpeg_start_decompress(&cinfo);

    // 이미지 데이터 저장을 위한 버퍼 생성
    unsigned char* image = (unsigned char*)malloc(cinfo.output_width * cinfo.output_height * cinfo.output_components);

    // 이미지 행 단위로 스캔하며 데이터 읽기
    while (cinfo.output_scanline < cinfo.output_height) {
        unsigned char* row_pointer = &image[(cinfo.output_height - cinfo.output_scanline - 1) * cinfo.output_width * cinfo.output_components];
        jpeg_read_scanlines(&cinfo, &row_pointer, 1);
    }

    // 이미지 출력
    changeToAscii(image, cinfo.output_width, cinfo.output_height,&info);

    // 메모리 해제 및 파일 닫기
    free(image);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(file);

    return 0;
}
