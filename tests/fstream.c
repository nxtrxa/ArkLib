#define ARK_NAMESPACE
#include "arklib.h"

int main (int argc, char** argv) {
	UNUSED (argc, argv);

	ark_file* f = ark_fstream_open ("text.txt", "r+");

	if (f != nullptr) {
		ark_fstream_write (f, "Hello, World!");
		ark_fstream_close (f);
	}

	return 0;
}
