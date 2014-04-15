#ifndef GENERATOR_HPP_40A6F75A_1F48_4B94_AA8B_643B8F687513_
#define GENERATOR_HPP_40A6F75A_1F48_4B94_AA8B_643B8F687513_

#include <pficommon/lang/function.h>
#include <pficommon/lang/shared_ptr.h>
#include <pficommon/lang/bind.h>

template <typename T>
typename T::ptr generate(const typename T::config& conf) {
  return typename T::ptr(new T(conf));
}

template <typename T, typename A1>
typename T::ptr generate(
    const typename T::config& conf,
    pfi::lang::function<typename A1::ptr()> gen1) {
  return typename T::ptr(new T(conf, gen1()));
}

template <typename T>
pfi::lang::function<typename T::ptr()> make_generator(
    const typename T::config& conf) {
  return pfi::lang::bind(&generate<T>, conf);
}

template <typename T, typename A1>
pfi::lang::function<typename T::ptr()> make_generator(
    const typename T::config& conf,
    pfi::lang::function<typename A1::ptr()> gen1) {
  return pfi::lang::bind(&generate<T, A1>, conf, gen1);
}

#endif  // GENERATOR_HPP_40A6F75A_1F48_4B94_AA8B_643B8F687513_
