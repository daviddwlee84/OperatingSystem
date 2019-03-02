# Trace Code Skill

## VIM Related

* `gf`: go file
  * e.g. put cursor on the filename (#include "filename.h")
* `Ctrl + o`: jump back

- `*`: search same word
- `n`/`N`: next/previous one

### Ctags

> this support *function* and *symbol*

Install ctags

```sh
sudo apt-get install ctags
```

Usage

On the top of the directory type

```sh
ctags -R
```

this will generate a tag file

In VIM you can use the key combination

* `Ctrl + ]`: go to defination
* `Ctrl + t`: back to previous tag

#### Ctags related plugin

* taglist
* [tagbar](https://github.com/majutsushi/tagbar)

## Grep

```sh
grep -inr keyword *
```

* `-i`: case-insensitive
* `-n`: prefix output with line numbers
* `-r`: recurse subdirectories
