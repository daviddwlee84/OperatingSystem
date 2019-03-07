# Process Practice

## fork()

* [fork.c](fork.c)

```txt
        ------- Parent Process -------> wait() --- Resume --->
       /                                  ^
fork()                                    |
       \                                  |
        -- Child Process --> exec() --> exit()
```

## Virtual Address Space Experiment

* [myval.c](myval.c)
* [test_myval_AddressSpace.sh](test_myval_AddressSpace.sh)

```sh
bash test_myval_AddressSpace.sh
```

Ideal result:

```txt
This is myval with argument 5
myval is 5, loc 0x10be8d020
myval is 5, loc 0x10be8d020
myval is 5, loc 0x10be8d020
myval is 5, loc 0x10be8d020
myval is 5, loc 0x10be8d020
This is myval with argument 6
myval is 6, loc 0x10be8d020
myval is 6, loc 0x10be8d020
myval is 6, loc 0x10be8d020
myval is 6, loc 0x10be8d020
myval is 6, loc 0x10be8d020
They have "same address" with different value.
==> this address is in "Virtual Address Space"
```

## Access same variable name

* [samevariable.c](samevariable.c)

## Multi-fork

* [multifork.c](multifork.c)
