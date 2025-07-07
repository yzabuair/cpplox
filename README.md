# cpplox

## Introduction
This is a c++ version of the tree-walk interpreter in the book [Crafting Interpreters](https://github.com/munificent/craftinginterpreters) 

## How To Build
Running ./build.sh will create an Xcode project.  It lives under build/cpplox.xcodeproj.  I have not tried this on other OS's or IDE, but in theory you can just
change the generator in the build script to target other system or IDE.

## How To Run
The executable ends up in build/debug/cpplox.  The executable can operate in REPL mode or run a script.  To run a script:
```
./cpplox <script_name.lox>
```

To run in REPL
```
./cpplox
*** Running REPL
.run to run the script.
.quit to exit REPL.
```

The REPL is not a great editor, so in general I wil type the script in an editor and then copy/pasta the script into the console.

## Design Choices
On errors we throw an exception and stop.

We mostly use std::unique_ptr, but ocassionaly we use std::shared_ptr.  This is because std::function can only use things that are copyable.

I don't have an Callable interface, instead we use a std::function to provide similar functionality.

We use C++23, but the only feature we really need is std::print from c++23.

The code is chosen to be relatively simple, but tries to use a "modern" version of C++.  Hopefully it should make it readable but give an example of using some newer C++ features.



