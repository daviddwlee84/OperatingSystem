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

## Use Docker with 32bit image

### Building

Build the clean version given by TA

```sh
bash build_clean_nachos.sh
# the outcome image will be nachos:0.0.1
```

Build the modified version in this github repository

```sh
bash build_modified_nachos.sh
# the outcome image will be nachos:latest and nachos:0.1 (default)

# or can take the argement of version
./build_modified_nachos.sh 0.1.1
```

Build only the specific subdirectory (Recommend)

```sh
# build threads for example (use any subdirectory name under nachos-3.4/code/)
./build_subdir_nachos.sh threads
# the outcome image will be nachos_<subdir>:latest
```

### Running

Running example

```sh
# Interactive
$ docker run -it nachos:0.0.1

# Interactive with mount local project
# (Run this in the root of this repositoy)
$ docker run -it -v $(pwd)/Nachos:/Nachos nachos:latest

# Run a single test
$ docker run nachos:latest nachos/nachos-3.4/code/threads/nachos
*** thread 0 looped 0 times
*** thread 1 looped 0 times
*** thread 0 looped 1 times
*** thread 1 looped 1 times
*** thread 0 looped 2 times
*** thread 1 looped 2 times
*** thread 0 looped 3 times
*** thread 1 looped 3 times
*** thread 0 looped 4 times
*** thread 1 looped 4 times
No threads ready or runnable, and no pending interrupts.
Assuming the program completed.
Machine halting!

Ticks: total 130, idle 0, system 130, user 0
Disk I/O: reads 0, writes 0
Console I/O: reads 0, writes 0
Paging: faults 0
Network I/O: packets received 0, sent 0

Cleaning up...
```

GDB debugging

```sh
# Example of debugging threads/nachos built by `./build_subdir_nachos.sh threads`
$ docker run -it nachos_threads gdb nachos/nachos-3.4/code/threads/nachos
(gdb) run -d t -q 1
```

### References

* [maojui/nachOS](https://github.com/maojui/nachOS)
* [nwtgck/nachos-build-dockerfile](https://github.com/nwtgck/nachos-build-dockerfile)

### Docker related

* [docker for beginners](https://docker-curriculum.com/)
  * [github](https://github.com/prakhar1989/docker-curriculum)
* [Docker ENTRYPOINT & CMD: Dockerfile best practices](https://medium.freecodecamp.org/docker-entrypoint-cmd-dockerfile-best-practices-abc591c30e21)
* [Arguments and variables in Docker](https://www.manifold.co/blog/arguments-and-variables-in-docker-94746642f64b)
* docker run
  * [Mounting a Folder in a Docker Container](https://youtu.be/MdRWkqcbLJI)
* docker build
  * [docker docs - docker build -t](https://docs.docker.com/engine/reference/commandline/build/#tag-an-image--t)
  * [docker docs - Best practices for writing Dockerfiles](https://docs.docker.com/v17.09/engine/userguide/eng-image/dockerfile_best-practices/)
  * [docker docs - Create a base image](https://docs.docker.com/v17.09/engine/userguide/eng-image/baseimages/)
  * docker arg
    * [docker docs - Dockerfile reference - ARG](https://docs.docker.com/engine/reference/builder/#arg)
  * docker cache
    * [Understanding the Docker Cache for Faster Builds](https://thenewstack.io/understanding-the-docker-cache-for-faster-builds/)
  * Multi-stage builds
    * [docker docs - Use multi-stage builds](https://docs.docker.com/v17.09/engine/userguide/eng-image/multistage-build/)
    * [Youtube - Drastically reduce the size of your DOCKER images with MULTISTAGE builds](https://youtu.be/KLOdisHW8rQ)
* docker alpine
  * [The 3 Biggest Wins When Using Alpine as a Base Docker Image](https://nickjanetakis.com/blog/the-3-biggest-wins-when-using-alpine-as-a-base-docker-image)

### Shell script related

* default-value syntax
  * [Default shell variables value](https://bash.cyberciti.biz/guide/Default_shell_variables_value)
  * [How to write a bash script that takes optional input arguments?](https://stackoverflow.com/questions/9332802/how-to-write-a-bash-script-that-takes-optional-input-arguments/33419280)
    * somecommand ${1:-foo}
  * [Assigning default values to shell variables with a single command in bash](https://stackoverflow.com/questions/2013547/assigning-default-values-to-shell-variables-with-a-single-command-in-bash)
  * [{var:=default} vs {var:-default} - what is difference?](https://stackoverflow.com/questions/24405606/var-default-vs-var-default-what-is-difference)
    * Positional or special parameters cannot be assigned using := way
