#ifndef CAFES_PARTICLE_SINGULARITY_ADD_SINGULARITY_HPP_INCLUDED
#define CAFES_PARTICLE_SINGULARITY_ADD_SINGULARITY_HPP_INCLUDED

#include <particle/particle.hpp>
#include <particle/singularity/singularity.hpp>
#include <particle/geometry/box.hpp>
#include <petsc/vec.hpp>

#include <petsc.h>
#include <iostream>

namespace cafes
{
  namespace singularity
  {
    #undef __FUNCT__
    #define __FUNCT__ "computesingularST"
    template<typename Shape>
    PetscErrorCode computesingularST(singularity<2, Shape> sing, 
                                     particle<Shape> const& p1, particle<Shape> const& p2,
                                     petsc::petsc_vec<2> sol,
                                     geometry::box<2, int> box,
                                     std::array<double, 2> const& h)
    {
      PetscErrorCode ierr;
      PetscFunctionBeginUser;

      const int Dimensions = 2;
      using position_type = geometry::position<Dimensions, double>;
      using position_type_i = geometry::position<Dimensions, int>;

      std::array<double, Dimensions> hs = {{h[0]/sing.scale, h[1]/sing.scale}};
      double coef = 1./(sing.scale*sing.scale);

      for(std::size_t j=box.bottom_left[1]; j<box.upper_right[1]; ++j)
      {
        for(std::size_t i=box.bottom_left[0]; i<box.upper_right[0]; ++i)
        {
          position_type_i pts_i = {i, j};
          auto ielem = fem::get_element(pts_i);

          for(std::size_t js=0; js<sing.scale; ++js)
          {
            for(std::size_t is=0; is<sing.scale; ++is){
              position_type pts = {i*h[0] + is*hs[0], j*h[1] + js*hs[1]};

              if (!p1.contains(pts) && !p2.contains(pts))
              {
                position_type pts_loc = {is*hs[0], js*hs[1]};
                
                auto bfunc = fem::P1_integration_grad(pts_loc, h);

                auto gradUsing = sing.get_grad_u_sing(pts);
                auto psing = sing.get_p_sing(pts);
                
                for (std::size_t j=0; j<bfunc.size(); ++j){
                  auto u = sol.at(ielem[j]);

                  for (std::size_t d1=0; d1<Dimensions; ++d1)
                  {
                    for (std::size_t d2=0; d2<Dimensions; ++d2)
                      u[d1] += coef*gradUsing[d1][d2]*bfunc[j][d2];
                    u[d1] -= coef*psing*bfunc[j][d1];
                  }
                }
              }
            }
          }
        }
      }
      PetscFunctionReturn(0);
    }

    #undef __FUNCT__
    #define __FUNCT__ "computesingularST"
    template<typename Shape>
    PetscErrorCode computesingularST(singularity<3, Shape> sing, 
                                     particle<Shape> const& p1, particle<Shape> const& p2,
                                     petsc::petsc_vec<3> sol,
                                     geometry::box<3, int> box,
                                     std::array<double, 3> const& h)
    {
      PetscErrorCode ierr;
      PetscFunctionBeginUser;

      const int Dimensions = 3;
      using position_type = geometry::position<Dimensions, double>;
      using position_type_i = geometry::position<Dimensions, int>;

      std::array<double, Dimensions> hs = {{ h[0]/sing.scale, 
                                             h[1]/sing.scale,
                                             h[2]/sing.scale }};

      double coef = 1./(sing.scale*sing.scale*sing.scale);

      for(std::size_t k=box.bottom_left[2]; k<box.upper_right[2]; ++k)
      {
        for(std::size_t j=box.bottom_left[1]; j<box.upper_right[1]; ++j)
        {
          for(std::size_t i=box.bottom_left[0]; i<box.upper_right[0]; ++i)
          {
            position_type_i pts_i = {i, j, k};
            auto ielem = fem::get_element(pts_i);

            for(std::size_t ks=0; ks<sing.scale; ++ks)
            {
              for(std::size_t js=0; js<sing.scale; ++js)
              {
                for(std::size_t is=0; is<sing.scale; ++is){
                  position_type pts = {i*h[0] + is*hs[0], 
                                       j*h[1] + js*hs[1],
                                       k*h[2] + ks*hs[2] };

                  if (!p1.contains(pts) && !p2.contains(pts))
                  {
                    position_type pts_loc = {is*hs[0], 
                                             js*hs[1],
                                             ks*hs[2]};
                    
                    auto bfunc = fem::P1_integration_grad(pts_loc, h);

                    auto gradUsing = sing.get_grad_u_sing(pts);
                    auto psing = sing.get_p_sing(pts);
                    
                    for (std::size_t j=0; j<bfunc.size(); ++j){
                      auto u = sol.at(ielem[j]);

                      for (std::size_t d1=0; d1<Dimensions; ++d1){
                        for (std::size_t d2=0; d2<Dimensions; ++d2)
                          u[d1] += coef*gradUsing[d1][d2]*bfunc[j][d2];
                        u[d1] -= coef*psing*bfunc[j][d1];
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      PetscFunctionReturn(0);
    }

    // #undef __FUNCT__
    // #define __FUNCT__ "computesingularBC"
    // template<typename Shape>
    // PetscErrorCode computesingularBC(singularity<2, Shape> sing, 
    //                                  particle<Shape> const& p1, particle<Shape> const& p2,
    //                                  petsc::petsc_vec<2> g,
    //                                  geometry::box<2, int> box,
    //                                  std::array<double, 2> const& h)
    // {
    //   PetscErrorCode ierr;
    //   PetscFunctionBeginUser;

    //   const int Dimensions = 2;
    //   using position_type = geometry::position<Dimensions, double>;
    //   using position_type_i = geometry::position<Dimensions, int>;

    //   for(std::size_t i=0; i<ctx.surf_points[ipart_1].size(); ++i)
    //   {



    //   std::array<double, Dimensions> hs = {{h[0]/sing.scale, h[1]/sing.scale}};
    //   double coef = 1./(sing.scale*sing.scale);

    //   for(std::size_t j=box.bottom_left[1]; j<box.upper_right[1]; ++j)
    //   {
    //     for(std::size_t i=box.bottom_left[0]; i<box.upper_right[0]; ++i)
    //     {
    //       position_type_i pts_i = {i, j};

    //       for(std::size_t js=0; js<sing.scale; ++js)
    //       {
    //         for(std::size_t is=0; is<sing.scale; ++is){
    //           position_type pts = {i*h[0] + is*hs[0], j*h[1] + js*hs[1]};

    //           if (!p1.contains(pts) && !p2.contains(pts))
    //           {
    //             position_type pts_loc = {is*hs[0], js*hs[1]};
                
    //             auto gradUsing = sing.get_grad_u_sing(pts);
    //             auto bfunc = fem::P1_integration_decomp(pts_loc, h);
    //             auto ielem = fem::get_element(pts_i);
    //             auto psing = sing.get_p_sing(pts);
                
    //             for (std::size_t j=0; j<bfunc.size(); ++j){
    //               auto u = sol.at(ielem[j]);

    //               for (std::size_t d1=0; d1<Dimensions; ++d1)
    //               {
    //                 for (std::size_t d2=0; d2<Dimensions; ++d2)
    //                   u[d1] += coef*gradUsing[d1][d2]*bfunc[j][d2];
    //                 u[d1] -= coef*psing*gradUsing[d1][d1];
    //               }
    //             }
    //           }
    //         }
    //       }
    //     }
    //   }
    //   PetscFunctionReturn(0);
    // }

    #undef __FUNCT__
    #define __FUNCT__ "add_singularity"
    template<std::size_t Dimensions, typename Ctx>
    PetscErrorCode add_singularity(Ctx& ctx, bool compute_stokes, bool compute_bc, bool compute_forces)
    {
      PetscErrorCode ierr;
      PetscFunctionBeginUser;

      auto overlap_box_func = geometry::overlap_box<Dimensions, int>;

      auto box = fem::get_DM_bounds<Dimensions>(ctx.problem.ctx->dm, 0);
      auto& h = ctx.problem.ctx->h;

      auto sol = petsc::petsc_vec<Dimensions>(ctx.problem.ctx->dm, ctx.problem.sol, 0);
      ierr = sol.fill(0.);CHKERRQ(ierr);

      //Loop on particles couples
      for (std::size_t ipart=0; ipart<ctx.particles.size()-1; ++ipart)
      {
        auto p1 = ctx.particles[ipart];
        for (std::size_t jpart=ipart+1; jpart<ctx.particles.size(); ++jpart)
        {
          auto p2 = ctx.particles[jpart];

          using shape_type = typename decltype(p1)::shape_type;
          auto sing = singularity<Dimensions, shape_type>(p1, p2);

          if (sing.is_singularity_)
          {
            auto pbox = overlap_box_func({(p1.center_ - sing.cutoff_dist_)/h, (p1.center_ + sing.cutoff_dist_)/h},
                                         {(p2.center_ - sing.cutoff_dist_)/h, (p2.center_ + sing.cutoff_dist_)/h});

            if (geometry::intersect(box, pbox))
            {
              auto new_box = geometry::box_inside(box, pbox);

              if (compute_stokes)
              {
                ierr = computesingularST(sing, p1, p2, sol, new_box, h);CHKERRQ(ierr);
              }

              // if (compute_bc)
              // {
              //   ierr = computesingularBC(sing, p1, p2, sol, new_box, h);CHKERRQ(ierr);
              // }

              // if (compute_forces)
              // {
              //   ierr = computesingularForces(sing, p1, p2, sol, new_box, h);CHKERRQ(ierr);
              // }
            }
          }
        }
      }

      ierr = sol.local_to_global(ADD_VALUES);CHKERRQ(ierr);

      PetscFunctionReturn(0);
    }
  }
}

#endif