// vim:filetype=cpp
/** @file
  * @copyright University of Warsaw
  * @section LICENSE
  * GPLv3+ (see the COPYING file or http://www.gnu.org/licenses/)
  * @brief timestepping routine for super droplets
  */

namespace libcloudphxx
{
  namespace lgrngn
  {
    template <typename real_t, int device>
    void particles_t<real_t, device>::step_sync(
      const opts_t<real_t> &opts,
      arrinfo_t<real_t> rhod_th,
      arrinfo_t<real_t> rhod_rv,
      const arrinfo_t<real_t> courant_x, // defaults to NULL-NULL pair (e.g. kinematic model)
      const arrinfo_t<real_t> courant_y, // defaults to NULL-NULL pair (e.g. kinematic model)
      const arrinfo_t<real_t> courant_z, // defaults to NULL-NULL pair (e.g. kinematic model)
      const arrinfo_t<real_t> rhod       // defaults to NULL-NULL pair (e.g. kinematic or boussinesq model)
    )
    {
      assert(!pimpl->should_now_run_async);

      // syncing in Eulerian fields (if not null)
      pimpl->sync(rhod_th,   pimpl->rhod_th);
      pimpl->sync(rhod_rv,   pimpl->rhod_rv);
      pimpl->sync(courant_x, pimpl->courant_x);
      pimpl->sync(courant_y, pimpl->courant_y);
      pimpl->sync(courant_z, pimpl->courant_z);
      pimpl->sync(rhod,      pimpl->rhod);

      // updating particle->cell look-up table
      // (before advection and sedimentation so that their order does not matter,
      if (opts.adve || opts.sedi)
        pimpl->hskpng_ijk();

      // updating Tpr look-up table (includes RH update)
      pimpl->hskpng_Tpr(); 

      // condensation/evaporation 
      if (opts.cond) 
      {
        for (int step = 0; step < opts.sstp_cond; ++step) 
        {   
          pimpl->cond(pimpl->opts_init.dt / opts.sstp_cond, opts.RH_max); 
          pimpl->hskpng_Tpr(); 
        } 

        // syncing out 
        pimpl->sync(pimpl->rhod_th, rhod_th);
        pimpl->sync(pimpl->rhod_rv, rhod_rv);
      }

      pimpl->should_now_run_async = true;
    }

    template <typename real_t, int device>
    void particles_t<real_t, device>::step_async(
      const opts_t<real_t> &opts
    ) {
      assert(pimpl->should_now_run_async);

      // changing droplet positions
      if (opts.adve) 
      {
        // advection 
        pimpl->adve(); 
        // boundary condition
        ; // TODO temporarily included in advection
      }

      // updating terminal velocities
      if (opts.sedi || opts.coal) 
        pimpl->hskpng_vterm_all();

      if (opts.sedi) 
      {
        // advection with terminal velocity
        pimpl->sedi();

        // recycling out-of-domain particles (due to precipitation)
        //if (opts.rcyc) assert(false && "unimplemented"), throw; // TODO
      }

      // chemistry
      //if (opts.chem) assert(false && "unimplemented"), throw; // TODO

      // coalescence (before diagnostics -> one sort less)
      if (opts.coal) 
      {
        for (int step = 0; step < opts.sstp_coal; ++step) 
        {
          // collide
          pimpl->coal(pimpl->opts_init.dt / opts.sstp_coal);

          // update invalid vterm 
          if (step + 1 != opts.sstp_coal)
            pimpl->hskpng_vterm_invalid(); 
        }
      }

      pimpl->should_now_run_async = false;
    }
  };
};
