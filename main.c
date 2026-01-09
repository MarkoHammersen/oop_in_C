/* as per https://www.state-machine.com/doc/AN_Simple_OOP_in_C.pdf */

#include <stdint.h>
#include <assert.h>

#define PI 3.14159265358979323846
static double pi = PI;

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

typedef struct {
  Shape super; /* <== inherits Shape */
  /* attributes added by this subclass... */
  uint16_t radius;
} Circle;

/* Polymorphism */
struct ShapeVtbl {
  uint32_t (*area)(Shape * const me);
  void (*draw)(Shape * const me);
};

/* constructor */
static void Rectangle_ctor(Rectangle * const me, int16_t x, int16_t y, uint16_t width, uint16_t height);
static void Circle_ctor(Circle * const me, int16_t x, int16_t y, uint16_t radius);
static void Shape_ctor(Shape * const me, int16_t x, int16_t y);

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
/* Circle's class implementations of its virtual functions... */
static void Circle_draw_(Shape* const me);
static uint32_t Circle_area_(Shape * const me);

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

void Circle_ctor(Circle * const me, int16_t x, int16_t y,
  uint16_t radius)
{
  static struct ShapeVtbl const vtbl = { /* vtbl of the Rectangle class */
    &Circle_area_,
    &Circle_draw_
  };
  Shape_ctor(&me->super, x, y); /* call the superclass' ctor */
  me->super.vptr = &vtbl; /* override the vptr */
  me->radius = radius;
}

static inline uint32_t Shape_area(Shape * const me) {
 return (*me->vptr->area)(me);
}

static inline void Shape_draw(Shape * const me){
  (*me->vptr->draw)(me);
}

uint32_t Rectangle_area_(Shape * const me) {
  Rectangle * const me_ = (Rectangle *)me; /* explicit downcast */
  return (uint32_t)me_->width * (uint32_t)me_->height;
}

void Rectangle_draw_(Shape * const me) {
  
}

uint32_t Circle_area_(Shape * const me) {
  Circle * const me_ = (Circle *)me; /* explicit downcast */
  return (uint32_t)(pi * (double)me_->radius * (double)me_->radius);
}

void Circle_draw_(Shape * const me) {
  
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
static inline void Shape_moveBy(Shape * const me, int16_t dx, int16_t dy) {
  me->x += dx;
  me->y += dy;
}

int main (void) {
  Rectangle r1, r2;
  Circle c1, c2;

  /* instantiate rectangles... */
  Rectangle_ctor(&r1, 0, 2, 10, 15);
  Rectangle_ctor(&r2, -1, 3, 5, 8);

  /* instantiate circles... */ 
  Circle_ctor(&c1, 1, -1, 7);
  Circle_ctor(&c2, 4, 0, 12);

  /* re-use inherited function from the superclass Shape... */
  Shape_moveBy((Shape *)&r1, -2, 3);
  Shape_moveBy(&r2.super, 2, -1);
  Shape_moveBy((Shape*)&c1, 0, 4);
  Shape_moveBy(&c2.super, -3, -3);

  Shape_area((Shape*)&r1);
  Shape_area(&r2.super);
  Shape_area((Shape*)&c1);
  Shape_area(&c2.super);

  Shape_draw((Shape*)&r1);
  Shape_draw(&r2.super);
  Shape_draw((Shape*)&c1);
  Shape_draw(&c2.super);

  return 0;
}