CFILES		= $(wildcard *.c)
XSLFILES	= $(wildcard *.xsl)
XSLSOURCES	= $(XSLFILES:.xsl=.c)
XSLHEADERS	= $(XSLFILES:.xsl=.h)
OBJS		= $(XSLSOURCES:.c=.o) $(CFILES:.c=.o)

CC			= gcc
CFLAGS		= -O0 -g -Wall -fmessage-length=0 $(shell pkg-config --cflags json libxml-2.0 libxslt libcurl)
LIBS		= $(shell pkg-config --libs json libxml-2.0 libxslt libcurl)
TARGET		= xml2json

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

all: $(TARGET) $(XSLHEADERS)

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(XSLSOURCES) $(XSLHEADERS)

%.h: %.xsl
	reswrap -ta -s -z -i -o $@ $<

%.c: %.xsl %.h
	reswrap -ta -s -z    -o $@ $<
	


	