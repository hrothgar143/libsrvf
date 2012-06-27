#include <boost/test/unit_test.hpp>

#include <fltk/run.h>
#include <vector>

#include "plot.h"
#include "plotwin.h"
#include "plf.h"
#include "srvf.h"

BOOST_AUTO_TEST_SUITE(plot_tests)

BOOST_AUTO_TEST_CASE(function_plot_test1)
{
  double samps_data[] = { 0.0, 1.0, 0.0, 1.0, 0.0 };
  size_t npts = sizeof(samps_data) / sizeof(double);
  srvf::Pointset samps(1, npts, samps_data);
  std::vector<double> params = srvf::util::linspace(0.0, 1.0, npts);
  srvf::Plf F(samps, params);

  srvf::FunctionPlot plot;
  plot.insert(F, srvf::Color(0.0, 0.0, 1.0));
  srvf::FltkGlPlotWindow plotwin(800, 400, "plot_tests/function_plot_test1");
  plotwin.add_plot(static_cast<srvf::Plot*>(&plot));
  plotwin.show();
  fltk::run();
}

BOOST_AUTO_TEST_SUITE_END()
