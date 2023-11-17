#include <stdio.h>
int main() {
    // 이미지 파일 경로
    char *filename = "your_image.jpg";

    // JPEG 파일을 읽기 모드로 열기
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("파일을 열 수 없습니다.");
        return 1;
    }