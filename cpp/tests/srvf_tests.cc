#include <boost/test/unit_test.hpp>

#include "srvf.h"
#include "plf.h"
#include "matrix.h"
#include "util.h"

BOOST_AUTO_TEST_SUITE(srvf_tests)

BOOST_AUTO_TEST_CASE(evaluate_test1)
{
  double samps_data[] = 
  {
    1.2, -0.5, 0.98, 100.01, -98.03, 
    0.0,  3.8, -0.5, -55.33, 100.00
  };
  double params_data[] = {0.0, 0.1, 0.12, 0.65, 0.8, 1.0};
  double tv_data[] = {
    0.0, 0.01, 0.0999, 0.1, 0.11, 0.11999, 
    0.12, 0.649, 0.65, 0.799, 0.8, 0.99, 1.0
  };
  double exp[] = 
  {
    1.2, 1.2, 1.2, -0.5, -0.5, -0.5, 0.98, 0.98, 
    100.01, 100.01, -98.03, -98.03, -98.03, 
    0.0, 0.0, 0.0, 3.8, 3.8, 3.8, -0.5, -0.5, 
    -55.33, -55.33, 100.00, 100.00, 100.00
  };
  int ncp=sizeof(params_data)/sizeof(double);
  int ntv=sizeof(tv_data)/sizeof(double);
  srvf::Matrix samps(2,ncp-1,samps_data);
  srvf::Matrix params(1,ncp,params_data);
  srvf::Matrix tv(1,ntv,tv_data);
  srvf::Matrix result(2,ntv);

  srvf::Srvf Q(samps,params);
  Q.evaluate(tv,result);
  for (int i=0; i<result.size(); ++i)
  {
    BOOST_CHECK_CLOSE(result(i),exp[i],1e-9);
  }
}

BOOST_AUTO_TEST_CASE(l2_norm_test1)
{
  double samps_data[] = {1.0, -1.0, 1.0, -1.0, -1.0};
  srvf::Matrix params=srvf::util::linspace(0.0, 1.0, 6);
  srvf::Matrix samps(1, 5, samps_data);
  srvf::Srvf Q(samps, params);
  double exp=1.0;
  double nrm=srvf::l2_norm(Q);
  BOOST_CHECK_CLOSE(exp,nrm,1e-9);
}

BOOST_AUTO_TEST_CASE(l2_product_test1)
{
  double params1_data[] = {0.0, 0.25, 0.8, 1.0};
  double params2_data[] = {0.0, 0.5, 1.0};
  double samps1_data[] = {1.0, -1.0, 1.0};
  double samps2_data[] = {1.0, -1.0};
  double expected=0.1;

  int ncp1=sizeof(params1_data)/sizeof(double);
  int ncp2=sizeof(params2_data)/sizeof(double);

  srvf::Matrix params1(1,ncp1,params1_data);
  srvf::Matrix params2(1,ncp2,params2_data);
  srvf::Matrix samps1(1,ncp1-1,samps1_data);
  srvf::Matrix samps2(1,ncp2-1,samps2_data);
  srvf::Srvf Q1(samps1,params1);
  srvf::Srvf Q2(samps2,params2);

  double ip=srvf::l2_product(Q1,Q2);
  BOOST_CHECK_CLOSE(ip,expected,1e-9);
}

BOOST_AUTO_TEST_CASE(l2_product_test2)
{
  double params1_data[] = {0.0, 0.2500000, 0.8, 0.99995, 1.0};
  double params2_data[] = {0.0, 0.0000005, 0.25, 0.80001, 1.0};
  double samps1_data[] = {1.0, -1.0, 1.0, -1.0};
  double samps2_data[] = {1.0, -1.0, 1.0, -1.0};
  double expected=-0.999879;

  int ncp1=sizeof(params1_data)/sizeof(double);
  int ncp2=sizeof(params2_data)/sizeof(double);

  srvf::Matrix params1(1,ncp1,params1_data);
  srvf::Matrix params2(1,ncp2,params2_data);
  srvf::Matrix samps1(1,ncp1-1,samps1_data);
  srvf::Matrix samps2(1,ncp2-1,samps2_data);
  srvf::Srvf Q1(samps1,params1);
  srvf::Srvf Q2(samps2,params2);

  double ip=srvf::l2_product(Q1,Q2);
  BOOST_CHECK_CLOSE(ip,expected,1e-4);
}

BOOST_AUTO_TEST_CASE(l2_distance_test1)
{
  double params1_data[] = {0.0, 0.25, 0.8, 1.0};
  double params2_data[] = {0.0, 0.5, 1.0};
  double samps1_data[] = {1.0, -1.0, 1.0};
  double samps2_data[] = {1.0, -1.0};
  double expected=1.341640786;

  int ncp1=sizeof(params1_data)/sizeof(double);
  int ncp2=sizeof(params2_data)/sizeof(double);

  srvf::Matrix params1(1,ncp1,params1_data);
  srvf::Matrix params2(1,ncp2,params2_data);
  srvf::Matrix samps1(1,ncp1-1,samps1_data);
  srvf::Matrix samps2(1,ncp2-1,samps2_data);
  srvf::Srvf Q1(samps1,params1);
  srvf::Srvf Q2(samps2,params2);

  double d=srvf::l2_distance(Q1,Q2);
  BOOST_CHECK_CLOSE(d,expected,1e-4);
}

BOOST_AUTO_TEST_CASE(l2_distance_test2)
{
  double params1_data[] = {0.0, 0.2500000, 0.8, 0.99995, 1.0};
  double params2_data[] = {0.0, 0.0000005, 0.25, 0.80001, 1.0};
  double samps1_data[] = {1.0, -1.0, 1.0, -1.0};
  double samps2_data[] = {1.0, -1.0, 1.0, -1.0};
  double expected=1.999939499;

  int ncp1=sizeof(params1_data)/sizeof(double);
  int ncp2=sizeof(params2_data)/sizeof(double);

  srvf::Matrix params1(1,ncp1,params1_data);
  srvf::Matrix params2(1,ncp2,params2_data);
  srvf::Matrix samps1(1,ncp1-1,samps1_data);
  srvf::Matrix samps2(1,ncp2-1,samps2_data);
  srvf::Srvf Q1(samps1,params1);
  srvf::Srvf Q2(samps2,params2);

  double d=srvf::l2_distance(Q1,Q2);
  BOOST_CHECK_CLOSE(d,expected,1e-4);
}

BOOST_AUTO_TEST_CASE(rotate_test1)
{
  double samps_data[] = 
  {
    0.2, 0.15, -1.2, 0.99, 5.0, 
    -1.0, 0.0, 1.0, 1.0, 0.0
  };
  int ncp=6; // number of sample points +1
  int dim=2;
  double R_data[] = 
  {
    0.8660254038, -0.5,
    0.5, 0.8660254038
  };
  double exp_data[] = {
     0.673205, 0.129904, -1.539230, 0.357365, 4.330127,
    -0.766025, 0.075000,  0.266025, 1.361025, 2.500000
  };
  srvf::Matrix samps(dim,ncp-1,samps_data);
  srvf::Matrix R(dim,dim,R_data);

  srvf::Srvf Q(samps);
  Q.rotate(R);

  BOOST_REQUIRE_EQUAL(Q.dim(),dim);
  BOOST_REQUIRE_EQUAL(Q.ncp(),ncp);
  for (int i=0; i<Q.samps().size(); ++i)
  {
    BOOST_CHECK_CLOSE(Q.samps()(i),exp_data[i],1e-3);
  }
}

BOOST_AUTO_TEST_CASE(linear_combination_test1)
{
  double params1_data[] = {0.0, 0.25, 0.8, 1.0};
  double params2_data[] = {0.0, 0.5, 1.0};
  double samps1_data[] = {1.0, -1.0, 1.0};
  double samps2_data[] = {1.0, -1.0};
  double w1=0.25;
  double w2=0.5;
  double exp_params[] = {0.0, 0.25, 0.5, 0.8, 1.0};
  double exp_samps[] = {0.75, 0.25, -0.75, -0.25};

  int ncp1=sizeof(params1_data)/sizeof(double);
  int ncp2=sizeof(params2_data)/sizeof(double);
  int exp_ncp=sizeof(exp_params)/sizeof(double);

  srvf::Matrix params1(1,ncp1,params1_data);
  srvf::Matrix params2(1,ncp2,params2_data);
  srvf::Matrix samps1(1,ncp1-1,samps1_data);
  srvf::Matrix samps2(1,ncp2-1,samps2_data);
  srvf::Srvf Q1(samps1,params1);
  srvf::Srvf Q2(samps2,params2);
  srvf::Srvf Q=srvf::linear_combination(Q1,Q2,w1,w2);

  BOOST_REQUIRE_EQUAL(Q.dim(),Q1.dim());
  BOOST_REQUIRE_EQUAL(Q.ncp(),exp_ncp);
  for (int i=0; i<exp_ncp; ++i)
  {
    BOOST_CHECK_CLOSE(Q.params()(i),exp_params[i],1e-9);
  }
  for (int i=0; i<exp_ncp-1; ++i)
  {
    BOOST_CHECK_CLOSE(Q.samps()(i),exp_samps[i],1e-9);
  }
}

BOOST_AUTO_TEST_CASE(refinement_test1)
{
  double params_data[] = {0.0, 0.25, 0.8, 1.0};
  double tv_data[] = {0.0, 0.5, 0.99};
  double samps_data[] = {1.0, -1.0, 1.0};
  double exp_params[] = {0.0, 0.25, 0.5, 0.8, 0.99, 1.0};
  double exp_samps[] = {1.0, -1.0, -1.0, 1.0, 1.0};

  int ncp=sizeof(params_data)/sizeof(double);
  int ntv=sizeof(tv_data)/sizeof(double);
  int exp_ncp=sizeof(exp_params)/sizeof(double);

  srvf::Matrix params(1,ncp,params_data);
  srvf::Matrix samps(1,ncp-1,samps_data);
  srvf::Matrix tv(1,ntv,tv_data);
  srvf::Srvf Q(samps,params);
  srvf::Srvf Qr=srvf::refinement(Q,tv);

  BOOST_REQUIRE_EQUAL(Qr.dim(),Q.dim());
  BOOST_REQUIRE_EQUAL(Qr.ncp(),exp_ncp);
  for (int i=0; i<exp_ncp; ++i)
  {
    BOOST_CHECK_CLOSE(Qr.params()(i),exp_params[i],1e-9);
  }
  for (int i=0; i<exp_ncp-1; ++i)
  {
    BOOST_CHECK_CLOSE(Qr.samps()(i),exp_samps[i],1e-9);
  }
}

BOOST_AUTO_TEST_CASE(gamma_action_test1)
{
  double Q_samps_data[] = {1.0};
  double Q_params_data[] = {0.0, 1.0};
  double gamma_samps_data[] = {0.0, 0.5, 1.0};
  double gamma_params_data[] = {0.0, 1.0/3.0, 1.0};
  double exp_params[] = {0.0, 1.0/3.0, 1.0};
  double exp_samps[] = {1.224744871, 0.8660254038};

  int Q_dim=1;
  int Q_ncp=sizeof(Q_params_data)/sizeof(double);
  int gamma_ncp=sizeof(gamma_samps_data)/sizeof(double);
  int exp_ncp=sizeof(exp_params)/sizeof(double);
  
  srvf::Matrix Q_samps(Q_dim,Q_ncp-1,Q_samps_data);
  srvf::Matrix Q_params(1,Q_ncp,Q_params_data);
  srvf::Matrix gamma_samps(1,gamma_ncp,gamma_samps_data);
  srvf::Matrix gamma_params(1,gamma_ncp,gamma_params_data);

  srvf::Srvf Q(Q_samps,Q_params);
  srvf::Plf gamma(gamma_samps,gamma_params);
  srvf::Srvf Qgamma=srvf::gamma_action(Q,gamma);

  BOOST_REQUIRE_EQUAL(Qgamma.dim(),1);
  BOOST_REQUIRE_EQUAL(Qgamma.ncp(),exp_ncp);
  for (int i=0; i<exp_ncp; ++i)
  {
    BOOST_CHECK_CLOSE(Qgamma.params()(i),exp_params[i],1e-3);
  }
  for (int i=0; i<exp_ncp-1; ++i)
  {
    BOOST_CHECK_CLOSE(Qgamma.samps()(i),exp_samps[i],1e-3);
  }
}

BOOST_AUTO_TEST_SUITE_END()
