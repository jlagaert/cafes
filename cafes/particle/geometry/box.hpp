// Copyright (c) 2016, Loic Gouarin <loic.gouarin@math.u-psud.fr>
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, 
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software without
//    specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.

#ifndef PARTICLE_GEOMETRY_BOX_HPP_INCLUDED
#define PARTICLE_GEOMETRY_BOX_HPP_INCLUDED

#include <particle/geometry/position.hpp>
#include <iostream>
#include <vector>

namespace cafes
{
  namespace geometry
  {
    /**
    * A box is a rectangular parallelepiped in 2D or 3D.
    * It is defined by its corners 
    * 
    * - bottom left
    * - upper right
    *
    * These corners must be position type of any type T.
    */
    template<typename T, std::size_t Dimensions>
    struct box
    {
      using position_type = position<T, Dimensions>;
      static constexpr std::size_t dimensions = Dimensions;

      position_type bottom_left; //!< position of the bottom left corner of the box
      position_type upper_right; //!< position of the upper right corner of the box

      /** 
      * the length of the box in the direction i
      * 
      * \param i the direction 
      */
      double length(std::size_t i) const { return std::abs(upper_right[i] - bottom_left[i]); }
      
      /**
      * the product of the lengths of the box
      */ 
      double length() const
      {
          auto result=1.;
          for(std::size_t i=0; i<Dimensions; ++i)
              result *= length(i);
          return result;
      }

      friend std::ostream &operator<<( std::ostream &output, 
                                       box const& b )
      { 
        output << "box : ( ";

        for(std::size_t i=0; i<Dimensions; ++i)
          output << b.bottom_left[i] << " ";

        output << "), ( ";

        for(std::size_t i=0; i<Dimensions; ++i)
          output << b.upper_right[i] << " ";

        output << ")";

        return output;            
      }

      auto get_box_points(std::integral_constant<int, 2> const&) const
      {
        std::vector<position_type> p;
        position_type x{bottom_left[0], upper_right[0]};
        position_type y{bottom_left[1], upper_right[1]};

        for(std::size_t d2=0; d2<2; ++d2)
          for(std::size_t d1=0; d1<2; ++d1)
            p.push_back({x[d1], y[d2]});
        return p;
      }

      auto get_box_points(std::integral_constant<int, 3> const&) const
      {
        std::vector<position_type> p;
        position_type x{bottom_left[0], upper_right[0]};
        position_type y{bottom_left[1], upper_right[1]};
        position_type z{bottom_left[2], upper_right[2]};

        for(std::size_t d3=0; d3<2; ++d3)
          for(std::size_t d2=0; d2<2; ++d2)
            for(std::size_t d1=0; d1<2; ++d1)
              p.push_back({x[d1], y[d2], z[d3]});

        return p;
      }

      /**
      * return the points of the box
      *
      * The output format for 2D problem is
      *
      *  \f{equation}{
      *  coords = [
      *  (x_{bl}, y_{bl}), 
      *  (x_{ur}, y_{bl}), 
      *  (x_{bl}, y_{ur}), 
      *  (x_{ur}, y_{ur}) 
      *  ]
      *  \f}
      *
      * and for 3D problem
      *
      *  \f{equation}{
      *  coords = \left[
      *  \begin{array}{l}
      *  (x_{bl}, y_{bl}, z_{bl}), 
      *  (x_{ur}, y_{bl}, z_{bl}), 
      *  (x_{bl}, y_{ur}, z_{bl}), 
      *  (x_{ur}, y_{ur}, z_{bl}),\\
      *  (x_{bl}, y_{bl}, z_{ur}), 
      *  (x_{ur}, y_{bl}, z_{ur}), 
      *  (x_{bl}, y_{ur}, z_{ur}), 
      *  (x_{ur}, y_{ur}, z_{ur})
      *  \end{array}
      *  \right]
      *  \f}
      */
      auto get_box_points() const
      {
        return get_box_points(std::integral_constant<int, Dimensions>{});
      }

    };

    /**
    * return true if two boxes have a non null intersection, false otherwise.
    *
    *   \param b1 : the first box
    *   \param b2 : the second box
    *   \return a boolean
    */
    template<typename T, std::size_t Dimensions>
    bool intersect(box<T, Dimensions> const& b1, box<T, Dimensions> const& b2)
    {
      for(std::size_t i=0; i<Dimensions; ++i){
        if (b1.bottom_left[i] > b2.upper_right[i]) return false;
        if (b1.upper_right[i] < b2.bottom_left[i]) return false;
      }
      return true;
    }

    /**
    * return the overlapping box between two boxes.
    */
    template<typename T, std::size_t Dimensions>
    box<T, Dimensions> overlap_box(box<T, Dimensions> const& b1, box<T, Dimensions> const& b2)
    {
      box<T, Dimensions> bout{};
      for(std::size_t i=0; i<Dimensions; ++i){
        bout.bottom_left[i] = std::max( b1.bottom_left[i], b2.bottom_left[i]);
        bout.upper_right[i] = std::min( b1.upper_right[i], b2.upper_right[i]);
      }
      return bout;
    }

    /**
    * return the box included the two boxes.
    */
    template<typename T, std::size_t Dimensions>
    box<T, Dimensions> union_box(box<T, Dimensions> const& b1, box<T, Dimensions> const& b2)
    {
      box<T, Dimensions> bout{};
      for(std::size_t i=0; i<Dimensions; ++i){
        bout.bottom_left[i] = std::min( b1.bottom_left[i], b2.bottom_left[i]);
        bout.upper_right[i] = std::max( b1.upper_right[i], b2.upper_right[i]);
      }
      return bout;
    }

    template<typename T, std::size_t Dimensions>
    box<T, Dimensions> box_inside(box<T, Dimensions> const& b1, box<T, Dimensions> const& b2)
    {
      box<T, Dimensions> bout{b1};
      for(std::size_t i=0; i<Dimensions; ++i){
        if (b2.bottom_left[i] > b1.bottom_left[i])
          bout.bottom_left[i] = b2.bottom_left[i];
        if (b2.upper_right[i] < b1.upper_right[i])
          bout.upper_right[i] = b2.upper_right[i];
      }
      return bout;
    }

    /**
    * Check if a point is in the box.
    */
    template<typename T, std::size_t Dimensions>
    bool point_inside(box<T, Dimensions> const& b, position<T, Dimensions> p){
      for(std::size_t i=0; i<Dimensions; ++i)
        if (p[i] < b.bottom_left[i] || p[i] >= b.upper_right[i]) // don't take the last point (fix for the border domain)
          return false;
      return true;
    }

    /**
    * Check if a part of the box b2 is inside the box b1
    */
    template<typename T, std::size_t Dimensions>
    bool check_box_inside(box<T, Dimensions> const& b1, box<T, Dimensions> const& b2)
    // check if a part of box b2 is inside the box b1
    {
      int count = 0;
      for(std::size_t i=0; i<Dimensions; ++i){
        if (b2.bottom_left[i] >= b1.bottom_left[i] && b2.bottom_left[i] <= b1.upper_right[i])
          count++;
        if (b2.upper_right[i] >= b1.bottom_left[i] && b2.upper_right[i] <= b1.upper_right[i])
          count++;
      }
      return (count>0)?true:false;
    }

  }
}

#endif
