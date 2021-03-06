## jc

### Introduction
This is a very simple library for generating Java bytecode. Currently, it can do a few simple things like reference libraries, create and initalize classes (which can be stored as class variables), perform some integer operations (including creating integer variables), and work with multiple internal functions. The library is four layers; at the bottom is the JavaFile class, which creates and setups the actual class file. The JavaPool class creates and manages the constant pool. The JavaFunc class allows you to create functions and the code that goes in those functions. Finally, the JavaBuilder class wraps all that around a much easier to use interface.

While the library code is separate, I'm creating a simple frontend for my Quik language to provide an easier method for testing (and better examples with how to use it). See the test folders for examples of the Quik syntax and the API.


### Useful links
For those wondering how I did this, these resources are helpful:    
https://medium.com/@davethomas_9528/writing-hello-world-in-java-byte-code-34f75428e0ad   
https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html    
https://en.wikipedia.org/wiki/Java_bytecode_instruction_listings   

The javap program is also very useful.
