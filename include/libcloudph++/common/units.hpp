/** @file
  * @copyright University of Warsaw
  * @section LICENSE
  * GPLv3+ (see the COPYING file or http://www.gnu.org/licenses/)
  * @brief Common include directives and using statements for Boost.units
  */

#pragma once

#if false //!defined(NVCC)

#  include <boost/units/systems/si.hpp>
#  include <boost/units/cmath.hpp>
#  include <boost/units/io.hpp>
   namespace si = boost::units::si;
   using boost::units::quantity;
   using boost::units::one;
   using boost::units::pow;
   using boost::units::root;
   using boost::units::multiply_typeof_helper;
   using boost::units::divide_typeof_helper;
   using boost::units::power_typeof_helper;
   using boost::units::static_rational;

#  include <boost/math/constants/constants.hpp>
   using boost::math::constants::pi;

#else
#  include <libcloudph++/common/detail/fake_units.hpp>  
   namespace si = libcloudphxx::common::detail::fake_units::si;
   using libcloudphxx::common::detail::fake_units::quantity;
   using libcloudphxx::common::detail::fake_units::divide_typeof_helper;
   using libcloudphxx::common::detail::fake_units::multiply_typeof_helper;
#endif
