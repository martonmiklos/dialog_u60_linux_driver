CC?=gcc
APP=lonifd

$(APP): lonifd.c
	$(CC) -o $(APP) -g -O0 lonifd.c

all: $(APP)

install: $(APP)
	install -d $(TARGET)/usr/bin/
	install $(APP) $(TARGET)/usr/bin/

clean:
	rm -f $(APP)
