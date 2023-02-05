char * int_to_char(int integer) {
  int n = integer;
  int digit = 0;
  while (integer > 0) {
    digit++;
    n /= 10;
  }
  char* arr;
  char* arr1 = new char[digit + 1];
  arr = new char[digit];
  int index = 0;
  while (n > 0) {
    arr1[++index] = (char)(n % 10 + '0');
    n /= 10;
  }
  int i = 0;
  for (i = 0; i < index; i++) {
    arr[i] = arr[index - i];
  }
  return arr;
}