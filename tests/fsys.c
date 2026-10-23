#define ARK_NAMESPACE

#define IMPORT_ARKLIB
#import "arklib.h"


int main (int argc, char** argv) {
	UNUSED (argc, argv);

	// ark_fsys_mktree ("proj/", "src/.gitignore", "src/main.c", "./iinclude/");

	// ark_fsys_rmrf ("proj");

	string s = make_string ("String");

	// strrappend (&s, " - FOO");

	// if (strcomp (s, "String View")) {
	// 	printf ("match\n");
	// } else {
	// 	printf (STRFMT "\n", STRARG (s));
	// }

	// class_string (s1, "STRING");
	// string str = ___ark__strconcat_fn__ (s1, " - FOO", nullptr);
	// printf (STRFMT "\n", STRARG (str));
	// destroy_string (&str);
	return 0;
}
