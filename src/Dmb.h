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

#ifndef _DMB
#define _DMB

#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "Interleaver.h"
#include "ReedSolomon.h"


class DmbStats
{
public:
    DmbStats();
    void reset();

    unsigned dmbFrame;
    unsigned mpgFrame;
    unsigned synFrame;
    unsigned rcvBytes;
    unsigned nulBytes;
    unsigned sndBytes;
    unsigned errBytes;
    unsigned corBytes;
};


class Dmb
{
public:
    Dmb(bool reverse = false);
    Dmb(const Dmb& clone);
    virtual ~Dmb();

    void setReverse(bool state);
    int encode(void* dataIn, unsigned long sizeIn, void* dataOut, unsigned long sizeOut);
    void reset();
    DmbStats getStats() { return stats; };

private:
    int sync(void* dataIn, unsigned long sizeIn);

    Interleaver interleaver;
    ReedSolomon encoder;
    unsigned char* buffer;
    unsigned bufferSize;
    unsigned bufferOffset;      // Encoded data written to output
    unsigned bufferLength;        // Encoded data
    unsigned inputOffset;
    unsigned outputOffset;
//    unsigned padding;           // Padding data written

    bool reverse;

    DmbStats stats;
};


#endif // _DMB
