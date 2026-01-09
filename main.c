/* as per https://www.state-machine.com/doc/AN_Simple_OOP_in_C.pdf */

#include <stdint.h>
#include <assert.h>

/* Shape's attributes... */
//typedef struct {
//  int16_t x; /* x-coordinate of Shape's position */
//  int16_t y; /* y-coordinate of Shape's position */
//} Shape;

/* Shape's attributes... */
struct ShapeVtbl; /* forward declaration */
typedef struct {
  struct ShapeVtbl const *vptr; /* <== Shape's Virtual Pointer */
  int16_t x; /* x-coordinate of Shape's position */
  int16_t y; /* y-coordinate of Shape's position */
} Shape;


typedef struct {
  Shape super; /* <== inherits Shape */
  /* attributes added by this subclass... */
  uint16_t width;
  uint16_t height;
} Rectangle;

/* constructor */
static void Rectangle_ctor(Rectangle * const me, int16_t x, int16_t y, uint16_t width, uint16_t height);

/* Shape's operations (Shape's interface)... */
static void Shape_ctor(Shape * const me, int16_t x, int16_t y);
static void Shape_moveBy(Shape * const me, int16_t dx, int16_t dy);

/* Polymorphism */
struct ShapeVtbl {
 uint32_t (*area)(Shape * const me);
 void (*draw)(Shape * const me);
 };

uint32_t Shape_area(Shape * const me);
void Shape_draw(Shape * const me);

/* Shape class implementation of its virtual functions... */
static uint32_t Shape_area_(Shape * const me);
static void Shape_draw_(Shape * const me);

/* Shape class implementations of its virtual functions... */
static uint32_t Shape_area_(Shape * const me) {
  assert(0); /* purely-virtual function should never be called */
  return 0U; /* to avoid compiler warnings */
}
static void Shape_draw_(Shape * const me) {
  assert(0); /* purely-virtual function should never be called */
}

/* Rectangle's class implementations of its virtual functions... */
static uint32_t Rectangle_area_(Shape * const me);
static void Rectangle_draw_(Shape * const me);
/* constructor */
void Rectangle_ctor(Rectangle * const me, int16_t x, int16_t y,
  uint16_t width, uint16_t height)
{
  static struct ShapeVtbl const vtbl = { /* vtbl of the Rectangle class */
    &Rectangle_area_,
    &Rectangle_draw_
  };
  Shape_ctor(&me->super, x, y); /* call the superclass' ctor */
  me->super.vptr = &vtbl; /* override the vptr */
  me->width = width;
  me->height = height;
}

static inline uint32_t Shape_area(Shape * const me) {
 return (*me->vptr->area)(me);
}

static uint32_t Rectangle_area_(Shape * const me) {
  Rectangle * const me_ = (Rectangle *)me; /* explicit downcast */
  return (uint32_t)me_->width * (uint32_t)me_->height;
}

static void Rectangle_draw_(Shape * const me) {
  
}

/* constructor */
void Shape_ctor(Shape * const me, int16_t x, int16_t y) {
  static struct ShapeVtbl const vtbl = { /* vtbl of the Shape class */
    &Shape_area_,
    &Shape_draw_
  };

  me->vptr = &vtbl; /* "hook" the vptr to the vtbl */
  me->x = x;
  me->y = y;
}

/* move-by operation */
void Shape_moveBy(Shape * const me, int16_t dx, int16_t dy) {
  me->x += dx;
  me->y += dy;
}

int main (void) {

  /* Test of Encapsulation */
  //Shape s1, s2; /* multiple instances of Shape */
  //Shape_ctor(&s1, 0, 1);
  //Shape_ctor(&s2, -1, 2);
  //Shape_moveBy(&s1, 2, -4);

  /* Test of Inheritance */
  Rectangle r1, r2;
 /* instantiate rectangles... */
 Rectangle_ctor(&r1, 0, 2, 10, 15);
 Rectangle_ctor(&r2, -1, 3, 5, 8);
 /* re-use inherited function from the superclass Shape... */
 Shape_moveBy((Shape *)&r1, -2, 3);
 Shape_moveBy(&r2.super, 2, -1);

 Shape_area((Shape*)&r1);

  return 0;
}