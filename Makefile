OBJS=raspi2jpeg.o
BIN=raspi2jpeg
DESTDIR=/usr/local/bin

CFLAGS+=-Wall -g -O3 $(shell pkg-config --cflags --libs MagickWand)
LDFLAGS+=-L/opt/vc/lib/ $(shell MagickWand-config --ldflags) -lbcm_host -lm

INCLUDES+=-I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux

all: $(BIN)

install: $(BIN)
	install -m 755 $(BIN) $(DESTDIR)/raspi2jpeg

%.o: %.c
	@rm -f $@ 
	$(CC) $(CFLAGS) $(INCLUDES) -g -c $< -o $@ -Wno-deprecated-declarations

$(BIN): $(OBJS)
	$(CC) -o $@ -Wl,--whole-archive $(OBJS) $(LDFLAGS) -Wl,--no-whole-archive -rdynamic

clean:
	@rm -f $(OBJS)
	@rm -f $(BIN)
