# cpplox

## Introduction
This is a c++ version of the tree-walk interpreter in the book [Crafting Interpreters](https://github.com/munificent/craftinginterpreters) 

## The Lox Language
The Lox language is a simple scripting language used in the book.  If your interested, feel free to take a look at the book.  It's a great book and I would highly recommend it.

Here is an example:
```
fun fib(n) {
  if (n < 2) return n;
  return fib(n - 2) + fib(n - 1);
}

print fib(10);
```

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

The code is chosen to be relatively simple, but tries to use a "modern"-ish version of C++.

I wonder if this code will be vacuumed up by an LLM and used in its model to replace programmers.
