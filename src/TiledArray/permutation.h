/*
 *  This file is a part of TiledArray.
 *  Copyright (C) 2013  Virginia Tech
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
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

#ifndef TILEDARRAY_PERMUTATION_H__INCLUED
#define TILEDARRAY_PERMUTATION_H__INCLUED

#include <TiledArray/error.h>
#include <TiledArray/type_traits.h>
#include <TiledArray/utility.h>
#include <array>

namespace TiledArray {

  // weirdly necessary forward declarations
  class Permutation;
  bool operator==(const Permutation&, const Permutation&);
  std::ostream& operator<<(std::ostream&, const Permutation&);

  namespace detail {

    /// Place holder object to represent a no permutation operation.
    struct NoPermutation {
      const NoPermutation& operator-() const { return *this; }
      template <typename Archive>
      void serialize(Archive&) { }
    };

    /// Copies an iterator range into an array type container.

    /// Permutes iterator range  \c [first_o, \c last_o) base on the permutation
    /// \c [first_p, \c last_p) and places it in \c result. The result object
    /// must define operator[](std::size_t).
    /// \arg \c [first_p, \c last_p) is an iterator range to the permutation
    /// \arg \c [irst_o is an input iterator to the beginning of the original array
    /// \arg \c result is a random access iterator that will contain the resulting permuted array
    template <typename Perm, typename Arg, typename Result>
    inline void permute_array(const Perm& perm, const Arg& arg, Result& result) {
      TA_ASSERT(perm.dim() == size(arg));
      TA_ASSERT(perm.dim() == size(result));

      const unsigned int n = perm.dim();
      for(unsigned int i = 0u; i < n; ++i) {
        const typename Perm::index_type pi = perm[i];
        result[pi] = arg[i];
      }
    }
  } // namespace detail

  /// Permutation

  /// Permutation class is used as an argument in all permutation operations on
  /// other objects. Permutations are performed with the following syntax:
  ///
  ///   b = p ^ a; // assign permeation of a into b given the permutation p.
  ///   a ^= p;    // permute a given the permutation p.
  class Permutation {
  public:
    typedef Permutation Permutation_;
    typedef unsigned int index_type;
    typedef std::vector<index_type> Array;
    typedef std::vector<index_type>::const_iterator const_iterator;

  private:

    std::vector<index_type> p_; ///< The permutation indices

    /// Validate input permutation data

    /// \return \c false if each element of [first, last) is unique and less
    /// than the size of the list.
    template <typename InIter>
    bool valid_(InIter first, InIter last) {
      bool result = true;
      const unsigned int n = std::distance(first, last);
      for(; first != last; ++first) {
        const unsigned int value = *first;
        result = result && (value < n) && (std::count(first, last, *first) == 1ul);
      }
      return result;
    }

    // Used to select the correct constructor based on input template types
    struct Enabler { };

  public:

    Permutation() = default;
    Permutation(const Permutation&) = default;
    Permutation(Permutation&&) = default;
    ~Permutation() = default;
    Permutation& operator=(const Permutation&) = default;
    Permutation& operator=(Permutation&& other) = default;

    /// Construct permutation from an iterator

    /// \tparam InIter An input iterator type
    /// \param first The beginning of the iterator range
    /// \param last The end of the iterator range
    template <typename InIter,
        typename std::enable_if<detail::is_input_iterator<InIter>::value>::type* = nullptr>
    Permutation(InIter first, InIter last) :
        p_(first, last)
    {
      TA_ASSERT( valid_(p_.begin(), p_.end()) );
    }

    /// Array constructor

    /// Construct permutation from an Array
    /// \param a The permutation array to be moved
    explicit Permutation(const std::vector<index_type>& a) :
        p_(a)
    {
      TA_ASSERT( valid_(a.begin(), a.end()) );
    }

    /// std::vector move constructor

    /// Move the content of the std::vector into this permutation
    /// \param a The permutation array to be moved
    explicit Permutation(std::vector<index_type>&& a) :
        p_(std::move(a))
    {
      TA_ASSERT( valid_(p_.begin(), p_.end()) );
    }

    /// Construct permutation with an initializer list

    /// \param list An initializer list of integers
    explicit Permutation(std::initializer_list<index_type> list) :
        Permutation(list.begin(), list.end())
    { }


    unsigned int dim() const { return p_.size(); }

    const_iterator begin() const { return p_.begin(); }
    const_iterator end() const { return p_.end(); }

    index_type operator[](unsigned int i) const { return p_[i]; }

    /// return *this ^ other
    Permutation& operator*=(const Permutation& other) {
      TA_ASSERT(other.p_.size() == p_.size());

      std::vector<index_type> result(p_.size());
      detail::permute_array(other, p_, result);
      p_ = std::move(result);
      return *this;
    }

    /// Returns the inverse permutation and will satisfy the following conditions.
    /// given c2 = p ^ c1
    /// c1 == ((-p) ^ c2);
    Permutation operator -() const { return inverse(); }


    /// Bool conversion

    /// \return \c true if the permutation is not empty, otherwise \c false.
    operator bool() const { return ! p_.empty(); }

    /// Not operator

    /// \return \c true if the permutation is empty, otherwise \c false.
    bool operator!() const { return p_.empty(); }

    /// Identity permutation factory function

    /// \param dim The number of dimensions in the
    /// \return An identity permutation for \c dim elements
    static Permutation identity(const unsigned int dim) {
      std::vector<index_type> result;
      result.reserve(dim);
      for(unsigned int i = 0u; i < dim; ++i)
        result.emplace_back(i);
      return Permutation(std::move(result));
    }

    /// Make the inverse of this permutation

    /// given <tt>c2 = p ^ c1</tt>
    /// <tt>c1 == (p.inverse() ^ c2);</tt>
    Permutation inverse() const {
      const std::size_t n = p_.size();
      std::vector<index_type> result;
      result.resize(n, 0ul);
      for(std::size_t i = 0ul; i < n; ++i) {
        const std::size_t pi = p_[i];
        result[pi] = i;
      }
      return Permutation(std::move(result));
    }

    /// Return a reference to the array that represents the permutation.
    const std::vector<index_type>& data() const { return p_; }

    template <typename Archive>
    void serialize(Archive& ar) {
      ar & p_;
    }

  };

  inline bool operator==(const Permutation& p1, const Permutation& p2) {
    return (p1.dim() == p2.dim()) && std::equal(p1.data().begin(), p1.data().end(), p2.data().begin());
  }

  inline bool operator!=(const Permutation& p1, const Permutation& p2) {
    return ! operator==(p1, p2);
  }

  inline std::ostream& operator<<(std::ostream& output, const Permutation& p) {
    std::size_t n = p.dim();
    output << "{";
    for (unsigned int dim = 0; dim < n - 1; ++dim)
      output << dim << "->" << p.data()[dim] << ", ";
    output << n - 1 << "->" << p.data()[n - 1] << "}";
    return output;
  }

  /// permute a std::array
  template <typename T, std::size_t N>
  inline std::array<T,N> operator*(const Permutation& perm, const std::array<T, N>& orig) {
    TA_ASSERT(perm.dim() == orig.size());
    std::array<T,N> result;
    detail::permute_array(perm, orig, result);
    return result;
  }

  /// permute a std::vector<T>
  template <typename T, typename A>
  inline std::vector<T> operator*(const Permutation& perm, const std::vector<T, A>& orig) {
    std::vector<T> result(perm.dim());
    detail::permute_array(perm, orig, result);
    return result;
  }


  /// permute a std::vector<T>
  template <typename T>
  inline std::vector<T> operator*(const Permutation& perm, const T* restrict const orig) {
    const unsigned int n = perm.dim();
    std::vector<T> result(n);
    for(unsigned int i = 0u; i < n; ++i) {
      const typename Permutation::index_type perm_i = perm[i];
      const T orig_i = orig[i];
      result[perm_i] = orig_i;
    }
    return result;
  }

  inline Permutation operator*(const Permutation& perm, const Permutation& p) {
    TA_ASSERT(perm.dim() == p.dim());
    return Permutation(perm * p.data());
  }

  namespace detail {
    /// permute a std::array
    template <typename T, std::size_t N>
    inline const std::array<T,N>& operator*(const NoPermutation&, const std::array<T, N>& orig) {
      return orig;
    }

    /// permute a std::vector<T>
    template <typename T, typename A>
    inline const std::vector<T, A>& operator*(const NoPermutation&, const std::vector<T, A>& orig) {
      return orig;
    }

    inline const Permutation& operator*(const NoPermutation&, const Permutation& p) {
      return p;
    }

  } // namespace detail

  template <typename T, std::size_t N>
  inline std::array<T,N>& operator*=(std::array<T,N>& a, const Permutation& perm) {
    const std::array<T,N> temp = a;
    detail::permute_array(perm, temp, a);
    return a;
  }

  template <typename T, typename A>
  inline std::vector<T, A>& operator*=(std::vector<T, A>& orig, const Permutation& perm) {
    const std::vector<T, A> temp = orig;
    detail::permute_array(perm, temp, orig);
    return orig;
  }

} // namespace TiledArray

#endif // TILEDARRAY_PERMUTATION_H__INCLUED
