Sun 3/80 projects - a very basic hello world

August 22, 2025

This began by copying my sun 3/280 printf demo, but most
of that code is not used.

A tip to sun3 hackers.  I have been using picocom to give
me a break to get back to the rom.  Then I was using k2
to do a full reset and test new code.  Just typing "b"
is faster and perfect in most caes.

Typing b! (then c) is super fast (it does not do a reset).

This demo has a very basic linker file.
Many normal C language features do not work properly.
A lack of zeroing of BSS variables is one example.
That will be a job for the future.
