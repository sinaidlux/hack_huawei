##############################################################################
# $Id: Makefile,v 1.3 2004/08/02 02:13:02 bu Exp $
#
# Copyright 2004, Bulent Yilmaz
# for Microbrew Software
#
# Part of Microbrew md5sum/sha1sum
# GNU textutils md5sum replacement
#
# http://www.microbrew.org/tools/md5sha1sum.html
#
# Microbrew md5sum/sha1sum is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# MicroSieve is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with MicroSieve; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
##############################################################################

include sys-setup.mk

CFLAGS+=-ggdb -Wall -O2

OBJS=parseopts.o md5sum.o

BINS=$(MD5BINNAME) $(SHA1BINNAME) $(RIPEMD160BINNAME)
all: $(BINS)

sys-setup.mk:
	@echo "Please run 'configure' first."
	@echo
	@/bin/false

md5sum.o: md5sum.c parseopts.h
parseopts.o: parseopts.c parseopts.h config.h

LDOBJS=md5sum.o parseopts.o
$(MD5BINNAME): $(LDOBJS)
	$(CC) $(LDOBJS) -o $(MD5BINNAME) $(LDFLAGS)

$(SHA1BINNAME) $(RIPEMD160BINNAME): $(MD5BINNAME)
	rm -f $@
	ln -s $(MD5BINNAME) $@

clean:
	rm -f *.o $(BINS)

distclean: clean
	rm sys-setup.mk

install: all
	cp $(MD5BINNAME) $(SHA1BINNAME) $(RIPEMD160BINNAME) $(BINDIR)
	chown bin:bin $(BINDIR)/$(MD5BINNAME) $(BINDIR)/$(SHA1BINNAME) \
              $(BINDIR)/$(RIPEMD160BINNAME)
	chmod 755 $(BINDIR)/$(MD5BINNAME)
