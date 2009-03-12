#include <shapetest.h>
#include <range1.h>
#include <range.h>
#include <shape.h>
#include <predicate.h>

using namespace TiledArray;

void ShapeTest() {

  typedef Range<4>::element_index::index eindex;
  typedef Range<4>::tile_index::index tindex;

  // Create a range for use with ShapeIterator.

  // Test with C-style Range Array constructor.
  eindex dim0[] = {0, 2, 4, 6};
  eindex dim1[] = {0, 2, 4, 6};
  eindex dim2[] = {0, 2, 4, 6};
  tindex tiles[3] = {2, 2, 2};

  Range1 rng_set[3] = {Range1(dim0, tiles[0]),
                       Range1(dim1, tiles[1]),
                       Range1(dim2, tiles[2]) };

  Range<3> rng(rng_set);

  std::cout << "Start ShapeIterator tests: " << std::endl;

  typedef PredShapeIterator<Range<3>::tile_iterator, DensePred<3> > DenseShape3_tile_iterator;
  typedef PredShapeIterator<Range<3>::element_iterator, DensePred<3> > DenseShape3_elem_iterator;
  typedef PredShapeIterator<Range<3>::tile_iterator, LowerTrianglePred<3> > LowerTriShape3_tile_iterator;


  std::cout << "Dense Predicate Iterator" << std::endl << "iterate over tiles:" << std::endl;

  DenseShape3_tile_iterator tile_it(rng.begin_tile(), rng.end_tile());
  for(; !(tile_it == rng.end_tile()); ++tile_it)
    std::cout << *tile_it << std::endl;

  std::cout << "iterate over elements:" << std::endl;

  DenseShape3_elem_iterator elem_it(rng.begin_element(), rng.end_element());
  for(; elem_it != rng.end_element(); ++elem_it)
    std::cout << *elem_it << std::endl;

  std::cout << "LowerTriangel Predicate Iterator" << std::endl << "iterator over tiles" << std::endl;

  LowerTriShape3_tile_iterator tri_tile_it(rng.begin_tile(), rng.end_tile());
  for(; tri_tile_it != rng.end_tile(); ++tri_tile_it)
    std::cout << *tri_tile_it << std::endl;

}
