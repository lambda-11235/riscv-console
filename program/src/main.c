volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);

int main() {
    VIDEO_MEMORY[0] = 'P';
    while(1) {}
    return 0;
}
