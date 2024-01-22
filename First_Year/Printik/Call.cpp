extern "C" int _printik(const char*, ...);

int main() {
    
    int number = 100;
    int eda = 3802;
    char love[] = {"love"};
    char I = 73;

    _printik("%c %s %h for %d%%!", I, love, eda, number);

    return 0;
}
