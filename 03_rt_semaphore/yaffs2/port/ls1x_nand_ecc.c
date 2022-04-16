/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This file contains an ECC algorithm, compatible mtd driver.
 */

#include <stdint.h>

/*
 * Pre-calculated 256-way 1 byte column parity
 */
static const unsigned char nand_ecc_precalc_table[] =
{
	0x00, 0x55, 0x56, 0x03, 0x59, 0x0c, 0x0f, 0x5a, 0x5a, 0x0f, 0x0c, 0x59, 0x03, 0x56, 0x55, 0x00,
	0x65, 0x30, 0x33, 0x66, 0x3c, 0x69, 0x6a, 0x3f, 0x3f, 0x6a, 0x69, 0x3c, 0x66, 0x33, 0x30, 0x65,
	0x66, 0x33, 0x30, 0x65, 0x3f, 0x6a, 0x69, 0x3c, 0x3c, 0x69, 0x6a, 0x3f, 0x65, 0x30, 0x33, 0x66,
	0x03, 0x56, 0x55, 0x00, 0x5a, 0x0f, 0x0c, 0x59, 0x59, 0x0c, 0x0f, 0x5a, 0x00, 0x55, 0x56, 0x03,
	0x69, 0x3c, 0x3f, 0x6a, 0x30, 0x65, 0x66, 0x33, 0x33, 0x66, 0x65, 0x30, 0x6a, 0x3f, 0x3c, 0x69,
	0x0c, 0x59, 0x5a, 0x0f, 0x55, 0x00, 0x03, 0x56, 0x56, 0x03, 0x00, 0x55, 0x0f, 0x5a, 0x59, 0x0c,
	0x0f, 0x5a, 0x59, 0x0c, 0x56, 0x03, 0x00, 0x55, 0x55, 0x00, 0x03, 0x56, 0x0c, 0x59, 0x5a, 0x0f,
	0x6a, 0x3f, 0x3c, 0x69, 0x33, 0x66, 0x65, 0x30, 0x30, 0x65, 0x66, 0x33, 0x69, 0x3c, 0x3f, 0x6a,
	0x6a, 0x3f, 0x3c, 0x69, 0x33, 0x66, 0x65, 0x30, 0x30, 0x65, 0x66, 0x33, 0x69, 0x3c, 0x3f, 0x6a,
	0x0f, 0x5a, 0x59, 0x0c, 0x56, 0x03, 0x00, 0x55, 0x55, 0x00, 0x03, 0x56, 0x0c, 0x59, 0x5a, 0x0f,
	0x0c, 0x59, 0x5a, 0x0f, 0x55, 0x00, 0x03, 0x56, 0x56, 0x03, 0x00, 0x55, 0x0f, 0x5a, 0x59, 0x0c,
	0x69, 0x3c, 0x3f, 0x6a, 0x30, 0x65, 0x66, 0x33, 0x33, 0x66, 0x65, 0x30, 0x6a, 0x3f, 0x3c, 0x69,
	0x03, 0x56, 0x55, 0x00, 0x5a, 0x0f, 0x0c, 0x59, 0x59, 0x0c, 0x0f, 0x5a, 0x00, 0x55, 0x56, 0x03,
	0x66, 0x33, 0x30, 0x65, 0x3f, 0x6a, 0x69, 0x3c, 0x3c, 0x69, 0x6a, 0x3f, 0x65, 0x30, 0x33, 0x66,
	0x65, 0x30, 0x33, 0x66, 0x3c, 0x69, 0x6a, 0x3f, 0x3f, 0x6a, 0x69, 0x3c, 0x66, 0x33, 0x30, 0x65,
	0x00, 0x55, 0x56, 0x03, 0x59, 0x0c, 0x0f, 0x5a, 0x5a, 0x0f, 0x0c, 0x59, 0x03, 0x56, 0x55, 0x00
};

/**************************************************************************************************
 * nand_calculate_ecc - Calculate 3-byte ECC for 256-byte block
 * @dat:		raw data
 * @ecc_code:	buffer for ECC
 **************************************************************************************************/

int nand_calculate_ecc(const unsigned char *dat, unsigned char *ecc_code)
{
	unsigned char idx, reg1, reg2, reg3, tmp1, tmp2;
	int i;

	/* Initialize variables
	 */
	reg1 = reg2 = reg3 = 0;

	/* Build up column parity
	 */
	for (i = 0; i < 256; i++)
	{
		/* Get CP0 - CP5 from table
		 */
		idx = nand_ecc_precalc_table[*dat++];
		reg1 ^= (idx & 0x3f);

		/* All bit XOR = 1 ?
		 */
		if (idx & 0x40)
		{
			reg3 ^= (unsigned char) i;
			reg2 ^= ~((unsigned char) i);
		}
	}

	/*
	 * Create non-inverted ECC code from line parity
	 */
	tmp1  = (reg3 & 0x80) >> 0; /* B7 -> B7 */
	tmp1 |= (reg2 & 0x80) >> 1; /* B7 -> B6 */
	tmp1 |= (reg3 & 0x40) >> 1; /* B6 -> B5 */
	tmp1 |= (reg2 & 0x40) >> 2; /* B6 -> B4 */
	tmp1 |= (reg3 & 0x20) >> 2; /* B5 -> B3 */
	tmp1 |= (reg2 & 0x20) >> 3; /* B5 -> B2 */
	tmp1 |= (reg3 & 0x10) >> 3; /* B4 -> B1 */
	tmp1 |= (reg2 & 0x10) >> 4; /* B4 -> B0 */

	tmp2  = (reg3 & 0x08) << 4; /* B3 -> B7 */
	tmp2 |= (reg2 & 0x08) << 3; /* B3 -> B6 */
	tmp2 |= (reg3 & 0x04) << 3; /* B2 -> B5 */
	tmp2 |= (reg2 & 0x04) << 2; /* B2 -> B4 */
	tmp2 |= (reg3 & 0x02) << 2; /* B1 -> B3 */
	tmp2 |= (reg2 & 0x02) << 1; /* B1 -> B2 */
	tmp2 |= (reg3 & 0x01) << 1; /* B0 -> B1 */
	tmp2 |= (reg2 & 0x01) << 0; /* B7 -> B0 */

	/* Calculate final ECC code
	 */
#ifdef CONFIG_NAND_ECC_SMC
	ecc_code[0] = ~tmp2;
	ecc_code[1] = ~tmp1;
#else
	ecc_code[0] = ~tmp1;
	ecc_code[1] = ~tmp2;
#endif
	ecc_code[2] = ((~reg1) << 2) | 0x03;

	return 0;
}

static inline int countbits(unsigned int byte)
{
	int res = 0;
	for ( ; byte; byte >>= 1)
		res += byte & 0x01;
	return res;
}

/**************************************************************************************************
 * nand_correct_data - Detect and correct bit error(s)
 * @dat:		raw data read from the chip
 * @read_ecc:	ECC from the chip
 * @calc_ecc:	the ECC calculated from raw data
 * Detect and correct a 1 bit error for 256 byte block
 **************************************************************************************************/

int nand_correct_data(unsigned char *dat, unsigned char *read_ecc, unsigned char *calc_ecc)
{
	unsigned char s0, s1, s2;

#ifdef CONFIG_NAND_ECC_SMC
	s0 = calc_ecc[0] ^ read_ecc[0];
	s1 = calc_ecc[1] ^ read_ecc[1];
	s2 = calc_ecc[2] ^ read_ecc[2];
#else
	s1 = calc_ecc[0] ^ read_ecc[0];
	s0 = calc_ecc[1] ^ read_ecc[1];
	s2 = calc_ecc[2] ^ read_ecc[2];
#endif
	if ((s0 | s1 | s2) == 0)
		return 0;

	/* Check for a single bit error
	 */
	if (((s0 ^ (s0 >> 1)) & 0x55) == 0x55 &&
	    ((s1 ^ (s1 >> 1)) & 0x55) == 0x55 &&
	    ((s2 ^ (s2 >> 1)) & 0x54) == 0x54)
	{

		unsigned int byteoffs, bitnum;

		byteoffs = (s1 << 0) & 0x80;
		byteoffs |= (s1 << 1) & 0x40;
		byteoffs |= (s1 << 2) & 0x20;
		byteoffs |= (s1 << 3) & 0x10;

		byteoffs |= (s0 >> 4) & 0x08;
		byteoffs |= (s0 >> 3) & 0x04;
		byteoffs |= (s0 >> 2) & 0x02;
		byteoffs |= (s0 >> 1) & 0x01;

		bitnum = (s2 >> 5) & 0x04;
		bitnum |= (s2 >> 4) & 0x02;
		bitnum |= (s2 >> 3) & 0x01;

		dat[byteoffs] ^= (1 << bitnum);

		return 1;
	}

	if (countbits(s0 | ((unsigned int)s1 << 8) | ((unsigned int)s2 <<16)) == 1)
		return 1;

	return -1;
}

/**************************************************************************************************
 * nand_read_page_swecc - software ecc based page read function
 **************************************************************************************************/

void nand_correct_page_swecc(unsigned char *buf, unsigned char *ecc_code, unsigned int pagesize,
							 unsigned int *ecc_fixed, unsigned int *ecc_unfixed)
{
	int i, eccsize = 256, retval = 0;
	int eccbytes = 3;
	int eccsteps = pagesize / 256;
	unsigned char *p = buf;
	unsigned char ecc_calc[pagesize / 256 * 3];

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize)
		nand_calculate_ecc(p, &ecc_calc[i]);

	eccsteps = pagesize / 256;
	p = buf;

	for (i = 0 ; eccsteps; eccsteps--, i += eccbytes, p += eccsize)
	{
		retval = nand_correct_data(p, &ecc_code[i], &ecc_calc[i]);

		if (retval == 1)
			*ecc_fixed += 1;
		else if (retval == -1)
			*ecc_unfixed += 1;
	}
}

/**************************************************************************************************
 * nand_write_page_swecc - software ecc based page write function
 **************************************************************************************************/

void nand_calculate_page_swecc(const unsigned char *buf, unsigned char *ecc_code, unsigned int pagesize)
{
	int eccsize = 256;
	int eccbytes = 3;
	int eccsteps = pagesize / 256;
	unsigned char ecc_calc[pagesize / 256 * 3];
	const unsigned char *p = buf;
	int i;

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize)
		nand_calculate_ecc(p, &ecc_calc[i]);

	for (i = 0; i < (pagesize / 256 * 3); i++)
		ecc_code[i] = ecc_calc[i];
}


