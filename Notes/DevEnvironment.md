# Development Environment

Because NachOS need to be built under 32bit.

## Install 32-bit libraries on 64-bit Ubuntu

* [How to compile 32-bit program on 64-bit gcc in C and C++](https://www.geeksforgeeks.org/compile-32-bit-program-64-bit-gcc-c-c/)
* [HowTo Compile a 32-bit Application Using gcc On the 64-bit Linux Version](https://www.cyberciti.biz/tips/compile-32bit-application-using-gcc-64-bit-linux.html)
* [Ubuntu apt-get install ia32 for 32-bit on 64-bit](http://www.sixarm.com/about/ubuntu-apt-get-install-ia32-for-32-bit-on-64-bit.html)

```sh
sudo apt-get install gcc-multilib g++-multilib
```

Test:

```c
#include <stdio.h>

int main(){
    long z;
    printf("Long int size is %i bytes long!\n", sizeof(z));
    return 0;
}
```

```sh
$ gcc -m32 -o output32 hello.c
$ ./output32
Long int size is 4 bytes long!

$ gcc -m64 -o output64 hello.c
$ ./output64
Long int size is 8 bytes long!
```

## Use 32bit image directly

...
