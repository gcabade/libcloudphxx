#pragma once 

#include <cassert>
#include <memory>

namespace libcloudphxx
{
  namespace common
  {
    namespace prtcls
    {
      // to make inclusion of Thrust not neccesarry
      enum {cpp, omp, cuda};

      // to allow storing instances for multiple backends in one container/pointer
      template <typename real_t>
      class particles_proto // TODO: rename to any?
      {
        public: 
        virtual void func() { assert(false); }   
      };  

      // prototype of what's implemented in the .tpp file
      template <typename real_t, int thrust_device_system>
      class particles : public particles_proto<real_t>
      {
        // pimpl stuff
        struct detail;
        std::auto_ptr<detail> pimpl;
    
        // the public API
        public:  
        particles(real_t sd_conc_mean); // 0D ctor
      };

      // to be explicitely instantiated
      template <typename real_t>
      particles_proto<real_t> *factory(real_t sd_conc_mean); // 0D factory
    };
  };
};
