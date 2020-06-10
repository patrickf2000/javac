#include <iostream>

#include <javabuilder.hh>
#include <javafile.hh>
#include <javapool.hh>

extern void parse(const char *path, JavaBuilder *b);

int main(int argc, char *argv[]) {
    auto *builder = new JavaBuilder("Test1");
    builder->useOutput();

	if (argc > 1) {
		parse(argv[1], builder);
	} else {
		parse("", builder);
	}
	
	builder->write();
	
	return 0;
}
