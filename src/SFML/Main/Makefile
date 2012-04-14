SRC = $(wildcard *.cpp ./android_native_app_glue/*.cpp)
OBJ = $(SRC:.cpp=.o)

ifeq ($(STATIC), yes)
    LIB     = libsfml-main-s.a
    LIBNAME = $(LIBPATH)/$(LIB)
    INSTALL = 
else
    LIB     = libsfml-main.so
    LIBNAME = $(LIBPATH)/$(LIB).$(VERSION)
    INSTALL = && $(LN) $(LNFLAGS) $(LIB).$(VERSION) $(DESTLIBDIR)/$(LIB)
endif

all: $(LIB)

libsfml-main-s.a: $(OBJ)
	$(AR) $(ARFLAGS) $(LIBNAME) $(OBJ)

libsfml-main.so: $(OBJ)
	$(CPP) $(LDFLAGS) -Wl,-soname,$(LIB).$(VERSION) -o $(LIBNAME) $(OBJ)

$(OBJ): %.o: %.cpp
	$(CPP) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf $(OBJ)

mrproper: clean
	@rm -rf $(LIBNAME)

install:
	@($(CP) $(LIBNAME) $(DESTLIBDIR) $(INSTALL))
