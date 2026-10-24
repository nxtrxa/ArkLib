# ArkLib

A **General purpose** C library

[About](about)

```
#define ARK_NAMESPACE
#import "ark/arklib.h"

int main () {
	monotonic_buffer (mbuff); // declares a static buffer

	string_view s  = make_string_view ("Hello,");
	string_view s2 = make_string_view ("World!");

	string cs = strconcat (MBA (mbuff), s, s2);

	printf (strfmt "\n", strarg (cs));

	darray (bstring) ss = strsplit ( // dynamic array of bstrings
       monotonic_buffer_allocator (mbuff),
       cs,
       "-"
    );

	$foreach (s $in_darray (ss)) {
		strtrim (s);
		printf (strfmt "\n", strarg (*s));
	}

    // Here `cs` and `ss` are allocated on the stack.

	return 0;
}
```
