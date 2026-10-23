#define ARK_NAMESPACE

#import "arklib.h"

BEGIN

static const cstring gs1 = "foo - baz - not - ark";
static const cstring gs2 = " - - concat - ";

ark_string_view ark_strtokenize (ark_string* s_, cstring tk_) {
	static ark_string* curr;
	if (s_) {
		curr = s_;
	}
	CString	  s		  = curr->data;
	sz		  tk_size = ARK_STRLEN (tk_);
	Char*	  p		  = ark_strffo (*s_, tk_);
	ptrdiff_t offset  = (ptrdiff_t)(p - s);
	return (ark_string_view) {
		.data = s,
		.len  = offset,
	};
}

int ___arklib__ () {
	monotonic_buffer (mbuff);

	string_view s  = make_string_view (gs1);
	string_view s2 = make_string_view (gs2);

	raii (string) cs = ark_strconcat (MBA (mbuff), s, s2);

	printf (strfmt "\n", strarg (cs));

	ark_darray (bstring) ss = ark_strsplit (MBA (mbuff), cs, "-");

	// $foreach (s $in_darray (ss)) {
	// 	// strtrim (s);
	// 	printf (strfmt "\n", strarg (*s));
	// }

	const char* tok = strtok (cs.data, "-");

	while (tok) {
		printf ("%s\n", tok);
		tok = strtok (nullptr, "-");
	}

	destroy_darray (ss, ark_destroy_bstring);
	return 0;
}

int ___pure_c__ () {
	char*  s	= gs1;
	size_t slen = strlen (s);

	char*  s2	 = gs2;
	size_t s2len = strlen (s2);

	size_t cslen		 = slen + s2len;
	char   cs[cslen + 1] = {};

	memcpy (cs, s, slen);
	memcpy (cs + slen, s2, s2len);

	printf ("%s\n", cs);

	const char* tok = strtok (cs, "-");
	while (tok) {
		printf ("%s\n", tok);
		tok = strtok (nullptr, "-");
	}
	return 0;
}

int main () {
	// return ___arklib__ ();
	// return ___pure_c__ ();

	class_array (i32, 4, arr);

	$foreach (e $in_array (arr)) {
		printf ("%i ", *e);
	}
}
