this project is a fork of [cch](https://github.com/tjps/cch) by tjps, forked with the intention of maintaining it.

# CCH #

CCH is a C++ preprocessor that removes the need to manually separate function declarations and implementation.  C++ code can be written in a single .cch file and CCH will automatically split it into corresponding .cc and .h files.  Keep build times, header size, and editor window switches to a minimum.

CCH is designed to be hooked into your build system as a step prior to compilation and offers numerous usages to fit any build system.

CCH emits #line directives so all line-number-dependent constructs (e.g. compiler errors, logging systems) are true to the original .cch file.

## Building ##

(I'm not really fluent with cmake and different build systems, so pullrequests with more detailed instructions are welcome)

```
git clone https://github.com/siladrenja/cch-sila
cd cch-sila
cmake .
```

And then build the given source files wth your desired build system

You can copy the given executeable into PATH or into your project's root folder (recommended to put it to .gitignore if the project is a git repo)

For configuring your editor, see [editor bindings](#editor-bindings) below.

## Example ##
##### .cch file:
```c++
#include <vector>

class foo : public bar {
  static const int shift = 2;
  int x;
public:
  foo(int a)
    : x(a>>shift) {}

  int compute(int a, int b) {
    for (int i = 0; i < 32; i++) {
      if (a & 0b1) {
        b++;
      }
    }
    return b * x;
  }
};
```

##### Generated .h:
```c++
#ifndef __readme_readme_cch__
#define __readme_readme_cch__
#include <vector>

class foo : public bar {
  static const int shift;
  int x;
public:
  foo(int a);

  int compute(int a, int b);
};

#endif
```

##### Generated .cc:
```c++
#include "readme/readme.cch.h"

  /* static */ const int foo::shift = 2;
  foo::foo(int a)
    : x(a>>shift) {}

  int foo::compute(int a, int b) {
    for (int i = 0; i < 32; i++) {
      if (a & 0b1) {
        b++;
      }
    }
    return b * x;
  }
```

## Editor bindings ##

The following are handy shortcuts to have .cch files handled as C++ code in your favorite editors:

<b>emacs</b> - add the following to your .emacs:
```lisp
(setq auto-mode-alist
    (cons '("\\.cch$" . c++-mode) auto-mode-alist))
```

<b>vim</b> - add the following to your .vimrc:
```
au BufEnter *.cch setf cpp
```

<b>ctags</b> - add .cch to the extension->language mapping when invoking ctags:
```
$ ctags ... --langmap="c++:+.cch" ...
```

<b>VSCode</b>
```
https://stackoverflow.com/questions/29973619/how-to-associate-a-file-extension-with-a-certain-language-in-vs-code
```
