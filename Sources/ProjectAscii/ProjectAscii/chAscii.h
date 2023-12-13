struct ImageInfo {
    char ascii_chars[11];
    char filename[256];
};

void changeToAscii(unsigned char* image, int width, int height, struct ImageInfo* info);