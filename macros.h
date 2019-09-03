/**********************************************************************
$Id: macros.h,v 1.1 2004/08/17 15:02:48 bu Exp $

Copyright 2004, Microbrew Software

by Bulent Yilmaz <bu@microbrew.org>

Microbrew Message Center is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

Microbrew Message Center is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
Microbrew Message Center; if not, write to the Free Software Foundation,
Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

**********************************************************************/

#ifndef __MACROS_H
#define __MACROS_H

/* Required to get strerror(errno) to work correctly */
#include <errno.h>

/* My favourite macro ever */
#define NNFREE(x) if((x) != NULL) free(x)

/* Some macros to make nice error output */
#define eprintf(fmt,args...) fprintf(stderr, __FILE__ ":%d: " fmt, __LINE__, args)
#define eputs(fmt) fprintf(stderr, __FILE__ ":%d: " fmt "\n", __LINE__)

#define perrprintf(fmt,args...) fprintf(stderr, __FILE__ ":%d: " fmt ": %s\n", __LINE__, args, strerror(errno))
#define perrputs(fmt) fprintf(stderr, __FILE__ ":%d: " fmt ": %s\n", __LINE__, strerror(errno))

#endif
