#define ARK_NAMESPACE

#include "arklib.h"

int main() {
	vf32x4	v1 = { 4.0, 7.0, 14.0, 47.0 };
	vf128x4 v2 = { 4.0, 7.0, 14.0, 47.0 };

	printf("%.1Lf\n", v1.x * v2.z);

	return 0;
}
