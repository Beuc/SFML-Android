SRC     = $(wildcard *.cpp ./Android/*.cpp ./EGL/*.cpp)
OBJ     = $(SRC:.cpp=.o)
OBJGLEW = $(SRCGLEW:.c=.o)

ifeq ($(STATIC), yes)
    LIB     = libsfml-window-s.a
    LIBNAME = $(LIBPATH)/$(LIB)
    INSTALL = 
else
    LIB     = libsfml-window.so
    LIBNAME = $(LIBPATH)/$(LIB).$(VERSION)
    INSTALL = && $(LN) $(LNFLAGS) $(LIB).$(VERSION) $(DESTLIBDIR)/$(LIB)
endif

all: $(LIB)

libsfml-window-s.a: $(OBJ)
	$(AR) $(ARFLAGS) $(LIBNAME) $(OBJ)

libsfml-window.so: $(OBJ)
	$(CPP) $(LDFLAGS) -Wl,-soname,$(LIB).$(VERSION) -o $(LIBNAME) $(OBJ) $(OBJGLEW) -lEGL

$(OBJ): %.o: %.cpp
	$(CPP) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf $(OBJ)

mrproper: clean
	@rm -rf $(LIBNAME)

install:
	@($(CP) $(LIBNAME) $(DESTLIBDIR) $(INSTALL))
