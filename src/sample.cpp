#include <pficommon/lang/shared_ptr.h>
#include <pficommon/lang/bind.h>

#include "factory.hpp"
#include "generator.hpp"

class nearest_neighbor {
 public:
  typedef pfi::lang::shared_ptr<nearest_neighbor> ptr;

  virtual ~nearest_neighbor() {}
};

class classifier {
 public:
  typedef pfi::lang::shared_ptr<classifier> ptr;

  virtual ~classifier() {}
};

class linear_classifier : public classifier {
 public:
  struct config {};

  explicit linear_classifier(const config& conf) {}
};

class knn_classifier : public classifier {
 public:
  struct config {};

  knn_classifier(const config& conf, nearest_neighbor::ptr nn) {}
};

class linear_classifier_factory : public factory<classifier> {
 public:
  pfi::lang::function<classifier::ptr()> make_generator(
      const pfi::text::json::json& config) const {
    linear_classifier::config conf;
    // set conf with config, and check errors
    return ::make_generator<linear_classifier>(conf);
  }
};

class knn_classifier_factory : public factory<classifier> {
 public:
  pfi::lang::function<classifier::ptr()> make_generator(
      const pfi::text::json::json& config) const {
    knn_classifier::config conf;
    // set conf with config, and check errors
    pfi::lang::function<nearest_neighbor::ptr()> gen_nn;
    // set nn generator with config and maybe factory manager (singleton?)
    return ::make_generator<knn_classifier, nearest_neighbor>(conf, gen_nn);
  }
};

int main() {
  factory_manager<classifier> fm;
  fm.register_factory(
      "linear",
      factory<classifier>::ptr(new linear_classifier_factory));
  fm.register_factory(
      "knn",
      factory<classifier>::ptr(new knn_classifier_factory));

  pfi::text::json::json config;
  fm.create("linear", config);
}
