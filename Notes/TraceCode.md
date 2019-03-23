# Trace Code Skill

## VIM Related

* `gf`: go file
  * e.g. put cursor on the filename (#include "filename.h")
* `Ctrl + o`: jump back

- `*`: search same word
- `n`/`N`: next/previous one

* [Stackoverflow - Jump to function definition in vim](https://stackoverflow.com/questions/635770/jump-to-function-definition-in-vim)
* [Vim Tips Wiki - Go to definition using g](https://vim.fandom.com/wiki/Go_to_definition_using_g)
* [Stackoverflow - How to jump to the beginning of the current function body in Vim?](https://stackoverflow.com/questions/2109503/how-to-jump-to-the-beginning-of-the-current-function-body-in-vim)

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
* Ctrlp

- [Make Your Vim Smarter Using Ctrlp and Ctags](https://medium.freecodecamp.org/make-your-vim-smarter-using-ctrlp-and-ctags-846fc12178a4)
- [Vim and Ctags](https://andrew.stwrt.ca/posts/vim-ctags/)

## Grep

```sh
grep -inr keyword *
```

* `-i`: case-insensitive
* `-n`: prefix output with line numbers
* `-r`: recurse subdirectories

### With find

```sh
find . -name *.c | xargs grep -rn keyword
```
