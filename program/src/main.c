
int main() {
    *((volatile char *)(0x50000000 + 0xFE800)) = 'P';
    return 0x11;
}
