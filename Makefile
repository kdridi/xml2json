RESWRAP		= /usr/local/bin/reswrap
PKGCONFIG	= /usr/local/bin/pkg-config

APIFILES	= $(wildcard api*.c)
XSLFILES	= $(wildcard *.xsl)
XSLSOURCES	= $(XSLFILES:.xsl=.c)
XSLHEADERS	= $(XSLFILES:.xsl=.h)
EXEFILES	= xml2json.c opml2json.c block.c
OBJS		= $(XSLSOURCES:.c=.o) $(APIFILES:.c=.o)

CC			= gcc
CFLAGS		= -O0 -g -Wall -fmessage-length=0 $(shell $(PKGCONFIG) --cflags json libxml-2.0 libxslt libcurl)
LIBS		= $(shell $(PKGCONFIG) --libs json libxml-2.0 libxslt libcurl)
TARGETS		= $(EXEFILES:.c=)

all: $(XSLHEADERS) $(OBJS) $(EXEFILES:.c=.o)
	$(CC) -o     block     block.o $(OBJS) $(LIBS)
	$(CC) -o  xml2json  xml2json.o $(OBJS) $(LIBS)
	$(CC) -o opml2json opml2json.o $(OBJS) $(LIBS)

clean:
	rm -f $(TARGETS) $(OBJS) $(EXEFILES:.c=.o)
	rm -f $(XSLSOURCES) $(XSLHEADERS)

%.h: %.xsl
	$(RESWRAP) -ta -s -z -i -o $@ $<

%.c: %.xsl %.h
	$(RESWRAP) -ta -s -z    -o $@ $<

%: %.o $(OBJS)
	$(CC) -o $@ $? $(OBJS) $(LIBS)