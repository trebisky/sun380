Sun 3/80 baremetal framework to test the lance driver

This is part of the sun3 bootrom project.
I had no way to test the lance driver, so I dug out
my Sun 3/80 board and rigged this up.

Go to the "src" directory to do the build

Note that all the include files required are copied to
the "h" directory and the links dev and sun3 point there
so that things will work.

The idea is that the lance code itself and the files in the
h directory are in now way changed from what is used in the
bootrom build.
