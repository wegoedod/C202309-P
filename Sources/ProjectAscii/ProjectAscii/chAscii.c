#include "chAscii.h"
#include <stdio.h>

// 이미지를 아스키 아트로 변환하는 함수
void changeToAscii(unsigned char* image, int width, int height, struct ImageInfo* info) {
    int gray;
    int index;

    for (int i = height; i > 0; i -= 10) {
        for (int j = 0; j < width; j += 10) {
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