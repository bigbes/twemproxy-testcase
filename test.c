#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#define MP_SOURCE 1

#include "tarantool-c/src/msgpuck/msgpuck.h"
#include "tarantool-c/src/tarantool.h"
#include "tarantool-c/src/tp.h"

#include "tarantool-c/src/session.h"
#include "tarantool-c/src/iproto.h"

void hexDump (char *desc, void *addr, int len) {
	int i;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;

	if (desc != NULL) printf ("%s:\n", desc);

	for (i = 0; i < len; i++) {
		if ((i % 16) == 0) {
			if (i != 0) printf ("  %s\n", buff);
			printf ("  %04x ", i);
		}
		printf (" %02x", pc[i]);
		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
			buff[i % 16] = '.';
		else
			buff[i % 16] = pc[i];
		buff[(i % 16) + 1] = '\0';
	}

	while ((i % 16) != 0) {
		printf ("   ");
		i++;
	}

	printf ("  %s\n", buff);
}


int main() {
	int32_t size1 = 0, size2 = 0;

	struct tbses s;
	tb_sesinit(&s);
	assert(tb_sesset(&s, TB_HOST, "localhost") != -1);
	assert(tb_sesset(&s, TB_PORT, 22121) != -1);
	assert(tb_sesconnect(&s) != -1);

	char greet[128];
	tb_sesrecv(&s, greet, 128, 1);

//	char *buf = NULL; assert(buf = (char *)calloc(512, 1));
	char buf[] = {0xce, 0x00, 0x00, 0x00, 0x1b, 0x82, 0x00, 0x01, 0x01, 0xce, 0x00, 0x00, 0x00, 0x00, 0x85, 0x10, 0xcd, 0x01, 0x18, 0x11, 0x02, 0x13, 0x00, 0x12, 0x64, 0x20, 0x91, 0xa4, 0x74, 0x65, 0x73, 0x74};

	int rc = tb_sessend(&s, buf, 27);
	assert(rc != -1);
	assert(tb_sessync(&s));
	sleep(1);
	rc = tb_sessend(&s, buf + 27, 5);
	assert(rc != -1);
	assert(tb_sessync(&s));

	tb_sesrecv(&s, buf, 5, 1);
	const char *p = buf;
	size1 = mp_decode_uint(&p);
	uint32_t rsize = tb_sesrecv(&s, buf + 5, 507, 0);
	printf("expected: %d bytes\n", size1);
	hexDump("expected", buf + 5, size1);
	printf("got: %d bytes\n", rsize);
	hexDump("got", buf + 5, rsize);
	assert(rsize == size1);
	return 0;
}
