#include <list>

#include "../utils/test.hpp"

#include "shape_impl.hpp"
#include "circle_impl.hpp"
#include "line_impl.hpp"
#include "square_impl.hpp"
#include "rectangle_impl.hpp"

static test_status_t CircleTest(void);
static test_status_t LineTest(void);
static test_status_t SquareTest(void);
static test_status_t RectangleTest(void);
static test_status_t ListPrintTest(void);
static void PrintAllShapes(std::list<ilrd::Shape*> list);


using namespace ilrd;

int main()
{
  RUNTEST(CircleTest);
  RUNTEST(LineTest);
  RUNTEST(SquareTest);
  RUNTEST(RectangleTest);
  RUNTEST(ListPrintTest);
}
static test_status_t CircleTest(void)
{
  Circle new_circle;
  std::cout <<'\n';
  new_circle.Draw();
  new_circle.Move(10);
  new_circle.Draw();
  new_circle.Move(20).Draw();
  return PASSED;

}
static test_status_t LineTest(void)
{
  Line new_line;
  std::cout << '\n';
  new_line.Draw();
  new_line.Move(10);
  new_line.Draw();
  return PASSED;
}
static test_status_t SquareTest(void)
{
  Square new_square;
  std::cout << '\n';
  new_square.Draw();
  new_square.Move(10);
  new_square.Draw();
  return PASSED;
}
static test_status_t RectangleTest(void)
{
  Rectangle new_rectangle;
  std::cout << '\n';
  new_rectangle.Draw();
  new_rectangle.Move(10);
  new_rectangle.Draw();
  return PASSED;
}
static test_status_t ListPrintTest(void)
{
  Circle new_circle;
  Line new_line;
  Square new_square;
  Rectangle new_rectangle;

  std::list<ilrd::Shape *> shapes_list;
  shapes_list.push_back(&new_circle);
  shapes_list.push_back(&new_line);
  shapes_list.push_back(&new_square);
  shapes_list.push_back(&new_rectangle);
  shapes_list.push_back(&new_line);
  
  std::cout <<'\n';
  PrintAllShapes(shapes_list);
  new_circle.Move(13);
  new_line.Move(15);
  new_rectangle.Move(10);
  new_square.Move(13);
  PrintAllShapes(shapes_list);
  return PASSED;
}

static void PrintAllShapes(std::list<Shape*> list)
{
  std::list<Shape *>::iterator runner = list.begin();
  while (runner != list.end())
  {
    (*runner++)->Draw();
  }
}
