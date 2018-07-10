/*
 * This file is a part of TiledArray.
 * Copyright (C) 2015  Virginia Tech
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef TILEDARRAY_SPECIAL_KRONECKER_DELTA_H__INCLUDED
#define TILEDARRAY_SPECIAL_KRONECKER_DELTA_H__INCLUDED

#include <tuple>
#include <memory>

#include <tiledarray_fwd.h>

#include <TiledArray/madness.h>

// Array class
#include <TiledArray/tensor.h>
#include <TiledArray/tile.h>
#include <TiledArray/tile_op/tile_interface.h>

// Array policy classes
#include <TiledArray/policies/dense_policy.h>
#include <TiledArray/policies/sparse_policy.h>

/// *generalized* (asymmetric) Kronecker delta

/// *generalized* (asymmetric) Kronecker delta is a product of \c _N ordinary Kronecker deltas
/// Definition:
/// KroneckerDeltaTile(b,k) = (b==k) ? 1 : 0
/// KroneckerDeltaTile(b0,k0,b1,k1,b2,k2...bN,kN) = KroneckerDeltaTile(b0,k0) KroneckerDeltaTile(b1,k1) ...`KroneckerDeltaTile(bN,kN)
///
/// \note This is a stateful data tile. Meant to be generated by its (stateless) lazy generator, \c LazyKroneckerDeltaTile.
///
/// \tparam _N the number of ordinal Kronecker deltas in this product
template<unsigned _N = 1>
  class KroneckerDeltaTile {
    public:
      // Constants
      static constexpr unsigned N = _N;
      // Concept typedefs
      typedef TiledArray::Range range_type; // range type
      typedef int value_type; // Element type
      typedef value_type numeric_type; // The scalar type that is compatible with value_type
      typedef size_t size_type; // Size type

    private:
      range_type range_;
      bool empty_;

    public:

      /// default constructor makes an empty tile
      KroneckerDeltaTile() : empty_(true) {
      }

      /// Productive ctor 1
      KroneckerDeltaTile(const range_type& range) : range_(range), empty_(is_empty(range_)) {
      }

      /// copy constructor (= deep copy)
      KroneckerDeltaTile(const KroneckerDeltaTile&) = default;

      /// assignment
      KroneckerDeltaTile&
      operator=(const KroneckerDeltaTile& other) = default;

      /// clone = copy
      KroneckerDeltaTile
      clone() const {
        KroneckerDeltaTile result(*this);
        return result;
      }

      range_type
      range() const {
        return range_;
      }

      bool empty() const { return empty_; }

      /// MADNESS compliant serialization
      template<typename Archive>
      void
      serialize(Archive& ar) {
        std::cout << "KroneckerDelta::serialize not implemented by design!" << std::endl;
        abort(); // should never travel
      }

    private:
      /// @return false if contains any nonzeros
      static bool is_empty(const range_type& range) {
        bool empty = false;
        TA_ASSERT(range.rank() == 2*N);
        auto lobound = range.lobound_data();
        auto upbound = range.upbound_data();
        for(auto i=0; i!=2*N && not empty; i+=2)
          empty = (upbound[i] > lobound[i+1] && upbound[i+1] > lobound[i]) ? true : false; // assumes extents > 0
        return empty;
      }

  }; // class KroneckerDeltaTile

// these are to satisfy interfaces, but not needed, actually

// Sum of hyper diagonal elements
template <unsigned _N>
typename KroneckerDeltaTile<_N>::numeric_type
trace(const KroneckerDeltaTile<_N>& arg);
// foreach(i) result += arg[i]
template <unsigned _N>
typename KroneckerDeltaTile<_N>::numeric_type
sum(const KroneckerDeltaTile<_N>& arg);
// foreach(i) result *= arg[i]
template <unsigned _N>
typename KroneckerDeltaTile<_N>::numeric_type
product(const KroneckerDeltaTile<_N>& arg);
// foreach(i) result += arg[i] * arg[i]
template <unsigned _N>
typename KroneckerDeltaTile<_N>::numeric_type
squared_norm(const KroneckerDeltaTile<_N>& arg);
// foreach(i) result = min(result, arg[i])
template <unsigned _N>
typename KroneckerDeltaTile<_N>::numeric_type
min(const KroneckerDeltaTile<_N>& arg);
// foreach(i) result = max(result, arg[i])
template <unsigned _N>
typename KroneckerDeltaTile<_N>::numeric_type
max(const KroneckerDeltaTile<_N>& arg);
// foreach(i) result = abs_min(result, arg[i])
template <unsigned _N>
typename KroneckerDeltaTile<_N>::numeric_type
abs_min(const KroneckerDeltaTile<_N>& arg);
// foreach(i) result = abs_max(result, arg[i])
template <unsigned _N>
typename KroneckerDeltaTile<_N>::numeric_type
abs_max(const KroneckerDeltaTile<_N>& arg);

// Permutation operation

// returns a tile for which result[perm ^ i] = tile[i]
template <unsigned N>
KroneckerDeltaTile<N> permute(const KroneckerDeltaTile<N>& tile,
                              const TiledArray::Permutation& perm) {
  abort();
}

// dense_result[i] = dense_arg1[i] * sparse_arg2[i]
template<typename T, unsigned _N>
  TiledArray::Tensor<T>
  mult (const KroneckerDeltaTile<_N>& arg1,
        const TiledArray::Tensor<T>& arg2) {
  abort();
}
// dense_result[perm ^ i] = dense_arg1[i] * sparse_arg2[i]
template<typename T, unsigned _N>
  TiledArray::Tensor<T>
  mult (const KroneckerDeltaTile<_N>& arg1,
        const TiledArray::Tensor<T>& arg2,
        const Permutation& perm) {
  abort();
}

// dense_result[i] *= sparse_arg1[i]
template<typename T, unsigned N>
  TiledArray::Tensor<T>&
  mult_to (TiledArray::Tensor<T>& result,
           const KroneckerDeltaTile<N>& arg1) {
    abort();
    return result;
  }

// Contraction operation

// GEMM operation with fused indices as defined by gemm_config:
// dense_result[i,j] = dense_arg1[i,k] * sparse_arg2[k,j]
template<typename T, unsigned N>
  TiledArray::Tensor<T>
  gemm (
      const KroneckerDeltaTile<N>& arg1,
      const TiledArray::Tensor<T>& arg2,
      const typename TiledArray::Tensor<T>::numeric_type factor,
      const TiledArray::math::GemmHelper& gemm_config) {

  // preconditions:
  // 1. implemented only outer product
  assert(gemm_config.result_rank() == gemm_config.left_rank() + gemm_config.right_rank());

  auto arg1_range = arg1.range();
  auto arg2_range = arg2.range();
  auto result_range = gemm_config.make_result_range<TiledArray::Range> (
      arg1_range, arg2_range);
  TiledArray::Tensor<T> result (result_range, 0);

  auto result_data = result.data();
  auto arg1_extents = arg1_range.extent_data();
  auto arg2_data = arg2.data();
  auto arg2_volume = arg2_range.volume();

  if (not arg1.empty ()) {
    switch (N) {
      case 1: {
        auto i0_range = std::min (arg1_extents[0], arg1_extents[1]);
        for (decltype(i0_range) i0 = 0; i0 != i0_range; ++i0) {
          auto result_i0i0_ptr = result_data
              + (i0 * arg1_extents[1] + i0) * arg2_volume;
          std::copy (arg2_data, arg2_data + arg2_volume, result_i0i0_ptr);
        }
      }
        break;
      case 2: {
        auto i0_range = std::min (arg1_extents[0], arg1_extents[1]);
        auto i1_range = std::min (arg1_extents[2], arg1_extents[3]);
        auto ndim23 = arg1_extents[2] * arg1_extents[3];
        for (decltype(i0_range) i0 = 0; i0 != i0_range; ++i0) {
          auto result_i0i0i1i1_ptr_offset = result_data
              + (i0 * arg1_extents[1] + i0) * ndim23 * arg2_volume;
          for (decltype(i1_range) i1 = 0; i1 != i1_range; ++i1) {
            auto result_i0i0i1i1_ptr = result_i0i0i1i1_ptr_offset
                + (i1 * arg1_extents[3] + i1) * arg2_volume;
            std::copy (arg2_data, arg2_data + arg2_volume, result_i0i0i1i1_ptr);
          }
        }
      }
        break;

      default:
        abort (); // not implemented
    }
  }

  return result;
}
// GEMM operation with fused indices as defined by gemm_config:
// dense_result[i,j] += dense_arg1[i,k] * sparse_arg2[k,j]
template<typename T, unsigned N>
  void
  gemm (
      TiledArray::Tensor<T>& result,
      const KroneckerDeltaTile<N>& arg1,
      const TiledArray::Tensor<T>& arg2,
      const typename TiledArray::Tensor<T>::numeric_type factor,
      const TiledArray::math::GemmHelper& gemm_config) {
  abort();
  }

#endif // TILEDARRAY_TEST_SPARSE_TILE_H__INCLUDED
