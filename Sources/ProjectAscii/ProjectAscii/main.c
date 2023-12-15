#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>
#include "chAscii.h"

int main() {
    // JPEG 이미지 파일 경로

    struct ImageInfo info;
    strcpy_s(info.ascii_chars, sizeof(info.ascii_chars), " .:-=+*#%@");
    char changeAscii;
    int charIndex;
    char changeAsciiword;
    while (1)
    {
        printf("아스키 아트 생성에 사용할 문자를 변경하시겠습니까?(Y/N)\n현재 문자(%s) ", info.ascii_chars, sizeof(info.ascii_chars));
        scanf_s(" %c", &changeAscii,sizeof(changeAscii));
        if (changeAscii == 'Y' || changeAscii == 'y') {
            printf("변경할 문자의 번호를 입력하세요.\n현재 문자(%s) ", &info.ascii_chars, sizeof(info.ascii_chars));
            scanf_s("%d", &charIndex);
            printf("변경할 문자를 입력하세요.\n현재 문자(%s) ", &info.ascii_chars, sizeof(info.ascii_chars));
            scanf_s(" %c", &changeAsciiword,sizeof(changeAsciiword));
            info.ascii_chars[charIndex - 1] = changeAsciiword;
        }
        else if (changeAscii == 'N' || changeAscii == 'n') {
            break;
        }
        else
        {
            printf("Y 또는 N만 입력해주세요.");
        }

        // 개행 문자 처리
        while (getchar() != '\n');
    }

    printf("jpeg 파일의 이름을 입력해주세요. ");
    scanf_s("%s", &info.filename, sizeof(info.filename));

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

    //사이즈 설정
    while (1)
    {
        printf("몇 곱하기 몇 픽셀 당 하나의 문자를 할당할 것인지 입력해주세요(숫자 하나만 입력): ");
        scanf_s("%d", &info.size);
        if (info.size > cinfo.output_width || info.size > cinfo.output_height) {
            printf("사진의 크기보다 클 수 없습니다.\n");
        }
        else {
            break;
        }
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