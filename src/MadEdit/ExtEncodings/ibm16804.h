/* Tables for conversion from and to IBM16804.
   Copyright (C) 2005-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Masahide Washizawa <washi@jp.ibm.com>, 2005.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "gap.h"

/*
 * IBM16804
 */

static const unsigned short ibm16804_2uni[256] =
{
  /*[0x00] =*/ 0x0000, /*[0x01] =*/ 0x0001, /*[0x02] =*/ 0x0002, /*[0x03] =*/ 0x0003,
  /*[0x04] =*/ 0x009c, /*[0x05] =*/ 0x0009, /*[0x06] =*/ 0x0086, /*[0x07] =*/ 0x007f,
  /*[0x08] =*/ 0x0097, /*[0x09] =*/ 0x008d, /*[0x0a] =*/ 0x008e, /*[0x0b] =*/ 0x000b,
  /*[0x0c] =*/ 0x000c, /*[0x0d] =*/ 0x000d, /*[0x0e] =*/ 0x000e, /*[0x0f] =*/ 0x000f,
  /*[0x10] =*/ 0x0010, /*[0x11] =*/ 0x0011, /*[0x12] =*/ 0x0012, /*[0x13] =*/ 0x0013,
  /*[0x14] =*/ 0x009d, /*[0x15] =*/ 0x0085, /*[0x16] =*/ 0x0008, /*[0x17] =*/ 0x0087,
  /*[0x18] =*/ 0x0018, /*[0x19] =*/ 0x0019, /*[0x1a] =*/ 0x0092, /*[0x1b] =*/ 0x008f,
  /*[0x1c] =*/ 0x001c, /*[0x1d] =*/ 0x001d, /*[0x1e] =*/ 0x001e, /*[0x1f] =*/ 0x001f,
  /*[0x20] =*/ 0x0080, /*[0x21] =*/ 0x0081, /*[0x22] =*/ 0x0082, /*[0x23] =*/ 0x0083,
  /*[0x24] =*/ 0x0084, /*[0x25] =*/ 0x000a, /*[0x26] =*/ 0x0017, /*[0x27] =*/ 0x001b,
  /*[0x28] =*/ 0x0088, /*[0x29] =*/ 0x0089, /*[0x2a] =*/ 0x008a, /*[0x2b] =*/ 0x008b,
  /*[0x2c] =*/ 0x008c, /*[0x2d] =*/ 0x0005, /*[0x2e] =*/ 0x0006, /*[0x2f] =*/ 0x0007,
  /*[0x30] =*/ 0x0090, /*[0x31] =*/ 0x0091, /*[0x32] =*/ 0x0016, /*[0x33] =*/ 0x0093,
  /*[0x34] =*/ 0x0094, /*[0x35] =*/ 0x0095, /*[0x36] =*/ 0x0096, /*[0x37] =*/ 0x0004,
  /*[0x38] =*/ 0x0098, /*[0x39] =*/ 0x0099, /*[0x3a] =*/ 0x009a, /*[0x3b] =*/ 0x009b,
  /*[0x3c] =*/ 0x0014, /*[0x3d] =*/ 0x0015, /*[0x3e] =*/ 0x009e, /*[0x3f] =*/ 0x001a,
  /*[0x40] =*/ 0x0020, /*[0x41] =*/ 0x00a0, /*[0x42] =*/ 0x0651, /*[0x43] =*/ 0xfe7d,
  /*[0x44] =*/ 0x0640, /*[0x45] =*/ 0x200b, /*[0x46] =*/ 0x0621, /*[0x47] =*/ 0x0622,
  /*[0x48] =*/ 0xfe82, /*[0x49] =*/ 0x0623, /*[0x4a] =*/ 0x00a2, /*[0x4b] =*/ 0x002e,
  /*[0x4c] =*/ 0x003c, /*[0x4d] =*/ 0x0028, /*[0x4e] =*/ 0x002b, /*[0x4f] =*/ 0x007c,
  /*[0x50] =*/ 0x0026, /*[0x51] =*/ 0xfe84, /*[0x52] =*/ 0x0624, /*[0x55] =*/ 0x0626,
  /*[0x56] =*/ 0x0627, /*[0x57] =*/ 0xfe8e, /*[0x58] =*/ 0x0628, /*[0x59] =*/ 0xfe91,
  /*[0x5a] =*/ 0x0021, /*[0x5b] =*/ 0x0024, /*[0x5c] =*/ 0x002a, /*[0x5d] =*/ 0x0029,
  /*[0x5e] =*/ 0x003b, /*[0x5f] =*/ 0x00ac, /*[0x60] =*/ 0x002d, /*[0x61] =*/ 0x002f,
  /*[0x62] =*/ 0x0629, /*[0x63] =*/ 0x062a, /*[0x64] =*/ 0xfe97, /*[0x65] =*/ 0x062b,
  /*[0x66] =*/ 0xfe9b, /*[0x67] =*/ 0x062c, /*[0x68] =*/ 0xfe9f, /*[0x69] =*/ 0x062d,
  /*[0x6a] =*/ 0x00a6, /*[0x6b] =*/ 0x002c, /*[0x6c] =*/ 0x0025, /*[0x6d] =*/ 0x005f,
  /*[0x6e] =*/ 0x003e, /*[0x6f] =*/ 0x003f, /*[0x70] =*/ 0xfea3, /*[0x71] =*/ 0x062e,
  /*[0x72] =*/ 0xfea7, /*[0x73] =*/ 0x062f, /*[0x74] =*/ 0x0630, /*[0x75] =*/ 0x0631,
  /*[0x76] =*/ 0x0632, /*[0x77] =*/ 0x0633, /*[0x78] =*/ 0xfeb3, /*[0x79] =*/ 0x060c,
  /*[0x7a] =*/ 0x003a, /*[0x7b] =*/ 0x0023, /*[0x7c] =*/ 0x0040, /*[0x7d] =*/ 0x0027,
  /*[0x7e] =*/ 0x003d, /*[0x7f] =*/ 0x0022, /*[0x80] =*/ 0x0634, /*[0x81] =*/ 0x0061,
  /*[0x82] =*/ 0x0062, /*[0x83] =*/ 0x0063, /*[0x84] =*/ 0x0064, /*[0x85] =*/ 0x0065,
  /*[0x86] =*/ 0x0066, /*[0x87] =*/ 0x0067, /*[0x88] =*/ 0x0068, /*[0x89] =*/ 0x0069,
  /*[0x8a] =*/ 0xfeb7, /*[0x8b] =*/ 0x0635, /*[0x8c] =*/ 0xfebb, /*[0x8d] =*/ 0x0636,
  /*[0x8e] =*/ 0xfebf, /*[0x8f] =*/ 0x0637, /*[0x90] =*/ 0x0638, /*[0x91] =*/ 0x006a,
  /*[0x92] =*/ 0x006b, /*[0x93] =*/ 0x006c, /*[0x94] =*/ 0x006d, /*[0x95] =*/ 0x006e,
  /*[0x96] =*/ 0x006f, /*[0x97] =*/ 0x0070, /*[0x98] =*/ 0x0071, /*[0x99] =*/ 0x0072,
  /*[0x9a] =*/ 0x0639, /*[0x9b] =*/ 0xfeca, /*[0x9c] =*/ 0xfecb, /*[0x9d] =*/ 0xfecc,
  /*[0x9e] =*/ 0x063a, /*[0x9f] =*/ 0xfece, /*[0xa0] =*/ 0xfecf, /*[0xa1] =*/ 0x00f7,
  /*[0xa2] =*/ 0x0073, /*[0xa3] =*/ 0x0074, /*[0xa4] =*/ 0x0075, /*[0xa5] =*/ 0x0076,
  /*[0xa6] =*/ 0x0077, /*[0xa7] =*/ 0x0078, /*[0xa8] =*/ 0x0079, /*[0xa9] =*/ 0x007a,
  /*[0xaa] =*/ 0xfed0, /*[0xab] =*/ 0x0641, /*[0xac] =*/ 0xfed3, /*[0xad] =*/ 0x0642,
  /*[0xae] =*/ 0xfed7, /*[0xaf] =*/ 0x0643, /*[0xb0] =*/ 0xfedb, /*[0xb1] =*/ 0x0644,
  /*[0xb2] =*/ 0xfef5, /*[0xb3] =*/ 0xfef6, /*[0xb4] =*/ 0xfef7, /*[0xb5] =*/ 0xfef8,
  /*[0xb8] =*/ 0xfefb, /*[0xb9] =*/ 0xfefc, /*[0xba] =*/ 0xfedf, /*[0xbb] =*/ 0x0645,
  /*[0xbc] =*/ 0xfee3, /*[0xbd] =*/ 0x0646, /*[0xbe] =*/ 0xfee7, /*[0xbf] =*/ 0x0647,
  /*[0xc0] =*/ 0x061b, /*[0xc1] =*/ 0x0041, /*[0xc2] =*/ 0x0042, /*[0xc3] =*/ 0x0043,
  /*[0xc4] =*/ 0x0044, /*[0xc5] =*/ 0x0045, /*[0xc6] =*/ 0x0046, /*[0xc7] =*/ 0x0047,
  /*[0xc8] =*/ 0x0048, /*[0xc9] =*/ 0x0049, /*[0xca] =*/ 0x00ad, /*[0xcb] =*/ 0xfeeb,
  /*[0xcd] =*/ 0xfeec, /*[0xcf] =*/ 0x0648, /*[0xd0] =*/ 0x061f, /*[0xd1] =*/ 0x004a,
  /*[0xd2] =*/ 0x004b, /*[0xd3] =*/ 0x004c, /*[0xd4] =*/ 0x004d, /*[0xd5] =*/ 0x004e,
  /*[0xd6] =*/ 0x004f, /*[0xd7] =*/ 0x0050, /*[0xd8] =*/ 0x0051, /*[0xd9] =*/ 0x0052,
  /*[0xda] =*/ 0x0649, /*[0xdb] =*/ 0xfef0, /*[0xdc] =*/ 0x064a, /*[0xdd] =*/ 0xfef2,
  /*[0xde] =*/ 0xfef3, /*[0xdf] =*/ 0x0660, /*[0xe0] =*/ 0x00d7, /*[0xe1] =*/ 0x2007,
  /*[0xe2] =*/ 0x0053, /*[0xe3] =*/ 0x0054, /*[0xe4] =*/ 0x0055, /*[0xe5] =*/ 0x0056,
  /*[0xe6] =*/ 0x0057, /*[0xe7] =*/ 0x0058, /*[0xe8] =*/ 0x0059, /*[0xe9] =*/ 0x005a,
  /*[0xea] =*/ 0x0661, /*[0xeb] =*/ 0x0662, /*[0xed] =*/ 0x0663, /*[0xee] =*/ 0x0664,
  /*[0xef] =*/ 0x0665, /*[0xf0] =*/ 0x0030, /*[0xf1] =*/ 0x0031, /*[0xf2] =*/ 0x0032,
  /*[0xf3] =*/ 0x0033, /*[0xf4] =*/ 0x0034, /*[0xf5] =*/ 0x0035, /*[0xf6] =*/ 0x0036,
  /*[0xf7] =*/ 0x0037, /*[0xf8] =*/ 0x0038, /*[0xf9] =*/ 0x0039, /*[0xfa] =*/ 0x20ac,
  /*[0xfb] =*/ 0x0666, /*[0xfc] =*/ 0x0667, /*[0xfd] =*/ 0x0668, /*[0xfe] =*/ 0x0669,
  /*[0xff] =*/ 0x009f
};

static int
ibm16804_mbtowc (ucs4_t *pwc, const unsigned char *s, int WXUNUSED(n))
{
   unsigned char c = *s;
   *pwc = (ucs4_t) ibm16804_2uni[c];
   return 1;
}

static const unsigned char ibm16804_page00[] = {
  0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f,
  0x16, 0x05, 0x25, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26,
  0x18, 0x19, 0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f,
  0x40, 0x5a, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d,
  0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61,
  0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
  0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f,
  0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
  0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6,
  0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6,
  0xe7, 0xe8, 0xe9, 0x00, 0x00, 0x00, 0x00, 0x6d,
  0x00, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
  0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
  0xa7, 0xa8, 0xa9, 0x00, 0x4f, 0x00, 0x00, 0x07,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x15, 0x06, 0x17,
  0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x09, 0x0a, 0x1b,
  0x30, 0x31, 0x1a, 0x33, 0x34, 0x35, 0x36, 0x08,
  0x38, 0x39, 0x3a, 0x3b, 0x04, 0x14, 0x3e, 0xff,
  0x41, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x6a, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x5f, 0xca, 0xe0, 0xa1,
  0x79, 0xc0, 0x00, 0x00, 0x00, 0xd0, 0x00, 0x46,
  0x47, 0x49, 0x52, 0x56, 0x55, 0x56, 0x58, 0x62,
  0x63, 0x65, 0x67, 0x69, 0x71, 0x73, 0x74, 0x75,
  0x76, 0x77, 0x80, 0x8b, 0x8d, 0x8f, 0x90, 0x9a,
  0x9e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0xab,
  0xad, 0xaf, 0xb1, 0xbb, 0xbd, 0xbf, 0xcf, 0xda,
  0xdc, 0x42, 0xdf, 0xea, 0xeb, 0xed, 0xee, 0xef,
  0xfb, 0xfc, 0xfd, 0xfe, 0x6c, 0x6b, 0x4b, 0x5c,
  0xe1, 0x00, 0x00, 0x00, 0x45, 0xfa, 0x42, 0x43,
  0x00, 0x00, 0x46, 0x47, 0x48, 0x49, 0x51, 0x52,
  0x52, 0x56, 0x57, 0x00, 0x00, 0x55, 0x55, 0x56,
  0x57, 0x58, 0x58, 0x59, 0x59, 0x62, 0x62, 0x63,
  0x63, 0x64, 0x64, 0x65, 0x65, 0x66, 0x66, 0x67,
  0x67, 0x68, 0x68, 0x69, 0x69, 0x70, 0x70, 0x71,
  0x71, 0x72, 0x72, 0x73, 0x73, 0x74, 0x74, 0x75,
  0x75, 0x76, 0x76, 0x77, 0x77, 0x78, 0x78, 0x80,
  0x80, 0x8a, 0x8a, 0x8b, 0x8b, 0x8c, 0x8c, 0x8d,
  0x8d, 0x8e, 0x8e, 0x8f, 0x8f, 0x8f, 0x8f, 0x90,
  0x90, 0x90, 0x90, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e,
  0x9f, 0xa0, 0xaa, 0xab, 0xab, 0xac, 0xac, 0xad,
  0xad, 0xae, 0xae, 0xaf, 0xaf, 0xb0, 0xb0, 0xb1,
  0xb1, 0xba, 0xba, 0xbb, 0xbb, 0xbc, 0xbc, 0xbd,
  0xbd, 0xbe, 0xbe, 0xbf, 0xbf, 0xcb, 0xcd, 0xcf,
  0xcf, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xde, 0xb2,
  0xb3, 0xb4, 0xb5, 0xb8, 0xb9, 0xb8, 0xb9
};

static const struct gap ibm16804_idx[] =
{
  { /*start:*/ 0x0000, /*end:*/ 0x00ad, /*idx:*/     0 },
  { /*start:*/ 0x00d7, /*end:*/ 0x00d7, /*idx:*/   -41 },
  { /*start:*/ 0x00f7, /*end:*/ 0x00f7, /*idx:*/   -72 },
  { /*start:*/ 0x060c, /*end:*/ 0x060c, /*idx:*/ -1372 },
  { /*start:*/ 0x061b, /*end:*/ 0x064a, /*idx:*/ -1386 },
  { /*start:*/ 0x0651, /*end:*/ 0x0651, /*idx:*/ -1392 },
  { /*start:*/ 0x0660, /*end:*/ 0x066d, /*idx:*/ -1406 },
  { /*start:*/ 0x2007, /*end:*/ 0x200b, /*idx:*/ -7959 },
  { /*start:*/ 0x20ac, /*end:*/ 0x20ac, /*idx:*/ -8119 },
  { /*start:*/ 0xfe7c, /*end:*/ 0xfefc, /*idx:*/ -64902 },
  { /*start:*/ 0xffff, /*end:*/ 0xffff, /*idx:*/     0 }
};

static int
ibm16804_wctomb (unsigned char *r, ucs4_t wc, int WXUNUSED(n))
{
   const struct gap* p2gap = ibm16804_idx;
   for (; p2gap->start < 0xffff ; p2gap++) {
      if (wc >= p2gap->start && wc <= p2gap->end) {
         *r = ibm16804_page00[wc - p2gap->idx];
         return 1;
      }
   }
   return RET_ILUNI;
}
