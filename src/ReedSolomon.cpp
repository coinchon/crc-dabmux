/*
   Copyright (C) 2005, 2006, 2007, 2008, 2009 Her Majesty the Queen in Right
   of Canada (Communications Research Center Canada)
   */
/*
   This file is part of CRC-DabMux.

   CRC-DabMux is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   CRC-DabMux is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with CRC-DabMux.  If not, see <http://www.gnu.org/licenses/>.
   */

#include "ReedSolomon.h"
#include <stdio.h>          // For galois.h ...
#include <string.h>         // For memcpy

extern "C" {
#include <fec.h>
}
#include <assert.h>
#include <stdlib.h> 

#define SYMSIZE     8


ReedSolomon::ReedSolomon(int N, int K, bool reverse, int gfpoly, int firstRoot, int primElem)
{
    setReverse(reverse);

    myN = N;
    myK = K;

    int symsize = SYMSIZE;
    int nroots = N - K;
    int pad = ((1 << symsize) - 1) - N;

    rsData = init_rs_char(symsize, gfpoly, firstRoot, primElem, nroots, pad);

    if (rsData == NULL) {
        fprintf(stderr, "ERROR: Invalid Reed Solomon parameters!\n");
        abort();
    }
}


ReedSolomon::~ReedSolomon()
{
    free_rs_char(rsData);
}


void ReedSolomon::setReverse(bool state)
{
    reverse = state;
}


int ReedSolomon::encode(void* data, void* fec, unsigned long size)
{
    unsigned char* input = reinterpret_cast<unsigned char*>(data);
    unsigned char* output = reinterpret_cast<unsigned char*>(fec);
    int ret = 0;

    if (reverse) {
        unsigned char* buffer = new unsigned char[myN];
        memcpy(buffer, input, myK);
        memcpy(&buffer[myK], output, myN - myK);

        ret = decode_rs_char(rsData, buffer, NULL, 0);
        if ((ret != 0) && (ret != -1)) {
            memcpy(input, buffer, myK);
            memcpy(output, &buffer[myK], myN - myK);
        }

        delete[] buffer;
    } else {
        encode_rs_char(rsData, input, output);
    }

    return ret;
}


int ReedSolomon::encode(void* data, unsigned long size)
{
    unsigned char* input = reinterpret_cast<unsigned char*>(data);
    int ret = 0;

    if (reverse) {
        ret = decode_rs_char(rsData, input, NULL, 0);
    } else {
        encode_rs_char(rsData, input, &input[188]);
    }

    return ret;
}
