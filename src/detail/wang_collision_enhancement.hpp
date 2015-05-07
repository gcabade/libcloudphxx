#pragma once

namespace libcloudphxx
{
  namespace lgrngn
  {
    namespace detail
    {
      // calculate collision efficiency enhancement due to turbulence
      // cf. Wang et al. (2009) atoms. sci. let. table 1
      template<class real_t, class n_t>
      BOOST_GPU_ENABLED
      real_t wang_collision_enhancement(const real_t &r1, const real_t &r2, const real_t &eps) // radii in meters, dissipation rate in m^2/s^3
      {
        // table definition
        const real_t R0[] = {10e-6, 20e-6, 30e-6, 40e-6, 50e-6, 60e-6, 100e-6};  // collector radius
        const real_t rat[] = {0., .1, .2, .3, .4, .5, .6, .7, .8, .9, 1.};       // ratio of radii

        const real_t eta_e[11][2][7] = {                                         // [rat][eps][R0] 
          1.74	1.74	1.773	1.49	1.207	1.207	1.0
          4.976	4.976	3.593	2.519	1.445	1.445	1.0
          1.46	1.46	1.421	1.245	1.069	1.069	1.0
          2.984	2.984	2.181	1.691	1.201	1.201	1.0
          1.32	1.32	1.245	1.123	1.000	1.000	1.0
          1.988	1.988	1.475	1.313	1.150	1.150	1.0
          1.250	1.250	1.148	1.087	1.025	1.025	1.0
          1.490	1.490	1.187	1.156	1.126	1.126	1.0
          1.186	1.186	1.066	1.060	1.056	1.056	1.0
          1.249	1.249	1.088	1.090	1.092	1.092	1.0
          1.045	1.045	1.000	1.014	1.028	1.028	1.0
          1.139	1.139	1.130	1.091	1.051	1.051	1.0
          1.070	1.070	1.030	1.038	1.046	1.046	1.0
          1.220	1.220	1.190	1.138	1.086	1.086	1.0
          1.000	1.000	1.054	1.042	1.029	1.029	1.0
          1.325	1.325	1.267	1.165	1.063	1.063	1.0
          1.223	1.223	1.117	1.069	1.021	1.021	1.0
          1.716	1.716	1.345	1.223	1.100	1.100	1.0
          1.570	1.570	1.244	1.166	1.088	1.088	1.0
          3.788	3.788	1.501	1.311	1.120	1.120	1.0
          20.3	20.3	14.6	8.61	2.60	2.60	1.0
          36.52	36.52	19.16	22.80	26.0	26.0	1.0
        };

        // find respective array positions
        real_t r, R;
        r1 > r2 ? R = r1, r = r2 : R = r2, r = r1;   // R > r
        if(R > 100e-6) return 1.;                    // no enhancement for large collector droplets

        int n_eps, n_R0, n_rat;

        eps <= 2.5e-2 ? n_eps = 0 : n_eps = 1;       // TODO: interpolate in epsilon space? cf. Benmoshe et al. 2012

        for(n_R0 = 0; n_R0 < 7; ++n_R0)
          if(R0[n_R0] > R) break;
      
        real_t ratio = r / R; 
        for(n_rat = 1; n_rat < 11; ++n_rat)
          if(rat[n_rat] > ratio) break;

        if(n_R0 == 0) return eta_e[n_rat][n_eps][n_R0]; // do not interpolate if both are smaller than 10um

        // two-dimensional linear interpolation
        // note: stagnant efficiency is also interpolated, hence we do it twice
        // TODO: prepare an already interpolated table of enhancement factors
        //       with exactly same layout as table of efficiencies, then
        //       first apply enhancement, interpolate afterward
        
      }
    };
  };
};
