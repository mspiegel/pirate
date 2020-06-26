#include <assert.h>
#include <endian.h>
#include <fenv.h>
#include <math.h>
#include <stdint.h>
#include <string.h>


struct primitives {
	float float_val __attribute__((aligned(4)));
	double double_val __attribute__((aligned(8)));
	int16_t short_val __attribute__((aligned(2)));
	int16_t int16_val __attribute__((aligned(2)));
	int32_t long_val __attribute__((aligned(4)));
	int32_t int32_val __attribute__((aligned(4)));
	int64_t long_long_val __attribute__((aligned(8)));
	int64_t int64_val __attribute__((aligned(8)));
	uint16_t unsigned_short_val __attribute__((aligned(2)));
	uint16_t uint16_val __attribute__((aligned(2)));
	uint32_t unsigned_long_val __attribute__((aligned(4)));
	uint32_t uint32_val __attribute__((aligned(4)));
	uint64_t unsigned_long_long_val __attribute__((aligned(8)));
	uint64_t uint64_val __attribute__((aligned(8)));
	char char_val __attribute__((aligned(1)));
	int8_t int8_val __attribute__((aligned(1)));
	uint8_t bool_val __attribute__((aligned(1)));
	uint8_t octet_val __attribute__((aligned(1)));
	uint8_t uint8_val __attribute__((aligned(1)));
};

struct primitives_wire {
	unsigned char float_val[4] __attribute__((aligned(4)));
	unsigned char double_val[8] __attribute__((aligned(8)));
	unsigned char short_val[2] __attribute__((aligned(2)));
	unsigned char int16_val[2] __attribute__((aligned(2)));
	unsigned char long_val[4] __attribute__((aligned(4)));
	unsigned char int32_val[4] __attribute__((aligned(4)));
	unsigned char long_long_val[8] __attribute__((aligned(8)));
	unsigned char int64_val[8] __attribute__((aligned(8)));
	unsigned char unsigned_short_val[2] __attribute__((aligned(2)));
	unsigned char uint16_val[2] __attribute__((aligned(2)));
	unsigned char unsigned_long_val[4] __attribute__((aligned(4)));
	unsigned char uint32_val[4] __attribute__((aligned(4)));
	unsigned char unsigned_long_long_val[8] __attribute__((aligned(8)));
	unsigned char uint64_val[8] __attribute__((aligned(8)));
	unsigned char char_val[1] __attribute__((aligned(1)));
	unsigned char int8_val[1] __attribute__((aligned(1)));
	unsigned char bool_val[1] __attribute__((aligned(1)));
	unsigned char octet_val[1] __attribute__((aligned(1)));
	unsigned char uint8_val[1] __attribute__((aligned(1)));
};

static_assert(sizeof(struct primitives) == sizeof(struct primitives_wire), "size of struct primitives not equal to wire protocol struct");

void encode_primitives(struct primitives* input, struct primitives_wire* output) {
	uint32_t float_val;
	uint64_t double_val;
	uint16_t short_val;
	uint16_t int16_val;
	uint32_t long_val;
	uint32_t int32_val;
	uint64_t long_long_val;
	uint64_t int64_val;
	uint16_t unsigned_short_val;
	uint16_t uint16_val;
	uint32_t unsigned_long_val;
	uint32_t uint32_val;
	uint64_t unsigned_long_long_val;
	uint64_t uint64_val;
	uint8_t char_val;
	uint8_t int8_val;
	uint8_t bool_val;
	uint8_t octet_val;
	uint8_t uint8_val;
	memcpy(&float_val, &input->float_val, sizeof(uint32_t));
	memcpy(&double_val, &input->double_val, sizeof(uint64_t));
	memcpy(&short_val, &input->short_val, sizeof(uint16_t));
	memcpy(&int16_val, &input->int16_val, sizeof(uint16_t));
	memcpy(&long_val, &input->long_val, sizeof(uint32_t));
	memcpy(&int32_val, &input->int32_val, sizeof(uint32_t));
	memcpy(&long_long_val, &input->long_long_val, sizeof(uint64_t));
	memcpy(&int64_val, &input->int64_val, sizeof(uint64_t));
	memcpy(&unsigned_short_val, &input->unsigned_short_val, sizeof(uint16_t));
	memcpy(&uint16_val, &input->uint16_val, sizeof(uint16_t));
	memcpy(&unsigned_long_val, &input->unsigned_long_val, sizeof(uint32_t));
	memcpy(&uint32_val, &input->uint32_val, sizeof(uint32_t));
	memcpy(&unsigned_long_long_val, &input->unsigned_long_long_val, sizeof(uint64_t));
	memcpy(&uint64_val, &input->uint64_val, sizeof(uint64_t));
	memcpy(&char_val, &input->char_val, sizeof(uint8_t));
	memcpy(&int8_val, &input->int8_val, sizeof(uint8_t));
	memcpy(&bool_val, &input->bool_val, sizeof(uint8_t));
	memcpy(&octet_val, &input->octet_val, sizeof(uint8_t));
	memcpy(&uint8_val, &input->uint8_val, sizeof(uint8_t));
	float_val = htobe32(float_val);
	double_val = htobe64(double_val);
	short_val = htobe16(short_val);
	int16_val = htobe16(int16_val);
	long_val = htobe32(long_val);
	int32_val = htobe32(int32_val);
	long_long_val = htobe64(long_long_val);
	int64_val = htobe64(int64_val);
	unsigned_short_val = htobe16(unsigned_short_val);
	uint16_val = htobe16(uint16_val);
	unsigned_long_val = htobe32(unsigned_long_val);
	uint32_val = htobe32(uint32_val);
	unsigned_long_long_val = htobe64(unsigned_long_long_val);
	uint64_val = htobe64(uint64_val);
	memcpy(&output->float_val, &float_val, sizeof(uint32_t));
	memcpy(&output->double_val, &double_val, sizeof(uint64_t));
	memcpy(&output->short_val, &short_val, sizeof(uint16_t));
	memcpy(&output->int16_val, &int16_val, sizeof(uint16_t));
	memcpy(&output->long_val, &long_val, sizeof(uint32_t));
	memcpy(&output->int32_val, &int32_val, sizeof(uint32_t));
	memcpy(&output->long_long_val, &long_long_val, sizeof(uint64_t));
	memcpy(&output->int64_val, &int64_val, sizeof(uint64_t));
	memcpy(&output->unsigned_short_val, &unsigned_short_val, sizeof(uint16_t));
	memcpy(&output->uint16_val, &uint16_val, sizeof(uint16_t));
	memcpy(&output->unsigned_long_val, &unsigned_long_val, sizeof(uint32_t));
	memcpy(&output->uint32_val, &uint32_val, sizeof(uint32_t));
	memcpy(&output->unsigned_long_long_val, &unsigned_long_long_val, sizeof(uint64_t));
	memcpy(&output->uint64_val, &uint64_val, sizeof(uint64_t));
	memcpy(&output->char_val, &char_val, sizeof(uint8_t));
	memcpy(&output->int8_val, &int8_val, sizeof(uint8_t));
	memcpy(&output->bool_val, &bool_val, sizeof(uint8_t));
	memcpy(&output->octet_val, &octet_val, sizeof(uint8_t));
	memcpy(&output->uint8_val, &uint8_val, sizeof(uint8_t));
}

void decode_primitives(struct primitives_wire* input, struct primitives* output) {
	uint32_t float_val;
	uint64_t double_val;
	uint16_t short_val;
	uint16_t int16_val;
	uint32_t long_val;
	uint32_t int32_val;
	uint64_t long_long_val;
	uint64_t int64_val;
	uint16_t unsigned_short_val;
	uint16_t uint16_val;
	uint32_t unsigned_long_val;
	uint32_t uint32_val;
	uint64_t unsigned_long_long_val;
	uint64_t uint64_val;
	uint8_t char_val;
	uint8_t int8_val;
	uint8_t bool_val;
	uint8_t octet_val;
	uint8_t uint8_val;
	memcpy(&float_val, &input->float_val, sizeof(uint32_t));
	memcpy(&double_val, &input->double_val, sizeof(uint64_t));
	memcpy(&short_val, &input->short_val, sizeof(uint16_t));
	memcpy(&int16_val, &input->int16_val, sizeof(uint16_t));
	memcpy(&long_val, &input->long_val, sizeof(uint32_t));
	memcpy(&int32_val, &input->int32_val, sizeof(uint32_t));
	memcpy(&long_long_val, &input->long_long_val, sizeof(uint64_t));
	memcpy(&int64_val, &input->int64_val, sizeof(uint64_t));
	memcpy(&unsigned_short_val, &input->unsigned_short_val, sizeof(uint16_t));
	memcpy(&uint16_val, &input->uint16_val, sizeof(uint16_t));
	memcpy(&unsigned_long_val, &input->unsigned_long_val, sizeof(uint32_t));
	memcpy(&uint32_val, &input->uint32_val, sizeof(uint32_t));
	memcpy(&unsigned_long_long_val, &input->unsigned_long_long_val, sizeof(uint64_t));
	memcpy(&uint64_val, &input->uint64_val, sizeof(uint64_t));
	memcpy(&char_val, &input->char_val, sizeof(uint8_t));
	memcpy(&int8_val, &input->int8_val, sizeof(uint8_t));
	memcpy(&bool_val, &input->bool_val, sizeof(uint8_t));
	memcpy(&octet_val, &input->octet_val, sizeof(uint8_t));
	memcpy(&uint8_val, &input->uint8_val, sizeof(uint8_t));
	float_val = be32toh(float_val);
	double_val = be64toh(double_val);
	short_val = be16toh(short_val);
	int16_val = be16toh(int16_val);
	long_val = be32toh(long_val);
	int32_val = be32toh(int32_val);
	long_long_val = be64toh(long_long_val);
	int64_val = be64toh(int64_val);
	unsigned_short_val = be16toh(unsigned_short_val);
	uint16_val = be16toh(uint16_val);
	unsigned_long_val = be32toh(unsigned_long_val);
	uint32_val = be32toh(uint32_val);
	unsigned_long_long_val = be64toh(unsigned_long_long_val);
	uint64_val = be64toh(uint64_val);
	memcpy(&output->float_val, &float_val, sizeof(uint32_t));
	memcpy(&output->double_val, &double_val, sizeof(uint64_t));
	memcpy(&output->short_val, &short_val, sizeof(uint16_t));
	memcpy(&output->int16_val, &int16_val, sizeof(uint16_t));
	memcpy(&output->long_val, &long_val, sizeof(uint32_t));
	memcpy(&output->int32_val, &int32_val, sizeof(uint32_t));
	memcpy(&output->long_long_val, &long_long_val, sizeof(uint64_t));
	memcpy(&output->int64_val, &int64_val, sizeof(uint64_t));
	memcpy(&output->unsigned_short_val, &unsigned_short_val, sizeof(uint16_t));
	memcpy(&output->uint16_val, &uint16_val, sizeof(uint16_t));
	memcpy(&output->unsigned_long_val, &unsigned_long_val, sizeof(uint32_t));
	memcpy(&output->uint32_val, &uint32_val, sizeof(uint32_t));
	memcpy(&output->unsigned_long_long_val, &unsigned_long_long_val, sizeof(uint64_t));
	memcpy(&output->uint64_val, &uint64_val, sizeof(uint64_t));
	memcpy(&output->char_val, &char_val, sizeof(uint8_t));
	memcpy(&output->int8_val, &int8_val, sizeof(uint8_t));
	memcpy(&output->bool_val, &bool_val, sizeof(uint8_t));
	memcpy(&output->octet_val, &octet_val, sizeof(uint8_t));
	memcpy(&output->uint8_val, &uint8_val, sizeof(uint8_t));
}

int validate_primitives(const struct primitives* input) {
	return 0;
}

void transform_primitives(struct primitives* input) {
}
