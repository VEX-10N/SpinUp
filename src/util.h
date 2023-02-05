void int_to_char(int integer) {
  int n = integer;
  int digit = 0;
  while (integer > 0) {
    digit++;
    n /= 10;
  }
  char* arr;
  char* arr1 = char[digit + 1]();
}