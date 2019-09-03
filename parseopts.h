/**********************************************************************
$Id: parseopts.h,v 1.3 2004/08/17 15:02:48 bu Exp $

Copyright 2004, Bulent Yilmaz
for Microbrew Software

Part of Microbrew md5sum/sha1sum
GNU textutils md5sum replacement

http://www.microbrew.org/tools/md5sha1sum.html

Microbrew md5sum/sha1sum is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

MicroSieve is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with MicroSieve; if not, write to the Free Software Foundation, Inc.,
59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

**********************************************************************/

#ifndef _PARSEOPTS_H
#define _PARSEOPTS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "macros.h"

#define FMODE_ASC 0
#define FMODE_BIN 1

#define PMODE_SUM 0
#define PMODE_VERIFY 1

#define VMODE_NORMAL 0
#define VMODE_SILENT 1

struct opts_t {
    int pmode;
    int fmode;
    int vmode;
    int filect;
    char **filenames;
    char **hashes;
};

struct opts_t *parseopts(int, char **);
#endif
